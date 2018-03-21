#include "MeshClass.h"



MeshClass::MeshClass(){

}

MeshClass::~MeshClass(){

}

void MeshClass::setVerts(std::vector<float> vect){
    verts = vect;
}
void MeshClass::setNormals(std::vector<float> norms){
    normals = norms;
}
void MeshClass::setTexcoords(std::vector<float> texc){
    texcoords = texc;
}
std::vector<float> MeshClass::getVerts(){
    return verts;
}
std::vector<float> MeshClass::getNormals(){
    return normals;
}
std::vector<float> MeshClass::getTexcoords(){
    return texcoords;
}
std::string MeshClass::getId(){
    return id;
}

void MeshClass::setId(std::string name){
    id = name;
}

