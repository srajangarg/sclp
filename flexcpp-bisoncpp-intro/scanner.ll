
%filenames="scanner"
%lex-source="scanner.cc"


%%

[0-9]+	{
		return Parser::NUM; 
	}

[-+*/]	{
		return matched()[0];
	}

\n	{     
		return matched()[0];
	}

[ \t]	{ }
.	{ 
		string error_message;
		error_message =  "Illegal character `" + matched();
		error_message += "' on line " + lineNr();
	        cerr << error_message;	
	        exit(1);
	}

%%
