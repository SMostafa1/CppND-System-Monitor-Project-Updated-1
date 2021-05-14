#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string HH, MM, SS;
    if(seconds / 3600 < 10 ) { 
        HH = "0"+ to_string(seconds / 3600);
        }
    else {
        MM = to_string(seconds / 3600);
    }

    if((seconds % 3600)/60 < 10 ) { 
        MM = "0"+ to_string((seconds % 3600)/60);
        }
    else {
        MM = to_string((seconds % 3600)/60);
    }


    if(seconds % 60 < 10 ) { 
        SS = "0"+ to_string(seconds % 60);
        }
    else {
        SS = to_string(seconds %60);
    }
    return HH + ":" + MM + ":" + SS; 
    }

