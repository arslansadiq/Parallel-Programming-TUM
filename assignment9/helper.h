#ifndef HELPER_H_
#define HELPER_H_

void reverse_str(char *str, int strlen);

double time_diff(const struct timespec *first, const struct timespec *second, struct timespec *diff);

void print(const char c[], int n);

#endif /* HELPER_H_ */
