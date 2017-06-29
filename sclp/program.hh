
#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <string>
#include <list>
#include <map>
#include <set>
#include "procedure.hh"
#define GLOB_SPACE "   "

using namespace std;

class Program;

extern Program program_object;

class Program
{
    Symbol_Table global_symbol_table;
    map<string, Procedure *> procedures;

public:
    struct String_Ast_Compt {
        bool operator()(const String_Ast *lhs, const String_Ast *rhs) const
        {
            return lhs->get_s() < rhs->get_s();
        }
    };
    set<String_Ast *, String_Ast_Compt> string_asts;

    Program();
    ~Program();
    void delete_all();

    void add_procedure(Procedure *proc, int line);
    void set_global_table(Symbol_Table &new_global_table);

    Symbol_Table_Entry &get_symbol_table_entry(string variable);
    Procedure *get_procedure(string name);

    void print_sym();
    void print();

    bool variable_proc_name_check(string symbol); // not required
    bool variable_in_symbol_list_check(string variable);
    void global_list_in_proc_check(); // not required
    bool variable_in_proc_map_check(string var);
    bool check_called_procedure_defined();
    // compile
    void compile();
    void print_assembly();
};

#endif
