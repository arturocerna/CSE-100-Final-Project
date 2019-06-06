#include "MovieS.hpp"
#include <string.h>
#include <set>
#include "ActorVector.hpp"
MovieS::MovieS(int yearReleased, string &film, bool weightFlag ) {
     year = yearReleased;
     name = film;
     if ( weightFlag == true ) {
            weight = 1 + (2018 - year);
     } else {
            weight = 1;
     }
}
