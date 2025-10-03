
#include "solver.hpp"
#include <stdexcept>
#include <iterator> // To find index of element in list (see Solver::evolve)

Solver::Solver(PenningTrap &trap) : trap(trap) {}
    
arma::vec Solver::total_force(int i, float t)
{
    arma::vec E_field = trap.E(trap[i].r, t);
    arma::vec B_field = trap.B(trap[i].r);
    arma::vec F_coulomb = trap.F(i);

    // Total force on particle i:
    arma::vec F_tot = trap[i].q * (E_field + arma::cross(trap[i].v, B_field)) + F_coulomb;

    return F_tot / trap[i].m;
}

void Solver::evolve_RK4(float dt, float t)
{
    // Temporary storage of positions and velocities.
    int n = trap.n;

    std::vector<arma::vec> r_temp(n);
    std::vector<arma::vec> v_temp(n);

    std::vector<arma::vec> k_r1(n), k_r2(n), k_r3(n), k_r4(n);
    std::vector<arma::vec> k_v1(n), k_v2(n), k_v3(n), k_v4(n);

    for (int i = 0; i < n; i++)
    {
        r_temp[i] = trap[i].r;
        v_temp[i] = trap[i].v;
    }

    // Compute k's
    // NOTE: fix time dependence, have set t=0 here
    for (int i = 0; i < n; i++)
    {
        k_r1[i] = dt * trap[i].v;
        k_v1[i] = dt * total_force(i, t);
    }
    
    for (int i = 0; i < n; i++)
    {
        trap[i].v = v_temp[i] + 0.5 * k_v1[i];
        trap[i].r = r_temp[i] + 0.5 * k_r1[i];
        k_v2[i] = dt * total_force(i, t);
        k_r2[i] = dt * trap[i].v;
    }
    
    for (int i = 0; i < n; i++)
    {
        trap[i].v = v_temp[i] + 0.5 * k_v2[i];
        trap[i].r = r_temp[i] + 0.5 * k_r2[i];
        k_v3[i] = dt * total_force(i, t);
        k_r3[i] = dt * trap[i].v;
    }
    
    for (int i = 0; i < n; i++)
    {
        trap[i].r = r_temp[i] + k_r3[i];
        trap[i].v = v_temp[i] + k_v3[i];
        k_r4[i] = dt * trap[i].v;
        k_v4[i] = dt * total_force(i, t);
    }

    // Update positions and velocities
    for (int i = 0; i < n; i++)
    {
        trap[i].v = v_temp[i] + (1.0 / 6.0) * (k_v1[i] + 2 * k_v2[i] + 2 * k_v3[i] + k_v4[i]);
        trap[i].r = r_temp[i] + (1.0 / 6.0) * (k_r1[i] + 2 * k_r2[i] + 2 * k_r3[i] + k_r4[i]);
    }
}

void Solver::evolve_FE(float dt, float t)
{

    for (int i = 0; i < trap.n; i++)
    {
        // Forward Euler update:
        trap[i].v += dt * total_force(i, t);
        trap[i].r += dt * trap[i].v;
    }
}


void Solver::evolve(float T, int n_steps, std::string method){
    
    /* 
        Checking what method to use and if it exists. Here, 'it' either becomes the pointer to the element where an equal element to 
        method exists in possible_solvers, or the next memory address after possible_solvers. If the latter, the method is not implemented
        (yet) and an error is thrown. 
    */ 
    std::vector<std::string>::iterator it =  std::find(
                                                possible_solvers.begin(),
                                                possible_solvers.end(),
                                                method); 

    bool method_exists =  it != possible_solvers.end();

    if(!method_exists){
        std::string possible_solvers_string = "[";
        for (int i=0; i<possible_solvers.size()-1; i++){
            possible_solvers_string += possible_solvers[i] + ", ";
        }
        possible_solvers_string += possible_solvers_string[possible_solvers.size()-1] + "]";

        throw std::invalid_argument("I don't know what " + method + " is. Possible solvers are: " + possible_solvers_string);
    }

    // The method does exist, and this is its index 
    int index_solver = std::distance(possible_solvers.begin(), it);
    evolver evolve_method = evolvers[index_solver]; // Becomes a pointer to the method RK4, FE, or ...

    int numb_particles = trap.n;
    float dt = T / n_steps;
    arma::vec t_vector = arma::linspace(0.0, T, n_steps + 1);
    particles_positions = arma::cube(numb_particles, n_steps+1, 3);
    particles_velocities = arma::cube(numb_particles, n_steps+1, 3);

    // Initial conditions:
    for (int n=0; n<numb_particles; n++){
        particles_positions.tube(n,0) = trap.particles[n].r;
        particles_velocities.tube(n,0) = trap.particles[n].v;
    }

    std::cout << particles_positions[0,0,0] << ", " << particles_positions[0,0,1] << ", " << particles_positions[0,0,2] << std::endl;
    
    for (int i=1; i<n_steps+1; i++){
        (this->*evolve_method)(dt, t_vector[i]); // Dereferencing the pointer evolve_method and call to this object.

        // Storing positions and velocities.
        for (int n=0; n<numb_particles; n++){
            particles_positions.tube(n,i) = trap.particles[n].r;
            particles_velocities.tube(n,i) = trap.particles[n].v;
        }
    }

    evolved = true; // The Penning trap has evolved in time.
}

void Solver::save(std::string identifier, std::string outdir){
    std::string positions_filename = outdir + identifier + "_r" + ".h5";
    std::string velocities_filename = outdir + identifier + "_v" + ".h5";

    particles_positions.save(positions_filename, arma::hdf5_binary);
    particles_velocities.save(velocities_filename, arma::hdf5_binary);
}