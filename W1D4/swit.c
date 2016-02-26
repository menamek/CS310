#include <stdio.h>

int main(){
	char x;
	int a,b,c;

	switch((x=getchar())!=	EOF){
		case '\t':
			a++;
		case '\n':
			b++;
		case ' ':
			c++;
		default:
			printf("nothing");
}
return (a+b+c);
}
