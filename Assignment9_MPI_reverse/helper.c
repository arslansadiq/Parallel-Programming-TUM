#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

double time_diff(const struct timespec *first, const struct timespec *second, struct timespec *diff)
{
	struct timespec tmp;
	const struct timespec *tmp_ptr;

	if (first->tv_sec > second->tv_sec || (first->tv_sec == second->tv_sec && first->tv_nsec > second->tv_nsec))
	{
		tmp_ptr = first;
		first = second;
		second = tmp_ptr;
	}

	tmp.tv_sec = second->tv_sec - first->tv_sec;
	tmp.tv_nsec = second->tv_nsec - first->tv_nsec;

	if (tmp.tv_nsec < 0)
	{
		tmp.tv_sec -= 1;
		tmp.tv_nsec += 1000000000;
	}

	if (diff != NULL )
	{
		diff->tv_sec = tmp.tv_sec;
		diff->tv_nsec = tmp.tv_nsec;
	}

	return tmp.tv_sec + tmp.tv_nsec / 1000000000.0;
}

void reverse_str(char *str, int strlen)
{	//We repeat the reverse n times to increase processing time
	for (int i = 0; i < 25000001; ++i)
	{
		char* begin = str, *end = str + strlen - 1;

		while (begin < end) {
			char tmp = *end;
			*end = *begin;
			*begin = tmp;

			begin++;
			end--;
		}
	}
}

void print(const char c[], int n) {
	for (int i = 0; i < n; i++)
		putchar(c[i]);
	putchar('\n');
}
