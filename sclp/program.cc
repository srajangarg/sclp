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

void Program::add_procedure(Procedure *proc, int line)
{
    procedures[proc->get_proc_name()] = proc;
}

void Program::set_global_table(Symbol_Table &new_global_table)
{
    global_symbol_table = new_global_table;
    global_symbol_table.set_table_scope(global);
}

Symbol_Table_Entry &Program::get_symbol_table_entry(string variable)
{
    return global_symbol_table.get_symbol_table_entry(variable);
}

Procedure *Program::get_procedure(string name)
{
    if (procedures.find(name) != procedures.end())
        return procedures[name];

    CHECK_INPUT(false, "Procedure corresponding to the name is not found", -1);
    return NULL;
}

void Program::print_sym()
{
}

void Program::print()
{
    ostream &file_buffer = command_options.get_output_buffer();

    if (!global_symbol_table.is_empty() || !string_asts.empty()) {
        file_buffer << "\n\t"
                    << ".data"
                    << "\n";
        global_symbol_table.print_assembly(file_buffer);

        for (auto st_ast : string_asts) {
            file_buffer << st_ast->get_label() << ":\t";
            file_buffer << " .asciiz \t";
            file_buffer << st_ast->get_s() << "\n";
        }
    }
    for (auto &pp : procedures)
        pp.second->print_assembly(file_buffer);
}

bool Program::variable_proc_name_check(string symbol)
{
    return false;
}

bool Program::variable_in_symbol_list_check(string variable)
{
    return global_symbol_table.variable_in_symbol_list_check(variable);
}

void Program::global_list_in_proc_check()
{
    global_symbol_table.global_list_in_proc_map_check();
}

bool Program::variable_in_proc_map_check(string var)
{
    if (procedures.find(var) != procedures.end())
        return true;
    return false;
}

bool Program::check_called_procedure_defined()
{
    for (auto &pp : procedures)
        if (pp.second->check_called() && !pp.second->check_defined())
            return false;

    return true;
}

void Program::compile()
{
    Procedure *main_procedure = get_procedure("main");

    for (auto &pp : procedures) {
        machine_desc_object.clear_local_register_mappings();
        pp.second->compile();
    }
}
