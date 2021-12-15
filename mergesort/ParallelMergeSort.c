#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 10

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;



struct divider { 
    int partNum;
    int partLow;
    int partHigh;
};
//merges 2 parts
void merge(int low, int middle, int high){

    int i,j;
    int leftP = (middle - low + 1);
    int rightP = (high - middle);

    int *left = malloc(leftP * sizeof(int));
    int *right = malloc(rightP * sizeof(int));

    for (i = 0; i < leftP; i++){
      left[i] = array[i + low];
    }
  
    for (i = 0; i < rightP; i++){
      right[i] = array[i + middle + 1];
    }
  
    int k = low;

    i = 0;
    j = 0;

//merges left and right
    while (i < leftP && j < rightP){
        if (left[i] <= right[j]){
            array[k++] = left[i++];
        }
        else{
            array[k++] = right[j++];
        }
    }

    while (i < leftP){//insert the values
      array[k++] = left[i++];
    }

    while (j < rightP){
      array[k++] = right[j++];
    }

    free(left);
    free(right);
}
//Merge_sort function
void merge_sort(int low, int high){
    int middle = low+(high - low)/2;

    if (low < high){
        merge_sort(low, middle);
        merge_sort(middle + 1, high);
        merge(low, middle, high);
    }
}
//multit threading function)
void *merge_sort123(void *arg){
    struct divider *divider = arg; 
    int low;
    int high;
    int num;

    high = divider->partHigh; 
    low = divider->partLow;

    int mid = low+(high-low)/2; 

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid+1, high);
        merge(low, mid, high);
    }

    return 0;
}

int main()
{
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;

    
    fillArrayWithRandomNumbers(array);

    printf("Unsorted array: ");
    printArray(array);
    struct divider *divider;


    pthread_t threads[2];
    struct divider list[2];

    int len = SIZE/2;
    int low = 0;

    for (int i = 0; i<2; i++, low += len) {
        divider = &list[i];
        divider->partNum = i;

        divider->partLow = i * (SIZE/2);
        divider->partHigh = (i+1) * (SIZE/2)-1;
    }

    for (int i=0; i<2; i++) {
        divider = &list[i];
        pthread_create(&threads[i], NULL, merge_sort123, divider); // passes the threads being created, passes the struct, and calls the function for multi threading
    }

    for (int i = 0; i < 2; i++){
        pthread_join(threads[i], NULL);
        merge(0, (SIZE / 2 - 1) / 2, SIZE / 2 - 1);
        merge(SIZE / 2, SIZE / 2 + (SIZE - 1 - SIZE / 2) / 2, SIZE - 1);
        merge(0, (SIZE - 1) / 2, SIZE - 1);
    }

    printf("Sort array:   ");
    printArray(array);

    return 0;

}