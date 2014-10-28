// A divide and conquer program in to find the shortest distance 
// between two points in a given set of points in O(nlogn) time.

#include <math.h>
#include <iostream>

using namespace std;

struct Point
{
	int x, y;
};
 
// Sorts array of points according to X coordinate
int compareX(const void * a, const void * b)
{
    Point * p1 = (Point *) a,  *p2 = (Point *) b;
    return (p1->x - p2->x);
}

// Sorts array of points according to Y coordinate
int compareY(const void * a, const void* b)
{
    Point * p1 = (Point *) a,   * p2 = (Point *) b;
    return (p1->y - p2->y);
}
 
// Computes (Euclidean) distance between two points
// dist((x, y), (a, b)) = [(x - a)^2 + (y - b)^2]^(1/2)
double dist(Point p1, Point p2)
{
    return sqrt( (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
 
// A Brute Force method to return the smallest distance 
// between two points in array P[] of size n
double bruteForce(Point P[], int n)
{
    double min = DBL_MAX;

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);

    return min;
}
 
// Finds minimum of two double values
double min(double x, double y)
{
    return (x < y) ? x : y;
}
 
 
// Finds the distance beween the closest points of strip of given size. 
// All points in strip[] are sorted according to y coordinate. 
// They all have an upper bound on minimum distance as d.
// Take note that from first galnce this method appears to be a O(n^2) method, 
// but it's a O(n) method as the inner loop runs at most 6 times
double stripClosest(Point strip[], int size, double d)
{
    double min = d;  // Initialize the minimum distance as d
 
    // Pick all points one by one and try the next points untill 
	// the difference netween y coordinates is smaller than d.
    // This is a proven fact that this loop runs at most 6 times
    for (int i = 0; i < size; ++i)
	{
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
		{
            if (dist(strip[i], strip[j]) < min)
			{
                min = dist(strip[i], strip[j]);
			}
		}
	}
 
    return min;
}
 
// A recursive function to find the smallest distance. 
// The array Px contains all points sorted according to x coordinates 
// and Py contains all points sorted according to y coordinates
double closestUtil(Point Px[], Point Py[], int n)
{
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(Px, n);
 
    // Find the middle point
    int mid = n/2;

    Point midPoint = Px[mid];
 
    // Divide points in sorted array y around the vertical line.
    // Assumption: All x coordinates are distinct.
    Point * Pyl = new Point[mid + 1];		// y sorted points on the left of the vertical line
    Point * Pyr = new Point[n - mid - 1];	// y sorted points on the right of the vertical line
    
	int li = 0, ri = 0;						// set indices of left and right subarrays
    
	for (int i = 0; i < n; i++)
    {
		if (Py[i].x <= midPoint.x)
			Pyl[li++] = Py[i];
		else
			Pyr[ri++] = Py[i];
    }
 
    // Consider the vertical line passing through the middle point.
    // Calculate the smallest distance dl on left of middle point and dr on right side
    double dl = closestUtil(Px, Pyl, mid);
    double dr = closestUtil(Px + mid, Pyr, n - mid);
 
    // Find the smaller of two distances
    double d = min(dl, dr);
 
    // Build an array strip[] that contains points close (closer than d)
    // to the line passing through the middle point
    Point * strip = new Point[n];

    int j = 0;

    for (int i = 0; i < n; i++)
	{
        if (abs(Py[i].x - midPoint.x) < d)
		{
            strip[j++] = Py[i];
		}
	}
 
    // Find the closest points in strip.  
	// Return the minimum of d and closest distance is strip[]
    return min(d, stripClosest(strip, j, d) );
}
 
// The main function that finds the shortest distance
// This method mainly uses closestUtil()
double closest(Point P[], int n)
{
    Point * Px = new Point[n];
    Point * Py = new Point[n];

    for (int i = 0; i < n; i++)
    {
        Px[i] = P[i];
        Py[i] = P[i];
    }
 
    qsort(Px, n, sizeof(Point), compareX);
    qsort(Py, n, sizeof(Point), compareY);
 
    // Use recursive function closestUtil() to find the shortest distance
    return closestUtil(Px, Py, n);
}
 
// Driver program
int main()
{
    Point P[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 5}};
    int n = sizeof(P) / sizeof(P[0]);

    cout << "The shortest distance between two points is " << closest(P, n) << endl;

	cin.get();

    return 0;
}