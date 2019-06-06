#ifndef MOVIE_HPP
#define MOVIE_HPP
#include <vector>
#include <string>
#include "ActorVector.hpp"
using namespace std;
class Movie {
public:
   string name;
   int year;
   int weight;
   vector<string> stars;
   Movie(int yearReleased, string &film, bool weightFlag );
};
#endif
