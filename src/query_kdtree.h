/*
 * query_kdtree.h
 *
 *  Created on: Jul 16, 2016
 *      Author: Siva
 */

#ifndef QUERY_KDTREE_H_
#define QUERY_KDTREE_H_

#include "kdtree.h"

KDTree<Point<float>, float> *load_kdtree (const char* archive_file_path);

void compute_nearest_neighbors (KDTree<Point<float>, float> *kdtree, const char* query_points_file_path,
		const char* output_file_path);


KDTree<Point<float>, float> *load_kdtree (const char* archive_file_path);

#endif /* QUERY_KDTREE_H_ */
