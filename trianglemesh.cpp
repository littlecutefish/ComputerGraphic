#include "trianglemesh.h"

// Constructor of a triangle mesh.
TriangleMesh::TriangleMesh()
{
    numVertices = 0;
    numTriangles = 0;
    objCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    objExtent = glm::vec3(0.0f, 0.0f, 0.0f);
}

// Destructor of a triangle mesh.
TriangleMesh::~TriangleMesh()
{
	// -------------------------------------------------------
	// Add your release code here.
	// -------------------------------------------------------
}

// Load the geometry and material data from an OBJ file.
bool TriangleMesh::LoadFromFile(const std::string& filePath, const bool normalized)
{	
	// Parse the OBJ file.
	// ---------------------------------------------------------------------------
    // Add your implementation here (HW1 + read *.MTL).
    // ---------------------------------------------------------------------------
    std::fstream file,mtl;
    std::string tmp;
    file.open(filePath, std::ios::in);
    if (!file)
    {
        std::cout << "Open file error!" << std::endl;
    }
    
    int p = -1, n = -1, t = -1, f = 0, use = -1;

    std::vector<glm::vec3> pos2;
    std::vector<glm::vec3> nor2;
    std::vector<glm::vec2> tex2;
    std::vector<std::vector<int>> face2;
    std::vector<std::string> color2;
    
    glm::vec2 X_maxmin = glm::vec2(0.0f, 0.0f);
    glm::vec2 Y_maxmin = glm::vec2(0.0f, 0.0f);
    glm::vec2 Z_maxmin = glm::vec2(0.0f, 0.0f);
    
    // 存mtl的變數
    std::string mtl_temp, color, mtl_value;
    float ns = 0.0;
    glm::vec3 ka = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 kd = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 ks = glm::vec3(0.0f,0.0f,0.0f);
    std::vector<glm::vec3> ka2;
    std::vector<glm::vec3> kd2;
    std::vector<glm::vec3> ks2;
    std::vector<float> ns2;
    std::vector<std::string> mtlName;
//    int num=0, newnum=0;
    
    std::vector<unsigned int> vertexindex;
    std::vector<std::vector<unsigned int>> vertexindex2;
    
//    SubMesh submesh;
    
    if (file.is_open()) {
        std::string line, mtlPath;

        
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            ss >> type;
            
            if(type == "mtllib"){  // 讀入mtl檔
                ss >> mtlPath;  // mtl 的檔案路徑
                
                for(int i = 0; i < mtlPath.length(); i++){
                    if(mtlPath[i] == '.') tmp = mtlPath.substr(0,i);
                }
                mtlPath = "/Users/liuliyu/Documents/Junior/Computer_graphic/HW2/ICG2022_HW2/ICG2022_HW2/models/"+ tmp + "/" + mtlPath;
                
                int flag_mtl = 0; //表示未讀過mtl檔案
                if(!flag_mtl){
                    mtl.open(mtlPath, std::ios::in);
                    if(!mtl){
                        std::cout << "Open mtl error!" << std::endl;
                    }
                    
                    flag_mtl = 1; //表示已經讀過mtl檔案了
                    
                    while (getline(mtl, line)) {
                        std::stringstream stemp(line);
                        stemp >> mtl_temp;
                        if (mtl_temp == "newmtl"){ // 存顏色
                            stemp >> color;
                            mtlName.push_back(color);
                        }
                        else if (mtl_temp == "Ns"){
                            stemp >> ns;
                            ns2.push_back(ns);
                        }
                        else if (mtl_temp == "Ka"){
                            stemp >> ka[0] >> ka[1] >>ka[2];
                            ka2.push_back(ka);
                        }
                        else if (mtl_temp == "Kd"){
                            stemp >> kd[0] >> kd[1] >>kd[2];
                            kd2.push_back(kd);
                        }
                        else if (mtl_temp == "Ks"){
                            stemp >> ks[0] >> ks[1] >>ks[2];
                            ks2.push_back(ks);
                            mtl_temp = " "; //還原初始化
                        }
                    }
                    mtl.close();
                }
            }
            
            // position
            if (type == "v") {
                glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
                ss >> pos.x >> pos.y >> pos.z;
                pos2.push_back(pos);
                objCenter.x += pos.x;
                objCenter.y += pos.y;
                objCenter.z += pos.z;
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
            
            else if (type == "usemtl"){
                use++;
                ss >> color;
                color2.push_back(color);

                if(vertexindex.size() != 0){
                    vertexindex2.push_back(vertexindex);
                    vertexindex.clear();
                }
            }
            
            std::vector<int> face(20), temp;
            while(ss >> face[0]) {
                if(type == "f") f++;
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
                
                if(temp.size() == 3){
                    for(int count = 0; count < 3; count++){
                        vertexindex.push_back(temp[count]);
                    }
                }
                else if (temp.size() > 3){
                    for(int count = 2; count < temp.size(); count++){
                        vertexindex.push_back(temp[0]);
                        vertexindex.push_back(temp[count - 1]);
                        vertexindex.push_back(temp[count]);
                    }
                }
            }
        }
        vertexindex2.push_back(vertexindex);
        
        VertexPTN vertexptn;

        for (int i = 0; i < pos2.size(); i++){
            vertexptn.position.x = pos2[i][0];
            vertexptn.position.y = pos2[i][1];
            vertexptn.position.z = pos2[i][2];
            vertices.push_back(vertexptn);
        }
        numVertices = p + 1;
        
        for (int i = 0; i < nor2.size(); i++){
            vertexptn.normal.x = nor2[i][0];
            vertexptn.normal.y = nor2[i][1];
            vertexptn.normal.z = nor2[i][2];
            vertices.push_back(vertexptn);
        }
        
        for (int i = 0; i < tex2.size(); i++){
            vertexptn.texcoord.x = tex2[i][0];
            vertexptn.texcoord.y = tex2[i][1];
            vertices.push_back(vertexptn);
        }
        
        file.close();
    }
    
    SubMesh submesh;
    PhongMaterial* phong[100];  //宣告一個指標p (動態配置記憶體) 之後要delete p
    for(int i = 0; i < use + 1; i++){
        phong[i] = new PhongMaterial();
        for(int j = 0; j < mtlName.size(); j++){
            if(color2[i] == mtlName[j]){
                phong[i]->SetNs(ns2[j]);
                phong[i]->SetKs(ks2[j]);
                phong[i]->SetKd(kd2[j]);
                phong[i]->SetKa(ka2[j]);
                phong[i]->SetName(mtlName[j]);
            }
        }
        submesh.material=phong[i];
        submesh.vertexIndices = vertexindex2[i];
        subMeshes.push_back(submesh);
    }

    objCenter.x /= p;
    objCenter.y /= p;
    objCenter.z /= p;
    
    for(int i = 0; i < subMeshes.size(); i++){
        numTriangles += (subMeshes[i].vertexIndices.size() / 3);
    }
    
	// Normalize the geometry data.
	if (normalized) {
		// -----------------------------------------------------------------------
		// Add your normalization code here (HW1).
		// -----------------------------------------------------------------------
        float max = X_maxmin.x - X_maxmin.y;
        if(max < Y_maxmin.x - Y_maxmin.y) max = Y_maxmin.x - Y_maxmin.y;
        if(max < Z_maxmin.x - Z_maxmin.y) max = Z_maxmin.x - Z_maxmin.y;
        
        objExtent.x = (X_maxmin.x - X_maxmin.y)/max;
        objExtent.y = (Y_maxmin.x - Y_maxmin.y)/max;
        objExtent.z = (Z_maxmin.x - Z_maxmin.y)/max;
        
        for(int i = 0; i < numVertices; i++){
            vertices[i].position.x = (vertices[i].position.x - objCenter.x)/max;
            vertices[i].position.y = (vertices[i].position.y - objCenter.y)/max;
            vertices[i].position.z = (vertices[i].position.z - objCenter.z)/max;
        }
	}
	return true;
}

// Show model information.
void TriangleMesh::ShowInfo()
{
	std::cout << "# Vertices: " << numVertices << std::endl;
	std::cout << "# Triangles: " << numTriangles << std::endl;
	std::cout << "Total " << subMeshes.size() << " subMeshes loaded" << std::endl;
	for (unsigned int i = 0; i < subMeshes.size(); ++i) {
		const SubMesh& g = subMeshes[i];
		std::cout << "SubMesh " << i << " with material: " << g.material->GetName() << std::endl;
		std::cout << "Num. triangles in the subMesh: " << g.vertexIndices.size() / 3 << std::endl;
	}
	std::cout << "Model Center: " << objCenter.x << ", " << objCenter.y << ", " << objCenter.z << std::endl;
	std::cout << "Model Extent: " << objExtent.x << " x " << objExtent.y << " x " << objExtent.z << std::endl;
}
