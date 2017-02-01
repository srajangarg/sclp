%filenames="scanner"
%lex-source="scanner.cc"

dig [0-9]
char  [a-zA-Z_]
operators [-+/\*]

%%
//ADD YOUR CODE HERE

float                       {
                                store_token_name("FLOAT");
                                return Parser::FLOAT;
                            }

int                         {
                                store_token_name("INTEGER");
                                return Parser::INTEGER;
                            }

return                      {return Parser::RETURN;}


void                        {
                                store_token_name("VOID");
                                return Parser::VOID;
                            }

=                           {
                                store_token_name("ASSIGN_OP");
                                return Parser::ASSIGN;
                            }

\(  |
\)  |
\;  |
\{  |
\}                          {
                                store_token_name("META CHAR");
                                return matched()[0];
                            }

{operators}                 {
                                store_token_name("ARITHOP");
                                return matched()[0];
                            }

{dig}+                      {
                                ParserBase::STYPE__ *val = getSval();
                                val->integer_value = atoi(matched().c_str());
                                store_token_name("NUM");
                                return Parser::INTEGER_NUMBER;
                            }

({dig}*\.{dig}+([eE][+-]{dig}+)?|{dig}+[eE][+-]{dig}+) {
                                ParserBase::STYPE__ *val = getSval();
                                val->double_value = atof(matched().c_str());
                                store_token_name("FNUM");
                                return Parser::DOUBLE_NUMBER;
                            }

{char}({char}|{dig})*       {
                                ParserBase::STYPE__ *val = getSval();
                                val->string_value = new std::string(matched());
                                store_token_name("NAME");
                                return Parser::NAME;
                            }


\n              |
";;".*          |
[ \t]*";;".*    |
[ \t]*"//".*    |
[ \t]                       {
                                if (command_options.is_show_tokens_selected())
                                ignore_token();
                            }

.                           { 
                                string error_message;
                                error_message =  "Illegal character `" + matched();
                                error_message += "' on line " + lineNr();
                                
                                CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, error_message, lineNr());
                            }
