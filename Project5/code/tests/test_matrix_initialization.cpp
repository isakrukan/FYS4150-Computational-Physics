#include "../include/EvolverAlgorithm.hpp"
#include <cassert> 
#include <vector>
#include <string>
#include <complex> 

void test_matrix_initialization(){
    /**
     * Tests using explicit forms for A and B for the M=5,6 cases that these matrices are initialized correctly by class CrankNicolson
     */

    // Parameters
    double eps = 1e-15; // Tolerance

    unsigned int N = 1;
    unsigned int M9 = 5;
    unsigned int M16 = 6;
    double h9 = 1.0 / (M9 - 1); 
    double h16 = 1.0 / (M16 - 1); 
    double dt = 0.000025;

    arma::cx_double r9 = arma::cx_double(0,1) * dt / (2.0 * h9*h9);
    arma::cx_double r16 = arma::cx_double(0,1) * dt / (2.0 * h16*h16);

    arma::cx_double i_unit(0, 1);

    arma::cx_double alpha9  = 1.0 + 4.0*r9  + i_unit * dt / 2.0;
    arma::cx_double beta9   = 1.0 - 4.0*r9  - i_unit * dt / 2.0;
    arma::cx_double alpha16 = 1.0 + 4.0*r16 + i_unit * dt / 2.0;
    arma::cx_double beta16  = 1.0 - 4.0*r16 - i_unit * dt / 2.0;


    // Computed:
    arma::cx_mat V9(M9-2, M9-2, arma::fill::ones);
    arma::cx_mat V16(M16-2, M16-2, arma::fill::ones); 
    CrankNicolsen test_9x9(V9, h9, dt);
    CrankNicolsen test_16x16(V16, h16, dt);

    // Expected:
    arma::cx_mat A_9x9 = {
        { alpha9, -r9,    0,     -r9,   0,     0,     0,     0,     0 },
        { -r9,    alpha9, -r9,   0,     -r9,   0,     0,     0,     0 },
        { 0,      -r9,    alpha9,0,     0,     -r9,   0,     0,     0 },
        { -r9,    0,      0,     alpha9,-r9,   0,     -r9,   0,     0 },
        { 0,      -r9,    0,     -r9,   alpha9,-r9,   0,     -r9,   0 },
        { 0,      0,      -r9,   0,     -r9,   alpha9,0,     0,     -r9 },
        { 0,      0,      0,     -r9,   0,     0,     alpha9,-r9,   0 },
        { 0,      0,      0,     0,     -r9,   0,     -r9,   alpha9,-r9 },
        { 0,      0,      0,     0,     0,     -r9,   0,     -r9,   alpha9 }
    };

    arma::cx_mat B_9x9 = {
        { beta9,  r9,     0,     r9,    0,     0,     0,     0,     0 },
        { r9,     beta9,  r9,    0,     r9,    0,     0,     0,     0 },
        { 0,      r9,     beta9, 0,     0,     r9,    0,     0,     0 },
        { r9,     0,      0,     beta9, r9,    0,     r9,    0,     0 },
        { 0,      r9,     0,     r9,    beta9, r9,    0,     r9,    0 },
        { 0,      0,      r9,    0,     r9,    beta9, 0,     0,     r9 },
        { 0,      0,      0,     r9,    0,     0,     beta9, r9,    0 },
        { 0,      0,      0,     0,     r9,    0,     r9,    beta9, r9 },
        { 0,      0,      0,     0,     0,     r9,    0,     r9,    beta9 }
    };


    arma::cx_mat A_16x16 = {
    { alpha16, -r16,    0,     0,   -r16,   0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    { -r16,   alpha16, -r16,     0,   0,    -r16,   0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    { 0,    -r16,   alpha16,  -r16,   0,    0,    -r16,   0,    0,    0,    0,     0,     0,     0,     0,     0     },
    { 0,    0,    -r16,    alpha16, 0,    0,    0,    -r16,   0,    0,    0,     0,     0,     0,     0,     0     },
    { -r16,   0,    0,     0,    alpha16, -r16,   0,    0,    -r16,   0,    0,     0,     0,     0,     0,     0     },
    { 0,    -r16,   0,     0,    -r16,   alpha16, -r16,   0,    0,    -r16,   0,     0,     0,     0,     0,     0     },
    { 0,    0,    -r16,    0,    0,    -r16,   alpha16, -r16,   0,    0,    -r16,    0,     0,     0,     0,     0     },
    { 0,    0,    0,     -r16,   0,    0,    -r16,   alpha16, 0,    0,    0,     -r16,    0,     0,     0,     0     },
    { 0,    0,    0,     0,    -r16,   0,    0,    0,    alpha16, -r16,   0,     0,     -r16,    0,     0,     0     },
    { 0,    0,    0,     0,    0,    -r16,   0,    0,    -r16,   alpha16, -r16,    0,     0,     -r16,    0,     0     },
    { 0,    0,    0,     0,    0,    0,    -r16,   0,    0,    -r16,   alpha16, -r16,    0,     0,     -r16,    0     },
    { 0,    0,    0,     0,    0,    0,    0,    -r16,   0,    0,    -r16,    alpha16, 0,     0,     0,     -r16    },
    { 0,    0,    0,     0,    0,    0,    0,    0,    -r16,   0,    0,     0,     alpha16, -r16,    0,     0     },
    { 0,    0,    0,     0,    0,    0,    0,    0,    0,    -r16,   0,     0,     -r16,    alpha16, -r16,    0     },
    { 0,    0,    0,     0,    0,    0,    0,    0,    0,    0,    -r16,    0,     0,     -r16,    alpha16, -r16    },
    { 0,    0,    0,     0,    0,    0,    0,    0,    0,    0,    0,     -r16,    0,     0,     -r16,    alpha16 }
    };

    arma::cx_mat B_16x16 = {
    { beta16, r16,    0,    0,    r16,    0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  r16,   beta16, r16,    0,    0,    r16,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  0,   r16,    beta16, r16,    0,    0,    r16,    0,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  0,    0,   r16,    beta16, 0,    0,    0,    r16,    0,    0,    0,     0,     0,     0,     0,     0     },
    {  r16,    0,   0,    0,    beta16, r16,    0,    0,    r16,    0,    0,     0,     0,     0,     0,     0     },
    {  0,   r16,    0,    0,    r16,    beta16, r16,    0,    0,    r16,    0,     0,     0,     0,     0,     0     },
    {  0,    0,   r16,    0,    0,    r16,    beta16, r16,    0,    0,    r16,     0,     0,     0,     0,     0     },
    {  0,    0,   0,    r16,    0,    0,    r16,    beta16, 0,    0,    0,     r16,     0,     0,     0,     0     },
    {  0,    0,   0,    0,    r16,    0,    0,    0,    beta16, r16,    0,     0,     r16,     0,     0,     0     },
    {  0,    0,   0,    0,    0,    r16,    0,    0,    r16,    beta16, r16,     0,     0,     r16,     0,     0     },
    {  0,    0,   0,    0,    0,    0,    r16,    0,    0,    r16,    beta16, r16,     0,     0,     r16,     0     },
    {  0,    0,   0,    0,    0,    0,    0,    r16,    0,    0,    r16,     beta16, 0,     0,     0,     r16     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    r16,    0,    0,     0,     beta16, r16,     0,     0     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    0,    r16,    0,     0,     r16,     beta16, r16,     0     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    0,    0,    r16,     0,     0,     r16,     beta16, r16     },
    {  0,    0,   0,    0,    0,    0,    0,    0,    0,    0,    0,     r16,     0,     0,     r16,     beta16 }
    };



    int size = (M9-2)*(M9-2);
    arma::sp_cx_mat A_computed_9x9 = test_9x9.get_A_matrix();
    arma::sp_cx_mat B_computed_9x9 = test_9x9.get_B_matrix();
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            

            double norm1 = std::abs(static_cast<arma::cx_double>(A_computed_9x9(i,j)) - A_9x9(i,j));
            double norm2 = std::abs(static_cast<arma::cx_double>(B_computed_9x9(i,j)) - B_9x9(i,j));

            assert(norm1 < eps);
            assert(norm2 < eps);
        }
    }

    size = (M16-2)*(M16-2);
    arma::sp_cx_mat A_computed_16x16 = test_16x16.get_A_matrix();
    arma::sp_cx_mat B_computed_16x16 = test_16x16.get_B_matrix();
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            
            double norm1 = std::abs(static_cast<arma::cx_double>(A_computed_16x16(i,j)) - A_16x16(i,j));
            double norm2 = std::abs(static_cast<arma::cx_double>(B_computed_16x16(i,j)) - B_16x16(i,j));

            assert(norm1 < eps);
            assert(norm2 < eps);
        }
    }

}