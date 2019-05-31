#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){
		int b[] = {1,0,0,1,0};
		int x=4;
		
		int n = 5;
		int height = ceil(log2(n))+1;
		int width = pow(2,height-1);
		int sums[height][width];
		int prefix[height][width];
		int i,j;
		#pragma omp parallel for private(i)
		for(i = 0; i<width; i++){
			if(i<n){
				sums[0][i] = b[i];
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
		for(i=height-1; i>=0; i--){
				#pragma omp parallel for private(j)
				for(j=0; j<(int)pow(2,height-i-1); j++){
					if(j==0){
						prefix[i][j] = sums[i][j];
					}
					else if(j%2==0){
						prefix[i][j] = prefix[i+1][(j-1)/2] + sums[i][j];
					}
					else{
						prefix[i][j] = prefix[i+1][j/2];
					}
				}
		}

		int sum = prefix[0][x];
		int near = 0;
		if(sum!=0){
				#pragma omp parallel for private(i)
				for(i = 0; i<=x; i++){
					if(prefix[0][i]==sum){
							if(b[i]==1){
									near = i;
							}
					}
				}
		}
		printf("%d\n", near);
}

