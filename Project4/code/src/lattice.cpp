#include "lattice.hpp"
#include "utils.hpp"

///////////// Lattice ///////////// 
void Lattice::initialize_spins(bool ordered)
{
    if (ordered)
    {
        
        grid.resize(rows, std::vector<Location>(cols, Location(1)));
    }
    else
    {
        std::uniform_int_distribution<int> dist(0, 1); // Uniform distribution for {0,1}.
        std::mt19937_64 generator;  // Random number generator.
        
        // Resizing grid and assigning spins:
        grid.resize(rows); // rows
        for (int i=0; i<rows; i++){

            grid[i].resize(cols); // cols
            for (int j=0; j<cols; j++){
                int spin = dist(generator); // 0 or 1
                if (spin == 0){ 
                    spin = -1; // 0 -> -1
                }

                grid[i][j] = Location(spin); // random {-1,1}
            }
        }
    }

    assign_pointers();
    initialize_energy();
    initialize_magnetization();
}

void Lattice::assign_pointers(){
    // Periodic boundary condition means that the boundary locations point "back" to the other side (respectively) of the lattice.

    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            int i_up = (i-1) * (i>0) + (rows-1) * (i==0);
            int i_down = (i+1) * (i<rows-1);

            int j_left = (j-1) * (j>0) + (cols-1) * (j==0);
            int j_right = (j+1) * (j<cols-1);

            grid[i][j].up = &grid[i_up][j];
            grid[i][j].down = &grid[i_down][j];

            grid[i][j].left = &grid[i][j_left];
            grid[i][j].right = &grid[i][j_right];
        }
    }
}

void Lattice::initialize_energy()
{
    E = 0.0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            E += grid[i][j].s * ((*grid[i][j].right).s + (*grid[i][j].down).s);
        }
    }
    E *= -J;
}

void Lattice::initialize_magnetization()
{
    M = 0;
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            M += grid[i][j].s;
        }
    }
}

void Lattice::flip_spin(int i, int j)
{
    // Flip the spin at (i, j)
    grid[i][j].s *= -1;

    // Update magnetization 
    M += 2 * grid[i][j].s;

    // Update energy
    E -= 2 * J * grid[i][j].s * ((*grid[i][j].up).s +
                                     (*grid[i][j].down).s +
                                     (*grid[i][j].left).s +
                                     (*grid[i][j].right).s);

}

int Lattice::get_spin(int i, int j) const
{
    return grid[i][j].s;
}

double Lattice::get_energy() const
{
    return E;
}

double Lattice::get_magnetization() const
{
    return M;
}

std::vector<Location>& Lattice::operator[](int row_index){
    return grid[row_index];
}