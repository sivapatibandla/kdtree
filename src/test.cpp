/*
 * test.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: Siva
 */



#if 0

void test (int num_points, int num_dimensions)
{
	if (!num_points) return;

	// initialize random seed
	srand (time(NULL));

	// generate random points
	KDTree<float> tree;
	vector<Point<float> > points;
	for (int i=0; i<num_points; i++)
	{
		vector<float> point;
		for (int j=0; j<num_dimensions; j++)
		{
			point.push_back(rand()%num_points);

		}
		Point<float> p (point, i);
		points.push_back(p);
	}

	// build kdtree
	//tree.build_kdtree (points);

	//cout << tree;
}

#endif
