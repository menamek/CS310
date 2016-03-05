#include <stdio.h>

typedef union {
long x;
double y;
}Unionld;

int main(){
Unionld ld;

ld.x=10000;
ld.y=77.56;
printf("%li\n",ld.x);
printf("%f\n",ld.y);
}
