.PHONY: runga plot

RADIUS=2

CAS_PER_GENOME=50

FITNESS_FILE=tmpr$(RADIUS).txt
OUTPUT_FILE=radius$(RADIUS).txt

#run the genetic algorithm!
all: runga
	./runga $(FITNESS_FILE) > $(OUTPUT_FILE); echo "\nfitness per generation:\n\n" >> $(OUTPUT_FILE); cat $(FITNESS_FILE) >> $(OUTPUT_FILE); rm $(FITNESS_FILE)

#look at the fitness graph of a run in progress
fitness:
	gnuplot -persist -e "plot '$(FITNESS_FILE)'"

runga:
	g++ runga.cpp -o runga "-DRADIUS=$(RADIUS)" "-DCAS_PER_GENOME=$(CAS_PER_GENOME)" -Wall -O3 -std=c++11 -lga -fopenmp

GENOME_FILE=tmpin.txt
DATA_FILE=plot.txt
PLOT_FILE=foo.png

#plot to file PLOT_FILE
plot: plotobj
	./plot $(GENOME_FILE) > $(DATA_FILE); gnuplot -e "infile = '$(DATA_FILE)'; outfile = '$(PLOT_FILE)' ; set term png size 400,1000 ; set out outfile" plot.p

#plot to temporary window
termplot: plotobj
	./plot $(GENOME_FILE) > $(DATA_FILE); gnuplot -persist -e "infile = '$(DATA_FILE)'" plot.p

plotobj:
	g++ plot.cpp -o plot "-DRADIUS=$(RADIUS)" "-DCAS_PER_GENOME=$(CAS_PER_GENOME)" -Wall -std=c++11 -lga -fopenmp

clean:
	rm runga plot