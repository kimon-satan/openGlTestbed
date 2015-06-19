#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vt; // per-vertex texture co-ords
layout (location = 2) in vec3 normals;

uniform mat4 view, proj, model;
uniform float time;

uniform sampler2D noiseTex;

uniform int rows;
uniform int cols;

out vec2 texture_coordinates;
out vec3 position_eye, normal_eye;

void main() {
    
    position_eye = vec3(view * model * vec4(vertex_position,1.0));
    normal_eye = vec3(view * model * vec4(normals,0.0));
	texture_coordinates = vt;
    float phase = gl_VertexID * 1.0;
    float incr_z = sin(time * 4.0 + phase) * 0.1;
    float incr_y = cos(time * 2.0 + vertex_position.y * 3.141 * 5.0) * 0.1;
    int y = gl_VertexID/rows;
    int x = gl_VertexID - y * cols;
    
    vec4 noise = texelFetch(noiseTex, ivec2(x,y),0);
    
	//gl_Position = proj * view * model * vec4 (vertex_position.x, vertex_position.y , vertex_position.z + noise.x, 1.0);
    //no noise for time being ... it's too complicated here
    //gl_Position = proj * view * model * vec4 (vertex_position.x, vertex_position.y , vertex_position.z , 1.0);
    gl_Position = proj * vec4(position_eye, 1.0);
}
