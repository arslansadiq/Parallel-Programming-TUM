#include<stdio.h>
int main()
{
int arr[3][3];
int (*img)[3][3]= &arr;
printf("%d\n",img[0][1]);
return 0;
}
