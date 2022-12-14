#include "helpers.h"
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int kRank, kNumPes;

    MPI_Comm_rank(MPI_COMM_WORLD, &kRank);
    MPI_Comm_size(MPI_COMM_WORLD, &kNumPes);

    if (argc != 4) {
        if (kRank == 0) {
            fprintf(stderr, "usage is: %s <M> <N> <P>\n", argv[0]);
        }
        exit(1);
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int P = atoi(argv[3]);
    int RowPerProc = 0;

    int seedb = 0;
    double* B = (double*)malloc(N * P * sizeof(double));
    gen_rand_mat(B, N, P, seedb);

    int root_rank = 0;

    if(M % kNumPes == 0){
        RowPerProc = M / kNumPes;
    }
    else{
        RowPerProc = M / kNumPes;
        if(kRank < M % kNumPes) RowPerProc+=1;
    }
    

    if(kRank == 0)
    {
        assert(M >= kNumPes);
        double* As = (double*)malloc(RowPerProc * N * sizeof(double));
        double* Cs = (double*)calloc(RowPerProc * P, sizeof(double));
        int seeda  = 1;
        double* A  = (double*)malloc(M * N * sizeof(double));
        gen_rand_mat(A, M, N, seeda);
        double* C  = (double*)malloc(M * P * sizeof(double));
        double* Cserial = (double*)calloc(M * P, sizeof(double));

        int rowperproc[kNumPes];
        for (unsigned int i = 0; i < kNumPes; i++)
        {
            if(M % kNumPes == 0){
                rowperproc[i] = M / kNumPes;
            }
            else{
                rowperproc[i] = M / kNumPes;
                if(i < M % kNumPes) rowperproc[i]+=1;
            }     
        }
        // for (unsigned int i = 0 ; i < kNumPes; i++)
        // {
        //     printf("%d ,", rowperproc[i]);
        // }
        int counts[kNumPes];
        int displacements[kNumPes];
        for (unsigned int i = 0; i < kNumPes; i++)
        {
            counts[i]        = rowperproc[i] * N; 
        }
        displacements[0] = 0;
        for (unsigned int i = 1; i < kNumPes; i++)
        {
            displacements[i] = displacements[i - 1] + counts[i - 1];
        }
        // printf("Counts\n");
        // for(int i = 0; i < kNumPes; i++)
        // {
        //     printf("%d ,", counts[i]);
        // }
        // printf("Disp\n");
        // for(int i = 0; i < kNumPes; i++)
        // {
        //     printf("%d ,", displacements[i]);
        // }
        int cnts[kNumPes];
        int disp[kNumPes];
        for (unsigned int i = 0; i < kNumPes; i++)
        {
            cnts[i] = rowperproc[i] * P; 
        }
        disp[0] = 0;
        for (unsigned int i = 1; i < kNumPes; i++)
        {
            disp[i] = disp[i - 1] + cnts[i - 1];
        }
        clock_t begin = clock();
        MPI_Scatterv(A, counts, displacements, MPI_DOUBLE, As, RowPerProc * N, 
        MPI_DOUBLE, root_rank, MPI_COMM_WORLD);
        
        
        serial_multiply(As, B, Cs, RowPerProc, N, P);
        
        MPI_Gatherv(Cs, RowPerProc * P, MPI_DOUBLE, C, cnts, disp, 
        MPI_DOUBLE, root_rank, MPI_COMM_WORLD);

        // serial_multiply(A, B, Cserial, M, N, P);
        // bool correct = is_correct(Cserial, C, M * P);
        // printf(correct ? "True" : "False");
        
        // printf("A --- \n");
        // print_mat(A, M, N);
        char mat_name = 'A';
        write_mat(A, mat_name, M, N);
        mat_name = 'B';
        write_mat(B, mat_name, N, P);
        mat_name = 'C';
        write_mat(C, mat_name, M, P);
        // printf("B --- \n");
        // print_mat(B, N, P);
        // printf("C --- \n");
        // print_mat(C, M, P);
        // printf("Cserial -- \n");
        // print_mat(Cserial, M, P);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Time taken: %f secs\n",time_spent);
        free(As);
        free(Cs);
        free(A);
        free(C);
        free(Cserial);
    }

    else 
    {
        double* As = (double*)malloc(RowPerProc * N * sizeof(double));
        double* Cs = (double*)calloc(RowPerProc * P, sizeof(double));

        MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, As, RowPerProc * N,
        MPI_DOUBLE, root_rank, MPI_COMM_WORLD);

        serial_multiply(As, B, Cs, RowPerProc, N, P);
        
        MPI_Gatherv(Cs, RowPerProc * P, MPI_DOUBLE, NULL, NULL, NULL, 
        MPI_DOUBLE, root_rank, MPI_COMM_WORLD);
        free(As);
        free(Cs);
    }
    // serial_multiply(A, B, C, M, N, P);

    // printf("A --- \n");
    // print_mat(A, M, N);
    // printf("B --- \n");
    // print_mat(B, N, P);
    // printf("C --- \n");
    // print_mat(C, M, P);

    free(B);
    MPI_Finalize();
    return EXIT_SUCCESS;

}
