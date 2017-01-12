all : run

run : compile
	java SuffixArrayConstruction $(d) $(in) $(out)

compile : SuffixArrayConstruction.class

SuffixArrayConstruction.class :
	javac SuffixArrayConstruction.java


clean: 
	rm SuffixArrayConstruction.class
