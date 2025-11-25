#include "../include/CrankNicolsen.hpp"
#include <cassert> 
#include <vector>
#include <string>

// A function that prints the structure of a sparse matrix to screen.
void print_sp_matrix_structure(const arma::sp_cx_mat& A)
{
    using namespace std;
    using namespace arma;

    // Declare a C-style 2D array of strings.
    string S[A.n_rows][A.n_cols];  

    // Initialise all the strings to " ".
    for (int i =0; i < A.n_rows; i++)
    {
        for (int j = 0; j < A.n_cols; j++)
        {
            S[i][j] = " ";
        }
    }

    // Next, we want to set the string to a dot at each non-zero element.
    // To do this we use the special loop iterator from the sp_cx_mat class
    // to help us loop over only the non-zero matrix elements.
    sp_cx_mat::const_iterator it     = A.begin();
    sp_cx_mat::const_iterator it_end = A.end();

    int nnz = 0;
    for(it; it != it_end; ++it)
    {
        S[it.row()][it.col()] = "•";
        nnz++;
    }

    // Finally, print the matrix to screen.
    cout << endl;
    for (int i =0; i < A.n_rows; i++)
    {
        cout << "| ";
        for (int j = 0; j < A.n_cols; j++)
        {
            cout << S[i][j] << " ";
        }
        cout <<  "|\n";
    }

    cout << endl;
    cout << "matrix size: " << A.n_rows << "x" << A.n_cols << endl;
    cout << "non-zero elements: " << nnz << endl ;
    cout << endl;
}

void test_matrix_initialization(){
    /**
     * Tests using explicit forms for A and B for the M=5,6 cases that these matrices are initialized correctly by class CrankNicolsen
     */

    unsigned int N = 1;
    unsigned int M9 = 5;
    unsigned int M16 = 6;
    arma::cx_vec a = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    arma::cx_vec b = {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    arma::cx_double r = 0.1;

    // Expected:
    arma::cx_mat A_9x9 = {
        { a[0], -r,   0,    -r,   0,    0,    0,    0,    0    },
        { -r,   a[1], -r,   0,    -r,   0,    0,    0,    0    },
        {  0,   -r,   a[2], 0,    0,    -r,   0,    0,    0    },
        { -r,   0,    0,    a[3], -r,   0,    -r,   0,    0    },
        {  0,   -r,   0,    -r,   a[4], -r,   0,    -r,   0    },
        {  0,   0,    -r,   0,    -r,   a[5], 0,    0,    -r   },
        {  0,   0,    0,    -r,   0,    0,    a[6], -r,   0    },
        {  0,   0,    0,    0,    -r,   0,    -r,   a[7], -r   },
        {  0,   0,    0,    0,    0,    -r,   0,    -r,   a[8] }
    };

    arma::cx_mat B_9x9 = {
        { b[0], r,    0,    r,    0,    0,    0,    0,    0    },
        { r,    b[1], r,    0,    r,    0,    0,    0,    0    },
        { 0,    r,    b[2], 0,    0,    r,    0,    0,    0    },
        { r,    0,    0,    b[3], r,    0,    r,    0,    0    },
        { 0,    r,    0,    r,    b[4], r,    0,    r,    0    },
        { 0,    0,    r,    0,    r,    b[5], 0,    0,    r    },
        { 0,    0,    0,    r,    0,    0,    b[6], r,    0    },
        { 0,    0,    0,    0,    r,    0,    r,    b[7], r    },
        { 0,    0,    0,    0,    0,    r,    0,    r,    b[8] }
    };

    arma::cx_mat A_16x16 = {
    { a[0], -r,    0,     0,   -r,   0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    { -r,   a[1], -r,     0,   0,    -r,   0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    { 0,    -r,   a[2],  -r,   0,    0,    -r,   0,    0,    0,    0,     0,     0,     0,     0,     0     },
    { 0,    0,    -r,    a[3], 0,    0,    0,    -r,   0,    0,    0,     0,     0,     0,     0,     0     },
    { -r,   0,    0,     0,    a[4], -r,   0,    0,    -r,   0,    0,     0,     0,     0,     0,     0     },
    { 0,    -r,   0,     0,    -r,   a[5], -r,   0,    0,    -r,   0,     0,     0,     0,     0,     0     },
    { 0,    0,    -r,    0,    0,    -r,   a[6], -r,   0,    0,    -r,    0,     0,     0,     0,     0     },
    { 0,    0,    0,     -r,   0,    0,    -r,   a[7], 0,    0,    0,     -r,    0,     0,     0,     0     },
    { 0,    0,    0,     0,    -r,   0,    0,    0,    a[8], -r,   0,     0,     -r,    0,     0,     0     },
    { 0,    0,    0,     0,    0,    -r,   0,    0,    -r,   a[9], -r,    0,     0,     -r,    0,     0     },
    { 0,    0,    0,     0,    0,    0,    -r,   0,    0,    -r,   a[10], -r,    0,     0,     -r,    0     },
    { 0,    0,    0,     0,    0,    0,    0,    -r,   0,    0,    -r,    a[11], 0,     0,     0,     -r    },
    { 0,    0,    0,     0,    0,    0,    0,    0,    -r,   0,    0,     0,     a[12], -r,    0,     0     },
    { 0,    0,    0,     0,    0,    0,    0,    0,    0,    -r,   0,     0,     -r,    a[13], -r,    0     },
    { 0,    0,    0,     0,    0,    0,    0,    0,    0,    0,    -r,    0,     0,     -r,    a[14], -r    },
    { 0,    0,    0,     0,    0,    0,    0,    0,    0,    0,    0,     -r,    0,     0,     -r,    a[15] }
    };

    arma::cx_mat B_16x16 = {
    { b[0], r,    0,    0,    r,    0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  r,   b[1], r,    0,    0,    r,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  0,   r,    b[2], r,    0,    0,    r,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  0,    0,   r,    b[3], 0,    0,    0,    r,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  r,    0,   0,    0,    b[4], r,    0,    0,    r,    0,    0,     0,     0,     0,     0,     0     },
    {  0,   r,    0,    0,    r,    b[5], r,    0,    0,    r,    0,     0,     0,     0,     0,     0     },
    {  0,    0,   r,    0,    0,    r,    b[6], r,    0,    0,    r,     0,     0,     0,     0,     0     },
    {  0,    0,   0,    r,    0,    0,    r,    b[7], 0,    0,    0,     r,     0,     0,     0,     0     },
    {  0,    0,   0,    0,    r,    0,    0,    0,    b[8], r,    0,     0,     r,     0,     0,     0     },
    {  0,    0,   0,    0,    0,    r,    0,    0,    r,    b[9], r,     0,     0,     r,     0,     0     },
    {  0,    0,   0,    0,    0,    0,    r,    0,    0,    r,    b[10], r,     0,     0,     r,     0     },
    {  0,    0,   0,    0,    0,    0,    0,    r,    0,    0,    r,     b[11], 0,     0,     0,     r     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    r,    0,    0,     0,     b[12], r,     0,     0     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    0,    r,    0,     0,     r,     b[13], r,     0     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    0,    0,    r,     0,     0,     r,     b[14], r     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    0,    0,    0,     r,     0,     0,     r,     b[15] }
    };

    // Matrices A and B should be created in constructor
    CrankNicolsen test_9x9(N, M9, a, b, r);
    CrankNicolsen test_16x16(N, M16, a, b, r);


    // print_sp_matrix_structure(test_9x9.A);
    // print_sp_matrix_structure(test_16x16.A);
    int size = (M9-2)*(M9-2);
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            
            // Real components:
            assert(test_9x9.A(i,j).real() == A_9x9(i,j));
            assert(test_9x9.B(i,j).real() == B_9x9(i,j));
        }
    }

    size = (M16-2)*(M16-2);
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            
            // Real components:
            assert(test_16x16.A(i,j).real() == A_16x16(i,j));
            assert(test_16x16.B(i,j).real() == B_16x16(i,j));
        }
    }

}