#ifndef __CrankNicolson_hpp__
#define __CrankNicolson_hpp__

#include <armadillo>

class CrankNicolson
{

private:

unsigned int N; ///< Time points 
unsigned int M; ///< Space points 
arma::cx_vec a;
arma::cx_vec b; 
arma::cx_double r;
const arma::cx_double i_unit;
arma::cx_vec b_result; ///< For storing result of \f$B\vec{u}^n = \vec{b}\f$.


/**
 * @brief Creates block diagonal matrix.
 * @param C Matrix to form as block diagonal matrix
 * @param c Elements along the diagonal of C.
 * @param r_ Elements along diagonal of matrices to subdiagonals.
 */
void create_matrix(arma::sp_cx_mat &C, arma::cx_vec c, arma::cx_double r_);

/**
 * @brief Computes the next element of \ref CrankNicolson::u "u".
 * @param n Index for \ref CrankNicolson::u "u".
 */
void update_u(const unsigned int n); 

public:



CrankNicolson(unsigned int N, unsigned int M, arma::cx_vec a, arma::cx_vec b, arma::cx_double r);
CrankNicolson(unsigned int N, unsigned int M, arma::cx_mat V, double h, double Delta_t, arma::cx_double r);

std::vector<arma::sp_cx_mat> u;
arma::sp_cx_mat A;
arma::sp_cx_mat B;

void create_A_matrix();
void create_B_matrix();

void initial_u(arma::cx_mat u);
void initial_u_Gaussian(arma::cx_mat u);

void solve_time_evolution();

};

#endif