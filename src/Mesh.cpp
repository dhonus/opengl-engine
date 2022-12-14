//
// Created by daniel on 11/16/22.
//

#include "../include/Mesh.h"

int indicesCount = 0;

Mesh::Mesh(const std::vector<float> &vertices, GLenum mode, int vertexCount, int positionSize, int normalsSize,
           int normalsOffset, int overallSize) {
    this->mode = mode;
    this->vertexCount = vertexCount;

    this->VBO = std::make_shared<Vbo>(vertices, vertexCount, positionSize, normalsSize, normalsOffset, overallSize);
    this->VAO = std::make_shared<Vao>(VBO);
    bind_vertex_array();
}

Mesh::Mesh(const std::string &fileName) {
    this->mode = GL_TRIANGLES;

    Assimp::Importer importer;
    unsigned int importOptions = aiProcess_OptimizeMeshes              // sloučení malých plošek
                                 | aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodně duplikuje
                                 | aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
                                 | aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy

    //aiProcess_GenNormals/ai_Process_GenSmoothNormals - vypocet normal s jemnych prechodem v pripade, ze objekt neobsahuje normaly

    std::string file = "../models/obj/" + fileName;
    const aiScene *scene = importer.ReadFile(file, importOptions);
    aiMesh *mesh = scene->mMeshes[0];

    if (scene) {
        count = mesh->mNumFaces * 3;
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            for (unsigned int j = 0; j < 3; j++) {
                data.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].z);
                data.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].z);
                data.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].z);
            }
        }
    }

    this->vertexCount = count;
    // get current time in milliseconds

    this->VBO = std::make_shared<Vbo>(data, vertexCount, 3, 3, 3, 8);
    this->VAO = std::make_shared<Vao>(VBO, true);

    bind_vertex_array();
    std::cout << "\t[->] Created mesh from file: " << fileName << std::endl;
}

void Mesh::bind_vertex_array() {
    this->VAO->bind_vertex_array();
}

void Mesh::draw_arrays() {
    glDrawArrays(mode, 0, vertexCount);
}