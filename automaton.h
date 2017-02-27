//important constants, Automaton class, and fitness function

#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <queue>
#include <string>

#include <ga/ga.h>

#define NLENGTH (2*RADIUS +1)
#define GENOME_LENGTH (1 << (2*RADIUS +1))

using namespace std;

//random number generators - static because they're deterministic
mt19937* randGen() {
	static mt19937 gen;
	return &gen;
}

float gaussRand() { //misleadingly titled ... just returns a rand in [0,1}
	static uniform_real_distribution<> dist(0,1);
	return dist(*randGen());
}

//1D CA with periodic boundaries, majority classification, update rule in genome
class Automaton {

public:
	Automaton(const GA1DBinaryStringGenome& _genome, unsigned _length, unsigned _maxSteps): 
	genome(_genome), length(_length), maxSteps(_maxSteps), steps(0) {
		initialize();
	}

	Automaton(const GA1DBinaryStringGenome& _genome):
	genome(_genome), length(201), maxSteps(500), steps(0) {
		initialize();
	}

	void step() {
		steps++;
		prevBoard = board;
		unsigned nsum = 0;
		//initialize the neighborhood values
		for (int i=0; i<NLENGTH; i++) {
			nsum += (prevBoard[i] << i);
		}
		//step through the board
		for (unsigned i=0; i<length; i++) {
			//nsum (a number encoding the values in the neighborhood) 
			//chooses the update rule to apply from genome
			board[(i+RADIUS)%length] = genome.gene(nsum);
			//as i increments, move the neighborhood 1 to the right
			nsum  = nsum >> 1;
			nsum ^= (prevBoard[(i+NLENGTH)%length] << (NLENGTH-1));
		}
	}

	const bool homogeneous() const {
		bool testval = board[0];
		for (unsigned i=0; i<length; i++)
			if (board[i] != testval) return false;
		return true;
	}

	//Returned fitness values
	//Perfect (homogeneous) classification: 1 + speed bonus in [0,1]
	//Imperfect classification: board fraction correct in [0,1]
	//Wrong answer: -2x the above
	const float majorityClassify() {
		for (unsigned i=0; i<maxSteps; i++) {
			step();
			if (homogeneous()) {
				float ans = (2*(board[0] == initMajority) - 1)*(2 - (float)i/(float)maxSteps);
				if (ans < 0)
					ans *= 2;
				return ans;
			}
		}
		float finalFrac = frac1s();
		float ans = (2*(initMajority == round(finalFrac)) - 1)*finalFrac;
		if (ans < 0)
			ans *= 2;
		return ans;	
	}

protected:

	float frac1s() const {
		float sum = 0;
		for (unsigned i=0; i<length; i++)
			sum += board[i];
		return sum/(float)length;
	}

	void initialize() {
		//fill board with random bools and count the 1's
		//initial distribution has random distribution param. in [0,1)
    	bernoulli_distribution dist(gaussRand());
    	board.reserve(length);
    	for (unsigned i=0; i<length; i++) 
    		board.push_back(dist(*randGen()));
    	initMajority = (frac1s() >= 0.5);
    	prevBoard = vector<bool>(length);
	}

	const GA1DBinaryStringGenome genome;
	const unsigned length;
	const unsigned maxSteps;
	bool initMajority;
	unsigned steps;
	vector<bool> board;
	vector<bool> prevBoard;

};

//returns avg success rate of majorityClassify() across CAS_PER_GENOME automata w/ g_in
float fitnessFunction(GAGenome& g_in) {
	GA1DBinaryStringGenome & g = (GA1DBinaryStringGenome &)g_in;
	float successes = 0;

	//openMP distributes this across all cores- a 4x speedup on my machine.
	#pragma omp parallel for reduction(+:successes)

	for (unsigned i=0; i<CAS_PER_GENOME; i++) {
		Automaton* a = new Automaton(g);
		successes += a->majorityClassify();
		delete a;
	}

	if (successes >= 0)
		return (float)successes / (float)CAS_PER_GENOME;
	return 0;
}

// return genome from a string of the form "10101110..."
GA1DBinaryStringGenome* strToGenome(const string& str) {
	GA1DBinaryStringGenome* g = new GA1DBinaryStringGenome(GENOME_LENGTH,fitnessFunction);
	for (unsigned i=0; i<str.length(); i++)
		g->gene(i, str[i]-'0');
	return g;
}
