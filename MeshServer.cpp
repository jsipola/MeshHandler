#include "MeshServer.h"
using namespace std;

struct DataQueue_t {
    queue<vector<float>> DataQ;
    mutex mutex_t;
};

void writeMeshData(fstream &dataFile, vector<float> vectorData, string name = "", string param = ""){
    //kirjoittaa fileen?
    //joka meshille erillinen file?
    // parametrit?
    //id, pos, vertices, normals, texcoors, jne
    //ascii vai bin
    
    int lineNum = -1;
    string line;
    if (name.length() != 0) {
        dataFile << name << "\n"; 
    }
    if (param.length() != 0) {
       dataFile << param << " ;";
    }
    
    for (const auto &e : vectorData) dataFile << e << " ;";
   
    dataFile << "\n"; 
}


void socketTCP(DataQueue_t &Dataqueue){
    //example from:
    //https://www.geeksforgeeks.org/socket-programming-cc/
    int serverfd, newsocket, valread = 1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *msg = "hello\n";

    cout << "THREAD" << endl;

    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("failed to make socket");
        exit(1);
    }
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("set sock opt failed");
        exit(1); 
    } 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT);

    if (bind(serverfd, (struct sockaddr*) &address, sizeof(address)) < 0){
        perror("failed bind");
        exit(1);
    }
    if (listen(serverfd, 3) < 0){
        perror("failed bind");
        exit(1);
    }
    if ((newsocket = accept(serverfd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("failed to accept");
        exit(1);
    }

    while (valread != 0) { 
        valread = read(newsocket, buffer, 1024);
        printf("%s", buffer);
        send(newsocket, msg, strlen(msg), 0);
        printf("msg sent\n");
        printf("from q: %d\n", Dataqueue.DataQ.front().size());
        Dataqueue.DataQ.pop();
    }
}

int main(int argc, char *argv[]){

    DataQueue_t data; 

    string input = "./models/Bedro.obj";
    //string input = "./models/cube.obj";

    tinyobj::attrib_t attr;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string error;
    bool ret = tinyobj::LoadObj(&attr, &shapes, &materials, &error, argv[1], "./models/", true);

    //bool ret = tinyobj::LoadObj(&attr, &shapes, &materials, &error, input.c_str(), "./models/", true);
    if (!error.empty()) cerr << error << endl;

    cout << shapes.size() << endl;
    cout << "MAIN" << endl;

//    thread t1(socketTCP, ref(data));

    if (!ret) exit(1);

    char* num;
    vector<float> verts = attr.vertices;
    vector<float> verts_alt;
    vector<float> normals = attr.normals;
    vector<float> normals_alt;
    vector<float> texcoords = attr.texcoords;
    vector<float> texcoords_alt;
    vector<float> triangles;
    vector<float> pos;
    vector<float> material;

    cout << "Verts: " << verts.size() / 3 << endl;
    cout << "Normals: " << normals.size() / 3 << endl;
    cout << "Texcoords: " << texcoords.size() / 2 << endl;

    fstream ff;
    char msg[40] = "./models/meshes/";
    strcat(msg, argv[2]);
    strcat(msg, ".txt");
    puts(msg);
    
    ff.open(msg, fstream::in | fstream::out | fstream::trunc);
    pos.push_back(300); // x
    pos.push_back(0); // y
    pos.push_back(0); // z
    pos.push_back(0); // pitch
    pos.push_back(0); // yaw
    pos.push_back(-90);// roll
    writeMeshData(ff, pos, "", "pos"); 
    writeMeshData(ff, verts, "", "v");
    writeMeshData(ff, normals, "", "n");
    writeMeshData(ff, texcoords, "", "tc");
    int counter = 0;
    for (size_t i=0; i < shapes.size(); i++) {
        size_t index = 0;
        cout << "Number of triangles: " << shapes[i].mesh.indices.size() / 3 << endl << endl;
        for (size_t x=0; x < shapes[i].mesh.indices.size();x++) {
            triangles.push_back(shapes[i].mesh.indices[x].vertex_index); 
        }
        if (materials.size() > 0) {
            material.push_back(materials[i].ambient[0]);
            material.push_back(materials[i].ambient[1]);
            material.push_back(materials[i].ambient[2]);
            material.push_back(materials[i].diffuse[0]);
            material.push_back(materials[i].diffuse[1]);
            material.push_back(materials[i].diffuse[2]);
            material.push_back(materials[i].specular[0]);
            material.push_back(materials[i].specular[1]);
            material.push_back(materials[i].specular[2]);
        }
        writeMeshData(ff, triangles, "", "tri");
        if (materials.size() > 0 && i < materials.size()){
            stringstream ss;
            string name =  materials[i].name.c_str() ;
            ss << "m ;" << name;
            string id = ss.str();
            writeMeshData(ff, material, "", id);
            material.clear();
        }
        ff << "s ;" << endl;
        triangles.clear();
    }

    unique_lock<mutex> mlock(data.mutex_t);
    uint8_t siz = verts.size();
    //for (int i=0;i< siz;++i) {
    data.DataQ.push(attr.vertices);
        //printf("%f ", attr.vertices[i]);
    //}
    mlock.unlock();

//    cout << verts_alt.size() / 3 << endl;
//    cout << "Triangles: " << triangles.size() / 3  << endl;
//    cout << "Verts alt: " << verts_alt.size() / 3 << endl;
/*
    fstream filee;
    //filee.open("mesh_cornell.txt");
    filee.open("./models/mesh_cube.txt");//, ios::app);
    string name = "cornell";
    writeMeshData(filee, verts, "", "v");
    writeMeshData(filee, normals, "", "n"); 
    writeMeshData(filee, texcoords, "", "tc");
    writeMeshData(filee, triangles, "", "tri");
    
    filee.close();
*/
//    string name = "cornell";
//    writeMeshData(ff, verts, "", "v");
//    writeMeshData(ff, normals_alt, "", "n"); 
//    writeMeshData(ff, texcoords_alt, "", "tc");
//    writeMeshData(ff, triangles, "", "tri");
//    ff << "s ;";
    ff.close();


    cout << "END" << endl;
//    t1.join();
    return 0;
}

