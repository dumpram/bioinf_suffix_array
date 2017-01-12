##############################################################################
# Makefile for mono enviroment
#
# Author: Ivan Pavic
##############################################################################


all : sads.exe

sads.exe : SufixArrayBuilder.cs
	mono-csc -out:$@ $<

run : all
	mono sads.exe $(d) $(out) $(in) 

clean :
	rm sads.exe
