#include "../include/Shader.h"
#include "vector"


Shader::Shader() {

    std::vector<float> points = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };

    std::vector<std::vector<glm::vec4>> points2 {
            { { -.5f, -.5f, .5f, 1 }, { 1, 1, 0, 1 } },
            { { -.5f, .5f, .5f, 1 }, { 1, 0, 0, 1 } },
            { { .5f, .5f, .5f, 1 }, { 0, 0, 0, 1 } },
    };

    this->VBO = new Vbo(points);
    this->VAO = new Vao(VBO);

    this->compile();
}

void Shader::compile() {
    //create and compile shaders
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
}

void Shader::use_program() {
    glUseProgram(shaderProgram);
}

Shader::~Shader() {
    delete VAO;
    delete VBO;
}
