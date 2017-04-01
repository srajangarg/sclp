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

Program program_object;

Program::Program()
{
}

Program::~Program()
{
}

void Program::delete_all()
{

}

void Program::set_procedure(Procedure * proc, int line)
{
	procedure = proc;
	// line??
}

void Program::set_global_table(Symbol_Table & new_global_table)
{
	global_symbol_table = new_global_table;
}

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable)
{
	return global_symbol_table.get_symbol_table_entry(variable);
}

void Program::print_sym()
{
}

void Program::print()
{
}

bool Program::variable_proc_name_check(string symbol)
{	
	if (procedure == NULL)
		return false;
	return (procedure->get_proc_name() != symbol);
}

bool Program::variable_in_symbol_list_check(string variable)
{
	return global_symbol_table.variable_in_symbol_list_check(variable);
}

void Program::global_list_in_proc_check()
{
	global_symbol_table.global_list_in_proc_map_check();
}

void Program::variable_in_proc_map_check(string var)
{
	// global_symbol_table.global_list_in_proc_map_check();
}

void Program::compile()
{
	procedure->compile();
	print_assembly();
}

void Program::print_assembly()
{
	procedure->print_assembly(command_options.get_output_buffer());
}