#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int rank(int x, int arr[1000], int l, int u){
		if(u-l==0){
				if(x<arr[l])
						return l;
				else
						return l+1;
		}
		if(x<arr[0])
			return 0;
		if(x>arr[u])
			return u+1;
		int mid = l + ((u-l) / 2);
		if(arr[mid] > x){
				return rank(x, arr, l, mid-1);
		}
		return rank(x, arr, mid+1, u);
}

int main(int argc, char *argv[]){
		int a[1000]={1,6,7,10,34};
		int b[1000]={2,3,4,29,36};
		int sa = 5;
		int sb = 5;
		int c[1000];
		
		int i;
		#pragma omp parallel for private(i)
		for(i=0; i<sa; i++){
				int r = rank(a[i], b, 0, sb-1);
				c[i+r]=a[i];
		}
		#pragma omp parallel for private(i)
		for(i=0; i<sb; i++){
				int r = rank(b[i], a, 0, sa-1);
				c[i+r]=b[i];
		}
		for(i=0; i<sa+sb; i++){
				printf("%d ", c[i]);
		}
		printf("\n");
}
