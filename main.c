#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N   16
#define BS  N/2

void printMatrix (int matrix[N][N]);
void printHalfMatrix (int matrix[BS][BS]);

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


    if (world_size < 4) {
        fprintf(stderr, "World size must be greater than 4 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (world_rank == 0){
    int i, j, a[N][N], b[N][N], c[N][N], a11[BS][BS], b11[BS][BS],a12[BS][BS], b12[BS][BS],a21[BS][BS], b21[BS][BS],a22[BS][BS], b22[BS][BS];

        for (i=0; i<N; i++) {
            for (j=0; j<N; j++) {
                a[i][j]= rand() % 10;
                b[i][j]= rand() % 10;
            }
        }


        printf("Primeira Matriz:\n");
        printMatrix(a);
        printf("Segunda matriz:\n");
        printMatrix(b);

        

        for (i=0; i<BS; i++) {
            for (j=0; j<BS; j++) {
                a11[i][j] = a[i][j];
                b11[i][j] = b[i][j];

                a12[i][j] = a[i][BS+j];
                b12[i][j] = b[i][BS+j];

                a21[i][j] = a[BS+i][j];
                b21[i][j] = b[BS+i][j];

                a22[i][j] = a[BS+i][BS+j];
                b22[i][j] = b[BS+i][BS+j];
            }
        }
        
        MPI_Send(&a11[0][0], BS*BS, MPI_INT,0,0, MPI_COMM_WORLD);
        MPI_Send(&b11[0][0], BS*BS, MPI_INT,0,0, MPI_COMM_WORLD);

        MPI_Send(&a12[0][0], BS*BS, MPI_INT,1,0, MPI_COMM_WORLD);
        MPI_Send(&b12[0][0], BS*BS, MPI_INT,1,0, MPI_COMM_WORLD);
        
        MPI_Send(&a21[0][0], BS*BS, MPI_INT,2,0, MPI_COMM_WORLD);
        MPI_Send(&b21[0][0], BS*BS, MPI_INT,2,0, MPI_COMM_WORLD);

        MPI_Send(&a22[0][0], BS*BS, MPI_INT,3,0, MPI_COMM_WORLD);
        MPI_Send(&b22[0][0], BS*BS, MPI_INT,3,0, MPI_COMM_WORLD);

    }

     //workers

    int i, j, a[BS][BS], b[BS][BS], c[BS][BS];

    MPI_Recv(&a, BS*BS, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&b, BS*BS, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    //printf("Processo %d \n", world_rank);
    //printf("Matriz A: \n");
    //printHalfMatrix(a);
    //printf("Matriz B: \n");
    //printHalfMatrix(b);

    for (i=0; i<BS; i++) {
            for (j=0; j<BS; j++) {
                c[i][j] = a[i][j] + b[i][j];
            }
        }

    //printf("Matriz Somada: \n");
    //printHalfMatrix(c);

    MPI_Send(&c, BS*BS, MPI_INT, 0, 0, MPI_COMM_WORLD);

    if (world_rank == 0){
        int c11[BS][BS], c12[BS][BS], c21[BS][BS], c22[BS][BS], cf[N][N];
        
        MPI_Recv(&c11, BS*BS, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&c12, BS*BS, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&c21, BS*BS, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&c22, BS*BS, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


        for (i=0; i<BS; i++) {
            for (j=0; j<BS; j++) {
                cf[i][j] = c11[i][j];
                cf[i][BS+j] = c12[i][j];
                cf[BS+i][j] = c21[i][j];
                cf[BS+i][BS+j] = c22[i][j];
            }
        }
        printf("Matriz Final Somada: \n");
        printMatrix(cf);
    }
    MPI_Finalize();
}

void printMatrix (int matrix[N][N])
{
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%d \t", matrix[i][j]);
        printf ("\n");
    }
    printf ("\n");
}

void printHalfMatrix (int matrix[BS][BS])
{
    int i, j;
    for (i = 0; i < BS; i++) {
        for (j = 0; j < BS; j++)
            printf("%d \t", matrix[i][j]);
        printf ("\n");
    }
    printf ("\n");
}