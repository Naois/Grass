# Instanced Grass Renderer

https://github.com/user-attachments/assets/d73a720b-9206-4d68-92d3-20ee12322ebe

https://github.com/user-attachments/assets/4e73eee5-3ae5-482c-bd07-e58c6d3b781b

This program contains an implementation of several techniques to produce performant grass rendering.

The grass blade geometry is computed procedurally in the vertex shader without reference to vertex attributes. Each vertex only uses its vertex ID and instance ID to produce a position.

A wind-like effect is simulated by adjusting the grass curvature using scrolling perlin noise. Additionally, to dehomogenise the grass motion, each blade flutters in response to the wind, with a randomised phase, preventing uncanny-looking local synchronisation.

Finally, the grass is rendered using phong shading, with an added occlusion factor to ensure that the parts of the grass closer to the ground look darker. This occlusion factor is computed from the distance along the grass blade.
