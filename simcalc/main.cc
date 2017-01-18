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


#include "parser.h"

int main()
{
	Parser simcalc;

	simcalc.parse();
} 
