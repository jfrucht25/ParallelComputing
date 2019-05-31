#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){
	
	int a[] = {8,7,19,5,3,24};
	int n = 6;
	int height = ceil(log2(n))+1;
	int width = pow(2,height-1);
	int sums[height][width];
	int i,j;
	#pragma omp parallel for private(i)
	for(i = 0; i<width; i++){
		if(i<n){
			sums[0][i] = a[i];
		}
		else {
			sums[0][i] = 0;
		}
	}
	for(i=1; i<height; i++){
			#pragma omp parallel for private(j)
			for(j=0; j<(int)pow(2,height-i-1); j++){
				sums[i][j] = sums[i-1][j*2] + sums[i-1][j*2+1];
			}
	}
	for(i=0; i<height; i++){
			for (j=0; j<(int)pow(2,height-i-1); j++){
					printf("%d ", sums[i][j]);
			}
			printf("\n");
	}
	
	printf("The sum is %d\n", sums[height-1][0]);
}
