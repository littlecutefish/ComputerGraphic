#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

// OpenGL and FreeGlut headers.
#include <GLUT/GLUT.h>

// GLM.
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// C++ STL headers.
#include <iostream>
#include <vector>



// VertexPTN Declarations.
struct VertexPTN
{
    VertexPTN() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        normal = glm::vec3(0.0f, 1.0f, 0.0f);
        texcoord = glm::vec2(0.0f, 0.0f);
    }
    VertexPTN(glm::vec3 p, glm::vec3 n, glm::vec2 uv) {
        position = p;  // 3��
        normal = n;       // 3��
        texcoord = uv; // 2��
    }
    glm::vec3 position;        // Vertex position.
    glm::vec3 normal;        // Vertex normal.
    glm::vec2 texcoord;        // Vertex texture coordinate (will be used later HWs)
};

// TriangleMesh Declarations.
class TriangleMesh
{
public:
    // TriangleMesh Public Methods.
    TriangleMesh();
    ~TriangleMesh();
    
    // Load the model from an *.OBJ file.
    bool LoadFromFile(const std::string& filePath, const bool normalized = true);
    
    // Apply transform on CPU.
    void ApplyTransformCPU(const glm::mat4x4& mvpMatrix);

    // Show model information.
    void ShowInfo();
    
    // ---------------------------------------------------------------------------
    // Add new methods if needed.
    int GetNumVertices(){
        return numVertices;
    }
    
    void CreateVertexBuffers(GLuint &vbo);
    
    void CreateIndexBuffers(GLuint &ibo);
    
    void RenderOBJ(GLuint &vbo, GLuint &ibo);
    
private:
    // TriangleMesh Private Methods.
    // ---------------------------------------------------------------------------
    // Add new methods if needed.
    // ---------------------------------------------------------------------------

    // TriangleMesh Private Data.
    std::vector<VertexPTN> vertices;
    std::vector<unsigned int> vertexIndices;
    int numVertices;
    int numTriangles;
    glm::vec3 objCenter;
    glm::vec3 objExtent;
    // ---------------------------------------------------------------------------
    // Add new data if needed.
    // ---------------------------------------------------------------------------
};

#endif
