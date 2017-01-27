%filenames="scanner"
%lex-source="scanner.cc"

dig [0-9]
char  [a-zA-Z_]

%%
//ADD YOUR CODE HERE

return                      {return Parser::RETURN;}

int                         {return Parser::INTEGER;}

float                       {return Parser::FLOAT;}

void                        {return Parser::VOID;}

=                           {return Parser::ASSIGN;}

\(  |
\)  |
\;  |
\{  |
\}  |
\+	|
\-	|
\*	|
\/	                        {return matched()[0];}

[-+]?{dig}+                 {
                                ParserBase::STYPE__ *val = getSval();
                                val->integer_value = atoi(matched().c_str());
                                return Parser::INTEGER_NUMBER;
                            }

[-+]?{dig}*\.{dig}+(e[+-]{dig}+)? {
                                ParserBase::STYPE__ *val = getSval();
                                val->double_value = atof(matched().c_str());
                                return Parser::DOUBLE_NUMBER;
                            }

{char}({char}|{dig})*       {
                                ParserBase::STYPE__ *val = getSval();
                                val->string_value = new std::string(matched());
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
