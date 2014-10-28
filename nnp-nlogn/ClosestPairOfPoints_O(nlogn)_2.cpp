// Divide and conquer program to find the shortest distance 
// between two points in a given set of points in O(nlogn) time.

#include<cmath>
#include<vector>
#include<iostream>

using namespace std;

// Global variable
vector< pair<double, double> > coordinates;

// Function prototypes
double ComputeEuclidianDistance(pair<int, int>, pair<int, int>);
pair< pair<double, double>, pair<double, double> > Find(int, int);

int main()
{
	int n, x, y;
	pair< pair<double, double>, pair<double, double> > result;

	do 
	{
		cout << "Specify the number of points you wish to enter in the range [2, 10]: ";
		cin >> n;
		
		if (n < 2 || n > 10)
			cout << "Invalid entry. Please try again ..." << endl;

	} while ( (n < 1 || n > 10) );


	cout << "Populate 2D plane with " << n << " points (valid range [1, 100])\n" << endl;

	for (int i = 0; i < n; i++)
	{
	
		do 
		{
			cout << "Enter coordinate X for the point: ";
			cin >> x;
		
			if (x < 1 || x > 100)
			{
				cout << "Invalid entry. Please try again ..." << endl;
				continue;
			}

		} while (x < 1 || x > 100);

		do 
		{
			cout << "Enter coordinate Y for the point: ";
			cin >> y;
		
			if (y < 1 || y > 100)
			{
				cout << "Invalid entry. Please try again ..." << endl;
				continue;
			}

		} while (y < 1 || y > 100);

		cout << endl;

		coordinates.push_back(make_pair(x, y));
	}
	
	result = Find(0, n - 1);
	
	cout << "The points with the following coordinates are nearest to each other." << endl;

	cout << "The distance between point (" 
		<<  result.first.first << ", " << result.first.second << ") and point (" 
		<<  result.second.first << ", " << result.second.second << ") is: "
		<< ComputeEuclidianDistance(result.first, result.second);
	
	cin.get();
	cin.get();

  return 0;
}

pair< pair<double, double>, pair<double, double> >Find(int L, int R)
{
	double Min = DBL_MAX;
	pair< pair<double, double>, pair<double, double> >result;
	
	if (R - L <= 4)
	{
		for (int i = L + 1; i <= R;i++)
		{
			for (int j = L; j < i;j++)
			{
				if (ComputeEuclidianDistance(coordinates[i], coordinates[j]) < Min)
				{
					Min = ComputeEuclidianDistance(coordinates[i], coordinates[j]);

					result = make_pair(coordinates[i], coordinates[j]);
				}
			}
		}
		
		return result;
	}
	else
	{
		double LM, RM;
		int mid = (int) floor((R + L) / 2);
		pair< pair<double, double>, pair<double, double> > Ch, Ra;
		
		Ch = Find(L, mid);
		Ra = Find(mid + 1, R);
		
		LM = ComputeEuclidianDistance(Ch.first, Ch.second);
		RM = ComputeEuclidianDistance(Ra.first, Ra.second);
		
		for (int i = L; i <= mid; i++)
		{
			for (int j = mid + 1; j <= R; j++)
			{
				if (ComputeEuclidianDistance(coordinates[i], coordinates[j]) < Min)
				{
					Min = ComputeEuclidianDistance(coordinates[i], coordinates[j]);
					result = make_pair(coordinates[i], coordinates[j]);
				}
			}
		}
		
		if (LM < Min || RM < Min)
		{
			if (RM < LM)
				return Ra;
			
			return Ch;
		}
		
		return result;
	}
}

double ComputeEuclidianDistance(pair<int, int> X, pair<int, int> Y)
{
	return sqrt( pow(X.first - Y.first, 2) + pow(X.second - Y.second, 2));
}