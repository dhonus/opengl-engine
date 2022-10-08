//
// Created by daniel on 26.9.22.
//

#ifndef OGL_TST_02_SHADER_H
#define OGL_TST_02_SHADER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vbo.h"
#include "Vao.h"
#include "Object.h"
class Object;

class Shader {
public:
    Shader();
    ~Shader();
    void bind_vertex_array();
    void compile();
    void draw(glm::mat4 t_matrix);
    void setShader();
    void error_check();

private:
    GLint MatId;
    GLuint shaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;
    /*
  const char* vertex_shader =
          "#version 330\n"
          "layout(location=0) in vec3 vp;"
          "out vec3 colors;"
          "void main () {"
          "     gl_Position = vec4 (vp, 1.0);"
          "     colors = vp;"
          "}";
  const char* fragment_shader =
          "#version 330\n"
          "out vec4 frag_colour;"
          "in vec3 colors;"
          "void main () {"
          "     frag_colour = vec4 (colors, 1.0);"
          "}";   * */

  const char* vertex_shader =
          "#version 330\n"
          "layout(location=0) in vec4 vp;"
          "layout(location=1) in vec4 color;"
          "out vec4 colors;"
          "uniform mat4 modelMatrix;"
          "void main () {"
          "     gl_Position = modelMatrix*vp;"
          "     colors = color;"
          "}";
  const char* fragment_shader =
          "#version 330\n"
          "out vec4 frag_colour;"
          "in vec4 colors;"
          "void main () {"
          "     frag_colour = colors;"
          "}";

};


#endif //OGL_TST_02_SHADER_H
