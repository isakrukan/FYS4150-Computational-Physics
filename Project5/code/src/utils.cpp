#include "utils.hpp"
#include "arg_parser.hpp"

arma::cx_double gaussian_wave_packet(double x, double y, double x_c, double y_c, double sigma_x, double sigma_y, double p_x, double p_y)
{
    double scaled_x = (x - x_c) / sigma_x;
    double scaled_y = (y - y_c) / sigma_y;

    arma::cx_double exponent = - scaled_x * scaled_x - scaled_y * scaled_y + I * (p_x * x + p_y * y);

    return std::exp(exponent);
}

arma::cx_double gaussian_wave_packet(double x, double y, Args args)
{
    double x_c = args.center_x;
    double y_c = args.center_y;
    double sigma_x = args.sigma_x;
    double sigma_y = args.sigma_y;
    double p_x = args.momentum_x;
    double p_y = args.momentum_y;

    return gaussian_wave_packet(x, y, x_c, y_c, sigma_x, sigma_y, p_x, p_y);
}


int get_flat_index(int i, int j, unsigned int M){
    return i*M + j;
}

#include <armadillo>
#include <vector>
#include <string>

// A function that prints the structure of a sparse matrix to screen.
void print_sp_matrix_structure(const arma::sp_cx_mat& A){
    using namespace std;
    using namespace arma;

    // Declare a C-style 2D array of strings.
    string S[A.n_rows][A.n_cols];  

    // Initialise all the strings to " ".
    for (int i =0; i < A.n_rows; i++)
    {
        for (int j = 0; j < A.n_cols; j++)
        {
            S[i][j] = " ";
        }
    }

    // Next, we want to set the string to a dot at each non-zero element.
    // To do this we use the special loop iterator from the sp_cx_mat class
    // to help us loop over only the non-zero matrix elements.
    sp_cx_mat::const_iterator it     = A.begin();
    sp_cx_mat::const_iterator it_end = A.end();

    int nnz = 0;
    for(it; it != it_end; ++it)
    {
        S[it.row()][it.col()] = "•";
        nnz++;
    }

    // Finally, print the matrix to screen.
    cout << endl;
    for (int i =0; i < A.n_rows; i++)
    {
        cout << "| ";
        for (int j = 0; j < A.n_cols; j++)
        {
            cout << S[i][j] << " ";
        }
        cout <<  "|\n";
    }

    cout << endl;
    cout << "matrix size: " << A.n_rows << "x" << A.n_cols << endl;
    cout << "non-zero elements: " << nnz << endl ;
    cout << endl;
}