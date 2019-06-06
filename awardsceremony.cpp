#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include "MatrixMultiply.hpp"
#include <unordered_map>
#include "Movie.hpp"
#include "ActorVector.hpp"
#define CORRECT_ARG 4
#define USAGE "Usage: ./awardsceremony movie_cast_tsv_file  k output_tsv_file" 
using namespace std;

int main(int argc, char *argv[]) {
  // TODO: Implement code for "Part 2: Link Predictor and Recommending New
  // Links"
 /* const char * testloadfile = "../tsv/movie_casts.tsv"; 
  ActorGraph testgraph;
  testgraph.loadFromFile(testloadfile, false);
  vector<string> testcostars = testgraph.findCostars("WILLIS, BRUCE");
  vector<string> testresults = testgraph.futureinteractions("WILLIS, BRUCE"); */
 if ( argc != CORRECT_ARG ) {
      cout << USAGE << endl;
      return 1;
  }
  char * movie_cast_file = argv[1];
  string output_location = argv[3];
  ActorGraph pray_it_works;
  vector<string> results = pray_it_works.kcores(movie_cast_file, stoi(argv[2]));
  ofstream output_file;
  output_file.open(output_location);
  output_file << "Actor" << endl;
  for (unsigned int i = 0; i < results.size(); i++) {
      output_file << results[i] << endl;
  }
  output_file.close();

  return 0;
}

