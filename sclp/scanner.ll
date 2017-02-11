%filenames="scanner"
%lex-source="scanner.cc"

dig [0-9]
char  [a-zA-Z_]
arith_operators [-+/\*]
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

void                        {
                                store_token_name("VOID");
                                return Parser::VOID;
                            }

if                          {
                                store_token_name("IF");
                                return Parser::IF;
                            }

else                        {
                                store_token_name("ELSE");
                                return Parser::ELSE;
                            }

while                       {
                                store_token_name("WHILE");
                                return Parser::WHILE;
                            }

do                          {
                                store_token_name("DO");
                                return Parser::DO;
                            }


"="                         {
                                store_token_name("ASSIGN_OP");
                                return Parser::ASSIGN;
                            }

\(  |
\)  |
\;  |
\{  |
\}  |
\,  |
\?  |
\:                          {
                                store_token_name("META CHAR");
                                return matched()[0];
                            }

{arith_operators}           {
                                store_token_name("ARITHOP");
                                return matched()[0];
                            }

"<"                         {
                                store_token_name("LT");
                                return Parser::LT;
                            }

"<="                        {
                                store_token_name("LE");
                                return Parser::LE;
                            }

">"                         {
                                store_token_name("GT");
                                return Parser::GT;
                            }

">="                        {
                                store_token_name("GE");
                                return Parser::GE;
                            }

"=="                        {
                                store_token_name("EQ");
                                return Parser::EQ;
                            }

"!="                        {
                                store_token_name("NE");
                                return Parser::NE;
                            }

"||"                        {
                                store_token_name("OR");
                                return Parser::OR;
                            }

"&&"                        {
                                store_token_name("AND");
                                return Parser::AND;
                            }

"!"                         {
                                store_token_name("NOT");
                                return Parser::NOT;
                            }


{dig}+                      {
                                ParserBase::STYPE__ *val = getSval();
                                val->integer_value = atoi(matched().c_str());
                                store_token_name("NUM");
                                return Parser::INTEGER_NUMBER;
                            }

{dig}+[eE][+-]?{dig}+             |
{dig}+\.{dig}*([eE][+-]?{dig}+)?  |
{dig}*\.{dig}+([eE][+-]?{dig}+)?  {
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
