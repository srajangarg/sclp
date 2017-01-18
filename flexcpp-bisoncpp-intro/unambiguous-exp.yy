%scanner scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%start E
%token NUM
%term id

%%

E : E '+' T
  | T
  ;

T : T '*' F
  | F
  ;

F : id
  ;
