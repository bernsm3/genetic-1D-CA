## Solving Majority Classification with Evolved 1D Cellular Automata

This is an implementation of the work described in Chapter 11 of [Complexity: A Guided Tour](https://www.amazon.com/Complexity-Guided-Tour-Melanie-Mitchell/dp/0199798109) by Dr. Melanie Mitchell (and [this article](http://web.cecs.pdx.edu/~mm/evca-review.pdf)) training cellular automata to solve the majority classification problem - output a homogeneous lattice with parity equal to the majority in a random starting lattice. My goal is to investigate how the optimal algorithm changes as the size of the neighborhood is varied.

### Dependencies/Setup

[OpenMP](http://www.openmp.org/) is used to parallelize the fitness function. If you don't have it, just delete both instances of `-fopenmp` from the makefile, but you probably do.

Matthew Wall's [GALib](http://lancet.mit.edu/ga/) is used for the genetic algorithm backend. I was able to get it on Ubuntu 16.04 as `sudo apt install libga2`, but I still had to download the source from [here](http://lancet.mit.edu/ga/dist/) and run `make install` to copy over the appropriate header files.

### How to use - short version

Look at the parameters in `runga.cpp`. Change them if you wish - by default, it is 4000 generations of 50 genomes each, which takes 1-2 hours to run on my machine. Set `RADIUS` to whatever you want in the makefile - this will generate genomes that are 2^(2\*RADIUS+1) bits long. Then run `make`. To check the progress of a running simulation, do `make fitness`. When it's finished, you can look at the results, copy a genome you want to see to `tmpin.txt`, and run `make termplot` to view its automaton.

### How to use - long version

In the `Automaton` constructor in `automaton.h`, you can change the dimensions of the board - default is a periodic 201-cell lattice for 500 steps. You can also change the way the fitness function is calculated in `automaton.h/Automaton::majorityClassify()` - the actual fitness function is an average of `CAS_PER_GENOME` calls to that (default is 50). Currently, the fitness is the fraction of the final boardstate with the correct parity in [0,1], plus an extra speed bonus in [0,1] for a reaching a homogeneous board before time runs out, with a -2x penalty for a wrong answer. This is the most time-consuming step, so it is parallelized with openMP, and you can shorten the runtime while increasing the risk of pathological genomes by decreasing `CAS_PER_GENOME`.

Plot the response to a specified initial board state by modifying and calling `setBoard()` in `plot.cpp`. If you want to plot to the file `PLOT_FILE`, do `make plot` instead of `make termplot`.

You can run multiple instances of the GA at once, just make sure they have different radii or change the filenames between runs so they don't overwrite their own output. Same goes for plotting - the only parameter you always have to manually adjust is `RADIUS`.  

Note that the endianness of this code is opposite the usual convention, e.g. [Rule 110](https://en.wikipedia.org/wiki/Rule_110) would be "01110110".
