
%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

// We decare a union of the values that we want our grammar symbols to hold.
// This becomes a type called STYPE__ internally within the parser.

%union 
{
	int integer_value;
	std::string * string_value;
};

// We qualify the grammar symbols with the field names of the union. In this
// simple case, we have types associated with the terminals only. In a general
// case, we will need to associate types like this with non-terminals also. The
// syntax remains same.

%token <integer_value> NUM
%token <string_value> NAME


%%

List 	: List NUM              { cout << "Parser found a number " <<  $2 << endl; }
 	| List NAME             { cout << "Parser found a name "   << *$2 << endl; }
	| NUM	                { cout << "Parser found a number " <<  $1 << endl; }
	| NAME	                { cout << "Parser found a name "   << *$1 << endl; }
	;
