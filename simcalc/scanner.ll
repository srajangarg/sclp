
/*********************************************************************************************

                      simcalc : A Simple Calculator Using Flexc++ and Bisonc++ 
                      --------------------------------------------------------

           About:

           Implemented by Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors (theory and lab)  at IIT
           Bombay.

           Release  date Jan  15, 2013.  Copyrights reserved  by Uday
           Khedker. This implemenation has been made available purely
           for academic purposes without any warranty of any kind.

           Explanations of the scanner parser interaction in simcalc can 
           be found at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/



%filenames="scanner"
%lex-source="scanner.cc"


%%

[0-9]+	{
 		ParserBase::STYPE__ *val = getSval();
                *val = atoi(matched().c_str());
		return Parser::NUM; 
	}

[+*]	{
		return matched()[0];
	}

\n	{     
		return matched()[0];
	}

[ \t]		{ }
.		{ 
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();
		        cerr << error_message;	
	                exit(1);
		}

%%
