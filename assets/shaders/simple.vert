#version 330

// fist line always should be the version of glsl 
//opengl 3.3 is compitable with 330  glsl


///out vertecies color from vertex shader and send it as in into
//  fragment shader
//out from vertex shader and in to fragment shader is called Varying
out vec4 vertex_color;


//get the vertecies from outside
//not uniform as uniform array access is slow and limited in storage

//layout : to make every object vertics location in the same location 
//-- in the shade memory
layout(location=0) in vec3 positions;
layout(location=1) in vec4 color;
//uniform vars are some vars that being sent from the main c++ code 
//- not from other shader
//

//uniform float time;


//creating 4x4 matrix
//MVP stands for Model View Projection

uniform mat4 MVP

void main(){

    // gl_position is where the pixel positioned and gl_VertexID is the index of the pixel
    //1.0 is the 4th dimentsion (need in the transformation)


    // the vertecies we keep moving in the x axis direction
    gl_Position = MVP*vec4(positions,1.0);
    vertex_color=color;
}
