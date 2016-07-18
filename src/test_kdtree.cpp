/*
 * test.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: Siva
 */

#include <gtest/gtest.h>
#include "kdtree.h"
#include <boost/date_time.hpp>
#include <iostream>

void generate_random_points (int num_points, int num_dimensions, vector<Point<double> *> &points)
{
	if (!num_points) return;

	// initialize random seed
	srand (time(NULL));

	// generate random points
	for (int i=0; i<num_points; i++)
	{
		vector<double> coordinates;
		for (int j=0; j<num_dimensions; j++)
		{
			coordinates.push_back(rand()%num_points);

		}
		Point<double> *p = new Point<double> (coordinates, i);
		points.push_back(p);
	}
}

/*
 * Goal of this test is to check whether performance of KD-Tree search is
 * better than brute-force search.
 */
TEST (TestKDTree, perf_test)
{
	std::cout << "Launched performance test to compare the performance of "
			"KD-Tree with brute-force algorithm" << std::endl;

	int brute_force_runtime_usec = 0;
	int kdtree_runtime_usec = 0;
	int num_query_points = 10000;
	int num_points = 100000;
	int num_dimensions = 3;
	vector <Point<double> *> points;
	vector <Point<double> *> query_points;
	KDTree <Point<double>, double> kdtree(num_dimensions);

	// generate input data set
	generate_random_points (num_points, num_dimensions, points);
	generate_random_points (num_query_points, num_dimensions, query_points);

	// construct tree
	kdtree.add_points(points);

	// find nearest points through both the ways, and record statistics
	int index = 0;
	while (index < num_query_points)
	{
		NearestPoint<double> *nearest_point = NULL;
		NearestPoint<double> *nearest_point_brute_force = NULL;

		boost::posix_time::ptime time_start(boost::posix_time::microsec_clock::local_time());
		kdtree.find_nearest_point_recursive(query_points[index], &nearest_point);
		boost::posix_time::ptime time_end(boost::posix_time::microsec_clock::local_time());
		boost::posix_time::time_duration duration(time_end - time_start);
		kdtree_runtime_usec += duration.total_microseconds();

		boost::posix_time::ptime time_start_bf(boost::posix_time::microsec_clock::local_time());
		kdtree.find_nearest_point_brute_force(query_points[index], &nearest_point_brute_force);
		boost::posix_time::ptime time_end_bf(boost::posix_time::microsec_clock::local_time());
		boost::posix_time::time_duration duration_bf(time_end_bf - time_start_bf);
		brute_force_runtime_usec += duration_bf.total_microseconds();

		// checking correctness of kd-tree algorithm
		/*std::cout << "query no: " << index << " " << *query_points[index] << " kd-id: " << nearest_point->get_id() << " " << nearest_point->get_distance() << " bf id: " << nearest_point_brute_force->get_id()
				<< " " << nearest_point_brute_force->get_distance() << " nearest point " << *nearest_point << " bf " << *nearest_point_brute_force << std::endl;*/
		ASSERT_EQ (nearest_point->get_id(), nearest_point_brute_force->get_id());
		index++;
	}

	// checking performance of kd-tree algorithm
	kdtree_runtime_usec /= num_query_points; // average
	brute_force_runtime_usec /= num_query_points; // average
	std::cout << "kd-tree avg usec: " <<  kdtree_runtime_usec << " brute force avg usec: " << brute_force_runtime_usec << std::endl;
	ASSERT_LE (kdtree_runtime_usec, brute_force_runtime_usec);
}

int main (int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
