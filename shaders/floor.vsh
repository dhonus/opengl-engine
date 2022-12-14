#version 330

layout(location=0) in vec4 vp;
layout(location=1) in vec4 color;
out vec4 colors;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
out vec3 worldPosition;

void main () {
    gl_Position = projectionMatrix*viewMatrix*modelMatrix*vp;
    colors = color;
    worldPosition = vec3(modelMatrix * vp);
}
