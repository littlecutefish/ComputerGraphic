// OpenGL and FreeGlut headers.
#include <GLUT/GLUT.h>

// GLM.
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ STL headers.
#include <iostream>
#include <vector>
#include <string>

// My headers.
#include "trianglemesh.h"

// Global variables.
const int screenWidth = 600;
const int screenHeight = 600;
TriangleMesh* mesh = nullptr;
GLuint vbo;  //Vertex Buffer Object
GLuint ibo;  //Index Buffer Object
std::string Path = "/Users/liuliyu/Documents/Junior/computer graphic/HW/ICG2022_HW1_Test_Models/Cube.obj";
std::string NewPath = "/Users/liuliyu/Documents/Junior/computer graphic/HW/ICG2022_HW1_Test_Models/Cube.obj";

// Function prototypes.
void SetupRenderState();
void SetupScene();
void ReleaseResources();
void RenderSceneCB();
void ProcessSpecialKeysCB(int, int, int);
void ProcessKeysCB(unsigned char, int, int);
void createMenu(void);
void mainMenu(int index);
void subMenu1(int index);
void subMenu2(int index);



// Callback function for glutDisplayFunc.
void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);

    // Render the triangle mesh.
    // ---------------------------------------------------------------------------
    // Add your implementation.
    if(Path != NewPath){
        SetupScene();
        Path = NewPath;
    }
    mesh->RenderOBJ(vbo, ibo);
    
    // ---------------------------------------------------------------------------
    
    glDisableVertexAttribArray(0);
    
    glutSwapBuffers();
}

// Callback function for glutSpecialFunc.
void ProcessSpecialKeysCB(int key, int x, int y)
{
    // Handle special (functional) keyboard inputs such as F1, spacebar, page up, etc.
    switch (key) {
    case GLUT_KEY_F1:
        // Render with point mode.
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //畫點
        break;
    case GLUT_KEY_F2:
        // Render with line mode.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //畫線
        break;
    case GLUT_KEY_F3:
        // Render with fill mode.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //填滿
        break;
    default:
        break;
    }
}

// Callback function for glutKeyboardFunc.
void ProcessKeysCB(unsigned char key, int x, int y)
{
    // Handle other keyboard inputs those are not defined as special keys.
    if (key == 27) {
        // Release memory allocation if needed.
        ReleaseResources();
        
        exit(0);
    }
}

void ReleaseResources()
{
    // Release dynamically allocated resources.
    // ---------------------------------------------------------------------------
    // Add your implementation.
    mesh->~TriangleMesh();
    // ---------------------------------------------------------------------------
}

void SetupRenderState()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glm::vec4 clearColor = glm::vec4(0.44f, 0.57f, 0.75f, 1.00f);
    glClearColor(
        (GLclampf)(clearColor.r),
        (GLclampf)(clearColor.g),
        (GLclampf)(clearColor.b),
        (GLclampf)(clearColor.a)
    );
}

void SetupScene()
{
    // Load a model from obj file.
    mesh = new TriangleMesh();
    // ---------------------------------------------------------------------------
    // Add your implementation.
    mesh->LoadFromFile(NewPath, true);
    mesh->ShowInfo();
    // ---------------------------------------------------------------------------
    
    // Build transformation matrices.
    glm::mat4x4 M(1.0f);

    // Camera transform.
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 2.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4x4 V = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    // Perspective projection.
    float fov = 40.0f;
    float aspectRatio = (float)screenWidth / (float)screenHeight;
    float zNear = 0.1f;
    float zFar = 100.0f;
    glm::mat4x4 P = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);

    // Apply CPU transformation.
    glm::mat4x4 MVP = P * V * M;
    
    mesh->ApplyTransformCPU(MVP);
    
    // Create vertex and index buffer.
    // ---------------------------------------------------------------------------
    // Add your implementation.
    // Generate the vertex buffer.
    mesh->CreateVertexBuffers(vbo);
    // Generate the index buffer.
    mesh->CreateIndexBuffers(ibo);
    // ---------------------------------------------------------------------------
}

void menu(int index){}

void mainMenu(int index)
{
    switch(index)
    {
        case 1:
            break;
        case 2:
            break;
    }
}

//定義按鈕功能
void subMenu1(int index)
{
    switch(index)
    {
        case 1:
            NewPath = "/Users/liuliyu/Documents/Junior/computer graphic/HW/ICG2022_HW1_Test_Models/Cube.obj";
            break;
        case 2:
            NewPath = "/Users/liuliyu/Documents/Junior/computer graphic/HW/ICG2022_HW1_Test_Models/Bunny.obj";
            break;
        case 3:
            NewPath = "/Users/liuliyu/Documents/Junior/computer graphic/HW/ICG2022_HW1_Test_Models/Koffing.obj";
            break;
        case 4:
            NewPath = "/Users/liuliyu/Documents/Junior/computer graphic/HW/ICG2022_HW1_Test_Models/Teapot.obj";
            break;
    }
}

void subMenu2(int index)
{
    switch(index)
    {
        case 1:
            glClearColor(0.7, 0.2, 0.2, 1.0);
            break;
        case 2:
            glClearColor(1.0, 0.5, 0.0, 1.0);
            break;
        case 3:
            glClearColor(0.44f, 0.57f, 0.75f, 1.00f);
            break;
        case 4:
            glClearColor(0.3, 0.7, 0.3, 0.0);
            break;
        case 5:
            glClearColor(0.0, 0.0, 0.0, 0.0);
            break;
    }
}

void createMenu()
{
    ///setting all the submenus
    //create sub-menu 1
    int submenu1 = glutCreateMenu(subMenu1);
    glutAddMenuEntry("Cube.obj", 1);//add option of sub manu 1
    glutAddMenuEntry("Bunny.obj", 2);
    glutAddMenuEntry("Koffing.obj", 3);
    glutAddMenuEntry("Teapot.obj", 4);
    
    int submenu2 = glutCreateMenu(subMenu2);
    glutAddMenuEntry("Red", 1);//add option of sub manu 1
    glutAddMenuEntry("Orange", 2);
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Green", 4);
    glutAddMenuEntry("Black", 5);
    
    ///create main menu
    glutCreateMenu(mainMenu);
    glutAddSubMenu("Choose OBJ File", submenu1);//add submenu to main menu
    glutAddSubMenu("Change BackGround Color", submenu2);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
}

int main(int argc, char** argv)
{
    // Setting window properties.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("HW1: OBJ Loader");
 
    
    // Initialization.
    SetupRenderState();
    SetupScene();
    
    // Register callback functions.
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
    glutSpecialFunc(ProcessSpecialKeysCB);
    glutKeyboardFunc(ProcessKeysCB);
    
    //callback event
    createMenu();
    
    // Start rendering loop.
    glutMainLoop();
    
    return 0;
}
