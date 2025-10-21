
#include "solver.hpp"
#include <stdexcept>
#include <iterator> // To find index of element in list (see Solver::evolve)

Solver::Solver(PenningTrap &trap) : trap(trap) {}

void Solver::evolve_RK4(double dt, double t){
    // Temporary storage of positions and velocities.
    int n = trap.n;

    std::vector<arma::vec> r_original(n);
    std::vector<arma::vec> v_original(n);

    std::vector<arma::vec> k_r1(n), k_r2(n), k_r3(n), k_r4(n);
    std::vector<arma::vec> k_v1(n), k_v2(n), k_v3(n), k_v4(n);

    for (int i=0; i<n; i++){
        r_original[i] = trap[i].r;
        v_original[i] = trap[i].v;
    }

    // Compute k's (div_masses is computed when calling Solver::evolve())
    for (int i=0; i<n; i++){
        k_r1[i] = dt * trap[i].v;
        k_v1[i] = dt * trap.F(i, t) * div_masses[i];
    }
    for (int i=0; i<n; i++){    // Updating the positions and velocities
        trap[i].v = v_original[i] + 0.5 * k_v1[i];
        trap[i].r = r_original[i] + 0.5 * k_r1[i];
    }

    for (int i=0; i<n; i++){
        k_r2[i] = dt * (v_original[i] + 0.5 * k_v1[i]);
        k_v2[i] = dt * trap.F(i, t + 0.5 * dt) * div_masses[i];
    }
    for (int i=0; i<n; i++){    // Updating the positions and velocities
        trap[i].v = v_original[i] + 0.5 * k_v2[i];
        trap[i].r = r_original[i] + 0.5 * k_r2[i];
    }
    
    for (int i=0; i<n; i++){
        k_r3[i] = dt *(v_original[i] + 0.5 * k_v2[i]);
        k_v3[i] = dt * trap.F(i, t + 0.5 * dt) * div_masses[i];
    }
    for (int i=0; i<n; i++){    // Updating the positions and velocities
        trap[i].v = v_original[i] + k_v3[i];
        trap[i].r = r_original[i] + k_r3[i];
    }
    
    for (int i=0; i<n; i++){
        k_r4[i] = dt * (v_original[i] + k_v3[i]);
        k_v4[i] = dt * trap.F(i, t + dt) * div_masses[i];
    }

    // Update positions and velocities
    for (int i=0; i<n; i++){
        trap[i].v = v_original[i] + (1.0 / 6.0) * (k_v1[i] + 2 * k_v2[i] + 2 * k_v3[i] + k_v4[i]);
        trap[i].r = r_original[i] + (1.0 / 6.0) * (k_r1[i] + 2 * k_r2[i] + 2 * k_r3[i] + k_r4[i]);
    }
}

void Solver::evolve_FE(double dt, double t){

    std::vector<arma::vec> Forces(trap.n);
    for (int i=0; i<trap.n; i++){
        Forces[i] = trap.F(i, t);
    }
    for (int i=0; i<trap.n; i++){
        // Forward Euler update:
        trap[i].v += dt * Forces[i] * div_masses[i];
        trap[i].r += dt * trap[i].v;
    }
}


void Solver::evolve(double T, int n_steps, std::string method){
    
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
    current_method = possible_solvers[index_solver]; // To be used in e.g. Solver::save

    int numb_particles = trap.n;
    double dt = T / n_steps;

    // Setting up time array and 3D 'cube' of (particles, steps, 3)
    arma::vec t_vector = arma::linspace(0.0, T, n_steps);
    particles_positions = arma::cube(numb_particles, n_steps, 3);
    particles_velocities = arma::cube(numb_particles, n_steps, 3);

    // Initial conditions and 1/m vector filling:
    div_masses.resize(numb_particles);
    for (int n=0; n<numb_particles; n++){
        particles_positions.tube(n,0) = trap.particles[n].r;
        particles_velocities.tube(n,0) = trap.particles[n].v;
        div_masses[n] = 1.0 / trap[n].m;
    }

    for (int i=1; i<n_steps; i++){
        (this->*evolve_method)(dt, t_vector[i]); // Dereferencing the pointer evolve_method and call to this object.

        // Storing positions and velocities.
        for (int n=0; n<numb_particles; n++){
            particles_positions.tube(n,i) = trap.particles[n].r;
            particles_velocities.tube(n,i) = trap.particles[n].v;
        }
    }

    evolved = true; // The Penning trap has evolved in time.
}

void Solver::save(std::string additional_info, std::string outdir, double T){
    int n_steps = particles_positions.n_cols;
    arma::mat particles_time = arma::linspace(0.0, T, n_steps);
    
    std::string numb_particles = "p" + std::to_string(trap.n);
    std::string interacting = "nonint"; std::string time_steps = std::to_string(n_steps);
    
    if (trap.interacting){
        interacting = "int";
    }
    std::string identifier = numb_particles + "-" + current_method + "-" + interacting + "-n" + time_steps;
    std::string filename = outdir + additional_info + identifier + ".h5";


    arma::hdf5_name time_hdf5 = arma::hdf5_name(filename, "times");
    arma::hdf5_name positions_hdf5 = arma::hdf5_name(filename, "positions", arma::hdf5_opts::append);
    arma::hdf5_name velocities_hdf5 = arma::hdf5_name(filename, "velocities", arma::hdf5_opts::append);

    particles_time.save(time_hdf5, arma::hdf5_binary);
    particles_positions.save(positions_hdf5, arma::hdf5_binary);
    particles_velocities.save(velocities_hdf5, arma::hdf5_binary);

    std::cout << "Successfully stored " << filename << std::endl;
}