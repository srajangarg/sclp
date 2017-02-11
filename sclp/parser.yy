%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union 
{
	pair<Data_Type, vector<string> * > * decl;
	vector<string> * string_vector;
	Symbol_Table * symbol_table;
	vector<Symbol_Table_Entry *> * symbol_entry_vector;
	Procedure * procedure;
	int integer_value;
	double double_value;
	std::string * string_value;
	Sequence_Ast * sequence_ast;
	Assignment_Ast * assignment_ast;
	Iteration_Statement_Ast * iteration_statement_ast;
	Selection_Statement_Ast * selection_statement_ast;
	Arithmetic_Expr_Ast * arithmetic_expr_ast;
	Relational_Expr_Ast * relational_expr_ast;
	Boolean_Expr_Ast * boolean_expr_ast;
	Ast * ast;
	//ADD CODE HERE
};

//ADD TOKENS HERE

%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME
%token RETURN INTEGER VOID FLOAT
%token ASSIGN
// %token LT LE GT GE EQ NE
// %token OR AND NOT
%token IF ELSE DO WHILE

%left CONDITIONAL
%left OR
%left AND
%left LT LE GT GE EQ NE
%left '+' '-'
%left '*' '/'
%right UMINUS NOT
%nonassoc '('

%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> variable_declaration_list
%type <symbol_entry_vector> variable_declaration
%type <decl> declaration
%type <string_vector> id_list
%type <sequence_ast> statement_list
%type <assignment_ast> assignment_statement
%type <ast> statement other_statement matched_statement
%type <selection_statement_ast> selection_statement matched_selection_statement unmatched_selection_statement
%type <iteration_statement_ast> while_statement
%type <ast> variable
%type <ast> constant
%type <arithmetic_expr_ast> arith_expression
%type <ast> relational_expression
%type <boolean_expr_ast> boolean_expression
%type <ast> expression_term

%start program

%%

program:
	declaration_list procedure_definition
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		program_object.set_procedure(current_procedure, get_line_number());
		program_object.global_list_in_proc_check();
	}
	}
;

declaration_list:
	procedure_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = new Symbol_Table();
		program_object.set_global_table(*global_table);
	}
	}
|
	variable_declaration_list
	procedure_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $1;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
|
	procedure_declaration
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		Symbol_Table * global_table = $2;

		CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

		program_object.set_global_table(*global_table);
	}
	}
;

procedure_declaration:
	VOID NAME '(' ')' ';'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT((*$2 == "main"), "Procedure name must be main in declaration");
	}
	}
;

procedure_definition:
	NAME '(' ')'
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");
		CHECK_INVARIANT((*$1 == "main"), "Procedure name must be main");

		string proc_name = *$1;

		current_procedure = new Procedure(void_data_type, proc_name, get_line_number());

		CHECK_INPUT ((program_object.variable_in_symbol_list_check(proc_name) == false),
			"Procedure name cannot be same as global variable", get_line_number());
	}
	}

	'{' optional_variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

		Symbol_Table * local_table = $6;

		if (local_table == NULL)
			local_table = new Symbol_Table();

		current_procedure->set_local_list(*local_table);
	}
	}

	statement_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
		Sequence_Ast* seq = $8;

		CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
		CHECK_INVARIANT((seq != NULL), "statement list cannot be null");

		current_procedure->set_sequence_ast(*seq);
	}
	}
;

optional_variable_declaration_list:
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = NULL;
	}
	}
|
	variable_declaration_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Declaration statement list cannot be null here");

		$$ = $1;
	}
	}
;

variable_declaration_list:
	variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Non-terminal declaration statement cannot be null");

		vector<Symbol_Table_Entry *> decl_stmt = *$1;

		// string decl_name = decl_stmt[0]->get_variable_name();
		// CHECK_INPUT ((program_object.variable_proc_name_check(decl_name) == false),
		// 		"Variable name cannot be same as the procedure name", get_line_number());

		// if(current_procedure != NULL)
		// {
		// 	CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
		// 		"Variable name cannot be same as procedure name", get_line_number());
		// }

		Symbol_Table * decl_list = new Symbol_Table();
		// decl_list->push_symbol(decl_stmt);


		for(int i = 0; i < decl_stmt.size(); i++)
		{
			string decl_name = decl_stmt[i]->get_variable_name();

			CHECK_INPUT((program_object.variable_proc_name_check(decl_name) == false),
				"Variable name cannot be same as the procedure name", get_line_number());
			
			if(current_procedure != NULL)
			{
				CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
					"Variable name cannot be same as procedure name", get_line_number());
			}

			CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false), 
					"Variable is declared twice", get_line_number());

			decl_list->push_symbol(decl_stmt[i]);
		}

		$$ = decl_list;
	}
	}
|
	variable_declaration_list variable_declaration
	{
	if (NOT_ONLY_PARSE)
	{
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		CHECK_INVARIANT(($2 != NULL), "The declaration statement cannot be null");
		CHECK_INVARIANT(($1 != NULL), "The declaration statement list cannot be null");

		vector<Symbol_Table_Entry *> decl_stmt = *$2;
		Symbol_Table * decl_list = $1;

		for(int i = 0; i < decl_stmt.size(); i++)
		{
			string decl_name = decl_stmt[i]->get_variable_name();
			
			CHECK_INPUT((program_object.variable_proc_name_check(decl_name) == false),
				"Procedure name cannot be same as the variable name", get_line_number());
			
			if(current_procedure != NULL)
			{
				CHECK_INPUT((current_procedure->get_proc_name() != decl_name),
					"Variable name cannot be same as procedure name", get_line_number());
			}

			CHECK_INPUT((decl_list->variable_in_symbol_list_check(decl_name) == false), 
					"Variable is declared twice", get_line_number());

			decl_list->push_symbol(decl_stmt[i]);
		}

		$$ = decl_list;
	}
	}
;

variable_declaration:
	declaration ';'
	{
	if (NOT_ONLY_PARSE)
	{
		pair<Data_Type, vector<string> * > * decl = $1;

		CHECK_INVARIANT((decl != NULL), "Declaration cannot be null");

		Data_Type type = decl->first;
		vector<string> name_list = *(decl->second);

		vector<Symbol_Table_Entry *> * decl_stmt = new vector<Symbol_Table_Entry *>;

		for(int i = 0; i < name_list.size(); i++)
		{
			Symbol_Table_Entry * decl_entry = new Symbol_Table_Entry(name_list[i], type, get_line_number());
			decl_stmt->push_back(decl_entry);
		}

		$$ = decl_stmt;
	}
	}
;

declaration:
	INTEGER id_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "id_list cannot be null");
		$$ = new pair<Data_Type, vector<string> * >(int_data_type, $2);
	}
	}
|
	FLOAT id_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "id_list cannot be null");
		$$ = new pair<Data_Type, vector<string> * >(double_data_type, $2);
	}
	}
;

id_list:
	NAME
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Name cannot be null");
		vector<string> * ids = new vector<string>;
		ids->push_back(*$1);
		$$ = ids;
	}
	}
|
	NAME ',' id_list
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($1 != NULL), "Name cannot be null");
		CHECK_INVARIANT(($3 != NULL), "id_list cannot be null");
		vector<string> * ids = $3;
		ids->push_back(*$1);
		$$ = ids;
	}
	}
;
statement_list:
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		Sequence_Ast * seq = new Sequence_Ast(get_line_number());
		$$ = seq;
	}
	}
|
	statement_list statement
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT(($1 != NULL), "Statement list cannot be null here");
		
		Sequence_Ast * seq = $1;
		Ast * ast_stmt = $2;
		
		seq->ast_push_back(ast_stmt);
		$$ = seq;
	}
	}
;

statement:
	unmatched_selection_statement
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	} 
	}
|
	matched_statement
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	} 
	}
;

matched_statement:
    matched_selection_statement
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	} 
	}
|
	other_statement
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	} 
	}
;

other_statement:
	assignment_statement
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	} 
	}
|
	while_statement
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $1;
	} 
	}
|
	'{' statement_list '}'
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $2;
	} 
	}
;

matched_selection_statement:
	IF '(' boolean_expression ')' matched_statement ELSE matched_statement
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($3 != NULL) && ($5 != NULL) && ($7 != NULL)),"boolean expression/statement block cannot be null");
		Selection_Statement_Ast * sel_stmt = new Selection_Statement_Ast($3, $5, $7, get_line_number());
		sel_stmt->check_ast();
		$$ = sel_stmt;
	}
	}
;

unmatched_selection_statement:
	IF '(' boolean_expression ')' statement
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($3 != NULL) && ($5 != NULL)),"boolean expression/statement block cannot be null");
		Sequence_Ast * empty_seq = new Sequence_Ast(get_line_number());
		Selection_Statement_Ast * sel_stmt = new Selection_Statement_Ast($3, $5, empty_seq, get_line_number());
		sel_stmt->check_ast();
		$$ = sel_stmt;
	}
	}
|
	IF '(' boolean_expression ')' matched_statement ELSE unmatched_selection_statement
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($3 != NULL) && ($5 != NULL) && ($7 != NULL)),"boolean expression/statement block cannot be null");
		Selection_Statement_Ast * sel_stmt = new Selection_Statement_Ast($3, $5, $7, get_line_number());
		sel_stmt->check_ast();
		$$ = sel_stmt;
	}
	}
;

// Make sure to call check_ast in assignment_statement and arith_expression
// Refer to error_display.hh for displaying semantic errors if any
assignment_statement:
	variable ASSIGN arith_expression ';'
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		Assignment_Ast * assignment_stmt = new Assignment_Ast($1, $3, get_line_number());
		assignment_stmt->check_ast();
		$$ = assignment_stmt;
	}
	}
;

while_statement:
	WHILE '(' boolean_expression ')' statement
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($3 != NULL) && ($5 != NULL)),"boolean expression/statement block cannot be null");
		Iteration_Statement_Ast * while_stmt = new Iteration_Statement_Ast($3, $5, get_line_number());
		while_stmt->check_ast();
		$$ = while_stmt;
	}
	}
;

arith_expression:
		//ADD RELEVANT CODE ALONG WITH GRAMMAR RULES HERE
		// SUPPORT binary +, -, *, / operations, unary -, and allow parenthesization
		// Connect the rules with the remaining rules given below
		arith_expression '+' arith_expression
		{
		if (NOT_ONLY_PARSE)
		{
			//ADD CODE HERE
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Plus_Ast($1, $3, get_line_number());
			$$->check_ast();
		} 
		}
|
		arith_expression '-' arith_expression
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Minus_Ast($1, $3, get_line_number());
			$$->check_ast();
		} 
		}
|
		arith_expression '*' arith_expression
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Mult_Ast($1, $3, get_line_number());
			$$->check_ast();
		} 
		}
|
		arith_expression '/' arith_expression
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
			$$ = new Divide_Ast($1, $3, get_line_number());
			$$->check_ast();
		} 
		}
|
		'(' arith_expression ')'
		{
		if (NOT_ONLY_PARSE)
		{
			//ADD CODE HERE
			$$ = $2;
			$$->check_ast();
		} 
		}
|
		expression_term
		{
		if (NOT_ONLY_PARSE)     
		{
			//ADD CODE HERE
			$$ = static_cast<Arithmetic_Expr_Ast*> ($1);
		}
		}
|
		'-' arith_expression %prec UMINUS
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($2 != NULL), "lhs/rhs cannot be null");
			$$ = new UMinus_Ast($2, NULL, get_line_number());
			$$->check_ast();
		}
		}
|
		boolean_expression '?' arith_expression ':' arith_expression %prec CONDITIONAL
		{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($1 != NULL), "boolean expression cannot be null");
			CHECK_INVARIANT((($3 != NULL) && ($5 != NULL)), "lhs/rhs cannot be null");
			$$ = new Conditional_Operator_Ast($1, $3, $5, get_line_number());
			$$->check_ast();
		}
		}
;

boolean_expression:
	boolean_expression OR boolean_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Boolean_Expr_Ast($1, boolean_or, $3, get_line_number());
		$$->check_ast();
	}
	}
|
	boolean_expression AND boolean_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Boolean_Expr_Ast($1, boolean_and, $3, get_line_number());
		$$->check_ast();
	}
	}
|
	NOT boolean_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT(($2 != NULL), "lhs/rhs cannot be null");
		$$ = new Boolean_Expr_Ast(NULL, boolean_not, $2, get_line_number());
		$$->check_ast();
	}
	}
	|
	'(' boolean_expression ')'
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = $2;
		$$->check_ast();
	}
	}
|
	relational_expression
	{
	if (NOT_ONLY_PARSE)
	{
		$$ = static_cast<Boolean_Expr_Ast*> ($1);
	}
	}
;

relational_expression:
	arith_expression LT arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Relational_Expr_Ast($1, less_than, $3, get_line_number());
		$$->check_ast();
	}
	}
|
	arith_expression LE arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Relational_Expr_Ast($1, less_equalto, $3, get_line_number());
		$$->check_ast();
	}
	}
|
	arith_expression GT arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Relational_Expr_Ast($1, greater_than, $3, get_line_number());
		$$->check_ast();
	}
	}
|
	arith_expression GE arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Relational_Expr_Ast($1, greater_equalto, $3, get_line_number());
		$$->check_ast();
	}
	}
|
	arith_expression EQ arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Relational_Expr_Ast($1, equalto, $3, get_line_number());
		$$->check_ast();
	}
	}
|
	arith_expression NE arith_expression
	{
	if (NOT_ONLY_PARSE)
	{
		CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");
		$$ = new Relational_Expr_Ast($1, not_equalto, $3, get_line_number());
		$$->check_ast();
	}
	}
;

expression_term:
	variable
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		$$ = $1;
	}
	}
|
	constant
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		$$ = $1;
	}
	}
;

variable:
	NAME
	{
	if (NOT_ONLY_PARSE)
	{

		Symbol_Table_Entry * var_table_entry;

		CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

		if (current_procedure->variable_in_symbol_list_check(*$1))
			 var_table_entry = &(current_procedure->get_symbol_table_entry(*$1));

		else if (program_object.variable_in_symbol_list_check(*$1))
			var_table_entry = &(program_object.get_symbol_table_entry(*$1));

		else
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared", get_line_number());

		$$ = new Name_Ast(*$1, *var_table_entry, get_line_number());

		delete $1;
	}
	}
;

constant:
	INTEGER_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		int num = $1;
		Ast * num_ast = new Number_Ast<int> (num, int_data_type, get_line_number());
		$$ = num_ast;
	}
	}
|
	DOUBLE_NUMBER
	{
	if (NOT_ONLY_PARSE)
	{
		//ADD CODE HERE
		double num = $1;
		Ast * num_ast = new Number_Ast<double> (num, double_data_type, get_line_number());
		$$ = num_ast;
	}
	}
;
