import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

q = 1     # Charge [e] (elementary charge)
m = 40    # Mass [u] (atomic mass unit)
d = 500   # Characteristic dimension [μm]

def analytical_solution(t, r_0, v_0, B0=9.65*1e1, V0=2.41*1e6):
    """
    Returns the analytical solution for the z-coordinate of a single particle in a Penning trap.
    where omega_z = sqrt(2 * q * V0 / (m * d^2)).
    """
    omega_0 = q * B0 / m
    omega_z = np.sqrt(2 * q * V0 / (m * d**2))

    x_0, y_0, z_0 = r_0
    v_x0, v_y0, v_z0 = v_0

    temp = np.sqrt(omega_0**2 - 2 * omega_z**2)
    omega_pos = 0.5 * (omega_0 + temp)
    omega_neg = 0.5 * (omega_0 - temp)
    omega_z = np.sqrt(2 * q * V0 / (m * d**2))

    A_pos =  (v_x0 + omega_neg * y_0) / (omega_neg - omega_pos)
    A_neg = -(v_x0 + omega_pos * y_0) / (omega_neg - omega_pos)
    
    f = A_pos*np.exp(1j*omega_pos*t) + A_neg*np.exp(1j*omega_neg*t)
    x = np.real(f)
    y = np.imag(f)
    z = z_0 * np.cos(omega_z * t) + (v_z0 / omega_z) * np.sin(omega_z * t)

    return x, y, z


def rel_error(data, r_0, v_0):
    """
    Computes the relative error between the numerical and analytical solutions.
    """
    t_numerical = data[:,0]
    z_numerical = data[:,3]

    _, _, z_analytical = analytical_solution(t_numerical, r_0, v_0)

    return np.abs(z_numerical - z_analytical) / np.abs(z_analytical)


r_0 = np.array([20, 0, 20])  # Initial position [um]
v_0 = np.array([0, 25, 0])    # Initial velocity
t = np.linspace(0, 50, 1000)  # Time array [μs]

for method in ['FE', 'RK4']:
    plt.figure(figsize=(6,4))
    for n_steps in [4000, 8000, 16000, 32000]:
        data = np.loadtxt(f'out/r1-single-{method}-n{n_steps}.txt')
        error = rel_error(data, r_0, v_0)
        plt.plot(data[:,0], error, label=f'n_steps={n_steps}')

    plt.xlabel(r'$t\ \ [\mu s]$')
    plt.ylabel(r'Relative error in $z$')
    plt.yscale('log')
    plt.legend()
    plt.savefig(f'out/relerror-{method}.pdf')