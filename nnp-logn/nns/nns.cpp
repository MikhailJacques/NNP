// The following program inputs command line arguments such as the dimension d, 
// the number of nearest neighbors k, the error bound, and the file names containing 
// the query and data point. The program allocates storage for data points, one query 
// point, and results, consisting of the nearest neighbor indices and the distances. 
// The program inputs the data points and builds a kd-tree search structure for these
// points. Then it reads query points, and for each computes k approximate nearest 
// neighbors with error bound, and outputs the results.
//
// After compiling it can be run as follows.
// 
// nns [-d dim] [-max m] [-nn k] [-e eps] [-df data] [-qf query] [-rf result]
//
// where:
//
//		dim		dimension of the space (default = 2)
//		m		maximum number of data points (default = 10000)
//		k		number of nearest neighbors per query (default 1)
//		eps		the error bound (default = 0.0)
//		data	name of file containing data points
//		query	name of file containing query points
//		result	name of file containing the results
//		
//		Results are sent to the standard output and to the results file, if one is specified.
//
//		For example, to run this demo you can supply either of the two commands below:
//
//			nns -d 2
//			nns -df data.pts -qf query.pts -rf result.pts
//
//		The first command generates random data and a query point.
//		The second command uses the 3 existing files.

#include "ui.h"	

// Driver program
int main(int argc, char **argv)
{
	int					num_points = 0;			// Actual number of data points
	ANNpointArray		data_points;			// Data points
	ANNpoint			query_point;			// Query point
	ANNidxArray			near_neighbor_idx;		// Near neighbor indices
	ANNdistArray		near_neighbor_distances;// Near neighbor distances
	ANNkd_tree *		kd_tree_adt;			// ADT search structure

	UserInterface UI;

	// Read command-line arguments
	UI.getArgs(argc, argv);						

	// Allocate query point
	query_point = annAllocPt(UI.dimension);

	// Allocate data points
	data_points = annAllocPts(UI.max_points, UI.dimension);

	// Allocate near neighbor indices
	near_neighbor_idx = new ANNidx[UI.k];

	// Allocate near neighbor distances
	near_neighbor_distances = new ANNdist[UI.k];														

	// Echo data points
	cout << "Data Points: \n";

	if (UI.results_out != NULL)
		*(UI.results_out) << "Data points: \n";

	while (num_points < UI.max_points && UI.readPoint(*(UI.data_in), data_points[num_points])) 
	{
		UI.printPoint(cout, data_points[num_points], num_points);

		if (UI.results_out != NULL)
		{
			UI.printPoint(*(UI.results_out), data_points[num_points], num_points);
		}

		num_points++;
	}

	// Construct k-d tree abstract data type search structure
	// Params: data points, number of points, dimension of space
	kd_tree_adt = new ANNkd_tree(data_points, num_points, UI.dimension);						

	// Echo query point(s)
	cout << "\n\nQuery points: \n";

	if (UI.results_out != NULL)
		*(UI.results_out) << "\n\nQuery points: \n";

	// Read query points
	while (UI.readPoint(*(UI.query_in), query_point)) 
	{		
		UI.printPoint(cout, query_point, UI.dimension);

		if (UI.results_out != NULL)
		{
			UI.printPoint(*(UI.results_out), query_point, UI.dimension);
		}

		// Perform the search
		// Params: query point, number of near neighbors, nearest neighbors (returned), distance (returned), error bound
		kd_tree_adt->annkSearch(query_point, UI.k, near_neighbor_idx, near_neighbor_distances, UI.eps);

		UI.printSummary(cout, &near_neighbor_idx, &near_neighbor_distances);

		if (UI.results_out != NULL)
			UI.printSummary(*(UI.results_out), &near_neighbor_idx, &near_neighbor_distances);
	}

	// Perform house cleaning tasks
	delete kd_tree_adt;
    delete [] near_neighbor_idx;							
    delete [] near_neighbor_distances;

	annClose();

	cin.get();

	return EXIT_SUCCESS;
}