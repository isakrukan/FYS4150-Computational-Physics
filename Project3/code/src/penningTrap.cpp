#include "penningTrap.hpp"
#include <cmath>
#include <stdexcept>
#include <string>

//******** Particle ********
Particle::Particle(float q, float m, arma::vec &r, arma::vec &v) 
                : q(q), m(m), r(r), v(v){

                }

//******** Penning Trap ********

PenningTrap::PenningTrap(float B_0, float V_0, float d)
                        : n(0), B_0(B_0), V_0(V_0), d(d){
                        }

void PenningTrap::add_particle(Particle &particle){
    particles.push_back(particle);

    n ++; // The Penning trap now contains one more particle;
}

void PenningTrap::add_particle(float q, float m, arma::vec &r, arma::vec &v){
    Particle particle = Particle(q, m, r, v);
    add_particle(particle);
}

Particle& PenningTrap::operator[](int i){
    if(i >= n || i < 0){
        throw std::invalid_argument("Index " + std::to_string(i) + " must be within [0, " + std::to_string(n) + "].");
    }

    return particles[i]; // The i-th particle.
}


arma::vec PenningTrap::E(arma::vec &r){
    arma::vec E_field = {V_0 / (d*d) * r[0], V_0 / (d*d) * r[1], - 2*V_0 / (d*d) * r[2]};

    return E_field;
} 

arma::vec PenningTrap::B(arma::vec &r){
    arma::vec B_field = {0, 0 , B_0};

    return B_field;
} 

arma::vec PenningTrap::F(int i){

    // Checking that i\in[0,n-1]
    if (i >= n || i < 0){
        throw std::invalid_argument("i = " + std::to_string(i) + "must be within [0, " + std::to_string(n) + "].");
    }
    arma::vec Force = arma::vec(3).fill(0);

    float prefactor = k_e * particles[i].q / particles[i].m;
    for (int j=0; j<n; j++){
        if (i != j){

            float norm = arma::norm(particles[i].r - particles[j].r, 2);
            Force += particles[j].q * (particles[i].r - particles[j].r) / std::pow(norm, 3);
        }
    }

    return prefactor * Force;
}