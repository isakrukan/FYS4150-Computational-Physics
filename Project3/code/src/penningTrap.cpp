#include "penningTrap.hpp"
#include <cmath>

//******** Particle ********
Particle::Particle(float q, float m, arma::vec &r, arma::vec &v) 
                : q(q), m(m), r(r), v(v){

                }

//******** Penning Trap ********

arma::vec PenningTrap::E(arma::vec &r){
    arma::vec E_field = {V_0 / (d*d) * r[0], V_0 / (d*d) * r[1], - 2*V_0 / (d*d) * r[2]};

    return E_field;
} 

arma::vec PenningTrap::B(arma::vec &r){
    arma::vec B_field = {0, 0 , B_0};

    return B_field;
} 

arma::vec PenningTrap::F(int i){

    arma::vec Force(3);

    float prefactor = k_e * particles[i].q / particles[i].m;

    for (int j=0; j<n; j++){
        if (i != j){

            float norm = arma::abs(particles[i].r - particles[j].r);
            Force += particles[j].q * (particles[i].r - particles[j].r) / std::pow(norm, 3)
        }
    }
}