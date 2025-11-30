

#include "EvolverAlgorithm.hpp"
#include "utils.hpp"

//********** Constructors **********//


EvolverAlgorithm::EvolverAlgorithm(double h, double Delta_t){

    M = int(1.0/h + 1.0);
    N = int(1.0/Delta_t + 1.0);
    this->h = double(1.0/(M-1));
    this->Delta_t = double(1.0/(N-1));
    M_set = true;
    N_set = true;
}

CrankNicolsen::CrankNicolsen(double h, double Delta_t) : EvolverAlgorithm(h, Delta_t){};

CrankNicolsen::CrankNicolsen(arma::cx_mat &V_, double h, double Delta_t) : EvolverAlgorithm(h, Delta_t){
    V = V_;
    V_set = true;
    initialize();
}

//********** Private methods **********//

void CrankNicolsen::initialize(){

    if (((M_set && N_set) && V_set) && (M_set && !initialized)){
        int size = (M-2)*(M-2);
        // Preallocating result of B*u^n:
        b_result.set_size(size);

        a.set_size(size);
        b.set_size(size);

        r = i_unit * Delta_t / (2.0 * h*h);
        for (int i=0; i < M-2; ++i) {
            for (int j=0; j < M-2; ++j) {
                int k = get_flat_index(i,j,M-2);
                a[k] = 1.0 + 4.0*r + i_unit * Delta_t/2.0 * V(i, j);
                b[k] = 1.0 - 4.0*r - i_unit * Delta_t/2.0 * V(i, j);
            }
        }



        // Creating matrices:
        create_A_matrix();
        create_B_matrix();

        initialized = true;   
    }
}
// Private methods:
void CrankNicolsen::create_matrix(arma::sp_cx_mat &C, arma::cx_vec &c, arma::cx_double r_) {

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

void CrankNicolsen::create_A_matrix(){
    int size = (M-2)*(M-2);
    A.set_size(size, size);
    create_matrix(A, a, -r);
    // print_sp_matrix_structure(A);
}

void CrankNicolsen::create_B_matrix(){
    int size = (M-2)*(M-2);
    B.set_size(size, size);
    create_matrix(B, b, r);
}

//********** Public methods **********//

arma::cx_vec CrankNicolsen::update_u(arma::cx_vec &u_internal){

    b_result = B * u_internal;

    arma::cx_vec u_new;  
    arma::spsolve(u_new, A, b_result, "superlu");

    return u_new;

}

void CrankNicolsen::set_potential(arma::cx_mat &V_){
    V = V_;
    V_set = true;
    initialize();
}
void CrankNicolsen::set_grid_size(const unsigned int M_){
    M = M_;
    h = double(1.0/(M-1));
    M_set = true;
    initialize();
}