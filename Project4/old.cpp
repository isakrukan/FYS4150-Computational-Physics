

        double E_now = lattice.get_energy();
        double M_now = lattice.get_magnetization();

        results.avg_energy += E_now;
        results.avg_energy_squared += E_now * E_now;
        results.avg_magnetization += M_now;
        results.avg_magnetization_abs += std::abs(M_now);
        results.avg_magnetization_squared += M_now * M_now;

        if (save)
        {
            outfile << cycle << ", "
                    << E_now << ", " << results.avg_energy / cycle << ", "
                    << M_now << ", " << results.avg_magnetization / cycle << "\n";
        }






        for (int cycle = 0; cycle < total_cycles; ++cycle)
        {
                double E_now = simulation_data(cycle, 1);
                double M_now = simulation_data(cycle, 2);

                results.avg_energy += E_now;
                results.avg_energy_squared += E_now * E_now;
                results.avg_magnetization += M_now;
                results.avg_magnetization_abs += std::abs(M_now);
                results.avg_magnetization_squared += M_now * M_now;
        }

        results.avg_energy /= total_cycles;
        results.avg_energy_squared /= total_cycles;
        results.avg_magnetization /= total_cycles;
        results.avg_magnetization_abs /= total_cycles;
        results.avg_magnetization_squared /= total_cycles;









        
        std::ofstream outfile(filename);

        if (save)
        {
                outfile << "cycle, E, <E>, M, <M>\n";
        }

        // Burn-in cycles
        for (int cycle = 0; cycle < burn_in_cycles; ++cycle)
        {
                cycle_MCMC(steps_per_cycle);
        }