#include <stdio.h>

int main (){
	int min;
	int max;
	int n;
	int m;
	int arr[10];
	printf("enter Nunms:");

	int tem=arr[0];
	for (int i=1; i<sizeof(arr); i++){
		scanf("%d",&n);
		if (tem>arr[i]){
			max=tem;

		}else{
			max=arr[i];
		}


	}

	printf("max=%d\n",max);
	printf("min=%d",min);
}
