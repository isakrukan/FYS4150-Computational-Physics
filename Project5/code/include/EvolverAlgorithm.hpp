#pragma once 

#include <armadillo>
#include <vector>

/**
 * @brief Abstract base class for wave function evolution algorithms.
 * 
 */
class EvolverAlgorithm{

public:

//*** Constructor(s) ***/

EvolverAlgorithm() = default;
EvolverAlgorithm(double h, double dt);

virtual ~EvolverAlgorithm() = default; ///< Destructor needed since we deal with polymorphism.

//********** Public methods **********//

/**
 * @brief Computes the next time step of wave function evolution.
 * @param n Time index.
 * @param u_internal Wave function excluding boundary points at time index n.
 */
virtual arma::cx_vec update_u(arma::cx_vec &u_internal) = 0; 

/**
 * @brief Sets the potential matrix.
 * @param V Potential matrix.
 */
virtual void set_potential(arma::cx_mat &V) = 0;

/**
 * @brief Sets the grid size.
 * @param M Grid size.
 */
virtual void set_grid_size(unsigned int M) = 0;

protected:

//*** Protected variables ***/
const arma::cx_double i_unit = arma::cx_double(0,1); ///< Imaginary unit.
unsigned int M; ///< Grid size.
double h; ///< Space separation.
double dt; ///< Time separation.
arma::cx_mat V; ///< Potential.
bool V_set = false; ///< Whether potential has been set.
bool M_set = false; ///< Whether grid size has been set.
bool dt_set = false; ///< Whether time separation has been set.

};

/**
 * @brief Class implementing the Crank-Nicolsen algorithm for wave function evolution, inheriting from \ref EvolverAlgorithm "EvolverAlgorithm".
 * 
 */
class CrankNicolsen : public EvolverAlgorithm{

public:

//*** Constructor(s) ***/

/**
 * @brief Construct a new Crank Nicolsen object.
 * 
 * @param V Potential.
 * @param h Space separation.
 * @param dt Time separation.
 */
CrankNicolsen(arma::cx_mat &V, double h, double dt);

/**
 * @brief Construct a new Crank Nicolsen object.
 * 
 * @param h Space separation.
 * @param dt Time separation.
 */
CrankNicolsen(double h, double dt);

//********** Public methods **********//

/**
 * @brief Computes the next time step of wave function evolution.
 * @param n Time index.
 * @param u_internal Wave function excluding boundary points at time index n.
 */
virtual arma::cx_vec update_u(arma::cx_vec &u_internal) override; 

virtual void set_potential(arma::cx_mat &V) override;
virtual void set_grid_size(unsigned int M) override;

const arma::sp_cx_mat& get_A_matrix() const { return A; }  ///< Returns matrix A (for testing purposes).
const arma::sp_cx_mat& get_B_matrix() const { return B; }  ///< Returns matrix B (for testing purposes).

private:

//*** Private variables ***/

bool initialized = false; ///< Whether matrices have been initialized.

arma::cx_vec a; ///< Diagonal elements of A matrix.  
arma::cx_vec b; ///< Diagonal elements of B matrix.
arma::cx_double r; ///< Diagonal elements of sub- and super-diagonals of A and B matrices.
arma::cx_vec b_result; ///< For storing result of \f$B\vec{u}^n = \vec{b}\f$.

arma::sp_cx_mat A; ///< Sparse matrix A.    
arma::sp_cx_mat B; ///< Sparse matrix B.

//********** Private methods **********//
/**
 * @brief Creates block diagonal matrix.
 * @param C Matrix to form as block diagonal matrix
 * @param c Elements along the diagonal of C.
 * @param r_ Elements along diagonal of matrices to subdiagonals.
 */
void create_matrix(arma::sp_cx_mat &C, arma::cx_vec &c, arma::cx_double r_);

/**
 * @brief Creates matrix A for Crank-Nicolsen algorithm.
 */
void create_A_matrix();

/**
 * @brief Creates matrix B for Crank-Nicolsen algorithm.
 */
void create_B_matrix();

/**
 * @brief Initializes matrices and vectors for Crank-Nicolsen algorithm.
 */
void initialize();
};
