################################################################################################
#
#                simcalc : A Simple Calculator Using Flexc++ and Bisonc++ 
#                --------------------------------------------------------
#
#               About:
#               Implemented by Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
#               Khedker (http://www.cse.iitb.ac.in/~uday)  for the courses
#               cs302+cs306: Language  Processors (theory and lab)  at IIT
#               Bombay.
#    
#               Release  date Jan  15, 2013.  Copyrights reserved  by Uday
#               Khedker. This implemenation has been made available purely
#               for academic purposes without any warranty of any kind.
#
#               Explanations of the scanner parser interaction in simcalc 
#               can be found at http://www.cse.iitb.ac.in/~uday/cfglp
#    
################################################################################################

OBJ = scanner.o parser.o main.o


CPP = g++
BISONCPP = bisonc++
FLEXCPP = flexc++
TGT = simcalc
PARSER = parser.yy
SCANNER = scanner.ll

CFLAGS = -g 

# flexc++ and bisonc++ create the files scanner.h and parser.h which are required to be manually
# updated by adding some functions in them. If these files already exist, they are not over-written.
# They are created only when they do not exist.
# This manual intervention requires us to do the work in three steps. 
# (a) In the first step, the $(SCANNER) and $(PARSER) scripts are processed without any dependency 
#     on the scanner.h and parser.h file (because they do not exist and will be generated).
# (b) In the second step, the  scanner.h and parser.h files are manually updated. 
# (c) In the third step, the $(SCANNER) and $(PARSER) are processed with a dependency on the 
#     scanner.h and parser.h file. Every time they are modified, the $(SCANNER) and $(PARSER) 
#     should be processed again.
# This Makefile provides commands for all the three steps.  The target all checks whether scanner.h
# file exists and executes steps (a) and (b). If it does, the Makefile calls itself recursively 
# for executing step (c). Otherwise it executes steps (a) and (c).

all:  
	@if [ -f "scanner.h" ]  ; then  make $(TGT) ; else \
		$(FLEXCPP) $(SCANNER);  \
		$(BISONCPP) --verbose $(PARSER); \
		sed -i '/public:/a Parser(){d_scanner.setSval(&d_val__);}\n' parser.h; \
		sed -i '/public:/a void setSval(ParserBase::STYPE__ * val){dval=val;}\nParserBase::STYPE__  * getSval(){return dval;}\n' scanner.h; \
		sed -i '/private:/a ParserBase::STYPE__ * dval;\n' scanner.h; \
		sed -i '/#include "scannerbase.h"/a #include "parserbase.h"\n#include <string>\nusing namespace std;' scanner.h; \
		echo " " ; \
		echo "Make first pass executed. Re-executing make"; \
		echo " " ; \
		make; \
        fi	

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x $(OBJ) -o $(TGT)

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x $(CFLAGS) -c $<

scanner.cc scannerbase.h: scanner.ih scanner.h $(SCANNER) parserbase.h
	$(FLEXCPP) $(SCANNER)

parser.cc parserbase.h: parser.ih parser.h $(PARSER)
	$(BISONCPP) --verbose $(PARSER)

clean :
	rm -rf *.o $(TGT)  scanner.cc parser.cc parser.yy.output scannerbase.h parserbase.h
	rm -rf cscope.out tags a.out

cleanall: clean 
	rm -f scanner.h parser.h scanner.ih parser.ih
