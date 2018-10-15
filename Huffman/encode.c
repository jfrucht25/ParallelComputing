#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
typedef struct Node
{
	char symbol ;
	int frequency ;
	struct Node* left ;
	struct Node* right ;
} TreeNode;
void fillCodes(char codes[256][1000], TreeNode* node, char str[1000]){
	if(node -> symbol == '*'){
		char left[1000];
		strcpy(left, str);
		char right[1000];
		strcpy(right, str);
		strcat(left, "0");
		strcat(right, "1");  
		fillCodes(codes, node -> left, left);
		fillCodes(codes, node -> right, right);
	}
	else{
		strcpy(codes[(int)(node->symbol)], str);	
	}
}
int main(int argc, char** argv){
	if(argc!=2){
		printf("Please provide a file name\n");
		return 1;
	}
	FILE* toEncode = fopen(argv[1],"r");
	char message[2000000] = {'\0'};
	int numbytes = fread(message, 1, 2000000, toEncode);
	printf("%s", message);;
	int len = strlen(message);
	int frequency[256];
	for(int i=0; i<2000000; i++){
		if(message[i] == '\0') break;
		if(message[i] == '\n') continue;
		frequency[message[i]]+=1;
	}
	int m = 0;
	TreeNode* arr[10000] = {NULL};
	for(int i=0; i<256; i++){
		if(frequency[i] == 0) continue;
		arr[m] = malloc(sizeof(TreeNode));
		arr[m] -> symbol = (char)i;
		arr[m] -> frequency = frequency[i];
		arr[m] -> left = NULL;
		arr[m] -> right = NULL;
		m++;
	}
	int unique = m;
	while(m>1){
		int small = 0;
		for(int i=0; i<m; i++){
			if(arr[i]->frequency < arr[small]->frequency){
				small = i;
			}
		}
		TreeNode* t = arr[small];
		arr[small] = arr[m-1];
		arr[m-1] = t;
		small = 0;
		for(int i=0; i<m-1; i++){
			if(arr[i]->frequency < arr[small]->frequency){
				small = i;
			}
		}
		t = arr[small];
		arr[small] = arr[m-2];
		arr[m-2] = t;
		
		TreeNode* newNode = malloc(sizeof(TreeNode));
		newNode -> symbol = '*';
		newNode -> frequency = arr[m-1]->frequency + arr[m-2]->frequency;
		newNode -> right = arr[m-1];
		newNode -> left = arr[m-2];
		
		arr[m-1] = NULL;
		arr[m-2] = newNode;
		m--;
	}
	char codes[256][1000];
	char empty[1000];
	fillCodes(codes, arr[0], empty);
	FILE* fout = fopen("out.txt", "w");
	fprintf(fout, "%d\n", unique);
	for(int i=0; i<256; i++){
		if(strlen(codes[i]) == 0) continue;	
		printf("%c%s\n", (char)i, codes[i]);
		fprintf(fout, "%c%s\n", (char)i, codes[i]);
	}
	for(int i=0; i<len; i++){
		printf("%s", codes[message[i]]);
		fprintf(fout, "%s", codes[message[i]]);
	} 
	printf("\n");
	fprintf(fout, "\n");
	fclose(fout);
	fclose(toEncode);
	return 0;
}
