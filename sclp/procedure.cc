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

Procedure::Procedure(Data_Type proc_return_type, string proc_name, int line)
{
    return_type = proc_return_type;
    name = proc_name;
    lineno = line;
    is_defined = false;
}

Procedure::~Procedure()
{
}

string Procedure::get_proc_name()
{
    return name;
}

void Procedure::set_sequence_ast(Sequence_Ast &sa)
{
    sequence_ast = &sa;
}

void Procedure::set_local_list(Symbol_Table &new_list)
{
    local_symbol_table = new_list;
    local_symbol_table.set_table_scope(local);
    local_symbol_table.assign_offsets();
}

void Procedure::set_formal_list(Symbol_Table &new_list)
{
    formal_symbol_table = new_list;
    formal_symbol_table.set_table_scope(formal);
    formal_symbol_table.set_start_offset_of_first_symbol(8);
    formal_symbol_table.assign_offsets();
}

vector<Data_Type> Procedure::get_arguments_data_type()
{
    auto stes = formal_symbol_table.get_table();
    vector<Data_Type> vv;

    for (auto it = stes.begin(); it != stes.end(); it++)
        vv.push_back((*it)->get_data_type());
    return vv;
}

vector<Symbol_Table_Entry *> Procedure::get_arguments_stes()
{
    // auto copy_table(formal_symbol_table);
    auto stes = formal_symbol_table.get_table();
    vector<Symbol_Table_Entry *> vv;
    int off = -formal_symbol_table.get_size() + 8;

    for (auto it = stes.begin(); it != stes.end(); it++) {
        Symbol_Table_Entry *ste = *it;
        string name = ste->get_variable_name();
        off += ste->get_width();

        Symbol_Table_Entry *newste = new Symbol_Table_Entry(name, ste->get_data_type(),
                                                            ste->get_lineno(), sp_ref);
        newste->set_start_offset(off);
        newste->set_symbol_scope(formal);
        vv.push_back(newste);
    }
    return vv;
}

void Procedure::set_defined()
{
    is_defined = true;
}

bool Procedure::check_defined()
{
    return is_defined;
}

Data_Type Procedure::get_return_type()
{
    return return_type;
}

Symbol_Table_Entry &Procedure::get_local_symbol_table_entry(string variable_name)
{

    return local_symbol_table.get_symbol_table_entry(variable_name);
}

Symbol_Table_Entry &Procedure::get_formal_symbol_table_entry(string variable_name)
{

    return formal_symbol_table.get_symbol_table_entry(variable_name);
}

void Procedure::print(ostream &file_buffer)
{
    print_assembly(file_buffer);
}

void Procedure::print_sym(ostream &file_buffer)
{
    print_assembly(file_buffer);
}

bool Procedure::variable_in_local_symbol_list_check(string variable)
{
    return local_symbol_table.variable_in_symbol_list_check(variable);
}

bool Procedure::variable_in_formal_symbol_list_check(string variable)
{
    return formal_symbol_table.variable_in_symbol_list_check(variable);
}

// compile
void Procedure::compile()
{
    sequence_ast->compile();
}

void Procedure::print_icode(ostream &file_buffer)
{
    sequence_ast->print_icode(file_buffer);
}
void Procedure::print_assembly(ostream &file_buffer)
{
    print_prologue(file_buffer);
    sequence_ast->print_assembly(file_buffer);
    print_epilogue(file_buffer);
}

void Procedure::print_prologue(ostream &file_buffer)
{
    file_buffer << "\n\t.text \t\t\t# The .text assembler directive indicates";
    file_buffer << "\n\t.globl " << name
                << "\t\t# The following is the code (as oppose to data)\n";
    file_buffer << name << ":\t\t\t\t# .globl makes main know to the\n";
    file_buffer << "\t\t\t\t# outside of the program.\n";
    file_buffer << "# Prologue begins\n";
    file_buffer << "\tsw $ra, 0($sp)		# Save the return address\n";
    file_buffer << "\tsw $fp, -4($sp)		# Save the frame pointer\n";
    file_buffer << "\tsub $fp, $sp, 8		# Update the frame pointer\n\n";
    file_buffer << "\tsub $sp, $sp, " << 8 - local_symbol_table.get_size()
                << "\t\t# Make space for the locals\n";
    file_buffer << "# Prologue ends\n\n";
}

void Procedure::print_epilogue(ostream &file_buffer)
{
    file_buffer << "\n# Epilogue Begins\n";
    file_buffer << "epilogue_" << name << ":\n";
    file_buffer << "\tadd $sp, $sp, " << 8 - local_symbol_table.get_size() << "\n";
    file_buffer << "\tlw $fp, -4($sp)\n";
    file_buffer << "\tlw $ra, 0($sp)\n";
    file_buffer << "\tjr        $31		# Jump back to the called procedure\n";
    file_buffer << "# Epilogue Ends\n\n";
}