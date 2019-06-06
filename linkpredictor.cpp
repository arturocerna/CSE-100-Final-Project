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
#define CORRECT_ARG 5
#define USAGE "Usage: ./linkpredictor movie_cast_tsv_file  actors_tsv_file output_future_tsv_file output_new_tsv_file" 
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
  char * pairs_file = argv[2];
  string output_future_file_location = argv[3];
  string output_new_file_location = argv[4];
  ActorGraph pray_it_works;
  pray_it_works.loadFromFile(movie_cast_file, false);
   // Initialize the file stream
  ifstream infile(pairs_file);
  // read in the pairs of files
  bool have_header = false;
  vector<string> actorstopredict;
  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline(infile, s)) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }
    actorstopredict.push_back(s);
  }
  if (!infile.eof()) {
    cerr << "Failed to read " << pairs_file << "!\n";
    return false;
  }
  infile.close();
  ofstream output_future_file;
  ofstream output_new_file;
  output_new_file.open(output_new_file_location);
  output_future_file.open(output_future_file_location);
  output_future_file << "Actor 1 Actor 2 Actor 3 Actor 4" << endl;
  output_new_file << "Actor 1 Actor 2 Actor 3 Actor 4" << endl;  
  for(unsigned int i = 0; i < actorstopredict.size(); i++) { 
    vector<string> results = pray_it_works.futureinteractions(actorstopredict[i]);
    if (results.size() == 8) {
      output_future_file << results[0] + '\t' + results[1] + '\t' + results[2] + '\t' + results[3] << endl;
      output_new_file << results[4] + '\t' + results[5] + '\t' + results[6] + '\t' + results[7] << endl;
    }
  }
  output_new_file.close(); 
  output_future_file.close(); 

  return 0;
}
