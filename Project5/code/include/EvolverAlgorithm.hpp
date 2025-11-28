#pragma once 

#include <armadillo>
#include <vector>

class EvolverAlgorithm{


protected:

const arma::cx_double i_unit;
std::vector<arma::cx_mat> u_internal;
unsigned int M;
arma::cx_mat V;

bool V_set = false;
bool M_set;

public:

EvolverAlgorithm() = default;
EvolverAlgorithm(unsigned int M): i_unit(arma::cx_double(0,1)), M(M) {} 

/**
 * @brief Computes the next time step of wave function evolution.
 * @param n Time index.
 */
virtual arma::cx_mat update_u(const unsigned int n) = 0; 

virtual void set_potential(arma::cx_mat V) = 0;
virtual void set_grid_size(unsigned int M) = 0;

};


class CrankNicolsen : public EvolverAlgorithm{

private:

double h;
double Delta_t;

bool h_Delta_t_set = false; ///< Whether \ref h "h" and \ref \Delta_t "Delta_t" is set or not.
bool initialized = false;

arma::cx_vec a;
arma::cx_vec b; 
arma::cx_double r;
arma::cx_mat b_result; ///< For storing result of \f$B\vec{u}^n = \vec{b}\f$.

arma::sp_cx_mat A;
arma::sp_cx_mat B;

int get_flat_index(int i, int j, unsigned int M);

/**
 * @brief Creates block diagonal matrix.
 * @param C Matrix to form as block diagonal matrix
 * @param c Elements along the diagonal of C.
 * @param r_ Elements along diagonal of matrices to subdiagonals.
 */
void create_matrix(arma::sp_cx_mat &C, arma::cx_vec c, arma::cx_double r_);

void create_A_matrix();
void create_B_matrix();

void initialize();

public:

CrankNicolsen(unsigned int M, arma::cx_mat V, double h, double Delta_t);
CrankNicolsen(unsigned int M, double h, double Delta_t);
CrankNicolsen(double h, double Delta_t);

/**
 * @brief Computes the next time step of wave function evolution.
 * @param n Time index.
 */
virtual arma::cx_mat update_u(const unsigned int n) override; 

virtual void set_potential(arma::cx_mat V) override;
virtual void set_grid_size(unsigned int M) override;
};
