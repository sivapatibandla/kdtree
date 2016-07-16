/*
 * build_kdtree.h
 *
 *  Created on: Jul 16, 2016
 *      Author: Siva
 */

#ifndef BUILD_KDTREE_H_
#define BUILD_KDTREE_H_

#include "kdtree.h"

KDTree<Point<float>, float> *build_kdtree(const char* points_file_path);

void save_kdtree (KDTree<Point<float>, float> *kdtree, const char* archive_file);


#endif /* BUILD_KDTREE_H_ */
