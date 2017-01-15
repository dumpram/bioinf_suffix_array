all : run

run : compile
	@java SuffixArrayConstruction $(d) $(in) $(out)

compile : SuffixArrayConstruction.class

SuffixArrayConstruction.class : SuffixArrayConstruction.java
	@javac SuffixArrayConstruction.java


clean:
	rm SuffixArrayConstruction.class
