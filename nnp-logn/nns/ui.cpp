
#include "ui.h"	

UserInterface::UserInterface( int k_d, int d, double e, int m_p, iostream * d_i, iostream * q_i,  iostream * r_o ) : 
	k(k_d), dimension(d), eps(e), max_points(m_p), data_in(d_i), query_in(q_i), results_out(r_o) { }

UserInterface::~UserInterface() 
{ 
	data_stream.close();
	query_stream.close();
	results_stream.close();
}

// Read point (false on EOF marker)
bool UserInterface::readPoint(iostream & in, ANNpoint p)			
{
	for (unsigned int i = 0; i < dimension; i++) 
	{
		if(!(in >> p[i])) 
			return false;
	}

	return true;
}

// Print point
void UserInterface::printPoint(ostream & out, ANNpoint p, int num_points)			
{
	out << "(" << p[0];

	for (int i = 1; i < dimension; i++) 
	{
		out << ", " << p[i];
	}

	out << ") ";

	if ((num_points + 1) % 8 == 0) 
		out << "\n";
}

// Print summary
void UserInterface::printSummary(ostream & out, ANNidxArray near_neighbor_idx[], ANNdistArray near_neighbor_distances[])			
{
	out << "\n\nNN:\tIndex\tDistance\n";

	for (int i = 0; i < k; i++) 
	{	
		// Unsquare the computed distance
		// *near_neighbor_distances[i] = sqrt(*near_neighbor_distances[i]);

		// Unsquare the computed distance and print out the summary
		out << i << "\t" << *near_neighbor_idx[i] << "\t" << sqrt(*near_neighbor_distances[i]) << "\n";
	}
}

// Get command-line arguments
void UserInterface::getArgs(int argc, char **argv)
{
	// Declare data, query and result file I/O streams
	// static ifstream data_stream, query_stream;
	// static ofstream results_stream;				

	// Check to see if there are no command line arguments
	if (argc <= 1) 
	{			
		// Alert the user and advise about proper usage of the program
		cerr << "Usage:\n\n" 
			<< "  nns [-d dim] [-max m] [-nn k] [-e eps] [-df data] [-qf query] [-rf result]\n\n"
			<< "  where:\n\n"
			<< "    dim		dimension of the space (default = 2)\n"
			<< "    m		maximum number of data points (default = 10000)\n"
			<< "    k		number of nearest neighbors per query (default 1)\n"
			<< "    eps		the error bound (default = 0.0)\n"
			<< "    data	name of file containing data points\n"
			<< "    query	name of file containing query points\n"
			<< "    result	name of file containing the results\n\n"
			<< " Results are sent to the standard output and to the results file, if specified.\n\n"
			<< " For example, to run this demo you can supply either of the two commands below:\n\n"
			<< "	nns -d 2\n"
			<< "	nns -df data.pts -qf query.pts -rf result.pts\n\n"
			<< " The first command generates random data and a query point.\n"
			<< " The second command uses the 3 existing files.\n";
			
		cin.get();

		exit(0);
	}

	int i = 1;

	// Read the command line arguments
	while (i < argc) 
	{						
		if (!strcmp(argv[i], "-d"))
		{			
			// Get the dimension
			dimension = atoi(argv[++i]);					
		}
		else if (!strcmp(argv[i], "-max"))
		{	
			// Get maximum number of points
			max_points = atoi(argv[++i]);			
		}
		else if (!strcmp(argv[i], "-nn"))
		{		
			// Get the number of nearest neighbors
			k = atoi(argv[++i]);					
		}
		else if (!strcmp(argv[i], "-e"))
		{		
			// Get the error bound
			sscanf(argv[++i], "%lf", &eps);			
		}
		else if (!strcmp(argv[i], "-df"))		
		{		
			// Get the data points file	
			data_stream.open(argv[++i], ios::in);	
			
			if (!data_stream) 
			{
				cerr << "Cannot open data file\n";
				exit(1);
			}

			data_in = &data_stream;			
		}
		else if (!strcmp(argv[i], "-qf"))		
		{		
			// Get the query point file	
			query_stream.open(argv[++i], ios::in);	

			if (!query_stream) 
			{
				cerr << "Cannot open query file\n";
				exit(1);
			}

			query_in = &query_stream;
		}
		else if (!strcmp(argv[i], "-rf"))		
		{		
			// Get the results file	
			results_stream.open(argv[++i], ios::out);

			if (!results_stream) 
			{
				cerr << "Cannot open results file\n";
				exit(1);
			}

			results_out = &results_stream;
		}
		else 
		{
			cerr << "Unrecognized option.\n";
			exit(1);
		}

		i++;
	}

	if (data_in == NULL ) 
	{
		// Since data file has not been specified
		// by the user, randomly generate data points
		generateDataPointsFile();

		// cerr << "-df and -qf options must be specified\n";
		// exit(1);
	}

	if (query_in == NULL) 
	{
		// Since query file has not been specified
		// by the user, randomly generate query points
		generateQueryPointsFile();
	}

	if (results_out == NULL) 
	{
		// Since query file has not been specified
		// by the user, randomly generate query points
		generateResultsFile();
	}
}

void UserInterface::generateDataPointsFile()
{
	// Open the data points file	
	data_stream.open("data.pts", ios::out);	
			
	if (!data_stream) 
	{
		cerr << "Cannot open data file\n";
		exit(1);
	}

	// Seed the random number generator
	srand(time(NULL) + 1); 

	int max_coordinate = sqrt(max_points);

	// Considering there are 10,000 spots for points, rill only 1 percent of the matrix with points.
	// Each randomly generated number represents a coordinate for a particular point.
	// A set of two consecutive randomly generated numbers represent x and y coordinates for a particular point.
	// An odd sequential number represents an x coordinate and an even sequential number represents y coordinate.
	// Take note that more than one point can have the same coordinates.
	for (int i = 1; i <= (max_points / 100); i++) 
	{
		// Pick a random number between 1 and (max_coordinate + 1), inclusive, and write it to a data file
		data_stream << 1 + (rand() % (max_coordinate + 1)) << " ";

		if (i % 20 == 0) 
		{
			data_stream << endl;
		}
	}

	data_stream.close();

	// Get the data points file	
	data_stream.open("data.pts", ios::in);	

	if (!data_stream) 
	{
		cerr << "Cannot open data file\n";
		exit(1);
	}

	data_in = &data_stream;
}

void UserInterface::generateQueryPointsFile()
{
	// Open the query point file	
	query_stream.open("query.pts", ios::out);

	if (!query_stream) 
	{
		cerr << "Cannot open query file\n";
		exit(1);
	}

	// Seed the random number generator
	srand(time(NULL)); 

	int max_coordinate = sqrt(max_points);

	for (int i = 1; i <= dimension; i++) 
	{
		// Pick random number between 1 and (max_coordinate + 1), inclusive, and write it to a query file
		query_stream << 1 + (rand() % (max_coordinate + 1)) << " ";
	}

	query_stream.close();

	// Get the query point file	
	query_stream.open("query.pts", ios::in);	

	if (!query_stream) 
	{
		cerr << "Cannot open query file\n";
		exit(1);
	}

	// Make this the query stream
	query_in = &query_stream;
}

void UserInterface::generateResultsFile()
{
	// Open the results file	
	results_stream.open("results.pts", ios::out);

	if (!results_stream) 
	{
		cerr << "Cannot open results file\n";
		exit(1);
	}

	// Current date/time based on current system
	time_t now = time(0);
   
   // Convert now to string form
   char * dt = ctime(&now);

   results_stream << "Local date and time: " << dt << endl;

	// Make this the results stream
	results_out = &results_stream;
}