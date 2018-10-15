#include <stdio.h>
#include <string.h>
#include <math.h>
int main(int argc, char** argv){
	if(argc!=2){
		printf("Please provide a file name\n");
		return 1;
	}
	char tree[1000000] = {'\0'};
	FILE* toDecode = fopen(argv[1],"r");
	int n;
	fscanf(toDecode, "%d", &n);
	fgetc(toDecode);
	for(int i = 0; i<n; i++){
		char value = fgetc(toDecode);
		int x = 1;
		char current;
		while(1){
			current = fgetc(toDecode);
			if(current == '\n') break;
			else if(current == '0') x = 2*x;
			else if(current == '1') x = 2*x + 1;
		}
		tree[x] = value;
	}
	char ch = '\0';
	char message[100000];
	int i = 0;
	int x = 1;
	int encodedBits = 0;
    	while (1){
		int numbytes = fread(&ch, sizeof(char), 1, toDecode);
		if(numbytes == 0) break;
		if(tree[x] != '\0'){
			message[i] = tree[x];
			i++;
			x = 1;
		}
		if(ch != '\n') encodedBits++;
		if(ch == '0') x = 2*x;
		if(ch == '1') x = 2*x + 1;
    	}
	printf("%s\n", message);
	int len = strlen(message);
	float compressionRatio = (float)(8*len-encodedBits)/(8*len);
	printf("The length of the string is %d characters\n", len);
	printf("There are %d bits in the Huffman encoded message\n", encodedBits);
	printf("The compression ratio is %f\n", compressionRatio);
	int freq[256] = {0};
	for(int i=0; i<len; i++){
		freq[message[i]]++;
	}
	double shannon = 0;
	for(int i=0; i<256; i++){
		if(freq[i] == 0) continue;
		shannon += freq[i] * -1*log((double)freq[i]/len)/log(2);
	}
	printf("The shannon minimum is %f bits\n", shannon);
	printf("Huffman encoding had %d more bits than the shannon minimum\n", encodedBits-(int)ceil(shannon));
	fclose(toDecode);
	return 0;
}
