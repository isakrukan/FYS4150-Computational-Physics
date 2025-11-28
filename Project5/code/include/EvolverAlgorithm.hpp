#pragma once 

#include <armadillo>
#include <vector>

class EvolverAlgorithm{


protected:

const arma::cx_double i_unit = arma::cx_double(0,1);
unsigned int M;
unsigned int N;
double h;
double Delta_t;
arma::cx_mat V;

bool V_set = false;
bool M_set = false;
bool N_set = false;

public:

EvolverAlgorithm() = default;
EvolverAlgorithm(double h, double Delta_t);

virtual ~EvolverAlgorithm() = default; // Destructor needed since we deal with polymorphism

/**
 * @brief Computes the next time step of wave function evolution.
 * @param n Time index.
 * @param u_internal Wave function excluding boundary points at time index n.
 */
virtual arma::cx_vec update_u(arma::cx_vec &u_internal) = 0; 

virtual void set_potential(arma::cx_mat &V) = 0;
virtual void set_grid_size(unsigned int M) = 0;

};


class CrankNicolsen : public EvolverAlgorithm{

private:

bool initialized = false;

arma::cx_vec a;
arma::cx_vec b; 
arma::cx_double r;
arma::cx_vec b_result; ///< For storing result of \f$B\vec{u}^n = \vec{b}\f$.

arma::sp_cx_mat A;
arma::sp_cx_mat B;

/**
 * @brief Creates block diagonal matrix.
 * @param C Matrix to form as block diagonal matrix
 * @param c Elements along the diagonal of C.
 * @param r_ Elements along diagonal of matrices to subdiagonals.
 */
void create_matrix(arma::sp_cx_mat &C, arma::cx_vec &c, arma::cx_double r_);

void create_A_matrix();
void create_B_matrix();

void initialize();

public:

CrankNicolsen(arma::cx_mat &V, double h, double Delta_t);
CrankNicolsen(double h, double Delta_t);

/**
 * @brief Computes the next time step of wave function evolution.
 * @param n Time index.
 * @param u_internal Wave function excluding boundary points at time index n.
 */
virtual arma::cx_vec update_u(arma::cx_vec &u_internal) override; 

virtual void set_potential(arma::cx_mat &V) override;
virtual void set_grid_size(unsigned int M) override;
};
