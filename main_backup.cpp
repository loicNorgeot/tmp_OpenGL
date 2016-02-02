#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <fstream>

class Shader;
class Object;

Shader* shader1;


void print(glm::vec3 v){
	std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}
void print(glm::mat4 m){
	for( int i = 0 ; i < 4 ; i++ )
		std::cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// SHADERS
////////////////////////////////////////////////////////////////////////////////////////////////////////

class Shader
{
public:
  	GLuint      ID;
  
  	Shader(){
  		ID = 0;
	}
  	void load(std::string vert_path, std::string frag_path){
	    // Crée les shaders
	    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	    GLint Result = GL_FALSE;
	    int InfoLogLength;

	    // Lit le code du vertex shader à partir du fichier
	    std::string VertexShaderCode;
	    std::ifstream VertexShaderStream(vert_path.c_str(), std::ios::in);
	    if(VertexShaderStream.is_open()){
	      std::string Line = "";
	      while(getline(VertexShaderStream, Line))
	        VertexShaderCode += "\n" + Line;
	      VertexShaderStream.close();
	    }
	    else{exit(144);}
		
	    // Lit le code du fragment shader à partir du fichier
	    std::string FragmentShaderCode;
	    std::ifstream FragmentShaderStream(frag_path.c_str(), std::ios::in);
	    if(FragmentShaderStream.is_open()){
	      std::string Line = "";
	      while(getline(FragmentShaderStream, Line))
	        FragmentShaderCode += "\n" + Line;
	      FragmentShaderStream.close();
	    }
	    else{exit(145);}

	    // Compile le vertex shader
	    printf("Compiling shader : %s\n", vert_path.c_str());
	    char const * VertexSourcePointer = VertexShaderCode.c_str();
	    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	    glCompileShader(VertexShaderID);
	    // Vérifie
	    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	    // Compile le fragment shader
	    printf("Compiling shader : %s\n", frag_path.c_str());
	    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	    glCompileShader(FragmentShaderID);
	    // Vérifie
	    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	    // Lit le programme
	    fprintf(stdout, "Linking program\n");
	    GLuint ProgramID = glCreateProgram();
	    glAttachShader(ProgramID, VertexShaderID);
	    glAttachShader(ProgramID, FragmentShaderID);
	    glLinkProgram(ProgramID);

	    // Vérifie le programme
	    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
	    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	    glDeleteShader(VertexShaderID);
	    glDeleteShader(FragmentShaderID);
	    ID = ProgramID;
  	}
};






////////////////////////////////////////////////////////////////////////////////////////////////////////
// OBJECT
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic ignored "-Wwrite-strings"

int  initProgram(int ID);
void createBuffer(GLuint *pBuffer, std::vector<float> *data);
void createBuffer(GLuint *pBuffer, std::vector<short> *data);
void createBuffer(GLuint *pBuffer, std::vector<int>   *data);
void bindBuffer(int attrib, int bufferType, GLuint buffer);
void freeBuffer();
void draw(int ID, int s, int mBuffer, int nBuffer, int iBuffer, int cBuffer = -1);
void send(int ID, float     f, 	char* name);
void send(int ID, glm::vec3 v, 	char* name);
void send(int ID, glm::mat4 &m, char* name);

//BUFFER OPERATIONS
void    createBuffer(GLuint *pBuffer, std::vector<float> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ARRAY_BUFFER, sizeof(float) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void    createBuffer(GLuint *pBuffer, std::vector<int> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void    createBuffer(GLuint *pBuffer, std::vector<short> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void    bindBuffer(int attrib, int bufferType, GLuint buffer){
    if(attrib!=-1)
        glEnableVertexAttribArray( attrib);
    glBindBuffer( bufferType, buffer);
    if(attrib!=-1)
        glVertexAttribPointer( attrib, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
}
void    freeBuffer(){
    for(int i = 0 ; i < 5 ; i++)
        glDisableVertexAttribArray(i);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
int     initProgram(int ID){
    glUseProgram(ID);
    for(int i = 0 ; i < 5 ; i++)
        glEnableVertexAttribArray(5);
    return ID;
}
void    draw(   int ID,         //Shader ID
                int s,          //Number of vertices
                int mBuffer,    //Vertexs BUffer
                int nBuffer,    //Normals Buffer
                int iBuffer,    //Indices Buffer
                int cBuffer     //Colors Buffer
            ){
    freeBuffer();
    //Vertex Buffer
    bindBuffer(0, GL_ARRAY_BUFFER, mBuffer);
    glBindAttribLocation(ID, 0, "vertex_position");
    //Indices Buffer
    bindBuffer(-1, GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    //Normal and Color Buffer
    if(nBuffer!=-1){
        bindBuffer(1, GL_ARRAY_BUFFER, nBuffer);
        glBindAttribLocation(ID, 1, "vertex_normal");
    }
    if(cBuffer != -1){
        bindBuffer(2, GL_ARRAY_BUFFER, cBuffer);
        glBindAttribLocation(ID, 2, "vertex_color");
    }
    //Drawing
    glDrawElements(GL_TRIANGLES, s, GL_UNSIGNED_INT, (void*)0);
    freeBuffer();
}
void send(int ID, glm::mat4 &m, char* name){
	glGetUniformLocation(	ID, name);
    glUniformMatrix4fv( 	ID, 1, GL_FALSE, &m[0][0]);
	
}
void send(int ID, glm::vec3 v, 	char* name){
	glGetUniformLocation(	ID, name);
    glUniform3f(			ID, v.x, v.y, v.z);
}
void send(int ID, float     f, 	char* name){
	glGetUniformLocation(	ID, name);
	glUniform1f( 		 	ID, f);
}

class Object{
public:
	
	int nbVertices;
	
    std::vector<float>  vertices;
    std::vector<float>  normals;
    std::vector<float>  colors;
    std::vector<int>    indices;

    GLuint              mBuffer;
    GLuint              nBuffer;
    GLuint              cBuffer;
    GLuint              iBuffer;
	
	glm::mat4			MODEL;
	
	Object(){
        vertices = std::vector<float>{
            -1.0, -1.0,  1.0,
             1.0, -1.0,  1.0,
             1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            -1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
             1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,
        };
		for(int i = 0 ; i < vertices.size() ; i++)
			vertices[i] = vertices[i];
		nbVertices = vertices.size()/3;
        indices = std::vector<int>{
            0, 1, 2,
            2, 3, 0,
            3, 2, 6,
            6, 7, 3,
            7, 6, 5,
            5, 4, 7,
            4, 5, 1,
            1, 0, 4,
            4, 0, 3,
            3, 7, 4,
            1, 5, 6,
            6, 2, 1
        };
		
		colors = {
			0, 0, 0,   
			0, 0, 1,   
			0, 1, 0,   
			0, 1, 1,
			1, 0, 0,   
			1, 0, 1,   
			1, 1, 0,
			1, 1, 1
	    };
		normals = {
			0, 0, 0,   
			0, 0, 1,   
			0, 1, 0,   
			0, 1, 1,
			1, 0, 0,   
			1, 0, 1,   
			1, 1, 0,
			1, 1, 1
	    };
	    createBuffer(&mBuffer, &vertices);
	    createBuffer(&iBuffer, &indices);
	    createBuffer(&nBuffer, &normals);
	    createBuffer(&cBuffer, &colors);
		std::cout << mBuffer << iBuffer << nBuffer << cBuffer << " " << " " << nbVertices<< std::endl;
	    freeBuffer();
		MODEL = glm::mat4(1.);
	}
	
	void render(){
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		
        int shaderID                = shader1->ID;

		//MODEL = glm::rotate(MODEL, (float)glm::radians(10.), glm::vec3(0,1,0));
		glm::mat4 PROJ = glm::perspective(glm::radians(45.0f), (float)1024 / (float)620, 0.1f, 100.f);
		glm::mat4 VIEW = glm::lookAt(	glm::vec3(3), //eye
										glm::vec3(0.,0.,0.), //center
										glm::vec3(0.,1.,0.)  //up
									);
        glm::mat4 MVP               = PROJ * VIEW * MODEL;
		
		MVP = MODEL;
		
		
        //send(shaderID, MODEL, 	"M");
		//send(shaderID, MVP, 	"MVP");
		//send(shaderID, VIEW, 	"V");
		glGetUniformLocation(	shaderID, "MVP");
	    glUniformMatrix4fv( 	shaderID, 1, GL_FALSE, &MVP[0][0]);
		
        draw(shaderID, nbVertices, mBuffer, nBuffer, iBuffer, cBuffer);
        freeBuffer();
	}	
};


    






////////////////////////////////////////////////////////////////////////////////////////////////////////
// MATRICES
////////////////////////////////////////////////////////////////////////////////////////////////////////









////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////////////////////////

void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
        if(key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* initWindow(const int resX, const int resY)
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

    if(window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);
	

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}



void display( GLFWwindow* window, Object* obj )
{
    while(!glfwWindowShouldClose(window))
    {
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Draw stuff
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        obj->render();

        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();
    }
}


int main(int argc, char** argv)
{
    GLFWwindow* window = initWindow(1024, 620);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
	// Create and bind a VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	Object* object1 = new Object();
	shader1 = new Shader();
	shader1->load("/Users/guestaccount/dev/Minimal_OPENGL/shaders/shader.vert", "/Users/guestaccount/dev/Minimal_OPENGL/shaders/shader.frag");
    if( NULL != window )
    {
        display( window, object1 );
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
