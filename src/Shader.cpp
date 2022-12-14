#include "../include/Shader.h"
#include "vector"
#include <iostream>
#include <fstream>
#include <assimp/types.h>


Shader::Shader(const std::string &vertexShader,
               const std::string &fragmentShader,
               Camera *&camera,
               const std::vector<std::shared_ptr<Light>> &lights, bool skybox) {
    this->camera = camera;
    this->camera->setShader(this);
    std::string vs = this->load(vertexShader);
    std::string fs = this->load(fragmentShader);
    this->vertex_shader = vs.c_str();
    this->fragment_shader = fs.c_str();
    this->lights = lights;
    this->skybox = skybox;
    this->compile();
}

std::string Shader::load(const std::string &t_shader){
    std::string filepath = "../shaders/" + t_shader;
    std::string temp{};
    output = "";

    std::ifstream theFile(filepath);

    if(!theFile){
        std::cout << "[CR] Could not find shader " << filepath << "\n" << std::flush;
        exit(-1);
    }

    while (getline (theFile, temp))
        output += temp + "\n";

    theFile.close();
    return output;
}

GLint Shader::uniformMapper(const std::string &uniformName){
    if (uniforms.find(uniformName) != uniforms.end())
        return uniforms[uniformName];

    uniforms[uniformName] = glGetUniformLocation(shaderProgram, uniformName.c_str());
    return uniforms[uniformName];
}

void Shader::compile() {

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    error_check();

    glProgramUniform1i(shaderProgram, uniformMapper("how_many_lights"), lights.size());
    /* LIGHTS */
    for (size_t i = 0; i < lights.size(); ++i){
        glProgramUniform3fv(shaderProgram, uniformMapper("lights[" + std::to_string(i) + "].color"), 1, glm::value_ptr(lights[i]->getColor()));
        glProgramUniform3fv(shaderProgram, uniformMapper("lights[" + std::to_string(i) + "].position"), 1, glm::value_ptr(lights[i]->getPosition()));
        glProgramUniform1i(shaderProgram, uniformMapper("lights[" + std::to_string(i) + "].type"), lights[i]->getType());
        if (lights[i]->getType() == 2 || lights[i]->getType() == 3)
            glProgramUniform3fv(shaderProgram, uniformMapper("lights[" + std::to_string(i) + "].direction"), 1, glm::value_ptr(lights[i]->getDirection()));
        if (lights[i]->getType() == 3)
            glProgramUniform1f(shaderProgram, uniformMapper("lights[" + std::to_string(i) + "].cutoff"), lights[i]->getCutoff());
    }

    glProgramUniform1f(shaderProgram, uniformMapper("foggy"), fog);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

}

void Shader::draw(glm::mat4 t_matrix, glm::vec3 t_objectColor) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(uniformMapper("modelMatrix"), 1, GL_FALSE, &t_matrix[0][0]);
    glUniform3fv(uniformMapper("objectColor"), 1, glm::value_ptr(t_objectColor));
    glUniform3fv(uniformMapper("lightPos"), 1, glm::value_ptr(lights[0]->getPosition()));
    glProgramUniform3fv(shaderProgram, uniformMapper("lightColor"), 1, glm::value_ptr(lights[0]->getColor()));
}

void Shader::error_check() const {
    GLint status;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, strInfoLog);
        std::cout << "[CR] Linker failure: " << strInfoLog;
        delete[] strInfoLog;
    }
}

void Shader::update(Subject& subject) {
    if(&subject == camera) {
        this->projection = camera->getPerspective();
        this->camMatrix = camera->getCamera();
        glProgramUniformMatrix4fv(shaderProgram, uniformMapper("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
        glProgramUniformMatrix4fv(shaderProgram, uniformMapper("viewMatrix"), 1, GL_FALSE, glm::value_ptr(camMatrix));
        glProgramUniform3fv(shaderProgram, uniformMapper("cameraPosition"), 1, glm::value_ptr(camera->getPosition()));
        for (size_t i = 0; i < lights.size(); ++i){
            if (lights[i]->getType() == 3){
                glProgramUniform3fv(shaderProgram, uniformMapper("lights[" + std::to_string(i) + "].position"), 1, glm::value_ptr(camera->getPosition()));
                glProgramUniform3fv(shaderProgram, uniformMapper("lights[" + std::to_string(i) + "].direction"), 1, glm::value_ptr(camera->getDirection()));
            }
        }
    }
}
