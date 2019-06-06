/*
 * ActorGraph.hpp
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP
#include <vector>
#include <unordered_map>
#include <string>
#include "ActorVector.hpp"
#include "Movie.hpp"
#include "ActorVectorS.hpp"
#include "MovieS.hpp"
#include <iostream>
using namespace std;
class ActorGraph {
 public:
  // Maybe add class data structure(s) here
  unordered_map<string, ActorVector *> actors;
  unordered_map<string, ActorVectorS *> actorsS;
  unordered_map<string, Movie *> films;
  unordered_map<string, MovieS *> filmsS;
  vector<vector<int>> adjacencymatrix;
  struct Path {
    vector<string> actorspath;
    vector<string> moviespath;
    int weight;
  };
  struct ComparePath {
    bool operator()(Path const & Path1, Path const & Path2) {
      return Path1.weight > Path2.weight;
    }
  };
  vector<string> kcores(const char *in_filename, int k);
  void findCostars(string Actor, vector<string> * costars);
  vector<string> findCoCostars(vector<string> * costars, vector<string> * cocostars);
  // vector<vector<int>> populateAdjMatrix(vector<string> * costars);
  vector<string> futureinteractions(string Actor);
  ActorGraph(void);
  string unweightedActorPath(const string &actor1, const string &actor2);
  string weightedActorPath(string actor1, string actor2);
  // Maybe add some more methods here...

  /** You can modify this method definition as you wish
   *
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 1 + (2018 -
   * movie_year), otherwise all edge weights will be 1
   *
   * return true if file was loaded sucessfully, false otherwise
   */
  bool loadFromFile(const char *in_filename, const bool use_weighted_edges);
  ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
