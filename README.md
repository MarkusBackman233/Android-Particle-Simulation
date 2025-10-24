

# Android Particle Simulation
* 2D Newtonian Particle Simulation
* Particle simulation that uses phone gyroscope and accelerometer to influence the particle.
* Written from scratch using OpenGL and c++, using java as a wrapper.

## Governing Equations

Newton's second law for a particle of mass $m$ at position $p(t)$ is:

$$
F(t) = m\,a(t) = m\,\frac{d^2p}{dt^2}.
$$



Writing acceleration $a$ explicitly:

$$
a(t) = \frac{F(t)}{m}.
$$

#### Explicit Euler

$$
p_{t+\Delta t} = p_t + v_t\,\Delta t,
$$
$$
v_{t+\Delta t} = v_t + a_t\,\Delta t.
$$

![Screen_Recording_20251024_215830_Game](https://github.com/user-attachments/assets/e92f0d4f-f8d6-49a7-8043-4957387068ee)
