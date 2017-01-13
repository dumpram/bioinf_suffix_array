##############################################################################
# Makefile for mono enviroment
#
# Author: Ivan Pavic
##############################################################################


all : run

sads.exe : SufixArrayBuilder.cs
	@mono-csc -out:$@ $<

run : sads.exe
	@mono sads.exe $(d) $(in) $(out) 

clean :
	rm sads.exe
