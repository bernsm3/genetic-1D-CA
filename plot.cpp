#include <sstream>
#include <fstream>

#include "automaton.h"

class PrintableAutomaton : public Automaton {

public:

	PrintableAutomaton(const string& str): Automaton(*strToGenome(str)) {}

	void print() {
		for (unsigned i=0; i<maxSteps; i++) {
			for (unsigned j=0; j<length; j++)
				cout << board[j] << ' ';
			cout << endl;
			step();
		}
		for (unsigned j=0; j<length; j++)
			cout << board[j] << ' ';
		cout << endl;
	}

	//optional - set the initial board state
	void setBoard() {

		for (unsigned i=0; i<length/4; i++)
			board[i] = 1;
		for (unsigned i=length/4; i<length; i++)
			board[100] = 0;
	}
};

template <typename T>
string to_string(T val) {
    ostringstream os;
    os << val;
    return os.str();
}

//try to use the current time to randomize board (mtrand is deterministic)
void randomize() {
	time_t timer;
	for (unsigned i=0; i<time(&timer)%1000; i++)
		gaussRand();
}

int main( int argc , char *argv[] ) {
	if (argc != 2)
		cerr << "Give arguments pls" << endl;

	ifstream infile(argv[1]);
	string genome;
	getline(infile,genome);
	PrintableAutomaton a(to_string(genome));
	randomize();
	// a.setBoard();
	a.print();

	return 0;
}