#ifndef ACTORVECTORS_HPP
#define ACTORVECTORS_HPP
#include <set>
#include <string>
using namespace std;
class ActorVectorS {
public:
    string actorName;
    int knumber;
    set<string> movies;
    ActorVectorS(string &name);
};
#endif
