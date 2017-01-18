%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%start E
%token NUM
%left '+'
%%
E : NUM    
	{ cout << "found an expression consisting of a number\n"; }
  | E '+' E  
	{ cout << "found a plus expression\n"; }
  ;
