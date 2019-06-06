#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include "Movie.hpp"
#include "ActorVector.hpp"
#define CORRECT_ARG 5
#define USAGE "Usage: ./refpathfinder movie_cast_tsv_file u/w pairs_tsv_file output_paths_tsv_file"
using namespace std;

int main(int argc, char *argv[]) {
  // TODO: Implement code for "Part 1: Path Finder"
  if ( argc != CORRECT_ARG ) {
      cout << USAGE << endl;
      return 1;  
  }
  char flag = *(argv[2]);
  bool weighted = false;
  switch (flag) {
	case 'u':
           break;
	case 'w':
	   weighted = true;
	   break;
        default:
           cout << USAGE << endl;
           return 1;
  }  

  char * movie_cast_file = argv[1];
  char * pairs_file = argv[3];
  string output_file_location = argv[4];
  ActorGraph pray_it_works;
  pray_it_works.loadFromFile(movie_cast_file, weighted);
   // Initialize the file stream
  ifstream infile(pairs_file);
  // read in the pairs of files
  bool have_header = false;
  vector<string> actor1;
  vector<string> actor2;
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

    istringstream ss(s);
    vector<string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline(ss, next, '\t')) break;

      record.push_back(next);
    }

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }
    actor1.push_back(record[0]);
    actor2.push_back(record[1]);
  }
  if (!infile.eof()) {
    cerr << "Failed to read " << pairs_file << "!\n";
    return false;
  }
  infile.close();
  ofstream outputfile;
  outputfile.open(output_file_location);
  outputfile << "(actor)--[movie#@year]-->(actor)--..." << endl;
  if (weighted) {
    for (unsigned int i = 0; i < actor1.size(); i++) {
      outputfile << pray_it_works.weightedActorPath(actor1[i], actor2[i]) << endl;
    }
  } else {
    for (int i = 0; i < actor1.size(); i++) {
      outputfile << pray_it_works.unweightedActorPath(actor1[i], actor2[i]) << endl;
    }
    outputfile.close();
  }
  return 0;
}
