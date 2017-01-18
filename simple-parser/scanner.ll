

%filenames="scanner"
%lex-source="scanner.cc"


%%

[[:digit:]]+	{

		// We first get a pointer to STYPE__ (which is the type of the values of
		// grammar symbols of the grammar rules in the parser script. We declare 
		// this  type using the %union directive in the parser.yy file.

 		ParserBase::STYPE__ *val = getSval();

		// In order to pass numbers, we first convert the lexeme which is a string,
                // to an integer value using the atoi function.
                // We then set the value using appropriate field name of the union declared
                // in the parser.yy file. In this case, we want to pass an integer value 
		// which is stored in the field name integer_value.

                val->integer_value = atoi(matched().c_str());

		// Finally we return the token.

		return Parser::NUM; 
	}

[[:alpha:]_][[:alpha:][:digit:]_]* {


					// We get a pointer to the value stack.
	
					ParserBase::STYPE__ * val = getSval();

					// In this case we to want to process the lexeme. We
					// wish to pass the string value directly. So we choose
					// the field name string_value from the union declared
					// in the parser.yy file.

					val->string_value = new std::string(matched());

					// Finally we return the token.

					return Parser::NAME; 
				}


[ \t\n]		{ }
.		{ 
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();
		        cerr << error_message;	
	                exit(1);
		}

%%
