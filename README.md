

# Android Particle Simulation
* 2D Newtonian Particle Simulation
* Particle simulation that uses phone gyroscope and accelerometer to influence the particle.
* Written using OpenGL Graphics, OpenGL Mathematics and c++, using java as a wrapper.

## Particle Integration and Collision Resolution

For each particle $i \in [0, N)$ with position $`\mathbf{p}_i = (x_i, y_i)`$ and velocity$`\mathbf{v}_i = (x_i, y_i)`$:
---

### 1. Spatial Cell Mapping

Each particle is mapped to a uniform grid cell:

$$
c_x = \left\lfloor \frac{x_i}{s} \right\rfloor, \quad
c_y = \left\lfloor \frac{y_i}{s} \right\rfloor
$$

where $\( s \)$ is the cell size.

---

### 2. Velocity and Position Integration

Each frame (or substep), the velocity and position are updated as:

$$
\mathbf{v}_i \leftarrow (\mathbf{v}_i + \mathbf{g} + \mathbf{a}) \, d
$$

$$
\mathbf{p}_i \leftarrow \mathbf{p}_i + \mathbf{v}_i \, \Delta t_{\text{sub}}
$$

where:
- $\( \mathbf{g} \)$ = gravity vector  
- $\( \mathbf{a} \)$ = external acceleration  
- $\( d \)$ = damping factor  
- $\( \Delta t_{\text{sub}} \)$ = substep time interval

---

### 3. Neighbor Cell Search

For collision handling, each particle checks neighboring cells within offsets:

$$
(\Delta x, \Delta y) \in \{-1, 0, 1\}^2
$$

and for each valid neighbor cell $\((n_x, n_y)\)$, all particles $\( j \)$ in that cell are considered.

We only process pairs with $\( j > i \)$ to avoid duplicates.

---

### 4. Particle–Particle Collision

For a neighbor particle \( j \):

Compute relative displacement:

$$
\boldsymbol{\delta} = \mathbf{p}_i - \mathbf{p}_j
$$

$$
d^2 = \boldsymbol{\delta} \cdot \boldsymbol{\delta}
$$

If $\( d^2 < (2r)^2 \)$, a collision occurs (where $\( r \)$ is particle radius).

Then:

$$
d = \sqrt{d^2}, \quad
\mathbf{n} = \frac{\boldsymbol{\delta}}{d}
$$

---

### 5. Positional Correction (Overlap Resolution)

Penetration depth:

$$
p = 2r - d
$$

Apply equal and opposite corrections:

$$
\mathbf{p}_i \leftarrow \mathbf{p}_i + \frac{1}{2} p\,\mathbf{n}, \quad
\mathbf{p}_j \leftarrow \mathbf{p}_j - \frac{1}{2} p\,\mathbf{n}
$$

---

### 6. Velocity Impulse (Elastic Collision)

Relative velocity along the collision normal:

$$
\mathbf{v}_{\text{rel}} = \mathbf{v}_i - \mathbf{v}_j
$$

$$
v_{\text{sep}} = \mathbf{v}_{\text{rel}} \cdot \mathbf{n}
$$

If $\( v_{\text{sep}} < 0 \)$ (particles moving toward each other):

Apply impulse with restitution coefficient $\( e \)$:

$$
\mathbf{J} = -e\,v_{\text{sep}}\,\mathbf{n}\,\frac{1}{2}
$$

Update velocities:

$$
\mathbf{v}_i \leftarrow \mathbf{v}_i + \mathbf{J}
$$

$$
\mathbf{v}_j \leftarrow \mathbf{v}_j - \mathbf{J}
$$

---

### Summary

This procedure models:
- Continuous gravity and damping integration  
- Spatial partitioning for efficient neighbor lookup  
- Overlap correction (position-based collision)  
- Elastic response via velocity impulse

---

**Symbols:**

| Symbol | Meaning |
|:--|:--|
| $\( \mathbf{p}_i \)$ | Position of particle \( i \) |
| $\( \mathbf{v}_i \)$ | Velocity of particle \( i \) |
| $\( \mathbf{g} \)$ | Gravity vector |
| $\( \mathbf{a} \)$ | External acceleration |
| $\( d \)$ | Damping factor |
| $\( e \)$ | Coefficient of restitution |
| $\( r \)$ | Particle radius |
| $\( c \)$ | Particle assigned cell |
| $\( \Delta t_{\text{sub}} \)$ | Substep timestep |

![Screen_Recording_20251024_215830_Game](https://github.com/user-attachments/assets/e92f0d4f-f8d6-49a7-8043-4957387068ee)
