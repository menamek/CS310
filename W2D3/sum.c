#include <stdio.h>

int sum (void* a,void* b){
return (a+b);
}

int main(){
printf("%d\n",sum(1.2,2));
}
