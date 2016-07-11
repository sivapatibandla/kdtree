/*
 * build_kdtree.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: Siva
 */

#include <iostream>
#include <fstream>
#include <string>
#include "kdtree.h"


using namespace std;

std::vector<std::string> split(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

int main() {

	cout << "!!!Application to build a KD Tree and save it to disk!!!" << endl; // prints !!!Hello World!!!

	ifstream file ( "/Users/Siva/Documents/workspace/kdtree/src/points.csv", ifstream::in );

	string line;

	// parse data points from input file
	int point_id = 0;
	vector <Point<float> > points;
	while (file.good())
	{
		getline(file, line);
		vector<std::string> words = split (line, ',');
		vector<float> coordinates;
		for (vector<string>::iterator it = words.begin(); it!=words.end(); it++)
		{
			coordinates.push_back(stof(*it));
		}
		Point<float> point (coordinates, point_id);
		points.push_back(point);

		cout << "line: " << line << " " << "point: " << point << endl;
		point_id++;
	}

	// build kd-tree
	KDTree<Point<float>, float> kdtree(points[0].get_coordinates().size());
	kdtree.add_points(points);

	cout << kdtree << endl;

	return 0;
}
