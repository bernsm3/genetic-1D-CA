#include "automaton.h"

//to improve on genomes generated in previous runs, put them in here and use
//this initializer. If you have none, you can use the default initializer or
//this with an empty prevWinners.
static void addPrevWinners(GAPopulation& gp) {
	gp.DefaultInitializer(gp);

	vector<string> prevWinners(0);
	//push back whatever genomes you want to start with
	// prevWinners.push_back("");
	
	for (unsigned i=0; i<prevWinners.size(); i++)
		gp.replace( strToGenome(prevWinners[i]) , -2 );

}

int main( int argc , char *argv[] ) {
	if (argc != 2)
		cerr << "Give arguments pls" << endl;

	//create a blank genome
	GA1DBinaryStringGenome genome(GENOME_LENGTH,fitnessFunction);

	//and a GA instance on a population of that genome
	GASimpleGA ga(genome);
/*
	int popsize  = 30;
	int ngen     = 400;
	float pmut   = 0.001;
	float pcross = 0.9;


	ga.populationSize(popsize);
	ga.nGenerations(ngen);
	ga.pMutation(pmut);
	ga.pCrossover(pcross);
*/

	//set parameters
	ga.scoreFilename(argv[1]);
	ga.flushFrequency(100);
	ga.nBestGenomes(5);
	ga.nGenerations(4000);
	ga.pMutation(0.05);

	GAPopulation gp(genome,50);
	gp.initializer(addPrevWinners);
	ga.population(gp);

	ga.initialize();

	cout<<"--- BEFORE ---"<<endl;
	cout << ga.statistics() << endl;
	cout<<"best 5 genomes: "<<endl;
	cout<< ga.statistics().bestPopulation() << endl;

	ga.evolve();

	cout<<"--- AFTER ---"<<endl;
	cout << ga.statistics() << endl;
	cout<<"best 5 genomes: "<<endl;
	cout<< ga.statistics().bestPopulation() << endl;
	return 0;
}