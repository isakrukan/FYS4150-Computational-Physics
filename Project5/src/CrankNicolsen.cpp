#include "CrankNicolsen.hpp"


int CrankNicolsen::get_flat_index(int i, int j, unsigned int M){
    return i*(M-2) + j;
}

void CrankNicolsen::create_A_matrix(float a0, float r){

}

void CrankNicolsen::create_B_matrix(float b0, float r){
    
}