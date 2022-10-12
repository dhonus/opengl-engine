//
// Created by daniel on 10/8/22.
//

#ifndef OGL_TST_02_TRANS_H
#define OGL_TST_02_TRANS_H

#include <glm/ext/matrix_float4x4.hpp>

//composite

class Trans {
private:
    glm::mat4 current;
public:
    Trans();
    glm::mat4 getMatrix();
    void scale();
    void move();
    void rotate(float deg, float quantifier);
};

class TransRotate : Trans {
private:
    glm::mat4 current;
    float alpha;
public:
    TransRotate();
};

#endif //OGL_TST_02_TRANS_H