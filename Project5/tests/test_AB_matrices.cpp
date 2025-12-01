#include "../include/CrankNicolsen.hpp"
#include <cassert>


void test_AB_matrices(){
    unsigned int M = 11;
    float a0 = 2; float r = 1;
    arma::mat A(M-2, M-2, arma::fill::zeros);
    
    // diagonal
    A(0,0) = a0;  A(1,1) = a0;  A(2,2) = a0;
    A(3,3) = a0;  A(4,4) = a0;  A(5,5) = a0;
    A(6,6) = a0;  A(7,7) = a0;  A(8,8) = a0;

    // row 0
    A(0,1) = -r;  A(0,3) = -r;

    // row 1
    A(1,0) = -r;  A(1,2) = -r;  A(1,4) = -r;

    // row 2
    A(2,1) = -r;  A(2,5) = -r;

    // row 3
    A(3,0) = -r;  A(3,4) = -r;  A(3,6) = -r;

    // row 4
    A(4,1) = -r;  A(4,3) = -r;  A(4,5) = -r;  A(4,7) = -r;

    // row 5
    A(5,2) = -r;  A(5,4) = -r;  A(5,8) = -r;

    // row 6
    A(6,3) = -r;  A(6,7) = -r;

    // row 7
    A(7,4) = -r;  A(7,6) = -r;  A(7,8) = -r;

    // row 8
    A(8,5) = -r;  A(8,7) = -r;


    CrankNicolsen testCrank(M, M, a0, a0, r);
    testCrank.create_A_matrix(a0, r);

    for (int i=0; i<M-2; i++){
        for (int j=0; j<M-2; j++){
            std::cout << testCrank.A(i,j) << std::endl;
        }
    }


}