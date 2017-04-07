#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "user-options.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "procedure.hh"
#include "program.hh"

CFA &Ast::create_store_stmt(RD *store_register)
{
    stringstream msg;
    msg << "No create_store_stmt() function for " << typeid(*this).name();
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_assembly()
{
    stringstream msg;
    msg << "No print_assembly() function for " << typeid(*this).name();
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_icode()
{
    stringstream msg;
    msg << "No print_icode() function for " << typeid(*this).name();
    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

CFA &Assignment_Ast::compile()
{
    CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
    CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

    CFA &load_stmt = rhs->compile();

    RD *l_reg = load_stmt.get_reg();
    CHECK_INVARIANT((l_reg != NULL), "Load register cannot be null in Assignment_Ast");
    l_reg->set_use_for_expr_result();

    CFA store_stmt = lhs->create_store_stmt(l_reg);

    CHECK_INVARIANT((l_reg != NULL), "Load register cannot be null in Assignment_Ast");
    l_reg->reset_use_for_expr_result();

    // Store the statement in ic_list
    list<ICS *> &ic_list = *new list<ICS *>;
    ic_list.splice(ic_list.end(), load_stmt.get_icode_list());
    ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

    CFA *assign_stmt = new CFA(ic_list, l_reg);
    return *assign_stmt;
}

/////////////////////////////////////////////////////////////////

CFA &Name_Ast::compile()
{
    CHECK_INVARIANT((variable_symbol_entry != NULL),
                    "variable_symbol_entry cannot be null in Name_Ast");

    CFA *cfa = new CFA();
    RD *reg;
    Tgt_Op op;

    if (get_data_type() == int_data_type) {
        op = load;
        reg = machine_desc_object.get_new_register<gp_data>();
    } else {
        op = load_d;
        reg = machine_desc_object.get_new_register<float_reg>();
    }

    MovS *m = new MovS(op, new MA_Opd(*variable_symbol_entry), new RA_Opd(reg));
    cfa->append_ics(*m);
    cfa->set_reg(reg);
    return *cfa;
}

CFA &Name_Ast::create_store_stmt(RD *store_register)
{
    CHECK_INVARIANT((variable_symbol_entry != NULL),
                    "variable_symbol_entry cannot be null in Name_Ast");

    CFA *cfa = new CFA();
    Tgt_Op op;

    if (get_data_type() == int_data_type)
        op = store;
    else
        op = store_d;

    MovS *m
        = new MovS(op, new RA_Opd(store_register), new MA_Opd(*variable_symbol_entry));
    cfa->append_ics(*m);
    return *cfa;
}

CFA &ArithTwoOp(Ast *lhs, Ast *rhs, Data_Type dt, Tgt_Op opint, Tgt_Op opdou)
{
    CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
    CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

    CFA &lhs_s = lhs->compile();
    CFA &rhs_s = rhs->compile();

    CHECK_INVARIANT((lhs_s.get_reg() != NULL), "Lhs register cannot be null");
    CHECK_INVARIANT((rhs_s.get_reg() != NULL), "Rhs register cannot be null");

    list<ICS *> &ic_list = *new list<ICS *>;
    ic_list.splice(ic_list.end(), lhs_s.get_icode_list());
    ic_list.splice(ic_list.end(), rhs_s.get_icode_list());

    RD *reg;
    Tgt_Op op;

    if (dt == int_data_type or void_data_type) {
        op = opint;
        reg = machine_desc_object.get_new_register<gp_data>();
    } else {
        op = opdou;
        reg = machine_desc_object.get_new_register<float_reg>();
    }

    lhs_s.get_reg()->reset_use_for_expr_result();
    rhs_s.get_reg()->reset_use_for_expr_result();
    ic_list.push_back(new CompS(op, new RA_Opd(reg), new RA_Opd(lhs_s.get_reg()),
                                new RA_Opd(rhs_s.get_reg())));

    CFA *arith = new CFA(ic_list, reg);
    return *arith;
}

CFA &RelOpFloat(Ast *lhs, Ast *rhs, Tgt_Op co, Tgt_Op bc, bool invert = false)
{
    CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
    CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

    CFA &lhs_s = lhs->compile();
    CFA &rhs_s = rhs->compile();

    CHECK_INVARIANT((lhs_s.get_reg() != NULL), "Lhs register cannot be null");
    CHECK_INVARIANT((rhs_s.get_reg() != NULL), "Rhs register cannot be null");

    auto regx = machine_desc_object.get_new_register<gp_data>();

    list<ICS *> &ic_list = *new list<ICS *>;
    ic_list.splice(ic_list.end(), lhs_s.get_icode_list());
    ic_list.splice(ic_list.end(), rhs_s.get_icode_list());

    string lab = Ast::get_new_label();
    ic_list.push_back(new MovS(imm_load, new Const_Opd<int>(0), new RA_Opd(regx)));
    ic_list.push_back(
        new CompS(co, NULL, new RA_Opd(lhs_s.get_reg()), new RA_Opd(rhs_s.get_reg())));
    ic_list.push_back(new ContS(bc, NULL, NULL, lab));
    ic_list.push_back(new MovS(imm_load, new Const_Opd<int>(1), new RA_Opd(regx)));
    ic_list.push_back(new LabS(label, NULL, lab));

    regx->reset_use_for_expr_result();

    RD *reg = machine_desc_object.get_new_register<gp_data>();

    if (invert)
        ic_list.push_back(
            new CompS(not_t, new RA_Opd(reg), new RA_Opd(reg), new Const_Opd<int>(1)));

    lhs_s.get_reg()->reset_use_for_expr_result();
    rhs_s.get_reg()->reset_use_for_expr_result();

    CFA *arith = new CFA(ic_list, reg);
    return *arith;
}

CFA &ArithOneOp(Ast *lhs, Data_Type dt, Tgt_Op opint, Tgt_Op opdou,
                bool is_not_type = false)
{
    CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

    RD *reg, *one = NULL;
    Tgt_Op op;
    list<ICS *> &ic_list = *new list<ICS *>;

    if (is_not_type) {
        one = machine_desc_object.get_new_register<gp_data>();
        ic_list.push_back(new MovS(imm_load, new Const_Opd<int>(1), new RA_Opd(one)));
    }

    CFA &lhs_s = lhs->compile();
    ic_list.splice(ic_list.end(), lhs_s.get_icode_list());
    CHECK_INVARIANT((lhs_s.get_reg() != NULL), "Lhs register cannot be null");

    if (dt == int_data_type) {
        op = opint;
        reg = machine_desc_object.get_new_register<gp_data>();
    } else {
        op = opdou;
        reg = machine_desc_object.get_new_register<float_reg>();
    }

    if (is_not_type)
        one->reset_use_for_expr_result();

    lhs_s.get_reg()->reset_use_for_expr_result();
    CompS *c = new CompS(op, new RA_Opd(reg), new RA_Opd(lhs_s.get_reg()),
                         (one == NULL) ? NULL : new RA_Opd(one));
    ic_list.push_back(c);

    CFA *arith = new CFA(ic_list, reg);
    return *arith;
}

CFA &CondOpIfElse(CFA &cond_s, CFA &then_s, CFA &else_s, string flabel, string slabel,
                  Data_Type dt, bool need_reg = false)
{
    RD *reg = NULL;

    ContS *bq
        = new ContS(beq, new RA_Opd(cond_s.get_reg()),
                    new RA_Opd(machine_desc_object.spim_register_table[zero]), flabel);

    list<ICS *> &ic1 = *new list<ICS *>, ic2 = *new list<ICS *>;

    ic1.splice(ic1.end(), cond_s.get_icode_list());
    ic1.push_back(bq);
    ic1.splice(ic1.end(), then_s.get_icode_list());

    ic2.push_back(new ContS(j, NULL, NULL, slabel));
    ic2.push_back(new LabS(label, NULL, flabel));
    ic2.splice(ic2.end(), else_s.get_icode_list());

    if (need_reg) {
        if (dt == int_data_type or dt == void_data_type)
            reg = machine_desc_object.get_new_register<gp_data>();
        else
            reg = machine_desc_object.get_new_register<float_reg>();

        CompS *or1 = new CompS(or_t, new RA_Opd(reg), new RA_Opd(then_s.get_reg()),
                               new RA_Opd(machine_desc_object.spim_register_table[zero]));
        ic1.push_back(or1);

        CompS *or2 = new CompS(or_t, new RA_Opd(reg), new RA_Opd(else_s.get_reg()),
                               new RA_Opd(machine_desc_object.spim_register_table[zero]));
        ic2.push_back(or2);

        then_s.get_reg()->reset_use_for_expr_result();
        else_s.get_reg()->reset_use_for_expr_result();
    }

    cond_s.get_reg()->reset_use_for_expr_result();

    ic2.push_back(new LabS(label, NULL, slabel));
    ic1.splice(ic1.end(), ic2);
    CFA *selection = new CFA(ic1, reg);
    return *selection;
}
///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
CFA &Number_Ast<DATA_TYPE>::compile()
{
    CFA *cfa = new CFA();

    RD *reg;
    Ics_Opd *opd;
    Tgt_Op op;

    if (get_data_type() == int_data_type) {
        reg = machine_desc_object.get_new_register<gp_data>();
        opd = new Const_Opd<int>(constant);
        op = imm_load;
    } else {
        reg = machine_desc_object.get_new_register<float_reg>();
        opd = new Const_Opd<double>(constant);
        op = imm_load_d;
    }

    MovS *m = new MovS(op, opd, new RA_Opd(reg));
    cfa->append_ics(*m);
    cfa->set_reg(reg);
    return *cfa;
}

///////////////////////////////////////////////////////////////////////////////

CFA &Relational_Expr_Ast::compile()
{
    if (lhs_condition->get_data_type() != double_data_type) {
        switch (rel_op) {
            case less_equalto:
                return ArithTwoOp(lhs_condition, rhs_condition, get_data_type(), sle,
                                  sle);
            case less_than:
                return ArithTwoOp(lhs_condition, rhs_condition, get_data_type(), slt,
                                  slt);
            case greater_than:
                return ArithTwoOp(lhs_condition, rhs_condition, get_data_type(), sgt,
                                  sgt);
            case greater_equalto:
                return ArithTwoOp(lhs_condition, rhs_condition, get_data_type(), sge,
                                  sge);
            case equalto:
                return ArithTwoOp(lhs_condition, rhs_condition, get_data_type(), seq,
                                  seq);
            case not_equalto:
                return ArithTwoOp(lhs_condition, rhs_condition, get_data_type(), sne,
                                  sne);
            default:
                CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Relational_Op not supported");
        }
    } else {
        switch (rel_op) {
            case less_equalto:
                return RelOpFloat(lhs_condition, rhs_condition, sle_d, bc1f);
            case less_than:
                return RelOpFloat(lhs_condition, rhs_condition, slt_d, bc1f);
            case greater_than:
                return RelOpFloat(lhs_condition, rhs_condition, sle_d, bc1f, true);
            case greater_equalto:
                return RelOpFloat(lhs_condition, rhs_condition, slt_d, bc1f, true);
            case equalto:
                return RelOpFloat(lhs_condition, rhs_condition, seq_d, bc1f);
            case not_equalto:
                return RelOpFloat(lhs_condition, rhs_condition, seq_d, bc1f, true);
            default:
                CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Relational_Op not supported");
        }
    }
}

//////////////////////////////////////////////////////////////////////

CFA &Boolean_Expr_Ast::compile()
{
    switch (bool_op) {
        case boolean_not:
            return ArithOneOp(rhs_op, get_data_type(), not_t, not_t, true);
        case boolean_and:
            return ArithTwoOp(lhs_op, rhs_op, get_data_type(), and_t, and_t);
        case boolean_or:
            return ArithTwoOp(lhs_op, rhs_op, get_data_type(), or_t, or_t);
        default:
            CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Boolean_Op not supported");
    }
}

///////////////////////////////////////////////////////////////////////

CFA &Selection_Statement_Ast::compile()
{
    CFA &cond_s = cond->compile();
    CFA &then_s = then_part->compile();
    CFA &else_s = else_part->compile();

    string flabel = Ast::get_new_label(), slabel = Ast::get_new_label();
    return CondOpIfElse(cond_s, then_s, else_s, flabel, slabel, cond->get_data_type());
}

///////////////////////////////////////////////////////////////////////////////////////////

CFA &Iteration_Statement_Ast::compile()
{
    CFA &cond_s = cond->compile();
    string flabel = Ast::get_new_label(), slabel = Ast::get_new_label();
    CFA &body_s = body->compile();

    list<ICS *> &ic_list = *new list<ICS *>;

    if (not is_do_form)
        ic_list.push_back(new ContS(j, NULL, NULL, slabel));

    ic_list.push_back(new LabS(label, NULL, flabel));
    ic_list.splice(ic_list.end(), body_s.get_icode_list());
    ic_list.push_back(new LabS(label, NULL, slabel));
    ic_list.splice(ic_list.end(), cond_s.get_icode_list());

    ContS *bq
        = new ContS(bne, new RA_Opd(cond_s.get_reg()),
                    new RA_Opd(machine_desc_object.spim_register_table[zero]), flabel);
    ic_list.push_back(bq);
    cond_s.get_reg()->reset_use_for_expr_result();

    CFA *selection = new CFA(ic_list, NULL);
    return *selection;
}

///////////////////////////////////////////////////////////////////////////////////////////

CFA &Plus_Ast::compile()
{
    return ArithTwoOp(lhs, rhs, get_data_type(), add, add_d);
}

/////////////////////////////////////////////////////////////////

CFA &Minus_Ast::compile()
{
    return ArithTwoOp(lhs, rhs, get_data_type(), sub, sub_d);
}

//////////////////////////////////////////////////////////////////

CFA &Mult_Ast::compile()
{
    return ArithTwoOp(lhs, rhs, get_data_type(), mult, mult_d);
}

////////////////////////////////////////////////////////////////////

CFA &Conditional_Operator_Ast::compile()
{
    CFA &cond_s = cond->compile();
    CFA &then_s = lhs->compile();
    CFA &else_s = rhs->compile();

    string flabel = Ast::get_new_label(), slabel = Ast::get_new_label();
    return CondOpIfElse(cond_s, then_s, else_s, flabel, slabel, lhs->get_data_type(),
                        true);
}

////////////////////////////////////////////////////////////////////

CFA &Divide_Ast::compile()
{
    return ArithTwoOp(lhs, rhs, get_data_type(), divd, div_d);
}

//////////////////////////////////////////////////////////////////////

CFA &UMinus_Ast::compile()
{
    return ArithOneOp(lhs, get_data_type(), uminus, uminus_d);
}

//////////////////////////////////////////////////////////////////////////////

CFA &Sequence_Ast::compile()
{
    for (list<Ast *>::iterator it = statement_list.begin(); it != statement_list.end();
         it++) {
        list<ICS *> &ic_list = (*it)->compile().get_icode_list();
        sa_icode_list.insert(sa_icode_list.end(), ic_list.begin(), ic_list.end());
    }
    return *(new CFA(sa_icode_list, NULL));
}

void Sequence_Ast::print_assembly(ostream &file_buffer)
{
    for (list<ICS *>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end();
         it++)
        (*it)->print_assembly(file_buffer);
}

void Sequence_Ast::print_icode(ostream &file_buffer)
{
    for (list<ICS *>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end();
         it++)
        (*it)->print_icode(file_buffer);
}

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;

CFA &Call_Ast::compile()
{
    list<ICS *> &ic_list = *new list<ICS *>;
    Tgt_Op op;
    RD *reg1, *reg2;
    auto args_stes = func->get_arguments_stes();

    for (int i = arg_list.size() - 1; i >= 0; i--) {
        auto arg = arg_list[i];
        auto ste = args_stes[i];

        CFA &ret = arg->compile();
        ic_list.splice(ic_list.end(), ret.get_icode_list());

        if (arg->get_data_type() == int_data_type)
            op = store;
        else
            op = store_d;

        MovS *m = new MovS(op, new RA_Opd(ret.get_reg()), new MA_Opd(*ste));
        ret.get_reg()->reset_use_for_expr_result();
        ic_list.push_back(m);
    }

    auto spsp = new RA_Opd(machine_desc_object.spim_register_table[sp]);
    if (arg_list.size() > 0)
        ic_list.push_back(
            new CompS(sub, spsp, spsp,
                      new Const_Opd<int>(func->get_formal_symbol_table_size() - 8)));
    ic_list.push_back(new ContS(jal, NULL, NULL, func->get_proc_name()));

    if (arg_list.size() > 0)
        ic_list.push_back(
            new CompS(add, spsp, spsp,
                      new Const_Opd<int>(func->get_formal_symbol_table_size() - 8)));

    if (func->get_return_type() == int_data_type) {
        op = mov;
        reg1 = machine_desc_object.get_new_register<gp_data>();
        reg2 = machine_desc_object.spim_register_table[v1];
        ic_list.push_back(new MovS(op, new RA_Opd(reg2), new RA_Opd(reg1)));

    } else if (func->get_return_type() == double_data_type) {
        op = mov_d;
        reg1 = machine_desc_object.get_new_register<float_reg>();
        reg2 = machine_desc_object.spim_register_table[f0];
        ic_list.push_back(new MovS(op, new RA_Opd(reg2), new RA_Opd(reg1)));

    } else {
        reg1 = NULL;
    }

    reg2->reset_use_for_expr_result();
    machine_desc_object.clear_local_register_mappings();

    CFA *ret_stmt = new CFA(ic_list, reg1);
    return *ret_stmt;
}

CFA &Return_Statement_Ast::compile()
{
    list<ICS *> &ic_list = *new list<ICS *>;

    if (return_val != NULL) {
        CFA &ret = return_val->compile();
        ic_list.splice(ic_list.end(), ret.get_icode_list());

        Tgt_Op op;
        RD *reg;

        if (return_val->get_data_type() == int_data_type) {
            op = mov;
            reg = machine_desc_object.spim_register_table[v1];
        } else {
            op = mov_d;
            reg = machine_desc_object.spim_register_table[f0];
        }

        MovS *m = new MovS(op, new RA_Opd(ret.get_reg()), new RA_Opd(reg));
        ret.get_reg()->reset_use_for_expr_result();
        ic_list.push_back(m);
    }

    ic_list.push_back(new ContS(j, NULL, NULL, "epilogue_" + proc->get_proc_name()));

    CFA *ret_stmt = new CFA(ic_list, NULL);
    return *ret_stmt;
}

CFA &Print_Ast::compile()
{
    list<ICS *> &ic_list = *new list<ICS *>;

    string sp = "$sp";
    auto sp_st = new Symbol_Table_Entry(sp, int_data_type, 0, sp_ref);
    auto sp_st2 = new Symbol_Table_Entry(sp, int_data_type, 0, sp_ref);
    sp_st->set_symbol_scope(global);
    sp_st2->set_symbol_scope(formal);
    sp_st2->set_start_offset(0);
    auto spsp = new MA_Opd(*sp_st);
    auto spfor = new MA_Opd(*sp_st2);
    auto rv0 = new RA_Opd(machine_desc_object.spim_register_table[v0]);
    auto ra0 = new RA_Opd(machine_desc_object.spim_register_table[a0]);
    auto rf12 = new RA_Opd(machine_desc_object.spim_register_table[f12]);

    CFA &arg_cfa = arg->compile();
    ic_list.splice(ic_list.end(), arg_cfa.get_icode_list());

    ic_list.push_back(new CompS(imm_add, spsp, spsp, new Const_Opd<int>(-4)));
    ic_list.push_back(new MovS(store, rv0, spfor));
    ic_list.push_back(new CompS(imm_add, spsp, spsp, new Const_Opd<int>(-4)));
    ic_list.push_back(new MovS(store, ra0, spfor));
    ic_list.push_back(new CompS(imm_add, spsp, spsp, new Const_Opd<int>(-8)));
    ic_list.push_back(new MovS(store_d, rf12, spfor));

    if (arg->get_data_type() == int_data_type) {
        ic_list.push_back(new MovS(mov, new RA_Opd(arg_cfa.get_reg()), ra0));
        ic_list.push_back(new MovS(imm_load, new Const_Opd<int>(1), rv0));
    } else if (arg->get_data_type() == double_data_type) {
        ic_list.push_back(new MovS(mov_d, new RA_Opd(arg_cfa.get_reg()), rf12));
        ic_list.push_back(new MovS(imm_load, new Const_Opd<int>(3), rv0));
    } else if (arg->get_data_type() == string_data_type) {
        // la $a0, string0
        string st = static_cast<String_Ast *>(arg)->get_label();
        auto lal = new Symbol_Table_Entry(st, int_data_type, 0, sp_ref);
        lal->set_symbol_scope(global);
        auto lala = new MA_Opd(*lal);

        ic_list.push_back(new MovS(la, lala, ra0));
        ic_list.push_back(new MovS(imm_load, new Const_Opd<int>(4), rv0));
    }

    if (arg_cfa.get_reg() != NULL)
        arg_cfa.get_reg()->reset_use_for_expr_result();
    ic_list.push_back(new LabS(syscall, NULL, ""));
    ic_list.push_back(new MovS(load_d, spfor, rf12));
    ic_list.push_back(new CompS(imm_add, spsp, spsp, new Const_Opd<int>(8)));
    ic_list.push_back(new MovS(load, spfor, ra0));
    ic_list.push_back(new CompS(imm_add, spsp, spsp, new Const_Opd<int>(4)));
    ic_list.push_back(new MovS(load, spfor, rv0));
    ic_list.push_back(new CompS(imm_add, spsp, spsp, new Const_Opd<int>(4)));

    CFA *ret_stmt = new CFA(ic_list, NULL);
    return *ret_stmt;
}

CFA &String_Ast::compile()
{
    label = "string" + to_string(program_object.string_asts.size());
    program_object.string_asts.insert(this);

    CFA *ret_stmt = new CFA(*new list<ICS *>, NULL);
    return *ret_stmt;
}