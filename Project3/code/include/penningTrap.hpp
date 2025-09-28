#include <armadillo>
#include <vector>

/**
 * @brief Representation of a particle with charge @ref q "q" and mass @ref m "m" located at @ref r "r" with a velocity @ref v "v". 
 * 
 */
class Particle{
private:

    float q;        ///< Electric charge [\f$ e \f$] (elementary charge).
    float m;        ///< Mass [\f$ u \f$] (atomic mass unit).
    arma::vec r;    ///< Position [\f$ \mu m \f$] (micrometer).
    arma::vec v;    ///< Velocity [\f$ \mu m/(\mu s)  \f$] (micrometer per microsecond).

public:

    /**
     * @brief Construct a new Particle object and assigns the properties of a particle in the obvious way.
     * 
     * @param q Electric charge [\f$ e \f$] (elementary charge).
     * @param m Mass [\f$ u \f$] (atomic mass unit).
     * @param r Position [\f$ \mu m \f$] (micrometer).
     * @param v Velocity [\f$ \mu m \mu^{-1} s^{-1}  \f$] (micrometer per microsecond).
     */
    Particle(float q, float m, arma::vec &r, arma::vec &v);
};

/**
 * @brief Representation of the Penning trap model. Particles are trapped within the potential given by @anchor eq_1
 * \f[
 *  V(x,y,z) = \frac{V_0}{2d^2}\left(2z^2 - x^2 -y^2\right),    \tag{1}
 * \f]
 * with a static homogeneous \f$B\f$-field pointing in the \f$z\f$-direction.
 * 
 */
class PenningTrap{

private:
    friend class Particle;  ///< A particle located inside the Penning trap.


    float k_e = 1.38935333 * 1e5; ///< Coulomb constant [\f$ u (\mu m)^3 (\mu s)^{-2}e^{-2} $\f] 
    float B_0;      ///< Magnetic field strength [\f$ u/(\mu s)e^{-1} $\f] (atomic mass unit per microsend per electric charge).
    float V_0;      ///< Applied potential [\f$ u/(\mu s)^2e^{-1} $\f] (atomic mass unit per microsend squared per electric charge).
    float d;        ///< Characteristic dimension [\f$ \mu m $\f] (micrometer).

    int n;          ///> Number of particles inside Penning trap.
    std::vector<Particle> particles; ///< Collection of particles inside Penning trap.

public:

    /**
     * @brief The electric field at position \f$ r = (x,y,z)\f$.
     * @details Eq. @ref eq_1 "(1)" gives that the electric field is given by @anchor eq_2
     * \f[
     *      \vec{E} = -\nabla V = \frac{V_0}{d^2}\left(x, y, -2z\right).
     * \f]
     * 
     * @param r Position [\f$ \mu m \f$ ] (micrometer). 
     * @return \f$ \vec{E}\f$
     */
    arma::vec E(arma::vec &r);

    /**
     * @brief The magnetic field at position \f$ r = (x,y,z)\f$, given by \f$\vec{B} = B_0 \hat{e}_z = (0,0,B_0)\f$.
     * 
     * @param r Position [\f$ \mu m \f$ ] (micrometer). 
     * @return \f$ \vec{B}\f$
     */
    arma::vec B(arma::vec &r);

    /**
     * @brief The forced applied to a @ref Particle "Particle" by index, due to the other particles inside the Penning trap. 
     * 
     * @param i The n-th particle in the collection of particles inside the Penning trap.
     * @return arma::vec 
     */
    arma::vec F(int i);

};