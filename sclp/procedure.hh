
#ifndef PROCEDURE_HH
#define PROCEDURE_HH

#include <string>
#include <map>
#include <list>

#define PROC_SPACE "   "
#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;

class Procedure
{
	Data_Type return_type;
	string name;
	Symbol_Table local_symbol_table;
	Symbol_Table formal_symbol_table;
	Sequence_Ast * sequence_ast;
	bool is_defined;
	int lineno;

public:
	Procedure(Data_Type proc_return_type, string proc_name, int line);
	~Procedure();

	Sequence_Ast* get_seqast()
	{
		return sequence_ast;
	}

	vector<Data_Type> get_arguments_data_type();
	vector<Symbol_Table_Entry*> get_arguments_stes();

	int get_formal_symbol_table_size()
	{
		return formal_symbol_table.get_size();
	}
	
	string get_proc_name();
  	void set_sequence_ast(Sequence_Ast & sa);
	void set_local_list(Symbol_Table & new_list);
	void set_formal_list(Symbol_Table & new_list);
	void set_defined();
	bool check_defined();
	Data_Type get_return_type();
	Symbol_Table_Entry & get_local_symbol_table_entry(string variable_name);
	Symbol_Table_Entry & get_formal_symbol_table_entry(string variable_name);

	void print(ostream & file_buffer);
	void print_sym(ostream & file_buffer);

	bool variable_in_local_symbol_list_check(string variable);
	bool variable_in_formal_symbol_list_check(string variable);

	// compile
	void compile();
	void print_icode(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);

private:
	void print_prologue(ostream & file_buffer);
	void print_epilogue(ostream & file_buffer);
};

#endif
