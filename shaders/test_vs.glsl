#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vt; // per-vertex texture co-ords

uniform mat4 view, proj;
uniform float time;

out vec2 texture_coordinates;

void main() {
	texture_coordinates = vt;
    float incr = sin(time + vertex_position.x * 3.141) * 0.25;
	gl_Position = proj * view * vec4 (vertex_position.x, vertex_position.y + incr, vertex_position.z, 1.0);
}
