#version 330

//the output fragment
out vec4 frag_color;

//uniform float time;

in vec4 vertex_color;


void main(){
    //vec4 tint =vec4(sin(10.0*time),sin(10.0*time+1.0),sin(10.0*time+2.0),1.0);
    frag_color =vertex_color;

}