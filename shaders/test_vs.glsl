#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vt; // per-vertex texture co-ords

uniform mat4 view, proj, model;
uniform float time;

out vec2 texture_coordinates;

void main() {
	texture_coordinates = vt;
    float incr_z = sin(time * 2.0 + vertex_position.x * 3.141 * 20.0) * 0.25;
    float incr_y = sin(time * 2.0 + vertex_position.z * 3.141 * 20.0) * 0.25;
	gl_Position = proj * view * model * vec4 (vertex_position.x, vertex_position.y + incr_y, vertex_position.z + incr_z, 1.0);
}
