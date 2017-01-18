
OBJ = scanner.o main.o

CPP = g++
FLEXCPP = flexc++
TGT = num
SCANNER = num.ll

CFLAGS = -g 

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x $(OBJ) -o $(TGT)

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x $(CFLAGS) -c $<

scanner.cc scannerbase.h: $(SCANNER) 
	$(FLEXCPP) $(SCANNER)

clean :
	rm -rf *.o $(TGT)  scanner.cc parser.cc *.yy.output scannerbase.h scanner.h scanner.ih
	rm -rf cscope.out tags a.out
