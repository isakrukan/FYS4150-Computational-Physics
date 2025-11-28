#include "DoubleSlit.hpp"

DoubleSlit::DoubleSlit(double L, double h, double V_0): h(h), V_0(V_0)
{

    M = std::floor(L/h);
    current_state.set_size(M, M);

    positions.set_size(M, M, 2);
    
    // Initialize x-coordinates
    for (int i = 0; i < M; i++)
    {
        positions.slice(0).row(i) = arma::linspace<arma::rowvec>(0, L - h, M);
        positions.slice(1).col(i) = arma::linspace<arma::colvec>(0, L - h, M);
    }

    set_potential();
}

// --- Public methods --- 

// Setters for initial state

void DoubleSlit::set_initial_state(arma::cx_mat state)
{
    current_state.set_size(M, M);

    current_state = state;
    normalize_state();
}

void DoubleSlit::set_initial_state(std::function<arma::cx_double(double, double)> func)
{
    current_state.set_size(M, M);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            double x = positions(i, j, 0);
            double y = positions(i, j, 1);
            current_state(i, j) = func(x, y);
        }
    }
    normalize_state();
}

// Getters for position and momentum components

double DoubleSlit::get_x(int i, int j)
{
    return positions.slice(0)(i, j);
}
double DoubleSlit::get_y(int i, int j)
{
    return positions.slice(1)(i, j);
}

// --- Private methods ---

void DoubleSlit::normalize_state()
{

    arma::cx_double normalization = 0.0;

    for (int i = 0; i < M; i++)
    {
        arma::cx_rowvec row = current_state.row(i);
        normalization += arma::dot(arma::conj(row), row);
    }

    current_state /= std::sqrt(normalization);
}

void DoubleSlit::set_potential()
{
    potential.set_size(M, M);
    potential.fill(0.0);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            double x = get_x(i, j);
            double y = get_y(i, j);

            // Check if the point is within the barrier region
            bool at_wall  = (std::abs(x - wall_position) < wall_thickness / 2.0);
            bool in_slit1 = (std::abs(y - slit1_center) < slit_aperture / 2.0);
            bool in_slit2 = (std::abs(y - slit2_center) < slit_aperture / 2.0);

            if (at_wall && not in_slit1 && not in_slit2)
            {
                potential(i, j) = V_0; // Inside the barrier region but outside the slits
            }
        }
    }
}


void DoubleSlit::solve(double T, double dt)
{

    // Choose r
    arma::cx_double r(0, 1);

    unsigned int N = T / dt;

    // Temporary parameters
    CrankNicolson solver(N, M, potential, h, dt, r);
    
    solver.initial_u(current_state);

    solver.create_A_matrix();
    solver.create_B_matrix();

    solver.solve_time_evolution();

    solution.set_size(M, M, N);

    arma::cube solution_real(M, M, N);
    arma::cube solution_imag(M, M, N);

    for (unsigned int n = 0; n < N; ++n)
    {
        arma::cx_mat psi = arma::cx_mat(solver.u[n]); // size x 1
        psi.reshape(M-2, M-2);       // (M-2) x (M-2)

        solution.slice(n).zeros();
        solution.slice(n).submat(1, 1, M - 2, M - 2) = psi;
        solution_real.slice(n) = arma::real(solution.slice(n));
        solution_imag.slice(n) = arma::imag(solution.slice(n));
    }

    // Save to HDF5 file
    solution_real.save(arma::hdf5_name("time_evolution.h5", "real", arma::hdf5_opts::replace));
    solution_imag.save(arma::hdf5_name("time_evolution.h5", "imaginary", arma::hdf5_opts::append));
}