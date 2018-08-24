#ifndef HELPER_H_
#define HELPER_H_

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

int str_cmatch(const char* a, const char* b);
struct timespec ts_diff(struct timespec a, struct timespec b);
double ts_to_double(struct timespec time);
int AlmostEqualRelative(double A, double B, double maxRelDiff);

#endif /* HELPER_H_ */
