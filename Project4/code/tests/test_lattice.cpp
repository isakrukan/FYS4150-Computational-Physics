#include "../include/lattice.hpp"
#include <cassert>

void test(){
    // Tests Lattice class.
    float eps = 1e-14;

    int L = 2;
    double J = 1.0;
    double T = 1.0;
    double beta = 1 / (T); // in units of 1/k_B.
    Lattice lattice2x2(L, J, T);

    // E and M initialization:
    lattice2x2.initialize_spins(true); // All spins in +1.
    double E = lattice2x2.get_energy();
    double M = lattice2x2.get_magnetization();

    double E_analytic = -J * 8;
    double M_analytic = 4;

    assert(std::abs(E - E_analytic) < eps);
    assert(std::abs(M - M_analytic) < eps);

    // Correct pointer setup:
    L = 16;
    Lattice lattice(L, J, T);
    lattice.initialize_spins();
    for (int i=0; i<L; i++){
        for (int j=0; j<L; j++){
            Location &location = lattice[i][j];
            assert(std::abs(1 - std::abs(location.s)) < eps);
        }
    }

    // Corner locations:
    Location &upper_left = lattice[0][0];
    Location &upper_right = lattice[0][L-1];
    Location &lower_left = lattice[L-1][0];
    Location &lower_right= lattice[L-1][L-1];
    
    assert(upper_left.left == &upper_right); 
    assert(upper_left.up == &lower_left); 

    assert(upper_right.right == &upper_left); 
    assert(upper_right.up == &lower_right); 

    assert(lower_left.left == &lower_right); 
    assert(lower_left.down == &upper_left); 

    assert(lower_right.right == &lower_left); 
    assert(lower_right.down == &upper_right); 


}

// double Lattice::get_energy() const
// {
//     return E;
// }
// double Lattice::get_magnetization() const
// {
//     return M;
// }

int main(){
    test();
}