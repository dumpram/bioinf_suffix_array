# Bioinformatics project, FER 2017.

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
- test-2.txt - 1 kB
- test-3.txt - 10 kB
- test-4.txt - 100 kB
- test-5.txt - 1 MB
- test-6.txt - 5 MB (fasta)

In every implementation input is text file, and lines which start with '>' are escaped.

Each directory has its own Makefile and readme.txt for specific instructions for running.
To test all implementations run script run_tests.sh

After running results of time and memory consumption are available for each language in
results-${language}.txt file.

Results can be plotted in matlab using plotter.m script. Profiling is done using cgmemtime tool:
https://github.com/gsauthof/cgmemtime
