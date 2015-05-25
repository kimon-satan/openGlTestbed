/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries separate legal notices                              |
|******************************************************************************|
| Texture Mapping                                                              |
| * I used Sean Barrett's stb_image library to load an image file into memory  |
| * I made a load_texture() function to copy this into a GL texture            |
\******************************************************************************/

#include "maths_funcs.h"
#include "gl_utils.h"
#include "primitives.h"
#include "noise.h"
#include "stb_image.h" // Sean Barrett's image loader - http://nothings.org/
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"
#define PI 3.1415968

int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* g_window = NULL;

bool load_texture (const char* file_name, GLuint* tex) {
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load (file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf (stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf (
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

    
    //swap the coodinate space for opengl
	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
    
	glGenTextures (1, tex); //make a texture pointer
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, *tex);
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap (GL_TEXTURE_2D);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    //dunno waht this stuff is
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	return true;
}



int main () {
	assert (restart_gl_log ());
	// use GLFW and GLEW to start GL context. see gl_utils.cpp for details
	assert (start_gl ());

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    Mesh gMesh = Primitives::CreatePlane(1.0, 1.0, 20,4);
    
    gMesh.mTransform = glm::rotate(glm::mat4(1.0f), (float)PI * -0.25f, glm::vec3(1.0f,0.0f,0.0f));
	
    
	GLuint points_vbo;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, gMesh.mNumValues * sizeof (float), gMesh.mVertices, GL_STATIC_DRAW);
    
    GLuint elements = 0;
    glGenBuffers (1, &elements);
    glBindBuffer (GL_ARRAY_BUFFER, elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gMesh.mNumIndices * sizeof(GLuint), gMesh.mIndices, GL_STATIC_DRAW);
    
	
	GLuint texcoords_vbo;
	glGenBuffers (1, &texcoords_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, texcoords_vbo);
	glBufferData (GL_ARRAY_BUFFER, gMesh.mNumTexCoords * 2 * sizeof (GLfloat), gMesh.mTexCoords, GL_STATIC_DRAW);
	
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
    
    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
	
    glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
	glBindBuffer (GL_ARRAY_BUFFER, texcoords_vbo);
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL); // normalise!*/
	

	
	GLuint shader_programme = create_programme_from_files (
		"shaders/test_vs.glsl", "shaders/test_fs.glsl");
	
	#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
	// input variables
	float near = 0.1f; // clipping plane
	float far = 100.0f; // clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	// matrix components
	float range = tan (fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	GLfloat proj_mat[] = {
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};
	
		
	float cam_speed = 1.0f; // 1 unit per second
	float cam_yaw_speed = 10.0f; // 10 degrees per second
	float cam_pos[] = {0.0f, 0.0f, 2.0f}; // don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;
    
    int model_mat_location = glGetUniformLocation( shader_programme, "model");
    glUseProgram (shader_programme);
	glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, &gMesh.mTransform[0][0]);
    
	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
	
	// load texture
	GLuint tex;
	assert (load_texture ("data/skulluvmap.png", &tex));
	
	
	//glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CW); // GL_CCW for counter clock-wise
	
	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;
	
		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float p = 4.0f/3;
    
        int vp_width = g_gl_height * 2 * p;
        int vp_height = g_gl_height * 2;
        
		glViewport (0, 0, vp_width, vp_height);
        
        
		glUseProgram (shader_programme);
        
        int time_loc = glGetUniformLocation (shader_programme, "time");
        glUniform1f(time_loc, (float)current_seconds);
		glBindVertexArray (vao);
        
        gMesh.mTransform = glm::rotate(gMesh.mTransform, (float)PI * -0.005f, glm::vec3(0.0f,0.0f,1.0f));
        
        int model_mat_location = glGetUniformLocation( shader_programme, "model");
        glUseProgram (shader_programme);
        glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, &gMesh.mTransform[0][0]);
        
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		// draw points 0-3 from the currently bound VAO with current in-use shader
        //glDrawArrays(gMesh.mDrawMode, 0, 6);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
		glDrawElements(gMesh.mDrawMode, gMesh.mNumIndices, GL_UNSIGNED_INT, gMesh.mIndices);
		// update other events like input handling 
		
		
		// control keys
		bool cam_moved = false;
		if (glfwGetKey (g_window, GLFW_KEY_A)) {
			cam_pos[0] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_D)) {
			cam_pos[0] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_PAGE_UP)) {
			cam_pos[1] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_PAGE_DOWN)) {
			cam_pos[1] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_W)) {
			cam_pos[2] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_S)) {
			cam_pos[2] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
			cam_yaw += cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
			cam_yaw -= cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
        
      
        
		// update view matrix
		if (cam_moved) {
			mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2])); // cam translation
			mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw); // 
			mat4 view_mat = R * T;
			glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
		}
		
		
        
        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_0)) {
            

            if(gMesh.mDrawMode == GL_LINE_STRIP){
                gMesh.mDrawMode = GL_TRIANGLE_STRIP;
            }else{
                gMesh.mDrawMode = GL_LINE_STRIP;
            }
            
            
        }
        
        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose (g_window, 1);
        }
        
        glfwPollEvents ();
        
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
    //gMesh.destroy();
	return 0;
}
