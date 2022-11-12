#include "trianglemesh.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

void splitStr2Vec(std::string s, std::vector<std::string>& buf);

// Constructor of a triangle mesh.
TriangleMesh::TriangleMesh()
{
    numVertices = 0;
    numTriangles = 0;
    objCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    objExtent = glm::vec3(0.0f, 0.0f, 0.0f);
    // ---------------------------------------------------------------------------
    // Feel free to add codes.
    // ---------------------------------------------------------------------------
}

// Destructor of a triangle mesh.
TriangleMesh::~TriangleMesh()
{
    vertices.clear();
    vertexIndices.clear();
    // ---------------------------------------------------------------------------
    // Feel free to add codes.
    // ---------------------------------------------------------------------------
}

// Load the geometry and material data from an OBJ file.
bool TriangleMesh::LoadFromFile(const std::string& filePath, const bool normalized)
{
    // Parse the OBJ file.
    std::fstream file;
    file.open(filePath, std::ios::in);
    if (!file)
    {
        std::cout << "Open file error!" << std::endl;
    }
    
    int p = -1, n = -1, t = -1, f = -1;

    std::vector<glm::vec3> pos2;
    std::vector<glm::vec3> nor2;
    std::vector<glm::vec2> tex2;
    std::vector<std::vector<int>> face2;
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glm::vec2 X_maxmin = glm::vec2(0.0f, 0.0f);
    glm::vec2 Y_maxmin = glm::vec2(0.0f, 0.0f);
    glm::vec2 Z_maxmin = glm::vec2(0.0f, 0.0f);
    
    if (file.is_open()) {
        std::string line;

        
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            ss >> type;
            
            std::string find_space = " ";
            
            // position
            if (type == "v") {
                glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
                ss >> pos.x >> pos.y >> pos.z;
                pos2.push_back(pos);
                center.x += pos.x;
                center.y += pos.y;
                center.z += pos.z;
                if(pos.x > X_maxmin.x) X_maxmin.x = pos.x; //x max
                else if(pos.x < X_maxmin.y) X_maxmin.y = pos.x; //x min
                
                if(pos.y > Y_maxmin.x) Y_maxmin.x = pos.y; //y max
                else if(pos.y < Y_maxmin.y) Y_maxmin.y = pos.y; //y min
                 
                if(pos.z > Z_maxmin.x) Z_maxmin.x = pos.z; //z max
                else if(pos.z < Z_maxmin.y) Z_maxmin.y = pos.z; //z min
                p++;
            }
            // normal
            else if (type == "vn") {
                glm::vec3 nor = glm::vec3(0.0f, 0.0f, 0.0f);
                ss >> nor.x >> nor.y >> nor.z;
                nor2.push_back(nor);
                n++;
            }
            // texcoord
            else if (type == "vt") {
                glm::vec2 tex = glm::vec2(0.0f, 0.0f);
                ss >> tex.x >> tex.y;
                tex2.push_back(tex);
                t++;
            }
            else if (type == "f") {
                f++;
            }
            
            std::vector<int> face(20), temp;
            while(ss >> face[0]) {
                if (type == "f") {
                    int k = 1;
                    char c;
                    for(int j = 0; j < 2; j++){
                        ss >> c >> face[k];
                        k++;
                    }
                    
                    for(int j = 0; j < 3; j++){
                        face[j] = abs(face[j]);
                    }
                    temp.push_back(face[0] - 1);
                    face2.push_back(face);
                }
                if(temp.size() == 3){
                    for(int count = 0; count < 3; count++){
                        vertexIndices.push_back(temp[count]);
                    }
                }
                else if (temp.size() > 3){
                    for(int count = 2; count < temp.size(); count++){
                        vertexIndices.push_back(temp[0]);
                        vertexIndices.push_back(temp[count - 1]);
                        vertexIndices.push_back(temp[count]);
                    }
                }
            }
            
        }
        VertexPTN vertexptn;

        for (int i = 0; i < pos2.size(); i++){
            vertexptn.position.x = pos2[i][0];
            vertexptn.position.y = pos2[i][1];
            vertexptn.position.z = pos2[i][2];
            vertices.push_back(vertexptn);
        }
        numVertices = p + 1;
        
        file.close();
    }
    
    center.x /= p;
    center.y /= p;
    center.z /= p;
    
    // Normalize the geometry data.
    if (normalized) {
        // -----------------------------------------------------------------------
        // Add your implementation.
        float max = X_maxmin.x - X_maxmin.y;
        if(max < Y_maxmin.x - Y_maxmin.y) max = Y_maxmin.x - Y_maxmin.y;
        if(max < Z_maxmin.x - Z_maxmin.y) max = Z_maxmin.x - Z_maxmin.y;
        
        objExtent.x = (X_maxmin.x - X_maxmin.y)/max;
        objExtent.y = (Y_maxmin.x - Y_maxmin.y)/max;
        objExtent.z = (Z_maxmin.x - Z_maxmin.y)/max;
        
        for(int i = 0; i < numVertices; i++){
            vertices[i].position.x = (vertices[i].position.x - center.x)/max;
            vertices[i].position.y = (vertices[i].position.y - center.y)/max;
            vertices[i].position.z = (vertices[i].position.z - center.z)/max;
        }
        
        // -----------------------------------------------------------------------
    }
    return true;
}

// Apply transform to vertices using GPU.
void TriangleMesh::ApplyTransformCPU(const glm::mat4x4& mvpMatrix)
{
    for (int i = 0 ; i < numVertices; ++i) {
        glm::vec4 p = mvpMatrix * glm::vec4(vertices[i].position, 1.0f);
        if (p.w != 0.0f) {
            float inv = 1.0f / p.w;
            vertices[i].position.x = p.x * inv;
            vertices[i].position.y = p.y * inv;
            vertices[i].position.z = p.z * inv;
        }
    }
}

// Show model information.
void TriangleMesh::ShowInfo()
{
    numTriangles = (int)vertexIndices.size() / 3;
    std::cout << "# Vertices: " << numVertices << std::endl;
    std::cout << "# Triangles: " << numTriangles << std::endl;
    std::cout << "Model Center: " << objCenter.x << ", " << objCenter.y << ", " << objCenter.z << std::endl;
    std::cout << "Model Extent: " << objExtent.x << " x " << objExtent.y << " x " << objExtent.z << std::endl;
}



void TriangleMesh::CreateVertexBuffers(GLuint &vbo)
{
    // Generate the vertex buffer for cube.
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTN) * numVertices, &(vertices[0]), GL_STATIC_DRAW);
}

void TriangleMesh::CreateIndexBuffers(GLuint &ibo){
    // Generate the index buffer.
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numTriangles * 3, &(vertexIndices[0]), GL_STATIC_DRAW);
}

void TriangleMesh::RenderOBJ(GLuint &vbo, GLuint &ibo){
    glColor3f(1.0f, 1.0f, 1.0f);    // Not good, only used for quick demo.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTN), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, numTriangles * 3, GL_UNSIGNED_INT, 0);
}
