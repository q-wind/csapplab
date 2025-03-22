/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans_32_32(int M, int N, int A[N][M], int B[M][N]);
void trans_64_64(int M, int N, int A[N][M], int B[M][N]);
void trans_61_67(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32 && N == 32)
        trans_32_32(M, N, A, B);
    else if (M == 64 && N == 64)
        trans_64_64(M, N, A, B);
    else
        trans_61_67(M, N, A, B);
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char trans_col_desc[] = "column-wise scan transpose";
void trans_col(int M, int N, int A[N][M], int B[M][N])
{
    // the same result with row-scan
    int i, j;

    for (j = 0; j < M; j++) {
        for (i = 0; i < N; i++) {
            B[j][i] = A[i][j];
        }
    }
}

char trans_zigzag_desc[] = "zig-zag access pattern";
void trans_zigzag(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        if (i % 2 == 0)
            for (j = 0; j < M; j++) {
                B[j][i] = A[i][j];
            }
        else
            for (j = M-1; j >= 0; j--) {
                B[j][i] = A[i][j];
            }
    }
}

char trans_32_32_desc[] = "for 32*32 matrix";
void trans_32_32(int M, int N, int A[N][M], int B[M][N])
{
    // s = 5, E = 1, b = 5: 32 sets, 1 line each set, 32 bytes(8 int) each line
    // A[32][32]: 一行 32int 占据 4 cache line(set)，八行填满整个 cache，然后再重新计投射的 set
    // 对应 set 关系：
    // 0 1 2 3
    // 4  ... 7
    //    ...
    // 28 ... 31

    // 8 * 8 blocking
    int n, m, i;
    for (n = 0; n < N; n += 8) {
        for (m = 0; m < M; m += 8) {
            for (i = 0; i < 8; i++) {
                // 对角线块 每行冲突替换造成额外miss
                // for (int j = 0; j < 8; j++) {
                //     B[j + m][i + n] = A[i + n][j + m];
                // }

                int a_0 = A[i+n][m], a_1 = A[i+n][1+m], a_2 = A[i+n][2+m], a_3 = A[i+n][3+m];
                int a_4 = A[i+n][4+m], a_5 = A[i+n][5+m], a_6 = A[i+n][6+m], a_7 = A[i+n][7+m];
                B[m][i+n] = a_0; B[1+m][i+n] = a_1; B[2+m][i+n] = a_2; B[3+m][i+n] = a_3; 
                B[4+m][i+n] = a_4; B[5+m][i+n] = a_5; B[6+m][i+n] = a_6; B[7+m][i+n] = a_7; 
                  
            }
        }
    }
}

char trans_64_64_desc[] = "for 64*64 matrix";
void trans_64_64(int M, int N, int A[N][M], int B[M][N])
{
    // s = 5, E = 1, b = 5: 32 sets, 1 line each set, 32 bytes(8 int) each line
    // A[64][64]: 一行 64int 占据 8 cache line(set)，四行填满整个 cache，然后再重新计投射的 set
    // 对应 set 关系：
    // 0 1 2 3 4 5 6 7
    // 8  ... 15
    //    ...
    // 24 ... 31

    // 4 * 4 分块: hit6497 miss1700
    // int n, m, i;
    // for (n = 0; n < N; n += 4) {
    //     for (m = 0; m < M; m += 4) {
    //         for (i = 0; i < 4; i++) {
    //             int a_0 = A[i+n][m], a_1 = A[i+n][1+m], a_2 = A[i+n][2+m], a_3 = A[i+n][3+m];
    //             B[m][i+n] = a_0; B[1+m][i+n] = a_1; B[2+m][i+n] = a_2; B[3+m][i+n] = a_3; 
    //         }
    //     }
    // }
    
    int n, m, i;
    int a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7;
    for (n = 0; n < N; n += 8) {
        for (m = 0; m < M; m += 8) {
            // 8*8 分块，再以 4*4 小块处理

            // A 左上、右上，转置后放在 B 左上、右上
            for (i = n; i < n+4; i++) {
                // 取得A连续8个，1miss
                a_0 = A[i][m], a_1 = A[i][1+m], a_2 = A[i][2+m], a_3 = A[i][3+m];
                a_4 = A[i][4+m], a_5 = A[i][5+m], a_6 = A[i][6+m], a_7 = A[i][7+m];
                // 为了利用B四行 cache，存在两列（i和i+4）中，4miss
                B[m][i] = a_0; B[1+m][i] = a_1; B[2+m][i] = a_2; B[3+m][i] = a_3; 
                B[m][i+4] = a_4; B[1+m][i+4] = a_5; B[2+m][i+4] = a_6; B[3+m][i+4] = a_7;
            }

            // B右上 到 B左下，A左下 到 B 右上
            // for (i = n+4; i < n+8; i++) {
            //     // 取得B i列（右上），hit
            //     a_0 = B[m][i]; a_1 = B[1+m][i]; a_2 = B[2+m][i]; a_3 = B[3+m][i];
            //     // 取得A i行（左下），1miss
            //     a_4 = A[i][m]; a_5 = A[i][1+m]; a_6 = A[i][2+m]; a_7 = A[i][3+m];
            //     // 该B列用A对应行填充
            //     B[m][i] = a_4; B[1+m][i] = a_5; 
            //     B[2+m][i] = a_6; B[3+m][i] = a_7;
            //     // B （右上 -> 左下）
            //     B[4+m][i-4] = a_0; B[5+m][i-4] = a_1; B[6+m][i-4] = a_2; B[7+m][i-4] = a_3;
            // }

            // 此时B的上4行都在cache中
            // B右上 到 B左下，A左下 到 B 右上
            for (i = m; i < m+4; i++) {
                // 取得B右上一行，hit
                a_0 = B[i][n+4]; a_1 = B[i][n+5]; a_2 = B[i][n+6]; a_3 = B[i][n+7];
                // 取得A左下一列，miss
                a_4 = A[n+4][i]; a_5 = A[n+5][i]; a_6 = A[n+6][i]; a_7 = A[n+7][i];
                // 该B行用A对应列填充
                B[i][n+4] = a_4; B[i][n+5] = a_5; B[i][n+6] = a_6; B[i][n+7] = a_7;
                // B原右上->左下  行
                B[i+4][n] = a_0; B[i+4][n+1] = a_1; B[i+4][n+2] = a_2; B[i+4][n+3] = a_3;
            }

            // A 右下 到 B 右下
            for (i = 4; i < 8; i++) {
                a_0 = A[i+n][4+m]; a_1 = A[i+n][5+m]; a_2 = A[i+n][6+m]; a_3 = A[i+n][7+m];
                B[4+m][i+n] = a_0; B[5+m][i+n] = a_1; B[6+m][i+n] = a_2; B[7+m][i+n] = a_3;
            }
        }
    }
}

char trans_61_67_desc[] = "for 61*67 matrix";
void trans_61_67(int M, int N, int A[N][M], int B[M][N])
{
    int n, m, i;
    int block = 17;
    for (n = 0; n < N; n += block) {
        for (m = 0; m < M; m += block) {
            for (i = n; i<N && i<n+block; i++) {
                for (int j = m; j<M && j<m+block; j++) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }
}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    // registerTransFunction(trans_col, trans_col_desc);
    // registerTransFunction(trans_zigzag, trans_zigzag_desc);
    // registerTransFunction(trans_32_32, trans_32_32_desc);
    // registerTransFunction(trans_64_64, trans_64_64_desc);
    registerTransFunction(trans_61_67, trans_61_67_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

