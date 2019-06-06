/*
 * ActorGraph.cpp
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in movie_casts.tsv.
 *
 * Feel free to modify any/all aspects as you wish.
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>
#include "Movie.hpp"
#include "ActorVector.hpp"
#include "MovieS.hpp"
#include "ActorVectorS.hpp"
using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char *in_filename,
                              const bool use_weighted_edges) {
  // Initialize the file stream
  ifstream infile(in_filename);
  bool have_header = false;

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

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1] + "#@" + record[2]);
    int movie_year = stoi(record[2]);
    // search movie map for movie title
    unordered_map<string, Movie*>::iterator er = films.find (movie_title);
    // if no such movie exists, add to movie map
    if (er == films.end()) {
        Movie * newMovie = new Movie(movie_year, movie_title, use_weighted_edges);
        newMovie->stars.push_back(actor_name);
        pair<string, Movie*> moviePair = make_pair(movie_title, newMovie);
	films.insert(moviePair);
        unordered_map<string, ActorVector*>::iterator it = actors.find (actor_name);
        if (it == actors.end()) {
            ActorVector * newActor = new ActorVector(actor_name);
	    newActor->movies.push_back(movie_title);
            pair<string, ActorVector*> actorPair = make_pair(actor_name, newActor);
	    actors.insert(actorPair);
        } else {
            (*it).second->movies.push_back(movie_title);
        }
    } else {
      (*er).second->stars.push_back(actor_name);
      unordered_map<string, ActorVector*>::iterator it = actors.find (actor_name);
      if (it == actors.end()) {
            ActorVector * newActor = new ActorVector(actor_name);
	    newActor->movies.push_back(movie_title);
            pair<string, ActorVector*> actorPair = make_pair(actor_name, newActor);
	    actors.insert(actorPair);
      } else {
            (*it).second->movies.push_back(movie_title);
      }
    } 
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}
vector<string> ActorGraph::futureinteractions(string Actor) {
  vector<string> * costars = new vector<string>();
  findCostars(Actor, costars);
  vector<string> cocostars;
  vector<string> * neighbors = new vector<string>();
  set<string>  visitedactors;
  unsigned int values[] = {0, 0, 0, 0};
  if (costars->size() < 4) 
     return *costars;
  vector<string> futureactors;
  futureactors.push_back("NULL");
  futureactors.push_back("NULL");
  futureactors.push_back("NULL");
  futureactors.push_back("NULL");
  for(unsigned int i = 0; i < costars->size(); i++) {
    findCostars(costars->at(i), neighbors);
    vector<string> intersection;
    intersection.reserve(min(neighbors->size(), costars->size()));
    set_intersection((*costars).begin(), (*costars).end(), (*neighbors).begin(), (*neighbors).end(), back_inserter(intersection));
    if ( intersection.size() > values[3] ) {
      for (unsigned int i = 0; i < neighbors->size(); i++)
        if (Actor.compare(neighbors->at(i)) != 0 )
          visitedactors.insert(neighbors->at(i));
      unsigned int num = intersection.size();
      string replace = costars->at(i);
      for(unsigned int j = 0; j < 4; j++) {
         if ( num > values[j]) {
           string tempstring = futureactors[j];
           futureactors[j] = replace;
           replace = tempstring;
           unsigned int temp = num;
           num = values[j];
           values[j] = temp;
         }
      }
    } 
  }
  cocostars.resize(visitedactors.size());
  copy(visitedactors.begin(), visitedactors.end(), cocostars.begin());
  sort(cocostars.begin(), cocostars.end());
  vector<string> setdifference(12000);
  vector<string>::iterator it = set_difference(cocostars.begin(), cocostars.end(), costars->begin(), costars->end(), setdifference.begin());
  setdifference.resize(it - setdifference.begin());
  vector<string> newinteractions = findCoCostars( costars, &setdifference);
  delete neighbors;
  delete costars;
  for(unsigned int i = 0; i < newinteractions.size(); i++) {
    futureactors.push_back(newinteractions[i]);
  }
  return futureactors;
}
vector<string> ActorGraph::findCoCostars(vector<string> * costars, vector<string> * cocostars) {
  unsigned int values[] = {0, 0, 0, 0};
  vector<string> futureactors;
  vector<string> * neighbors = new vector<string>();
  futureactors.push_back("NULL");
  futureactors.push_back("NULL");
  futureactors.push_back("NULL");
  futureactors.push_back("NULL");
  for(unsigned int i = 0; i < (*cocostars).size(); i++) {
    vector<string> intersection;
    findCostars(cocostars->at(i), neighbors);
    intersection.reserve(min((*neighbors).size(), (*costars).size()));
    set_intersection((*costars).begin(), (*costars).end(), (*neighbors).begin(), (*neighbors).end(),back_inserter(intersection));
    if ( intersection.size() > values[3] ) {
      unsigned int num = intersection.size();
      string replace = cocostars->at(i);
      for(unsigned int j = 0; j < 4; j++) {
         if ( num > values[j]) {
           string tempstring = futureactors[j];
           futureactors[j] = replace;
           replace = tempstring;
           unsigned int temp = num;
           num = values[j];
           values[j] = temp;
         }
      }
    } 
  }
  delete neighbors;
  return futureactors;
}
void ActorGraph::findCostars(string Actor, vector<string> * costars) {
  set<string> visitedactors;
  ActorVector * mainactor = actors[Actor];
  for(unsigned int i = 0; i < mainactor->movies.size(); i++) {
    Movie * currentmovie = films[mainactor->movies[i]];
    for (unsigned int m = 0; m < currentmovie->stars.size(); m++) {
        if (currentmovie->stars[m].compare(Actor) != 0)
          visitedactors.insert(currentmovie->stars[m]);
    }
  }
  costars->resize(visitedactors.size());
  copy(visitedactors.begin(), visitedactors.end(), (*costars).begin());
  sort((*costars).begin(), (*costars).end()); 
}
string ActorGraph::unweightedActorPath(const string &actor1, const string &actor2) {
  queue<Path> pathqueue;
  set<string> visitedmovies;
  set<string> visitedactors;
  ActorVector * initialActor = actors[actor1];
  int moviesize = initialActor->movies.size();
  for (int i = 0; i < moviesize; i++) {
     Path newPath;
     newPath.moviespath.push_back(initialActor->movies[i]);
     visitedmovies.insert(initialActor->movies[i]);
     newPath.actorspath.push_back(initialActor->actorName);
     newPath.weight = 1;
     pathqueue.push(newPath);
  }
  visitedactors.insert(actor1);
  while (!pathqueue.empty()) {
    Path currentPath = pathqueue.front();
      pathqueue.pop();
      Movie * currentMovie = films[currentPath.moviespath[currentPath.moviespath.size()-1]];
      moviesize = currentMovie->stars.size();
      for (int j = 0; j < moviesize; j++) {
         if (actor2.compare(currentMovie->stars[j]) == 0) {
           string Returnstring; 
           for (unsigned int i = 0; i < currentPath.actorspath.size();  i++) {
             Returnstring += "(" + currentPath.actorspath[i] + ")--[" + currentPath.moviespath[i] + "]-->"; 
           }
             return Returnstring + "(" + actor2 + ")";
         }
      }   
      // if it lacks effeciency, this is where its gonna be
      for (int k = 0; k < moviesize; k++) {
         if ( visitedactors.find(currentMovie->stars[k]) == visitedactors.end() ) {
           ActorVector *  newactorpath = actors[currentMovie->stars[k]];
           visitedactors.insert(currentMovie->stars[k]);
           for (unsigned int m = 0; m < newactorpath->movies.size(); m++) {
             if ( visitedmovies.find(newactorpath->movies[m]) == visitedmovies.end()) {
               Path addPath = currentPath;
               addPath.actorspath.push_back(newactorpath->actorName);
               addPath.moviespath.push_back(newactorpath->movies[m]);
               visitedmovies.insert(newactorpath->movies[m]);
               addPath.weight++;
               pathqueue.push(addPath);        
             }
           }
         }
      }
      
    }
  return actor1;
}
string ActorGraph::weightedActorPath(string actor1, string actor2) {
  priority_queue<Path, vector<Path>, ComparePath> pathqueue;
  set<string> visitedmovies;
  set<string> visitedactors;
  ActorVector * initialActor = actors[actor1];
  int moviesize = initialActor->movies.size();
  for (int i = 0; i < moviesize; i++) {
     Path newPath;
     newPath.moviespath.push_back(initialActor->movies[i]);
     visitedmovies.insert(initialActor->movies[i]);
     newPath.actorspath.push_back(initialActor->actorName);
     newPath.weight = films[initialActor->movies[i]]->weight;
     pathqueue.push(newPath);
  }
  visitedactors.insert(actor1);
  while (!pathqueue.empty()) {
    Path currentPath = pathqueue.top();
      pathqueue.pop();
      Movie * currentMovie = films[currentPath.moviespath[currentPath.moviespath.size()-1]];
      moviesize = currentMovie->stars.size();
      for (int j = 0; j < moviesize; j++) {
         if (actor2.compare(currentMovie->stars[j]) == 0) {
           string Returnstring;
           for (unsigned int i = 0; i < currentPath.actorspath.size();  i++) {
             Returnstring += "(" + currentPath.actorspath[i] + ")--[" + currentPath.moviespath[i] + "]-->";
           }
             return Returnstring + "(" + actor2 + ")";
         }
      }
      // if it lacks effeciency, this is where its gonna be
      for (int k = 0; k < moviesize; k++) {
         if ( visitedactors.find(currentMovie->stars[k]) == visitedactors.end() ) {
           ActorVector *  newactorpath = actors[currentMovie->stars[k]];
           visitedactors.insert(currentMovie->stars[k]);
           for (unsigned int m = 0; m < newactorpath->movies.size(); m++) {
             if ( visitedmovies.find(newactorpath->movies[m]) == visitedmovies.end()) {
               Path addPath = currentPath;
               addPath.actorspath.push_back(newactorpath->actorName);
               addPath.moviespath.push_back(newactorpath->movies[m]);
               visitedmovies.insert(newactorpath->movies[m]);
               addPath.weight += films[newactorpath->movies[m]]->weight;
               pathqueue.push(addPath);
             }
           }
         }
      }

    }
  return actor1;
  
}
vector<string> ActorGraph::kcores(const char *in_filename, int k) {
  // Initialize the file stream
  ifstream infile(in_filename);
  bool have_header = false;

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

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1] + "#@" + record[2]);
    int movie_year = stoi(record[2]);
    // TODO: we have an actor/movie relationship, now what?
    // search movie map for movie title
    unordered_map<string, MovieS*>::iterator er = filmsS.find (movie_title);
    // if no such movie exists, add to movie map
    if (er == filmsS.end()) {
        MovieS * newMovie = new MovieS(movie_year, movie_title, false);
        newMovie->stars.insert(actor_name);
        pair<string, MovieS*> moviePair = make_pair(movie_title, newMovie);
        filmsS.insert(moviePair);
        unordered_map<string, ActorVectorS*>::iterator it = actorsS.find (actor_name);
        if (it == actorsS.end()) {
            ActorVectorS * newActor = new ActorVectorS(actor_name);
            pair<string, ActorVectorS*> actorPair = make_pair(actor_name, newActor);
            actorsS.insert(actorPair);
        }
    } else {
      unordered_map<string, ActorVectorS*>::iterator it = actorsS.find (actor_name);
      if (it == actorsS.end()) {
            ActorVectorS * newActor = new ActorVectorS(actor_name);
            newActor->movies = (*er).second->stars;
            pair<string, ActorVectorS*> actorPair = make_pair(actor_name, newActor);
            actorsS.insert(actorPair);
            set<string>::iterator movieit;
            for (movieit = er->second->stars.begin();  movieit != er->second->stars.end(); movieit++) {
              ActorVectorS * oldActor = actorsS[*movieit];
              oldActor->movies.insert(newActor->actorName);
            }
      } else {
          set<string>::iterator movieit;
          for (movieit = er->second->stars.begin();  movieit != er->second->stars.end(); movieit++) {
            ActorVectorS * oldActor = actorsS[*movieit];
            oldActor->movies.insert(actor_name);
            it->second->movies.insert(*movieit);
          }
      }
      (*er).second->stars.insert(actor_name);
    }
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
  }
  infile.close();
  unordered_map<string, ActorVectorS*>::iterator it;
  bool iscompleted = false;
  while (!iscompleted) {
    iscompleted = true;
    for (it = actorsS.begin(); it != actorsS.end(); it++) {
      if (it->second != NULL) {
        if ((*it).second->movies.size() < k ) {
          iscompleted = false;
          set<string>::iterator currentit;
          for (currentit = it->second->movies.begin(); currentit != (*it).second->movies.end(); currentit++){
            ActorVectorS * decrease = actorsS[*currentit];
            if (decrease != NULL) {
              set<string>::iterator actorit = decrease->movies.find(it->first);
              decrease->movies.erase(actorit);
            }
          }
          delete it->second;
          it->second = NULL;
        }
      }
    }
  }
  vector<string> result;
  for (it = actorsS.begin(); it != actorsS.end(); it++) {
    if (it->second != NULL)
      result.push_back(it->first);
  }
 sort(result.begin(), result.end());
 return result;
}
ActorGraph::~ActorGraph() {
  unordered_map<string, ActorVector *>::iterator it;
  unordered_map<string, Movie *>::iterator er;
  for (it = actors.begin(); it != actors.end(); it++) {
    delete (it->second);
  }
  for (er = films.begin(); er != films.end(); er++) {
    delete (er->second);
  } 
  unordered_map<string, ActorVectorS *>::iterator its;
  unordered_map<string, MovieS *>::iterator ers;
  for (its = actorsS.begin(); its != actorsS.end(); its++) {
    delete (its->second);
  }
  for (ers = filmsS.begin(); ers != filmsS.end(); ers++) {
    delete (ers->second);
  } 
}
