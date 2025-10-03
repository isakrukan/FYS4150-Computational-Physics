
#include "solver.hpp"

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