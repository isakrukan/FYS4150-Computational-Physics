#include "lattice.hpp"
#include "utils.hpp"

void Lattice::initialize_spins(bool ordered)
{
    if (ordered)
    {
        grid = arma::imat(rows, cols);
        grid.fill(1);
    }
    else
    {
        grid = arma::randi<arma::imat>(rows, cols, arma::distr_param(0, 1));
        grid.replace(0, -1);
    }

    initialize_energy();
    initialize_magnetization();
}

void Lattice::initialize_energy()
{
    E = 0.0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int spin  = grid(i, j);
            int right = grid(i, pbc(j + 1, cols));
            int below = grid(pbc(i + 1, rows), j);

            E += -J * spin * (right + below);
        }
    }
}

void Lattice::initialize_magnetization()
{
    M = arma::accu(grid);
}

void Lattice::flip_spin(int i, int j)
{
    // Flip the spin at (i, j)
    grid(i, j) *= -1;

    // Update magnetization 
    M -= 2 * grid(i, j);

    // Update energy
    int up    = get_spin(pbc(i - 1, rows), j);
    int down  = get_spin(pbc(i + 1, rows), j);
    int left  = get_spin(i, pbc(j - 1, cols));
    int right = get_spin(i, pbc(j + 1, cols));
    E -= 2 * J * grid(i, j) * (up + down + left + right);

}

int Lattice::get_spin(int i, int j) const
{
    return grid(i, j);
}

double Lattice::get_energy() const
{
    return E;
}
double Lattice::get_magnetization() const
{
    return M;
}