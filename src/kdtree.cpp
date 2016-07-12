//============================================================================
// Name        : kdtree.cpp
// Author      : Siva Teja Patibandla
// Version     :
// Copyright   : Your copyright notice
// Description : KD-Tree library in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <algorithm>
#include <limits>
#include <array>

#include "kdtree.h"

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

using namespace std;
