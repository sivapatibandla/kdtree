/*
 * build_kdtree_main.cpp
 *
 *  Created on: Jul 16, 2016
 *      Author: Siva
 */

#include <iostream>
#include <cassert>

#include "build_kdtree.h"
#include "kdtree.h"

using namespace std;

/*
 * Inputs:
 * 1. full path for file containing points
 * 2. full path for the location to save the generated KD-tree
 */
int main (int argc, char **argv)
{
	cout << "Launching KD-Tree build application..." << endl;

	cout << "Processing input arguments..." << endl;
	if (argc != 3)
	{
		cout << "Program expects 2 arguments but only " << argc-1 << " arguments provided" << endl;
		cout << "Please run the program as follows: ./build full_path_to_points_file full_path_to_archive_file" << endl;
		assert (0);
	}

	const char* points_file_path = argv[1];
	const char* archive_file_path = argv[2];

	cout << "Points file path:" << points_file_path << endl;
	cout << "Archive file path:" << archive_file_path << endl;

	// build KD-tree
	cout << "Building KD-Tree..." << endl;
	KDTree<Point<float>, float> *kdtree = build_kdtree (points_file_path);

	// save KD-tree
	if (kdtree)
	{
		cout << "Saving KD-Tree..." << endl;
		save_kdtree (kdtree, archive_file_path);
	}

	cout << "Exiting build KD-Tree application"  << endl;
}

