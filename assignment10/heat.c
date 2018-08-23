#include "heat.h"

void init_sources(const int nsources , int sources [nsources][2], int n){
    sources [0][0] = n * 3 / 4;
    sources [0][1] = n * 3 / 4;
    sources [1][0] = n / 4;
    sources [1][1] = n / 4;
    sources [2][0] = n * 3 / 4;
    sources [2][1] = n / 4;
    sources [3][0] = n / 4;
    sources [3][1] = n * 3 / 4;
}
