#include "helpers.h"

void gen_rand_mat(double* A, int m, int n, int seed)
{
    // m - no. of rows
    // n - no. of columns
    srand(seed);
    for(unsigned int i = 0 ; i < m; i++)
    {
        for(unsigned int j = 0; j < n; j++)
        {
            A[i*n + j] = (double)rand() / RAND_MAX;
        }
    }
}

bool is_correct(double* Cs, double* Cp, int siz)
{
    bool flag = true;
    for (unsigned int i = 0 ; i < siz; i++)
    {
        if(fabs(Cs[i] - Cp[i]) > 1e-5)flag = false;
    }
    return flag;
}

void print_mat(double* A, int m, int n)
{
    //m - no. of rows
    //n - no. of cols
    for(unsigned int i = 0; i < m; i++)
    {
        for(unsigned int j = 0; j < n; j++)
        {
            printf("%f, ", A[i*n + j]);
        }
        printf("\n");
    }
}

void serial_multiply(double* A, double* B, double* C, int m, int n, int p)
{
    // A - mxn
    // B - nxp
    // C - mxp = A*B
    // #pragma omp parallel for
    for(unsigned int i = 0; i < m; i++)
    {
        for(unsigned int k = 0; k < n; k++)
        {
            for(unsigned int j = 0; j < p; j++)
            {
                C[p*i + j] += A[n*i + k] * B[p*k + j];
            }
        }
    }
}

void write_mat(double* A, char matname, int m, int n)
{
    const char* filename = "/Users/john/Programs/parallel_matmul/output/mat.txt";
    FILE* fptr = fopen(filename, "a");
    if (fptr == NULL)
    {
        fprintf(stderr, "Cannot open %s for appending\n", filename);
    }
    fprintf(fptr, "%c,%d,%d\n", matname, m, n);

    for(unsigned int i = 0; i < m; i++)
    {
        for(unsigned int j = 0; j < n; j++)
        {
            if( j != n - 1)fprintf(fptr, "%0.10f,", A[i*n + j]);
            else fprintf(fptr, "%0.10f", A[i*n + j]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);

}
