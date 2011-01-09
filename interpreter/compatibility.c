#include "compatibility.h"


/*----------------------------------------------------------------------*/
static Bool is3_0Alpha(char version[]) {
    return version[3] == 3 && version[2] == 0 && version[0] == 'a';
}

/*----------------------------------------------------------------------*/
static Bool is3_0Beta(char version[]) {
    return version[3] == 3 && version[2] == 0 && version[0] == 'b';
}

/*----------------------------------------------------------------------*/
static int correction(char version[]) {
    return version[1];
}

/*======================================================================*/
Bool isPreAlpha5(char version[4]) {
    return is3_0Alpha(version) && correction(version) <5;
}

/*======================================================================*/
Bool isPreBeta2(char version[4]) {
    return is3_0Alpha(version) || (is3_0Beta(version) && correction(version) == 1);
}
