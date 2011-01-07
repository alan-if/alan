#include "compatibility.h"


/*======================================================================*/
Bool isPreAlpha5(char version[4]) {
    return version[3] == 3 && version[2] == 0 && version[0] == 'a' && version[1] <5;
}


/*======================================================================*/
Bool isPreBeta1(char version[4]) {
    return version[3] == 3 && version[2] == 0 && version[0] == 'a';
}
