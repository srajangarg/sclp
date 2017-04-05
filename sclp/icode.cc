#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

/****************************** Class Ics_Opd *****************************/

RD * Ics_Opd::get_reg()
{
	//TODO
	return NULL;
}

Symbol_Table_Entry * Ics_Opd::get_symbol_entry()
{
	//TODO
	return NULL;
}

/****************************** Class Mem_Addr_Opd *****************************/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se) 
{
	//TODO
	symbol_entry = &se;
}

MA_Opd & Mem_Addr_Opd::operator=(const MA_Opd & rhs)
{
	//TODO
	symbol_entry = rhs.symbol_entry;
	return *this;
}

Symbol_Table_Entry * Mem_Addr_Opd::get_symbol_entry()
{
	return symbol_entry;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();
	CHECK_INVARIANT(((symbol_scope == local) || (symbol_scope == global)),
			"Wrong scope value");

	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();

	if (symbol_scope == local)
	{
		file_buffer << symbol_entry->get_start_offset() << "($fp)";
	}
	else if (symbol_scope == global)
	{
		file_buffer << symbol_entry->get_variable_name();
	}
	else
	{
		if (symbol_entry->get_ref_offset() == sp_ref)
			file_buffer << symbol_entry->get_start_offset() << "($sp)";
		else
			file_buffer << symbol_entry->get_start_offset() << "($fp)";
	}
}

/****************************** Class Register_Addr_Opd *****************************/

Register_Addr_Opd::Register_Addr_Opd(RD * reg) 
{
	//TODO
	register_description = reg;
}

RD * Register_Addr_Opd::get_reg()    
{ 
	//TODO
	return register_description;
}

RA_Opd& Register_Addr_Opd::operator=(const RA_Opd& rhs)
{
	//TODO
	register_description = rhs.register_description;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer << register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer << "$" << register_description->get_name();
}

/****************************** Class Const_Opd *****************************/

template <class DATA_TYPE>
Const_Opd<DATA_TYPE>::Const_Opd(DATA_TYPE n) 
{
	//TODO
	num = n;
}

template <class DATA_TYPE>
Const_Opd<DATA_TYPE> & Const_Opd<DATA_TYPE>::operator=(const Const_Opd<DATA_TYPE> & rhs)
{
	//TODO
	num = rhs.num;
	return *this;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_ics_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer << num;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_asm_opd(ostream & file_buffer) 
{
	//TODO
	file_buffer << num;
}

/****************************** Class Icode_Stmt *****************************/

Instruction_Descriptor & Icode_Stmt::get_op()
{ 
	return op_desc; 
}

Ics_Opd * Icode_Stmt::get_opd1()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

Ics_Opd * Icode_Stmt::get_opd2()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

Ics_Opd * Icode_Stmt::get_result()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void Icode_Stmt::set_opd1(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void Icode_Stmt::set_opd2(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void Icode_Stmt::set_result(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

/*************************** Class Move_IC_Stmt *****************************/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;   
	result = res; 
}

Ics_Opd * Move_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Move_IC_Stmt::get_result()        { return result; }

void Move_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Move_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

MovS& Move_IC_Stmt::operator=(const MovS& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	result = rhs.result; 

	return *this;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

	string operation_name = op_desc.get_name();

	switch (op_desc.get_ic_format())
	{
	case i_r_op_o1: 
		file_buffer << "\t" << operation_name << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << "\n";

		break;

	default: 
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	switch (op_desc.get_assembly_format())
	{
	case a_op_r_o1: 
		file_buffer << "\t" << op_name << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";

		break; 

	case a_op_o1_r: 
		file_buffer << "\t" << op_name << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		result->print_asm_opd(file_buffer);
		file_buffer << "\n";

		break; 

	default: 
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

/*************************** Class Compute_IC_Stmt *****************************/

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op op, Ics_Opd * res, Ics_Opd * o1, Ics_Opd * o2)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	opd2 = o2;
	result = res;
}

Ics_Opd * Compute_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Compute_IC_Stmt::get_opd2()          { return opd2; }
Ics_Opd * Compute_IC_Stmt::get_result()        { return result; }

void Compute_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Compute_IC_Stmt::set_opd2(Ics_Opd * io)   { opd2 = io; }
void Compute_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

CompS& Compute_IC_Stmt::operator=(const CompS& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	result = rhs.result; 

	return *this;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer)
{	
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a compute IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a compute IC Stmt");

	string operation_name = op_desc.get_name();

	switch (op_desc.get_ic_format())
	{
	case i_r_o1_op_o2: 
		file_buffer << "\t" << operation_name << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << " , ";
		opd2->print_ics_opd(file_buffer);
		file_buffer << "\n";

		break;

	case i_r_op_o1:
		file_buffer << "\t" << operation_name << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << "\n";

		break;

	default: 
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	switch (op_desc.get_assembly_format())
	{
	case a_op_r_o1_o2:
		file_buffer << "\t" << op_name << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer << "\n";

		break;

	case a_op_o1_o2_r: 
		file_buffer << "\t" << op_name << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer << ", ";
		result->print_asm_opd(file_buffer);
		file_buffer << "\n";

		break;

	case a_op_r_o1:
		file_buffer << "\t" << op_name << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";
		break;

	case a_op_o1_r:
		file_buffer << "\t" << op_name << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		result->print_asm_opd(file_buffer);
		file_buffer << "\n";
		break;
	
	default: 
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

/*************************** Class Control_Flow_IC_Stmt *****************************/

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	opd2 = o2;
	offset = label;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Control_Flow_IC_Stmt::get_opd2()          { return opd2; }
string Control_Flow_IC_Stmt::get_Offset()        { return offset; }

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io)   { opd2 = io; }
void Control_Flow_IC_Stmt::set_Offset(string s) { offset = s; }

ContS& Control_Flow_IC_Stmt::operator=(const ContS& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	offset = rhs.offset;

	return *this;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer)
{
	string operation_name = op_desc.get_name();

	switch (op_desc.get_ic_format())
	{
	case i_op_o1_o2_st:
		file_buffer << "\t" << operation_name << ":    \t";
		opd1->print_ics_opd(file_buffer);
		file_buffer << " , ";
		opd2->print_ics_opd(file_buffer);
		file_buffer << " : goto " << offset << "\n";

		break;

	case i_op_st:
		file_buffer << "\tgoto " << offset << "\n";

		break;

	default:
		cout<<op_desc.get_ic_format()<<endl;
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer)
{
	string op_name = op_desc.get_mnemonic();

	switch (op_desc.get_assembly_format())
	{
	case a_op_o1_o2_st:
		file_buffer << "\t" << op_name << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer << ", " << offset << "\n";
		break;

	case a_op_st:
		file_buffer << "\t" << op_name << " " << offset << "\n";
		break;

	default:
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

/*************************** Class Label_IC_Stmt *****************************/

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op op, Ics_Opd * o1, string off)
{
	CHECK_INVARIANT((machine_desc_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_desc_object.spim_instruction_table[op]);
	opd1 = o1;
	offset = off;
}

Ics_Opd * Label_IC_Stmt::get_opd1()          { return opd1; }
string Label_IC_Stmt::get_offset()          { return offset; }

void Label_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Label_IC_Stmt::set_offset(string off)   { offset = off; }

LabS& Label_IC_Stmt::operator=(const LabS& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	offset = rhs.offset;

	return *this;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer)
{
	file_buffer << "\n" << offset << ":\n";
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer)
{
	if (offset != "")
		file_buffer << "\n" << offset << ":\n";
	else
		file_buffer << "\tsyscall\n";
}

/******************************* Class Code_For_Ast ****************************/

Code_For_Ast::Code_For_Ast()
{
	//TODO
	result_register = NULL;
}

Code_For_Ast::Code_For_Ast(list<ICS *> & ic_l, RD * reg)
{
	//TODO
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(ICS & ic_stmt)
{
	//TODO
	ics_list.push_back(&ic_stmt);
}

list<ICS *> & Code_For_Ast::get_icode_list()  
{ 
	//TODO 
	return ics_list;
}

RD * Code_For_Ast::get_reg()
{
	//TODO
	return result_register;
}

void Code_For_Ast::set_reg(RD * reg)
{
	//TODO
	result_register = reg;
}

CFA& Code_For_Ast::operator=(const CFA& rhs)
{
	//TODO
	result_register = rhs.result_register;
	ics_list = rhs.ics_list;
	return *this;
}

/************************ class Instruction_Descriptor ********************************/

Tgt_Op Instruction_Descriptor::get_op()                   	{ return inst_op; }
string Instruction_Descriptor::get_name()                       { return name; }
string Instruction_Descriptor::get_mnemonic()                   { return mnemonic; }
string Instruction_Descriptor::get_ic_symbol()                  { return ic_symbol; }
Icode_Format Instruction_Descriptor::get_ic_format()            { return ic_format; }
Assembly_Format Instruction_Descriptor::get_assembly_format()   { return assem_format; }

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string temp_name, string temp_mnemonic, 
						string ic_sym, Icode_Format ic_form, Assembly_Format as_form)
{
	inst_op = op;
	name = temp_name; 
	mnemonic = temp_mnemonic;
	ic_symbol = ic_sym;
	ic_format = ic_form;
	assem_format = as_form;
}

Instruction_Descriptor::Instruction_Descriptor()
{
	inst_op = nop;
	name = "";
	mnemonic = "";
	ic_symbol = "";
	ic_format = i_nsy;
	assem_format = a_nsy;
}

template class Const_Opd<int>;
template class Const_Opd<double>;
