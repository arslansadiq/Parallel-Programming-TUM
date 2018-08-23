#include "vis.h"

void visualize(){
        printf("\n---------------------------\n");
        printf("List of Geniuses in family:\n");
        printf("---------------------------\n\n");
        int j;
        for(j = 0; j < NUMNODES; j++){
                // Well.. I think that's the definition of a genius :)
                if(genius[j] >= 140){
                        printf("%s%d %d\n", names[j % 31],j/31, genius[j]);
		}
        }
}

