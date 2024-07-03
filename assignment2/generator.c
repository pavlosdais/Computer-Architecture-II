#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// here we define the range of array sizes our generator will generate
#define SIZE 26
#define START_SIZE 8
#define STEP 2
#define END_SIZE SIZE

// create a range to our number values by defining a max value
#define NUM_LIMIT 64

// function prototypes
static void print_array(int *array, const char *array_name, FILE *file);
static void take_subarray(int *array_source, int ***array_dest, unsigned int size_num);
static void destroy_subarray(int **array, int size_num);

int main(void)
{
    // create the file containing arrays X,Y,Z
    FILE *arrFile = fopen("arrays.txt", "w");
    if (arrFile == NULL)
    {
        fprintf(stderr, "Error opening file arrays.txt\n");
        exit(EXIT_FAILURE);
    }

    const unsigned int n = NUM_LIMIT;
    const unsigned int m = SIZE;
    int X[SIZE*SIZE], Y[SIZE*SIZE], Z[SIZE*SIZE];

    // Make array X
    for (unsigned int i = 0, elements = m * m; i < elements; i++)
    {
        int num = rand() % n;
        if (rand() % 4 == 0)
            num = -num;
        X[i] = num;
    }
    print_array(X, "arrX", arrFile);

    // Make array Y
    for (unsigned int i = 0, elements = m * m; i < elements; i++)
    {
        int num = rand() % n;
        if (rand() % 4 == 0)
            num = -num;
        Y[i] = num;
    }
    print_array(Y, "arrY", arrFile);

    // Make array Z
    for (unsigned int i = 0, elements = m * m; i < elements; i++)
    {
        int num = rand() % n;
        if (rand() % 4 == 0)
            num = -num;
        Z[i] = num;
    }
    print_array(Z, "arrZ", arrFile);

    fclose(arrFile);

    // Close file with the arrays, now it is time to start solving
    // store the results in .txt files
    for (unsigned int size_num = START_SIZE; size_num <= END_SIZE; size_num += STEP) {
        
        // Suppose Z = Z + X*Y is similar to C = C + A*B
        int** A;
        int** B;
        int** C;
        take_subarray(X, &A, size_num);
        take_subarray(Y, &B, size_num);
        take_subarray(Z, &C, size_num);

        // Multiply the (size_num x size_num) arrays
        // simple solution
        for (register unsigned int i = 0; i < size_num; i++)
        {
            for (register unsigned int j = 0; j < size_num; j++)
            {
                int result = 0;
                for (register unsigned int k = 0; k < size_num; k++)
                    result += A[i][k] * B[k][j];
                C[i][j] += result;
            }
        }
        
        // Create a file to store the solution
        char fileName[50];
        sprintf(fileName, "solution_arr%d.txt", size_num);
        FILE *solution_file = fopen(fileName, "w");
        if (solution_file == NULL)
        {
            fprintf(stderr, "Error opening file %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        // Print the results in hexadecimal form (we have words)
        for (unsigned int i = 0; i < size_num; i++) {
            for (unsigned int j = 0; j < size_num; j++)
                fprintf(solution_file, "%X\n", C[i][j]);
        }
        fclose(solution_file);

        destroy_subarray(A, size_num);
        destroy_subarray(B, size_num);
        destroy_subarray(C, size_num);
    }

    exit(EXIT_SUCCESS);
}

void print_array(int *array, const char *array_name, FILE *file)
{
    fprintf(file, "\n%s:\n", array_name);

    unsigned int counter = 0;
    for (unsigned int i = 0; i < SIZE; i++)
    {
        fprintf(file, ".word");
        for (unsigned int j = 0; j < SIZE; j++)
        {
            fprintf(file, " %d", array[counter++]);
            int nl = 0;
            if(j != SIZE-1)
            {
                nl = 1;
                fprintf(file, ",");
            }

            if (j != SIZE-1 && END_SIZE > 26 && j != 31 && !nl)
                fprintf(file, ",");
            
            if (END_SIZE > 26 && j == 31)
            {
                fprintf(file, "\n");
                fprintf(file, ".word");
            }
        }
        fprintf(file, "\n");
    }

    assert(counter == SIZE * SIZE);
}

void take_subarray(int *array_source, int ***array_dest, unsigned int size_num)
{
    assert(array_source != NULL);
    assert(size_num > 0);

    // Allocate memory for the array of pointers (rows)
    *array_dest = (int **)malloc(size_num * sizeof(int *));

    // Check for malloc failure
    if (*array_dest == NULL)
    {
        fprintf(stderr, "Memory allocation for rows failed\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each row
    for (unsigned  i = 0; i < size_num; i++)
    {
        (*array_dest)[i] = (int *)malloc(size_num * sizeof(int));

        // Check for malloc failure
        if ((*array_dest)[i] == NULL)
        {
            fprintf(stderr, "Memory allocation for row %d failed\n", i);

            // Free previously allocated memory to avoid memory leaks
            for (unsigned int j = 0; j < i; j++)
            {
                free((*array_dest)[j]);
            }
            free(*array_dest);
            exit(EXIT_FAILURE);
        }
    }

    // Create 2d subarray
    unsigned int counter = 0;
    for (unsigned int i = 0; i < size_num; i++)
    {
        for (unsigned int j = 0; j < size_num; j++)
        {
            (*array_dest)[i][j] = array_source[counter++];
        }
    }
}

void destroy_subarray(int **array, int size_num)
{
    // Free memory for each row
    for (int i = 0; i < size_num; i++)
    {
        free(array[i]);
    }

    // Free memory for the array of pointers (rows)
    free(array);
}
