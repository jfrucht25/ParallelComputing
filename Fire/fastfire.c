#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define COLS 1000
#define ROWS 1000
#define TRIALS 128
#define DIFFERENT_P 100


//I got this implementation of a queue from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
// A structure to represent a queue 
struct Queue 
{ 
    int front, rear, size; 
    unsigned capacity; 
    int* array; 
}; 
  
// function to create a queue of given capacity.  
// It initializes size of queue as 0 
struct Queue* createQueue(unsigned capacity) 
{ 
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;  
    queue->rear = capacity - 1;  // This is important, see the enqueue 
    queue->array = (int*) malloc(queue->capacity * sizeof(int)); 
    return queue; 
} 
  
// Queue is full when size becomes equal to the capacity  
int isFull(struct Queue* queue) 
{  return (queue->size == queue->capacity);  } 
  
// Queue is empty when size is 0 
int isEmpty(struct Queue* queue) 
{  return (queue->size == 0); } 
  
// Function to add an item to the queue.   
// It changes rear and size 
void enqueue(struct Queue* queue, int item) 
{ 
    if (isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1)%queue->capacity; 
    queue->array[queue->rear] = item; 
    queue->size = queue->size + 1; 
    //printf("%d enqueued to queue\n", item); 
} 
  
// Function to remove an item from queue.  
// It changes front and size 
int dequeue(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
    int item = queue->array[queue->front]; 
    queue->front = (queue->front + 1)%queue->capacity; 
    queue->size = queue->size - 1; 
    return item; 
} 
  
// Function to get front of queue 
int front(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
    return queue->array[queue->front]; 
} 
  
// Function to get rear of queue 
int rear(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
    return queue->array[queue->rear]; 
} 


void print_grid(char a[ROWS][COLS]){
	for(int i=0; i < ROWS; i++){
		for(int j=0; j<COLS; j++){
			printf("%c", a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
bool fire_out(char a[ROWS][COLS]){
	for(int i=0; i < ROWS; i++){
                for(int j=0; j<COLS; j++){
                	if(a[i][j] == '*') return false;
		}
        }
	return true;
}
struct Queue* queue;
int run_fire(double p){
	char grid[ROWS][COLS];
	for(int i=0; i < ROWS; i++){
                for(int j=0; j<COLS; j++){
                	double v = rand() * 1.0 / RAND_MAX;
			if(v < p) grid[i][j] = 'T';
			else grid[i][j] = '.';
		}
        }
	for(int i = 0; i<ROWS; i++){
		if(grid[i][0] == 'T'){
			grid[i][0] = '*';
			enqueue(queue, COLS*i);
		}
	}
	int steps = 0;
	while(!isEmpty(queue)){
		for(int i=0; i<queue -> size; i++){
			int pos = dequeue(queue);
			int r = pos / COLS;
			int c = pos - (r*COLS);
			grid[r][c] = '.';
			if(r<(ROWS-1) && grid[r+1][c] == 'T'){
				grid[r+1][c] = '*';
				enqueue(queue, COLS*(r+1)+c);
			}
			if(c<(COLS-1) && grid[r][c+1] == 'T'){
				grid[r][c+1] = '*';
				enqueue(queue, COLS*(r)+c+1);
			}
			if(r>0 && grid[r-1][c] == 'T'){
				grid[r-1][c] = '*';
				enqueue(queue, COLS*(r-1)+c);
			}
			if(c>0 &&grid[r][c-1] == 'T'){
				grid[r][c-1] = '*';
				enqueue(queue, COLS*r+c-1);
			}
        	}
		steps++;
	}
	return steps;
}


int main(){
	srand(time(NULL));
	queue = createQueue(COLS*ROWS);
	for(int i=0; i<DIFFERENT_P; i++){
		double p = (i*1.0)/(DIFFERENT_P);
		int sum = 0;
		for(int j=0; j<TRIALS; j++){
			sum+=run_fire(p);
		}
		double avg = (sum*1.0)/TRIALS/COLS;
		printf("%f\t%f\n", p, avg);
	}
	return 0;
}
