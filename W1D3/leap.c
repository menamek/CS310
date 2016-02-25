#include <stdio.h>

int main (long c){

	if((c%400==0) || ((c%4==0)&&(c%100!=0))){
	return 1;
	}else{
		return 0;}}
