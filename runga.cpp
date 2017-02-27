#include "automaton.h"

//to improve on genomes generated in previous runs, put them in here and use
//this initializer. If you have none, you can use the default initializer or
//this with an empty prevWinners.
static void addPrevWinners(GAPopulation& gp) {
	gp.DefaultInitializer(gp);

	vector<string> prevWinners(0);
	//push back whatever genomes you want to start with
	// prevWinners.push_back("");
	prevWinners.push_back("00000010010101000101011001011011000011100000111010100101011111000000101000110100010011010110101001101001101000000001011101100110001011110110011000001010011001010111010110100000000011111011101000011000111010111000001001110100000010101000101110011011101000010111011101100111011011001110010010001111101010010101111111101101010001010100110110110010000011000110111110111010000010110101001110100010110100011111110101000000101010111110111001111001111111000001001010111110110111000010110000111101011011100110111110001101");

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
	ga.nGenerations(10000);
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