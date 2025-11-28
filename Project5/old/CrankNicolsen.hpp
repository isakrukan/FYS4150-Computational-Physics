#include <armadillo>

class CrankNicolsen
{

private:

arma::cx_vec a;
arma::cx_vec b; 
arma::cx_double r;
const arma::cx_double i_unit;
arma::cx_mat b_result; ///< For storing result of \f$B\vec{u}^n = \vec{b}\f$.
std::vector<arma::cx_mat> u_internal;

int get_flat_index(int i, int j, unsigned int M);

/**
 * @brief Creates block diagonal matrix.
 * @param C Matrix to form as block diagonal matrix
 * @param c Elements along the diagonal of C.
 * @param r_ Elements along diagonal of matrices to subdiagonals.
 */
void create_matrix(arma::sp_cx_mat &C, arma::cx_vec c, arma::cx_double r_);

/**
 * @brief Computes the next element of \ref CrankNicolsen::u "u".
 * @param n Index for \ref CrankNicolsen::u "u".
 */
void update_u(const unsigned int n); 


void initialize();

public:



CrankNicolsen(unsigned int N, unsigned int M, unsigned int T, arma::cx_vec a, arma::cx_vec b, arma::cx_double r);
CrankNicolsen(unsigned int N, unsigned int M, unsigned int T, arma::cx_mat V, double h=0.005, double Delta_t=2.5*1e-5);

arma::sp_cx_mat A;
arma::sp_cx_mat B;


void create_A_matrix();
void create_B_matrix();


/**
 * @brief Initializes the wave function like a Gaussian wave packet.
 * @param xy_c Center of Gaussian.
 * @param sigma_xy Initial width of Gaussian.
 * @param p_xy Wave packet momentum.
 */
void initial_u_Gaussian(std::vector<double> xy_c, std::vector<double> sigma_xy, std::vector<double> p_xy);


};