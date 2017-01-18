%filenames="scanner"
%lex-source="scanner.cc"


digit [0-9]
operator [-+*/]


%%
{digit}+    { std::cout << "Found a number whose lexeme is '" << matched() << "'\n"; }
{operator}  { std::cout << "Found an operator whose lexeme is '" << matched()[0] << "'\n"; }
.           { std::cout << "Found an unrecoganized character '" << matched()[0] << "'\n"; }
