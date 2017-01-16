# Bioinformatics project, FER 2017.

This is project for course: http://www.fer.unizg.hr/predmet/bio

This repository contains implementation of SA-DS suffix array construction algorithm based on this paper:
https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/ge-nong/Two%20Efficient%20Algorithms%20for%20Linear%20Time%20Suffix%20Array%20Construction.pdf

There are 5 implementations of algorithm:
- Java - Ivan Pavic
- C# - Ivan Pavic
- C - Tin Vlašić
- Python - Jasna Janković
- Python (alternative version) - Bruno Rosan

Tests for the algorithm are in directory tests. Tests are:
- test-1.txt - 100 B
- test-2.txt - 500 B
- test-3.txt - 1 kB
- test-4-txt - 5 kB
- test-5.txt - 10 kB
- test-6.txt - 50 kB
- test-7.txt - 100 kB
- test-8.txt - 500 kB
- test-9.txt - 1000 kB
- test-10.txt - 5 MB
- test-11.txt - 5.4 MB (fasta)

In every implementation input is text file, and lines which start with '>' are escaped.

Each directory has its own Makefile and readme.txt for specific instructions for running.
To test all implementations run script run_tests.sh

After running results of time and memory consumption are available for each language in
results-${language}.txt file.

Results can be plotted in matlab using plotter.m script. Profiling is done using cgmemtime tool:
https://github.com/gsauthof/cgmemtime
