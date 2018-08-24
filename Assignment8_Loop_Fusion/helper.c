#include <stdlib.h>
#include <math.h>

#define sign(x) ((x > 0) - (x < 0))

int str_cmatch(const char* a, const char* b)
{
	int i = 0;

	while (a[i] != 0 && b[i] != 0 && a[i] == b[i])
		i++;

	return i;
}

struct timespec ts_diff(struct timespec a, struct timespec b)
{
	a.tv_sec = a.tv_sec - b.tv_sec;
	a.tv_nsec = a.tv_nsec - b.tv_nsec;

	a.tv_sec = abs(a.tv_sec) - 1 * ((sign(a.tv_sec) * sign(a.tv_nsec)) < 0);
	a.tv_nsec = abs(1000000000 * ((sign(a.tv_sec) * sign(a.tv_nsec)) < 0) - abs(a.tv_nsec));

	return a;
}

double ts_to_double(struct timespec time)
{
	return time.tv_sec + time.tv_nsec / 10e9;
}

int AlmostEqualRelative(double A, double B, double maxRelDiff)
{
	// Calculate the difference.
	double diff = fabs(A - B);
	A = fabs(A);
	B = fabs(B);
	// Find the largest
	float largest = (B > A) ? B : A;

	if (diff <= largest * maxRelDiff)
		return 1;
	return 0;
}
