#include "lattice.hpp"
#include "utils.hpp"

///////////// Lattice ///////////// 
void Lattice::initialize_spins(bool ordered)
{
    if (ordered)
    {
        
        grid_new.resize(rows, std::vector<Location>(cols, Location(1)));
    }
    else
    {
        std::uniform_int_distribution<int> dist(0, 1); // Uniform distribution for {0,1}.
        std::mt19937_64 generator;  // Random number generator.
        
        // Resizing grid and assigning spins:
        grid_new.resize(rows); // rows
        for (int i=0; i<rows; i++){

            grid_new[i].resize(cols); // cols
            for (int j=0; j<cols; j++){
                int spin = dist(generator); // 0 or 1
                if (spin == 0){ 
                    spin = -1; // 0 -> -1
                }

                grid_new[i][j] = Location(spin); // random {-1,1}
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

            grid_new[i][j].up = &grid_new[i_up][j];
            grid_new[i][j].down = &grid_new[i_down][j];

            grid_new[i][j].left = &grid_new[i][j_left];
            grid_new[i][j].right = &grid_new[i][j_right];
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
            E += grid_new[i][j].s * ((*grid_new[i][j].right).s + (*grid_new[i][j].down).s);
        }
    }
    E *= -J;
}

void Lattice::initialize_magnetization()
{
    M = 0;
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            M += grid_new[i][j].s;
        }
    }
}

void Lattice::flip_spin(int i, int j)
{
    // Flip the spin at (i, j)
    grid_new[i][j].s *= -1;

    // Update magnetization 
    M += 2 * grid_new[i][j].s;

    // Update energy
    E -= 2 * J * grid_new[i][j].s * ((*grid_new[i][j].up).s +
                                     (*grid_new[i][j].down).s +
                                     (*grid_new[i][j].left).s +
                                     (*grid_new[i][j].right).s);

}

int Lattice::get_spin(int i, int j) const
{
    return grid_new[i][j].s;
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
    return grid_new[row_index];
}