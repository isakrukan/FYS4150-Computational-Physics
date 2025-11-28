

#include "EvolverAlgorithm.hpp"


// CrankNicolsen::CrankNicolsen(const unsigned int M, arma::cx_vec a, arma::cx_vec b, arma::cx_double r) : EvolverAlgorithm(M), a(a), b(b), r(r){
//     initialize();
// }

CrankNicolsen::CrankNicolsen(const unsigned int M, arma::cx_mat V_, double h, double Delta_t) : EvolverAlgorithm(M), h(h), Delta_t(Delta_t){
    V = V_;
    initialize();
}


CrankNicolsen::CrankNicolsen(double h, double Delta_t) : h(h), Delta_t(Delta_t){
    h_Delta_t_set = true;
    if ((h_Delta_t_set && V_set) && (M_set && !initialized)){
        initialize();
    }
};


void CrankNicolsen::set_potential(arma::cx_mat V_){
    V = V_;
    if ((h_Delta_t_set && V_set) && (M_set && !initialized)){
        initialize();
    }
}
void CrankNicolsen::set_grid_size(const unsigned int M_){
    M = M_;
    if ((h_Delta_t_set && V_set) && (M_set && !initialized)){
        initialize();
    }
}

void CrankNicolsen::initialize(){
    // Preallocating result of B*u^n:
    b_result.set_size(M-2, M-2);

    a.set_size(M-2);
    b.set_size(M-2);
    r = i_unit * Delta_t / (2.0 * h*h);
    for (int k=0; k<M-2; k++){
        a[k] = arma::cx_double(1,0) + 4.0*r + i_unit * Delta_t/2.0 * V(k,k);
        b[k] = arma::cx_double(1,0) - 4.0*r - i_unit * Delta_t/2.0 * V(k,k);
    }

    // Creating matrices:
    create_A_matrix();
    create_B_matrix();
}
// Private methods:
void CrankNicolsen::create_matrix(arma::sp_cx_mat &C, arma::cx_vec c, arma::cx_double r_) {

    for (int ij=0; ij<C.n_rows-(M-2); ij += M-2){ // Along the diagonal, except last
        for (int k=ij; k < ij + M-3; k++){

            // Matrix on diagonal:
            C(k, k) = c(k);
            C(k+1, k) = r_;
            C(k, k+1) = r_;

            // "Matrices" away from diagonal:
            C(k, k+M-2) = r_;
            C(k+M-2, k) = r_;
        }

        // Matrix on diagonal:
        C(ij+M-3, ij+M-3) = c(ij+M-3);

        // // "Matrices" away from diagonal:
        C(ij+M-3, ij+M-3+M-2) = r_;
        C(ij+M-3+M-2, ij+M-3) = r_;
    }

    // Last matrix on diagonal:
    for (int k=C.n_rows-(M-2); k < C.n_rows - 1; k++){
        C(k, k) = c(k);
        C(k+1, k) = r_;
        C(k, k+1) = r_;
    }

    // // Lower right element:
    C(C.n_rows-1, C.n_rows-1) = c(C.n_rows-1);
}


int CrankNicolsen::get_flat_index(int i, int j, unsigned int M){
    return i*(M-2) + j;
}

arma::cx_mat CrankNicolsen::update_u(const unsigned int n){
    b_result = B * u_internal[n];
    // u_internal[n+1] = arma::spsolve(A, b_result);
    return arma::spsolve(A, b_result);
}


// Public methods:
void CrankNicolsen::create_A_matrix(){
    int size = (M-2)*(M-2);
    A.set_size(size, size);
    create_matrix(A, a, -r);
}

void CrankNicolsen::create_B_matrix(){
    int size = (M-2)*(M-2);
    B.set_size(size, size);
    create_matrix(B, b, r);
}