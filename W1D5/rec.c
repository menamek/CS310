#include<stdio.h>

int  mul(int i){
if (i==0){
	return;
}
while(i<=5){
	mul (i+1);
 printf("%d * %d = %d", i, mul(i+1),i*mul(i+1)); 
}
}


int main(){
mul(8);
}
