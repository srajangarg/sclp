%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%start E
%token NUM
%left '+' '-'
%left '*' '/'
%%
E : NUM    
	{ cout << "found an expression consisting of a number\n"; }
  | E '+' E  
	{ cout << "found a plus expression\n"; }
  | E '*' E  
	{ cout << "found a mult expression\n"; }
  | E '-' E  
	{ cout << "found a sub expression\n"; }
  | E '/' E  
	{ cout << "found a div expression\n"; }
  ;
