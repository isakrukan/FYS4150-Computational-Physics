#include "penningTrap.hpp"
#include "solver.hpp"
#include "arg_parser.hpp"
#include <iostream>

const std::string outdir = "out/";

void simulate_single_particle(double T, uint n_steps, arma::vec r, arma::vec v, const std::string &id)
{
    arma::mat t_mat(n_steps + 1, 1);
    arma::mat r_mat(n_steps + 1, 3);
    arma::mat v_mat(n_steps + 1, 3);
    t_mat(0) = 0;
    r_mat.row(0) = r.t();
    v_mat.row(0) = v.t();

    PenningTrap trap;
    trap.add_particle(1, 40, r, v);
    Solver solver(trap);

    double dt = T / n_steps;

    for (int i = 1; i < n_steps + 1; i++)
    {
        solver.evolve_RK4(dt);
        
        t_mat(i) = i * dt;
        r_mat.row(i) = r.t();
        v_mat.row(i) = v.t();
    }


    std::string r_file = outdir + "r" + id + "-single" + "-n" + std::to_string(n_steps) + ".txt";
    std::string v_file = outdir + "v" + id + "-single" + "-n" + std::to_string(n_steps) + ".txt";
    
    r_mat.insert_cols(0, t_mat);
    v_mat.insert_cols(0, t_mat);
    r_mat.save(r_file, arma::raw_ascii);
    v_mat.save(v_file, arma::raw_ascii);
}

void simulate_double_particles(double T, uint n_steps, arma::vec r1, arma::vec v1, arma::vec r2, arma::vec v2)
{

    arma::mat t_mat(n_steps + 1, 1);
    arma::mat r1_mat(n_steps + 1, 3);
    arma::mat r2_mat(n_steps + 1, 3);
    arma::mat v1_mat(n_steps + 1, 3);
    arma::mat v2_mat(n_steps + 1, 3);
    t_mat(0) = 0;
    r1_mat.row(0) = r1.t();
    r2_mat.row(0) = r2.t();
    v1_mat.row(0) = v1.t();
    v2_mat.row(0) = v2.t();

    PenningTrap trap;
    trap.add_particle(1, 40, r1, v1);
    trap.add_particle(1, 40, r2, v2);
    Solver solver(trap);

    double dt = T / n_steps;

    for (int i = 1; i < n_steps + 1; i++)
    {
        solver.evolve_RK4(dt);
        r1_mat.row(i) = r1.t();
        r2_mat.row(i) = r2.t();
        v1_mat.row(i) = v1.t();
        v2_mat.row(i) = v2.t();
        t_mat(i) = i * dt;
    }

    std::string r_file_1 = outdir + "r1-interacting-n" + std::to_string(n_steps) + ".txt";
    std::string r_file_2 = outdir + "r2-interacting-n" + std::to_string(n_steps) + ".txt";
    std::string v_file_1 = outdir + "v1-interacting-n" + std::to_string(n_steps) + ".txt";
    std::string v_file_2 = outdir + "v2-interacting-n" + std::to_string(n_steps) + ".txt";

    r1_mat.insert_cols(0, t_mat);
    r2_mat.insert_cols(0, t_mat);
    v1_mat.insert_cols(0, t_mat);
    v2_mat.insert_cols(0, t_mat);

    r1_mat.save(r_file_1, arma::raw_ascii);
    r2_mat.save(r_file_2, arma::raw_ascii);
    v1_mat.save(v_file_1, arma::raw_ascii);
    v2_mat.save(v_file_2, arma::raw_ascii);
};

int main(int argc, char *argv[])
{

    Args args = parse_args(argc, argv);

    // Problem 8
    arma::vec r1 = {20, 0, 20};
    arma::vec v1 = {0, 25, 0};
    arma::vec r2 = {25, 25, 0};
    arma::vec v2 = {0, 40, 5};

    simulate_single_particle(args.T, args.n_steps, r1, v1, "1");
    simulate_single_particle(args.T, args.n_steps, r2, v2, "2");
    simulate_double_particles(args.T, args.n_steps, r1, v1, r2, v2);

    return 0;
}
