#ifndef MOVIES_HPP
#define MOVIES_HPP
#include <set>
#include <string>
using namespace std;
class MovieS {
public:
   string name;
   int year;
   int weight;
   set<string> stars;
   MovieS(int yearReleased, string &film, bool weightFlag );
};
#endif
