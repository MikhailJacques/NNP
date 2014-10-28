
#ifndef UI_H
#define UI_H

#include <cstring>		// string manipulation
#include <string>		// string manipulation
#include <iostream>		// console I/O
#include <fstream>		// file I/O
#include <math.h>		// math functions
#include <ctime>		// seeding srand
#include <ANN/ANN.h>	// ANN declarations

using namespace std;	// make std:: accessible

class UserInterface
{
	private:
	
		void generateDataPointsFile();
		void generateQueryPointsFile();
		void generateResultsFile();

	public:

		UserInterface( int k = 1, int dimension = 2, double eps = 0, int max_points = 10000, 
			iostream * data_in = NULL, iostream * query_in = NULL,  iostream * results_out = NULL );

		~UserInterface();

		// Get command-line arguments
		void getArgs(int argc, char **argv);

		// Read point (false on EOF marker)
		bool readPoint(iostream & in, ANNpoint p);			

		// Print point
		void printPoint(ostream & out, ANNpoint p, int num_points);

		// Print summary
		void printSummary(ostream & out, ANNidxArray near_neighbor_idx[], ANNdistArray near_neighbor_distances[]);

		// Global parameters that are set in getArgs()
		int				k;				// Number of nearest neighbors
		int				dimension;		// Dimension
		double			eps;			// Error bound
		int				max_points;		// Maximum number of data points
		iostream *		data_in;		// Input for data points
		iostream *		query_in;		// Input for query points
		iostream *		results_out;	// Output for results

		// Declare data, query and result file I/O streams
		fstream data_stream, query_stream, results_stream;	
};

#endif