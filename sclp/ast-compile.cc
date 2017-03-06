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

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
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

Code_For_Ast & Assignment_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null in Assignment_Ast");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null in Assignment_Ast");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null in Assignment_Ast");
	load_register->set_use_for_expr_result();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null in Assignment_Ast");
	load_register->reset_use_for_expr_result();

	// Store the statement in ic_list
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	ic_list = load_stmt.get_icode_list();
	ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt = new Code_For_Ast(ic_list, load_register);
	return *assign_stmt;
}


/////////////////////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile()
{
	CHECK_INVARIANT((variable_symbol_entry != NULL), "variable_symbol_entry cannot be null in Name_Ast");

	Code_For_Ast *cfa = new Code_For_Ast();
	machine_desc_object.clear_local_register_mappings();
	cfa->set_reg(machine_desc_object.get_new_register<gp_data>());
	return *cfa;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((variable_symbol_entry != NULL), "variable_symbol_entry cannot be null in Name_Ast");

	Code_For_Ast *cfa = new Code_For_Ast();
	Tgt_Op op;
	machine_desc_object.clear_local_register_mappings();

	if (get_data_type() == int_data_type)
		op = store;
	else
		op = store_d;

	Move_IC_Stmt *m = new Move_IC_Stmt(op, new Register_Addr_Opd(store_register),
										   new Mem_Addr_Opd(*variable_symbol_entry));
	cfa->append_ics(*m);
	cfa->set_reg(store_register);
	return *cfa;
}


///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	Code_For_Ast *cfa = new Code_For_Ast();

	Register_Descriptor * reg;
	Ics_Opd * opd;
	Tgt_Op op;
	machine_desc_object.clear_local_register_mappings();

	if (get_data_type() == int_data_type)
	{
		reg = machine_desc_object.get_new_register<gp_data>();
		opd = new Const_Opd<int>(constant);
		op = imm_load;
	}
	else
	{
		reg = machine_desc_object.get_new_register<float_reg>();
		opd = new Const_Opd<double>(constant);
		op = imm_load_d;
	}

	Move_IC_Stmt *m = new Move_IC_Stmt(op, opd, new Register_Addr_Opd(reg));
	cfa->append_ics(*m);
	cfa->set_reg(reg);
	return *cfa;
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile()
{
	
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Boolean_Expr_Ast::compile()
{
	
}
///////////////////////////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile()
{
	
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile()
{
	
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile()
{
	
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile()
{
	
}

//////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile()
{
	
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Operator_Ast::compile()
{
	
}


////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile()
{
	
}


//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile()
{
	
}

//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile()
{	
	for (list<Ast*>::iterator it = statement_list.begin(); it != statement_list.end(); it++)
	{	
		list<Icode_Stmt *>& ic_list = (*it)->compile().get_icode_list();
		sa_icode_list.insert(sa_icode_list.end(), ic_list.begin(), ic_list.end());
	}
}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{
	for (list<Icode_Stmt*>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); it++)
		(*it)->print_assembly(file_buffer);
}

void Sequence_Ast::print_icode(ostream & file_buffer)
{
	for (list<Icode_Stmt*>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); it++)
		(*it)->print_icode(file_buffer);
}

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
