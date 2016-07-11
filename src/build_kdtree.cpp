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
#include <cassert>


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

vector <Point<float> *> read_points_from_file (const char *file_name, int &num_dimensions)
{
	ifstream file ( file_name, ifstream::in );

	int point_id = 0;
	vector <Point<float> *> points;

	// Extract no. of dimensions
	string line;
	vector<std::string> words;
	if (file.good())
	{
		getline(file, line);
		words = split (line, ',');
		num_dimensions = words.size();
		file.seekg (0, ios::beg);
	}

	// extract points
	while (file.good())
	{
		getline(file, line);
		words = split (line, ',');
		//assert (words.size() == num_dimensions);
		// ignore lines which are not properly formatted. TODO: wrap this in a try & catch block
		if (words.size() != num_dimensions) {point_id++;continue;}

		//cout << "line " << line << endl;
		// extract coordinates
		vector<float> coordinates;
		for (vector<string>::iterator it = words.begin(); it!=words.end(); it++)
		{
			coordinates.push_back(stof(*it));
		}

		Point<float> *point = NULL;
		try
		{
		  point = new Point<float>(coordinates, point_id);
		  points.push_back(point);
		}
		catch(std::bad_alloc& exc)
		{
		  cout << "bad alloc" << endl;
		}

		point_id++;
	}
	file.close();

	return points;
}

int main() {

	cout << "!!!Application to build a KD Tree and save it to disk!!!" << endl; // prints !!!Hello World!!!

	int num_dimensions = 0;
	const char *file_name = "/Users/Siva/Documents/workspace/kdtree/src/sample_data.csv";
	//const char *file_name = "/Users/Siva/Documents/workspace/kdtree/src/points.csv";
	vector <Point<float> *> points = read_points_from_file (file_name, num_dimensions);

	// build kd-tree
	if (points.size() > 0)
	{
		KDTree<Point<float>, float> kdtree(num_dimensions);
		kdtree.add_points(points);
		cout << kdtree << endl;
	}

	return 0;
}
