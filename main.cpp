#include <iostream>


//library to load api functions from the driver of the GPU
//note that GLFW has some defaults vars for opengl if we want
// to ignore it just make sure to include glad first
#include<glad/gl.h>

#include<GLFW/glfw3.h>//lib to be able to open working window through any platform
//note : first build is usually longer than average build due to loading the libaryr
// but then it got cached

//will be used to load shader
#include<string>
#include<fstream>

//mathmatics library 
#include<glm/glm.hpp>

////construct out vertex struct so that it can have less space
struct vec3{
    float x,y,z;
};

struct Color{
    uint8_t r,g,b,a;
};

struct Vertex{
    vec3 position;
    Color color;
};


//function to load shader from file and use it with opengl
// takes the path of the shader file and its type (frag , glsl, vert) 
GLuint load_shader(const std::string& path , GLenum shader_type){
    
    //the 3 following line to read the path
    std::ifstream file(path);
    std::string sourceCode = std::string(std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>());
    const char* sourceCodeCStr = sourceCode.c_str();

    //creating the shader
    GLuint shader = glCreateShader(shader_type);
    
    //assign the used shader to the gl shader
    // note that we cant use string with opengl (only char pointers)
    glShaderSource(shader ,1 ,&sourceCodeCStr,nullptr);
    glCompileShader(shader);  

    //////////////////////////
    ///for gl code error
    /////////////////////////

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::cerr << "ERROR IN " << path << std::endl;
        std::cerr << logStr << std::endl;
        delete[] logStr;
        glDeleteShader(shader);
        return 0;
    }

    return shader; 
}

int main() {
    if(!glfwInit()){ //returns true if successfully start glfw and false if there is an error
        std::cerr<<"Failed to initialize GLFW\n";
        exit(1);
    }
    //we should specify the used version if openGL before creating the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);//3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR ,3);//3


    //function to dispaly a window (read function documentation to know parameters and return type)
    GLFWwindow* window=glfwCreateWindow(640,480,"Hello Trianlge",nullptr,nullptr);

    
    //this to spicfy that the loaded function will be  to the current window
    // this should be specfied before loading the opengl fucntions
    glfwMakeContextCurrent(window);

    if(!window){
        std::cerr<<"Failed to create Window\n";
        glfwTerminate();
        exit(1);
    }
 


    //gladLoadGL : function in glad library to load API function form the GPU Driver
    //glfwGetProcAddress : string with the name of the API function on the driver
    if(!gladLoadGL(glfwGetProcAddress)){
        std::cerr<<"Failed to Load OpenGL\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    //some color config but not actually using it
    glClearColor(0,0,0,1);


    //creating program to be able to use both shaders in the same program
    GLuint program = glCreateProgram();

    //loading the vertex shader
    GLuint vs = load_shader("assets/shaders/simple.vert",GL_VERTEX_SHADER);
    ///loading the fragment shader
    GLuint fs = load_shader("assets/shaders/simple.frag",GL_FRAGMENT_SHADER);
    //now we have program with linked shaders to the same program
    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);

    //after linking the program we no need for the shader
    glDeleteShader(vs);
    glDeleteShader(fs);


    Vertex vertices[4]={
        {{-0.5 ,-0.5,0.0 },   {255,0  ,0  ,255}},
        {{ 0.5 ,-0.5,0.0 },   {0  ,255,0  ,255}},
        {{-0.5 ,0.5 ,0.0 },   {0  ,0  ,255,255}},
    
        {{ 0.5 ,0.5 ,0.0 },   {255,255,0  ,255}},
    };

    ///elements buffer so that it take s the index of vertecies and 
    //-form any element
    uint16_t elements[6]={0,1,2,3,2,1}; 
    //create some empty buffer
    GLuint vertex_buffer;
    //generate the buffer
    glGenBuffers(1,&vertex_buffer);
    //bind the buffer to some type of GL_Buffers 
    //note we bind once then any kind of communication with buffer will be with the binded one
    //untill we bind anoother buffer
    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
    //finlay pass the actual data to the binded buffer
    glBufferData(GL_ARRAY_BUFFER,4*sizeof(Vertex),vertices,GL_STATIC_DRAW);


    GLuint element_buffer;
    //generate the buffer
    glGenBuffers(1,&element_buffer);
    //bind the buffer to some type of GL_Buffers 
    //note we bind once then any kind of communication with buffer will be with the binded one
    //untill we bind anoother buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,element_buffer);
    //finlay pass the actual data to the binded buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(uint16_t),elements,GL_STATIC_DRAW);



    // WE MUST  generate vertex array in order to be able to draw
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    // we bind the vertex array to use it to be able to send vertex buffer to vertex shader
    glBindVertexArray(vertex_array);



    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);

    GLuint position_loc =0; //glGetAttribLocation(program,"positions");
    
    //enable this loc so that i can send data to it if not it will only accepet zeros
    glEnableVertexAttribArray(position_loc);
    //this function is to make the open gl understand the vertex buffer
    glVertexAttribPointer(position_loc,3,GL_FLOAT,false,sizeof(Vertex),nullptr);


    GLuint color_loc =1; // glGetAttribLocation(program,"color");
    
    //enable this loc so that i can send data to it if not it will only accepet zeros
    glEnableVertexAttribArray(color_loc);
    //this function is to make the open gl understand the vertex buffer
    glVertexAttribPointer(color_loc,4,GL_UNSIGNED_BYTE ,true,sizeof(Vertex),(void*)(offsetof(Vertex,color)));



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,element_buffer);

    ///unbind vertex array
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)){//// while loop to keep winodw open until we close it form x button //not: u should read input from user in order to know
        float time= (float)glfwGetTime();




        //every var has a location in the memory
        //we want to 
        GLuint time_loc=glGetUniformLocation(program,"time");
        std::cout<<"Time uniform location " << time_loc<<std::endl;

        //1f because time is float  
        glUniform1f(time_loc,time);

        glClearColor(
           0.0 , 0.0 , 0.0 , 1.0
        );

        //glClear to actually clear the back buffer(screen)
        glClear(GL_COLOR_BUFFER_BIT);//
        /////swap the buffers (front with back) after clearing so that 
        //// clearing can be visible



            //FINAly drawing
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        //glDrawArrays(GL_TRIANGLES,0,6);
        
        //now we draw elements instead
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,(void*)0);



        //drawing another triangle with differs in time
        
        //glUniform1f(time_loc,time+1.0);

        //glDrawArrays(GL_TRIANGLES,0,3);


        glfwSwapBuffers(window);

        glfwPollEvents();//to get input from user 
    }
    
    //delete program at the end of the session
    glDeleteProgram(program);
    glfwDestroyWindow(window); /// destroy window before code exits
    glfwTerminate(); ///function to terminate glfw
    return 0;
}
