#include "stdio.h"
#include <string>
#include <vector>

class MeshClass {

    public:
        MeshClass();
        ~MeshClass();

        void setVerts(std::vector<float> vect);
        void setNormals(std::vector<float> norms);
        void setTexcoords(std::vector<float> texc);
        void setId(std::string name);

        std::string getId();
        std::vector<float> getVerts();
        std::vector<float> getNormals();
        std::vector<float> getTexcoords();

    private:
        std::string id;
        std::vector<float> verts;
        std::vector<float> normals;
        std::vector<float> texcoords;
 
        
};
