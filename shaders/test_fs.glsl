#version 410

in vec2 texture_coordinates;
in vec3 position_eye, normal_eye;
uniform sampler2D basic_texture;
out vec4 frag_colour;
uniform float time;

uniform mat4 view, proj, model;

vec3 light_position_world = vec3(0.0, 2.0, 1.0);
vec3 Ls = vec3(1.0,1.0,1.0);
vec3 Ld = vec3(1.0,0.0,0.0);
vec3 La = vec3(0.2,0.2,0.2);
vec3 Ks = vec3(1.0,1.0,1.0);
vec3 Kd = vec3(0.7,0.7,0.7);
vec3 Ka = vec3(1.0,1.0,1.0);
float specular_exponent = 50.0;


void main() {
   
	vec4 texel = texture (basic_texture, texture_coordinates);
    
    vec3 light_position_eye = vec3(view * vec4(light_position_world, 1.0));
    vec3 distance_light_eye = light_position_eye - position_eye;
    vec3 direction_to_light_eye = normalize(distance_light_eye);
    
    vec3 reflection_eye = reflect(-direction_to_light_eye, normal_eye);
    vec3 surface_to_viewer_eye = normalize(-position_eye);
    float dot_prod_specular = dot(reflection_eye, surface_to_viewer_eye);
    dot_prod_specular = max(dot_prod_specular, 0.0);
    float specular_factor = pow(dot_prod_specular, specular_exponent);
    
    float dot_prod = dot(direction_to_light_eye, normal_eye);
    dot_prod = max(dot_prod ,0.0);
    
    
    vec3 Ia = La * Ka * vec3(texel); //ambient intensity
    vec3 Id = vec3(texel) * Kd * dot_prod;
    //vec3 Id = Ld * Kd * dot_prod;
    vec3 Is = Ls * Ks * specular_factor;
    
	//frag_colour = texel;
    frag_colour = vec4(Is + Id + Ia, 1.0);
    //frag_colour = texelFetch(noiseTex, ivec2(0,0), 0);

}
