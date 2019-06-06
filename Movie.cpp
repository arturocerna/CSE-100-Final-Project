#include "Movie.hpp"
#include <string.h>
#include <vector>
#include "ActorVector.hpp"
Movie::Movie(int yearReleased, string &film, bool weightFlag ) {
     year = yearReleased;
     name = film;
     if ( weightFlag == true ) {
            weight = 1 + (2018 - year);
     } else {
            weight = 1;
     }
}
