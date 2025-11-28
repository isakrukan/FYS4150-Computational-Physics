#include "CrankNicolson.hpp"


CrankNicolson::CrankNicolson(unsigned int N, unsigned int M, arma::cx_vec a, arma::cx_vec b, arma::cx_double r) : N(N), M(M), a(a), b(b), r(r), i_unit(0,1){
    
    create_A_matrix();
    create_B_matrix();
}

CrankNicolson::CrankNicolson(unsigned int N, unsigned int M, arma::cx_mat V, double h, double Delta_t, arma::cx_double r) : N(N), M(M), r(r), i_unit(0,1){
    
    int size = (M-2)*(M-2);
    a.set_size(size);
    b.set_size(size);
    
    for (int k=0; k<M-2; k++){
        a[k] = arma::cx_double(1,0) + 4.0*r + i_unit * Delta_t/2.0 * V(k,k);
        b[k] = arma::cx_double(1,0) - 4.0*r - i_unit * Delta_t/2.0 * V(k,k);
    }
    
    b_result.set_size(size, 1);
    u.resize(N, arma::sp_cx_mat(size, 1));
    create_A_matrix();
    create_B_matrix();
}


// Private methods:
void CrankNicolson::create_matrix(arma::sp_cx_mat &C, arma::cx_vec c, arma::cx_double r_) {

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


void CrankNicolson::update_u(const unsigned int n){
    b_result = B * u[n];
    u[n+1] = arma::spsolve(A, b_result, "superlu");

    // Had some problems getting spsolve to work...
    //
    // using "lapack":
    //   > warning: spsolve(): not enough memory to use LAPACK based solver
    //   > terminate called after throwing an instance of 'std::runtime_error'
    //   > what():  spsolve(): solution not found
    //
    // using "superlu":
    //   > terminate called after throwing an instance of 'std::logic_error'
    //   > what() : spsolve() : use of SuperLU must be enabled
}


// Public methods:
void CrankNicolson::create_A_matrix(){
    int size = (M-2)*(M-2);
    A.set_size(size, size);
    create_matrix(A, a, -r);
}

void CrankNicolson::create_B_matrix(){
    int size = (M-2)*(M-2);
    B.set_size(size, size);
    create_matrix(B, b, r);
}

void CrankNicolson::initial_u(arma::cx_mat u_){
    if ((u_.n_rows == u_.n_cols) && (u_.n_rows == M-2)){
        u[0] = u_;
    }
}

void CrankNicolson::solve_time_evolution() {
    
    // Temporary...
    for (unsigned int n = 0; n < N - 1; ++n) {
        update_u(n);
    }
}