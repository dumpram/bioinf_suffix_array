##############################################################################
# Makefile for mono enviroment
#
# Author: Ivan Pavic
##############################################################################


all : run

sads.exe : SufixArrayBuilder.cs
	mono-csc -out:$@ $<

run : sads.exe
	mono sads.exe $(d) $(out) $(in) 

clean :
	rm sads.exe
