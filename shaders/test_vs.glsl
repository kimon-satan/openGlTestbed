#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vt; // per-vertex texture co-ords

uniform mat4 view, proj, model;
uniform float time;

uniform sampler2D noiseTex;

out vec2 texture_coordinates;

void main() {
	texture_coordinates = vt;
    float phase = gl_VertexID * 1.0;
    float incr_z = sin(time * 4.0 + phase) * 0.1;
    float incr_y = cos(time * 2.0 + vertex_position.y * 3.141 * 5.0) * 0.1;
    int y = gl_VertexID/20;
    int x = gl_VertexID - y * 20;
    
    vec4 noise = texelFetch(noiseTex, ivec2(x,y),0);
    
	gl_Position = proj * view * model * vec4 (vertex_position.x, vertex_position.y , vertex_position.z + noise.x, 1.0);
}
