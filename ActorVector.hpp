#ifndef ACTORVECTOR_HPP
#define ACTORVECTOR_HPP
#include <vector>
#include <string.h>
#include "Movie.hpp"
using namespace std;
class ActorVector {
public:
    string actorName;
    int knumber;
    vector<string> movies;
    ActorVector(string &name);
};
#endif 
