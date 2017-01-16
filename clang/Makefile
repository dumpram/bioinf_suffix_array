
all : run

run : SA-DS.out
	@./SA-DS.out $(d) $(in) $(out)

SA-DS.out : SA-DS.c d_critical.c d_critical.h
	@gcc -Wall -g SA-DS.c d_critical.c -I . -o $@
