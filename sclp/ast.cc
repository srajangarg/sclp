#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"

int Ast::labelCounter;

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::is_value_zero()
{
	stringstream msg;
	msg << "No is_value_zero() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Data_Type dt)
{
	stringstream msg;
	msg << "No set_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	//ADD CODE HERE
	lhs = temp_lhs;
	rhs = temp_rhs;	
	node_data_type = rhs->get_data_type();
	ast_num_child = binary_arity;
	lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	//ADD CODE HERE
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	string tstr = "";
	Symbol_Table_Entry tste;
	Name_Ast temp(tstr, tste, 0);

	// use typeid(), get_data_type()
	//ADD CODE HERE
	if (rhs->get_data_type() == lhs->get_data_type())
		if (ast_num_child == binary_arity)
			if (typeid(*lhs) == typeid(temp))
				return true;

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, 
		"Assignment statement data type not compatible", lineno);
	return false;
}

void Assignment_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n         Asgn:\n";
	file_buffer<<"            LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"            RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	//ADD CODE HERE
	variable_symbol_entry = &var_entry;
	name = var_entry.get_variable_name();
	node_data_type = variable_symbol_entry->get_data_type();
	lineno = line;
	ast_num_child = zero_arity;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	// refer to functions for Symbol_Table_Entry 
	//ADD CODE HERE
	CHECK_INVARIANT((variable_symbol_entry != NULL), "Symbol_Table_Entry should be non null");
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	//ADD CODE HERE
	CHECK_INVARIANT((variable_symbol_entry != NULL), "Symbol_Table_Entry should be non null");
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	node_data_type = dt;
	variable_symbol_entry->set_data_type(dt);
}

void Name_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"Name : "<<variable_symbol_entry->get_variable_name();
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	// use Ast_arity from ast.hh
	//ADD CODE HERE
	ast_num_child = zero_arity;
	constant = number;
	node_data_type = constant_data_type;
	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	//ADD CODE HERE
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	node_data_type = dt;
}

template <class DATA_TYPE>
bool Number_Ast<DATA_TYPE>::is_value_zero()
{
	//ADD CODE HERE
	return (constant == 0);
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"Num : "<<constant;
}

///////////////////////////////////////////////////////////////////////////////

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	//ADD CODE HERE
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)
{
	//ADD CODE HERE
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	// use get_data_type(), typeid()
	//ADD CODE HERE
	if (ast_num_child == binary_arity)
		if (lhs->get_data_type() == rhs->get_data_type())
			return true;

	if (ast_num_child == unary_arity)
		return true;

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Arithmetic statement data type not compatible", lineno);
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	// set arity and data type
	//ADD CODE HERE
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
	lhs = l;
	rhs = r;
	lineno = line;
}

void Plus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"            Arith: PLUS\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
	lhs = l;
	rhs = r;
	lineno = line;
}

void Minus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"            Arith: MINUS\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
	lhs = l;
	rhs = r;
	lineno = line;
}

void Mult_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"            Arith: MULT\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
	lhs = l;
	rhs = r;
	lineno = line;
}

void Divide_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"            Arith: DIV\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	//ADD CODE HERE
	ast_num_child = unary_arity;
	node_data_type = l->get_data_type();
	lhs = l;
	rhs = r;
	lineno = line;
}

void UMinus_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer<<"\n";
	file_buffer<<"            Arith: UMINUS\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")";
}

//////////////////////////////////////////////////////////////////////

Sequence_Ast::Sequence_Ast(int line)
{
	lineno = line;
}

Sequence_Ast::~Sequence_Ast()
{}

void Sequence_Ast::ast_push_back(Ast * ast)
{
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer)
{
}

template class Number_Ast<double>;
template class Number_Ast<int>;
// Ast::labelCounter = 1;
