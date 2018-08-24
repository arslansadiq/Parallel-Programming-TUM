#ifndef DS_H
#define DS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEVEL 10
#define NUMNODES 1024

extern char *names[31];

struct bin_tree {
	int data;
	int IQ;
	int id;
  	char name[20];
	struct bin_tree * right, * left;
};

typedef struct bin_tree tree;
// THAT WOULD BE A GOOD RESULTS ARRAY
int genius[NUMNODES];
extern int g_node_id;

#endif
