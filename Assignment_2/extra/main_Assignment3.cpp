// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Helpers.h"
#include<fstream>
#include<iostream>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#else
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#endif

// OpenGL Mathematics Library
#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

// Timer
#include <chrono>

// VertexBufferObject wrapper
//VertexBufferObject VBO;
std::vector<VertexBufferObject> VBOList;
std::vector<VertexBufferObject> VBONList;
std::vector<VertexBufferObject> VBONList2;
VertexBufferObject VBTest;
VertexBufferObject VBTest2;
// Contains the vertex positions
//Eigen::MatrixXf V(2,3);
std::vector<glm::vec3> V(36);
std::vector<glm::vec3> Vf(6);
std::vector<glm::vec3> VSky(36);
std::vector<glm::vec3> debugV(6);
int start = 0;
int num = 0;
int vertexNum, faceNum, edgeNum;
int vertexNum2, faceNum2, edgeNum2;
std::vector<glm::vec3> points1;
std::vector<glm::vec3> points2;
std::vector<glm::vec3> points(3);
std::vector<glm::vec3> normal1;
std::vector<glm::vec3> normal2;
std::vector<glm::vec3> normal3;
std::vector<glm::vec3> normalV1;
std::vector<glm::vec3> normalV2;
std::vector<glm::vec3> normalV3;
std::vector<glm::vec3> normal00;
std::vector<glm::vec3> normal11;
std::vector<glm::vec3> normal22;
std::vector<glm::vec3> normal33;
Program program;
Program shadowProgram;
Program skyboxProgram;
Program rfProgram;
glm::vec3 cameraPos;
glm::vec3 cameraTarget;
glm::vec3 cameraDirection;
glm::vec3 up; 
glm::vec3 cameraRight;
glm::vec3 cameraUp;
glm::mat4 view(1.0f);
glm::mat4 proj(1.0f);
int pmod = 0;
int tmod = 0;
int smod = 0;
int selected = -1;
int sc = 0;
glm::vec3 bcenter1;
glm::vec3 bcenter2;
glm::vec3 bcenter3;
/*view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
  		   glm::vec3(0.0f, 0.0f, 0.0f), 
  		   glm::vec3(0.0f, 1.0f, 0.0f));*/

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the position of the mouse in the window
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Get the size of the window
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Convert screen position to world coordinates
    double x = ((xpos/double(width))*2)-1;
    double y = (((height-1-ypos)/double(height))*2)-1; // NOTE: y axis is flipped in glfw

    // Update the position of the first vertex if the left button is pressed
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        /*glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);
        glm::vec4 ray_eye = glm::inverse(proj) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 1.0f);
        glm::vec4 ray_world = glm::inverse(view) * ray_eye;
        glm::vec3 ray = glm::vec3(ray_world.x, ray_world.y, ray_world.z);
        ray = glm::normalize(ray);*/
        glm::mat4 toWorld1 = glm::inverse(proj);
        glm::mat4 toWorld2 = glm::inverse(view);
        glm::vec4 from = toWorld1 * glm::vec4(x, y, -1.0f, 1.0f);
        glm::vec4 to = toWorld1 * glm::vec4(x, y, 100.0f, 1.0f);
        from = glm::vec4(from.x/from.w, from.y/from.w, from.z/from.w, 1.0f);
        to = glm::vec4(to.x/to.w, to.y/to.w, to.z/to.w, 1.0f);
        from = toWorld2 * from;
        to = toWorld2 * to;
        //printf("toWorld1:%f",toWorld1[0][0]);
        //printf("toWorld2:%f",toWorld2[0][0]);
        /*points[0] = glm::vec3(from.x/from.w, from.y/from.w, from.z/from.w);
        points[1] = glm::vec3(to.x/from.w, to.y/from.w, to.z/from.w);
        printf("from:%f,%f,%f\n",from.x,from.y,from.z);
        printf("to:%f,%f,%f\n",to.x,to.y,to.z);
        VBTest.update(points);
        glm::vec3 P = glm::vec3(from.x/from.w, from.y/from.w, from.z/from.w);
        glm::vec3 dir = glm::vec3(to.x/to.w-from.x/from.w, to.y/to.w-from.y/from.w, to.z/to.w-from.z/from.w);
        */
        points[0] = glm::vec3(from.x, from.y, from.z);
        points[2] = glm::vec3(to.x, to.y, to.z);
        printf("from:%f,%f,%f\n",from.x,from.y,from.z);
        printf("to:%f,%f,%f\n",to.x,to.y,to.z);
        //printf("Q:%f,%f,%f\n",points[2].x,points[2].y,points[2].z);
        //VBTest.update(points);
        glm::vec3 P = glm::vec3(from.x, from.y, from.z);
        glm::vec3 dir = glm::vec3(to.x-from.x, to.y-from.y, to.z-from.z);
        
        double mint = 1e12;
        int ind = -1;
        glm::vec3 Qmin = glm::vec3(0.0f, 0.0f, 0.0f);
        for(int i=0;i<VBOList.size();i++){
            int type = VBOList[i].type;
            glm::mat4 trans = VBOList[i].trans;
            if(type == 1){
                for(int j=0;j<12;j++){
                    glm::vec3 norm = normal1[j*3];
                    glm::vec4 n4 = glm::vec4(norm.x,norm.y,norm.z,1.0f);
                    n4 = VBOList[i].rotat * n4;
                    norm = glm::vec3(n4.x,n4.y,n4.z);
                    //double d = glm::dot(norm, V[j*3]);
                    if(abs(glm::dot(norm,dir)) < 1e-12){
                        printf("skip\n");
                        continue;
                    }
                    //double t = (d - glm::dot(norm,P))/(glm::dot(norm,dir));
                    //glm::vec3 Q = glm::vec3(P.x+t*dir.x, P.y+t*dir.y, P.z+t*dir.z);
                    glm::vec4 v1 = glm::vec4(V[j*3].x, V[j*3].y, V[j*3].z, 1.0f);
                    glm::vec4 v2 = glm::vec4(V[j*3+1].x, V[j*3+1].y, V[j*3+1].z, 1.0f);
                    glm::vec4 v3 = glm::vec4(V[j*3+2].x, V[j*3+2].y, V[j*3+2].z, 1.0f);
                    v1 = VBOList[i].trans * v1;
                    v2 = VBOList[i].trans * v2;
                    v3 = VBOList[i].trans * v3;
                    glm::vec3 va = glm::vec3(v1.x, v1.y, v1.z);
                    glm::vec3 vb = glm::vec3(v2.x, v2.y, v2.z);
                    glm::vec3 vc = glm::vec3(v3.x, v3.y, v3.z);
                    double d = glm::dot(norm, va);
                    double t = (d - glm::dot(norm,P))/(glm::dot(norm,dir));
                    glm::vec3 Q = glm::vec3(P.x+t*dir.x, P.y+t*dir.y, P.z+t*dir.z);
                    double inTri1 = glm::dot(glm::cross(vb-va,Q-va),norm);
                    double inTri2 = glm::dot(glm::cross(vc-vb, Q-vb),norm);
                    double intri3 = glm::dot(glm::cross(va-vc, Q-vc),norm);
                    if((inTri1>=0 && inTri2>=0 && intri3>=0) || (inTri1<=0 && inTri2<=0 && intri3<=0)){
                        printf("t:%f\n",abs(t));
                        if(abs(t)<mint){
                            mint = abs(t);
                            ind = i;
                            Qmin = Q;
                            //printf("t:%f",abs(t));
                        }
                    }
                }
            }
            else if(type == 2){
                for(int j=0;j<points1.size()/3;j++){
                    glm::vec3 norm = normal2[j*3];
                    glm::vec4 n4 = glm::vec4(norm.x,norm.y,norm.z,1.0f);
                    n4 = VBOList[i].rotat * n4;
                    norm = glm::vec3(n4.x,n4.y,n4.z);
                    //double d = glm::dot(norm, points1[j*3]);
                    if(abs(glm::dot(norm,dir)) < 1e-12){
                        continue;
                    }
                    //double t = (d - glm::dot(norm,P))/(glm::dot(norm,dir));
                    //glm::vec3 Q = glm::vec3(P.x+t*dir.x, P.y+t*dir.y, P.z+t*dir.z);
                    glm::vec4 v1 = glm::vec4(points1[j*3].x, points1[j*3].y, points1[j*3].z, 1.0f);
                    glm::vec4 v2 = glm::vec4(points1[j*3+1].x, points1[j*3+1].y, points1[j*3+1].z, 1.0f);
                    glm::vec4 v3 = glm::vec4(points1[j*3+2].x, points1[j*3+2].y, points1[j*3+2].z, 1.0f);
                    v1 = VBOList[i].trans * v1;
                    v2 = VBOList[i].trans * v2;
                    v3 = VBOList[i].trans * v3;
                    glm::vec3 va = glm::vec3(v1.x, v1.y, v1.z);
                    glm::vec3 vb = glm::vec3(v2.x, v2.y, v2.z);
                    glm::vec3 vc = glm::vec3(v3.x, v3.y, v3.z);
                    double d = glm::dot(norm, va);
                    double t = (d - glm::dot(norm,P))/(glm::dot(norm,dir));
                    glm::vec3 Q = glm::vec3(P.x+t*dir.x, P.y+t*dir.y, P.z+t*dir.z);
                    double inTri1 = glm::dot(glm::cross(vb-va,Q-va),norm);
                    double inTri2 = glm::dot(glm::cross(vc-vb, Q-vb),norm);
                    double intri3 = glm::dot(glm::cross(va-vc, Q-vc),norm);
                    //double inTri1 = glm::dot(glm::cross(points1[j*3+1]-points1[j*3],Q-points1[j*3]),norm);
                    //double inTri2 = glm::dot(glm::cross(points1[j*3+2]-points1[j*3+1], Q-points1[j*3+1]),norm);
                    //double intri3 = glm::dot(glm::cross(points1[j*3]-points1[j*3+2], Q-points1[j*3+2]),norm);
                    if((inTri1>=0 && inTri2>=0 && intri3>=0) || (inTri1<=0 && inTri2<=0 && intri3<=0)){
                        printf("t:%f\n",abs(t));
                        if(abs(t)<mint){
                            mint = abs(t);
                            ind = i;
                            Qmin = Q;
                            //printf("t:%f",abs(t));
                        }
                    }
                }
            }
            else if(type == 3){
                for(int j=0;j<points2.size()/3;j++){
                    glm::vec3 norm =normal3[j*3];
                    glm::vec4 n4 = glm::vec4(norm.x,norm.y,norm.z,1.0f);
                    n4 = VBOList[i].rotat * n4;
                    norm = glm::vec3(n4.x,n4.y,n4.z);
                    //double d = glm::dot(norm, points2[j*3]);
                    if(abs(glm::dot(norm,dir)) < 1e-12){
                        continue;
                    }
                    //double t = (d - glm::dot(norm,P))/(glm::dot(norm,dir));
                    //glm::vec3 Q = glm::vec3(P.x+t*dir.x, P.y+t*dir.y, P.z+t*dir.z);
                    glm::vec4 v1 = glm::vec4(points2[j*3].x, points2[j*3].y, points2[j*3].z, 1.0f);
                    glm::vec4 v2 = glm::vec4(points2[j*3+1].x, points2[j*3+1].y, points2[j*3+1].z, 1.0f);
                    glm::vec4 v3 = glm::vec4(points2[j*3+2].x, points2[j*3+2].y, points2[j*3+2].z, 1.0f);
                    v1 = VBOList[i].trans * v1;
                    v2 = VBOList[i].trans * v2;
                    v3 = VBOList[i].trans * v3;
                    glm::vec3 va = glm::vec3(v1.x, v1.y, v1.z);
                    glm::vec3 vb = glm::vec3(v2.x, v2.y, v2.z);
                    glm::vec3 vc = glm::vec3(v3.x, v3.y, v3.z);
                    double d = glm::dot(norm, va);
                    double t = (d - glm::dot(norm,P))/(glm::dot(norm,dir));
                    glm::vec3 Q = glm::vec3(P.x+t*dir.x, P.y+t*dir.y, P.z+t*dir.z);
                    double inTri1 = glm::dot(glm::cross(vb-va,Q-va),norm);
                    double inTri2 = glm::dot(glm::cross(vc-vb, Q-vb),norm);
                    double intri3 = glm::dot(glm::cross(va-vc, Q-vc),norm);
                    //double inTri1 = glm::dot(glm::cross(points2[j*3+1]-points2[j*3],Q-points2[j*3]),norm);
                    //double inTri2 = glm::dot(glm::cross(points2[j*3+2]-points2[j*3+1], Q-points2[j*3+1]),norm);
                    //double intri3 = glm::dot(glm::cross(points2[j*3]-points2[j*3+2], Q-points2[j*3+2]),norm);
                    if((inTri1>=0 && inTri2>=0 && intri3>=0) || (inTri1<=0 && inTri2<=0 && intri3<=0)){
                        printf("t:%f\n",abs(t));
                        if(abs(t)<mint){
                            mint = abs(t);
                            ind = i;
                            Qmin = Q;
                            //printf("t:%f",abs(t));
                        }
                    }
                }
            }
        }
        if(ind>=0){
            selected = ind;
            points[1] = Qmin;
            //VBTest.update(points);
            printf("t_final:%f\n",mint);
        }
        else{
            printf("no select\n");
            selected = -1;
            points[1] = glm::vec3(0.0f,0.0f,0.0f);
            //VBTest.update(points);
        }
    }
        //V[0] = glm::vec2(xworld, yworld);
        //glm::vec3 P = glm::vec3(cameraPos.x+xworld, cameraPos.y)
    // Upload the change to the GPU
    //VBO.update(V);
    return ;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Update the position of the first vertex if the keys 1,2, or 3 are pressed
    switch (key)
    {
        case  GLFW_KEY_1:
            {
                if(action==GLFW_PRESS){
                    VertexBufferObject VB;
                    VB.init();
                    VB.vnum = V.size();
                    VB.type = 1;
                    VB.center = bcenter1;
                    VB.update(V);
                    VBOList.push_back(VB);
                    //points.insert(points.end(),V.begin(),V.end());
                }
            }
            break;
        case GLFW_KEY_2:
            {
                if(action==GLFW_PRESS){
                    VertexBufferObject VB;
                    VB.init();
                    VB.vnum = points1.size();
                    VB.type = 2;
                    VB.center = bcenter2;
                    VB.update(points1);
                    VBOList.push_back(VB);
                    //points.insert(points.end(),points1.begin(),points1.end());
                }
            }
            break;
        case  GLFW_KEY_3:
            {
                if(action==GLFW_PRESS){
                    VertexBufferObject VB;
                    VB.init();
                    VB.vnum = points2.size();
                    VB.type = 3;
                    VB.center = bcenter3;
                    VB.update(points2);
                    VBOList.push_back(VB);
                    //points.insert(points.end(),points2.begin(),points2.end());
                }
            }
            break;
        case GLFW_KEY_8:
            {
                if(action==GLFW_PRESS){
                    smod = 0;
                }
            }
            break;
        case GLFW_KEY_9:
            {
                if(action==GLFW_PRESS){
                    smod = 1;
                }
            }
            break;
        case GLFW_KEY_0:
            {
                if(action==GLFW_PRESS){
                    smod = 2;
                }
            }
            break;
        case GLFW_KEY_W:
            if(action==GLFW_PRESS){
                //cameraPos = glm::vec3(cameraPos.x,cameraPos.y+0.1f,cameraPos.z);
                cameraPos = glm::vec3(cameraPos.x+cameraUp.x/50.0f,cameraPos.y+cameraUp.y/50.0f,cameraPos.z+cameraUp.z/50.0f);
                cameraDirection = glm::normalize(cameraPos - cameraTarget);
                up = glm::vec3(0.0f, 1.0f, 0.0f); 
                cameraRight = glm::normalize(glm::cross(up, cameraDirection));
                cameraUp = glm::cross(cameraDirection, cameraRight);
                //cameraPos = glm::vec3(cameraPos.x,cameraPos.y+0.1f,cameraPos.z);
            }
            break;
        case GLFW_KEY_S:
            if(action==GLFW_PRESS){
                //cameraPos = glm::vec3(cameraPos.x,cameraPos.y-0.1f,cameraPos.z);
                cameraPos = glm::vec3(cameraPos.x-cameraUp.x/50.0f,cameraPos.y-cameraUp.y/50.0f,cameraPos.z-cameraUp.z/50.0f);
                cameraDirection = glm::normalize(cameraPos - cameraTarget);
                up = glm::vec3(0.0f, 1.0f, 0.0f); 
                cameraRight = glm::normalize(glm::cross(up, cameraDirection));
                cameraUp = glm::cross(cameraDirection, cameraRight);
                //cameraPos = glm::vec3(cameraPos.x,cameraPos.y-0.1f,cameraPos.z);
            }
            break;
        case GLFW_KEY_A:
            if(action==GLFW_PRESS){
                //cameraPos = glm::vec3(cameraPos.x-0.1f,cameraPos.y,cameraPos.z);
                cameraPos = glm::vec3(cameraPos.x-cameraRight.x/50.0f,cameraPos.y-cameraRight.y/50.0f,cameraPos.z-cameraRight.z/50.0f);
                cameraDirection = glm::normalize(cameraPos - cameraTarget);
                up = glm::vec3(0.0f, 1.0f, 0.0f); 
                cameraRight = glm::normalize(glm::cross(up, cameraDirection));
                cameraUp = glm::cross(cameraDirection, cameraRight);
                //cameraPos = glm::vec3(cameraPos.x-0.1f,cameraPos.y,cameraPos.z);
            }
            break;
        case GLFW_KEY_D:
            if(action==GLFW_PRESS){
                //cameraPos = glm::vec3(cameraPos.x+0.1f,cameraPos.y,cameraPos.z);
                cameraPos = glm::vec3(cameraPos.x+cameraRight.x/50.0f,cameraPos.y+cameraRight.y/50.0f,cameraPos.z+cameraRight.z/50.0f);
                cameraDirection = glm::normalize(cameraPos - cameraTarget);
                up = glm::vec3(0.0f, 1.0f, 0.0f); 
                cameraRight = glm::normalize(glm::cross(up, cameraDirection));
                cameraUp = glm::cross(cameraDirection, cameraRight);
                //cameraPos = glm::vec3(cameraPos.x-0.1f,cameraPos.y,cameraPos.z);
            }
            break;
        case GLFW_KEY_Q:
            if(action==GLFW_PRESS){
                //cameraPos = glm::vec3(cameraPos.x,cameraPos.y,cameraPos.z+0.1f);
                cameraPos = glm::vec3(cameraPos.x+cameraDirection.x/50.0f,cameraPos.y+cameraDirection.y/50.0f,cameraPos.z+cameraDirection.z/50.0f);
                cameraDirection = glm::normalize(cameraPos - cameraTarget);
                up = glm::vec3(0.0f, 1.0f, 0.0f); 
                cameraRight = glm::normalize(glm::cross(up, cameraDirection));
                cameraUp = glm::cross(cameraDirection, cameraRight);
            }
            break;
        case GLFW_KEY_E:
            if(action==GLFW_PRESS){
                //cameraPos = glm::vec3(cameraPos.x,cameraPos.y,cameraPos.z-0.1f);
                cameraPos = glm::vec3(cameraPos.x-cameraDirection.x/50.0f,cameraPos.y-cameraDirection.y/50.0f,cameraPos.z-cameraDirection.z/50.0f);
                cameraDirection = glm::normalize(cameraPos - cameraTarget);
                up = glm::vec3(0.0f, 1.0f, 0.0f); 
                cameraRight = glm::normalize(glm::cross(up, cameraDirection));
                cameraUp = glm::cross(cameraDirection, cameraRight);
            }
            break;
        case GLFW_KEY_P:
            if(action==GLFW_PRESS){
                pmod = 0;
            }
            break;
        case GLFW_KEY_O:
            if(action==GLFW_PRESS){
                pmod = 1;
            }
            break;
        case GLFW_KEY_B:
            if(action==GLFW_PRESS){
                tmod = 0;
            }
            break;
        case GLFW_KEY_N:
            if(action==GLFW_PRESS){
                tmod = 1;
            }
            break;
        case GLFW_KEY_M:
            if(action==GLFW_PRESS){
                tmod = 2;
            }
            break;
        case GLFW_KEY_U:
            if(action==GLFW_PRESS){
                if(tmod==0){
                    printf("press u\n");
                    glm::mat4 transl = glm::mat4(1.0f);
                    transl = glm::translate(transl, glm::vec3(cameraUp.x/10.0f, cameraUp.y/10.0f, cameraUp.z/10.0f));
                    if(selected>=0){
                        printf("trans changed\n");
                        VBOList[selected].trans = transl * VBOList[selected].trans;
                        for(int i=0;i<4;i++){
                            for(int j=0;j<4;j++){
                                printf("%f ",VBOList[selected].trans[i][j]);
                            }
                            printf("\n");
                        }
                    }
                    glm::vec4 t1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                    t1 = transl * t1;
                    printf("t1: %f, %f, %f\n",t1.x,t1.y,t1.z);
                }
                else if(tmod==2){
                    glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                    c4 = VBOList[selected].trans * c4;
                    glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                    glm::mat4 toO = glm::mat4(1.0f);
                    toO = glm::translate(toO,dir);
                    glm::mat4 rot = glm::mat4(1.0f);
                    rot = glm::rotate(rot, glm::radians(10.0f),cameraRight);
                    glm::mat4 fromO = glm::mat4(1.0f);
                    fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                    VBOList[selected].trans = fromO * rot * toO * VBOList[selected].trans;
                    VBOList[selected].rotat = rot * VBOList[selected].rotat;
                }
            }
            break;
        case GLFW_KEY_J:
            if(action==GLFW_PRESS){
                if(tmod==0){
                    glm::mat4 transl = glm::mat4(1.0f);
                    transl = glm::translate(transl, glm::vec3(-cameraUp.x/10.0f, -cameraUp.y/10.0f, -cameraUp.z/10.0f));
                    if(selected>=0){
                        VBOList[selected].trans = transl * VBOList[selected].trans;
                    }
                }
                else if(tmod==2){
                    glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                    c4 = VBOList[selected].trans * c4;
                    glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                    glm::mat4 toO = glm::mat4(1.0f);
                    toO = glm::translate(toO,dir);
                    glm::mat4 rot = glm::mat4(1.0f);
                    rot = glm::rotate(rot, glm::radians(-10.0f),cameraRight);
                    glm::mat4 fromO = glm::mat4(1.0f);
                    fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                    VBOList[selected].trans = fromO * rot * toO * VBOList[selected].trans;
                    VBOList[selected].rotat = rot * VBOList[selected].rotat;
                }
            }
            break;
        case GLFW_KEY_K:
            if(action==GLFW_PRESS){
                if(tmod==0){
                    glm::mat4 transl = glm::mat4(1.0f);
                    transl = glm::translate(transl, glm::vec3(cameraRight.x/10.0f, cameraRight.y/10.0f, cameraRight.z/10.0f));
                    if(selected>=0){
                        VBOList[selected].trans = transl * VBOList[selected].trans;
                    }
                }
                else if(tmod==2){
                    glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                    c4 = VBOList[selected].trans * c4;
                    glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                    glm::mat4 toO = glm::mat4(1.0f);
                    toO = glm::translate(toO,dir);
                    glm::mat4 rot = glm::mat4(1.0f);
                    rot = glm::rotate(rot, glm::radians(10.0f),cameraUp);
                    glm::mat4 fromO = glm::mat4(1.0f);
                    fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                    VBOList[selected].trans = fromO * rot * toO * VBOList[selected].trans;
                    VBOList[selected].rotat = rot * VBOList[selected].rotat;
                }
            }
            break;
        case GLFW_KEY_H:
            if(action==GLFW_PRESS){
                if(tmod==0){
                    glm::mat4 transl = glm::mat4(1.0f);
                    transl = glm::translate(transl, glm::vec3(-cameraRight.x/10.0f, -cameraRight.y/10.0f, -cameraRight.z/10.0f));
                    if(selected>=0){
                        VBOList[selected].trans = transl * VBOList[selected].trans;
                    }
                }
                else if(tmod==2){
                    glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                    c4 = VBOList[selected].trans * c4;
                    glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                    glm::mat4 toO = glm::mat4(1.0f);
                    toO = glm::translate(toO,dir);
                    glm::mat4 rot = glm::mat4(1.0f);
                    rot = glm::rotate(rot, glm::radians(-10.0f),cameraUp);
                    glm::mat4 fromO = glm::mat4(1.0f);
                    fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                    VBOList[selected].trans = fromO * rot * toO * VBOList[selected].trans;
                    VBOList[selected].rotat = rot * VBOList[selected].rotat;
                }
            }
            break;
        case GLFW_KEY_I:
            if(action==GLFW_PRESS){
                if(tmod==0){
                    glm::mat4 transl = glm::mat4(1.0f);
                    transl = glm::translate(transl, glm::vec3(cameraDirection.x/10.0f, cameraDirection.y/10.0f, cameraDirection.z/10.0f));
                    if(selected>=0){
                        VBOList[selected].trans = transl * VBOList[selected].trans;
                    }
                }
                else if(tmod==1){
                    if(selected>=0){
                        glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                        c4 = VBOList[selected].trans * c4;
                        glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                        glm::mat4 toO = glm::mat4(1.0f);
                        toO = glm::translate(toO,dir);
                        glm::mat4 sca = glm::mat4(1.0f);
                        sca = glm::scale(sca, glm::vec3(1.1f,1.1f,1.1f));
                        glm::mat4 fromO = glm::mat4(1.0f);
                        fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                        VBOList[selected].trans = fromO * sca * toO * VBOList[selected].trans;
                    }
                }
                else if(tmod==2){
                    glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                    c4 = VBOList[selected].trans * c4;
                    glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                    glm::mat4 toO = glm::mat4(1.0f);
                    toO = glm::translate(toO,dir);
                    glm::mat4 rot = glm::mat4(1.0f);
                    rot = glm::rotate(rot, glm::radians(10.0f),cameraDirection);
                    glm::mat4 fromO = glm::mat4(1.0f);
                    fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                    VBOList[selected].trans = fromO * rot * toO * VBOList[selected].trans;
                    VBOList[selected].rotat = rot * VBOList[selected].rotat;
                }
            }
            break;
        case GLFW_KEY_Y:
            if(action==GLFW_PRESS){
                if(tmod==0){
                    glm::mat4 transl = glm::mat4(1.0f);
                    transl = glm::translate(transl, glm::vec3(-cameraDirection.x/10.0f, -cameraDirection.y/10.0f, -cameraDirection.z/10.0f));
                    if(selected>=0){
                        VBOList[selected].trans = transl * VBOList[selected].trans;
                    }
                }
                else if(tmod==1){
                    if(selected>=0){
                        glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                        c4 = VBOList[selected].trans * c4;
                        glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                        glm::mat4 toO = glm::mat4(1.0f);
                        toO = glm::translate(toO,dir);
                        glm::mat4 sca = glm::mat4(1.0f);
                        sca = glm::scale(sca, glm::vec3(0.9f,0.9f,0.9f));
                        glm::mat4 fromO = glm::mat4(1.0f);
                        fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                        VBOList[selected].trans = fromO * sca * toO * VBOList[selected].trans;
                    }
                }
                else if(tmod==2){
                    glm::vec4 c4 = glm::vec4(VBOList[selected].center.x,VBOList[selected].center.y,VBOList[selected].center.z,1.0f);
                    c4 = VBOList[selected].trans * c4;
                    glm::vec3 dir = glm::vec3(-c4.x,-c4.y,-c4.z);
                    glm::mat4 toO = glm::mat4(1.0f);
                    toO = glm::translate(toO,dir);
                    glm::mat4 rot = glm::mat4(1.0f);
                    rot = glm::rotate(rot, glm::radians(-10.0f),cameraDirection);
                    glm::mat4 fromO = glm::mat4(1.0f);
                    fromO = glm::translate(fromO,glm::vec3(-dir.x,-dir.y,-dir.z));
                    VBOList[selected].trans = fromO * rot * toO * VBOList[selected].trans;
                    VBOList[selected].rotat = rot * VBOList[selected].rotat;
                }
            }
            break;
        case GLFW_KEY_EQUAL:
            if(action==GLFW_PRESS){
                sc = 1 - sc;
            }
            break;
        case GLFW_KEY_MINUS:
            if(action==GLFW_PRESS){
                if(selected>0){
                    VBOList[selected].rf = 1 - VBOList[selected].rf;
                }
            }
            break;
        default:
            break;
    }
    
    // Upload the change to the GPU
    //VBO.update(points);
}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Activate supersampling
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Ensure that we get at least a 3.2 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    #ifndef __APPLE__
      glewExperimental = true;
      GLenum err = glewInit();
      if(GLEW_OK != err)
      {
        /* Problem: glewInit failed, something is seriously wrong. */
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      }
      glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
      fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Initialize the VAO
    // A Vertex Array Object (or VAO) is an object that describes how the vertex
    // attributes are stored in a Vertex Buffer Object (or VBO). This means that
    // the VAO is not the actual object storing the vertex data,
    // but the descriptor of the vertex data.
    VertexArrayObject VAO;
    VAO.init();
    VAO.bind();
    

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory
    //VBO.init();

    /*float vertices[] = 
    {
        // Vertices for the left triangle
        -0.8f, +0.8f,  // Repeated Vertex
        +0.8f, -0.8f,  // Repeated Vertex
        -0.8f, -0.8f,

        // Vertices for the right triangle
        -0.8f, +0.8f,  // Repeated Vertex
        +0.8f, +0.8f,
        +0.8f, -0.8f   // Repeated Vertex
    };*/

    GLuint elements[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };
    /*GLuint ebo;
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ARRAY_BUFFER, ebo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

    /*GLuint ele;
    glGenBuffers(1,&ele);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ele);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    */
    //std::vector<glm::vec3> points1;
    printf("Pos 1\n");
    std::vector<glm::vec3> colors;
    std::vector<int> indexList;
    std::string line;
    std::ifstream fin("../data/bunny.off");
    if (!fin.is_open())
    {
    
    
        std::cout << "Open file failed" << std::endl;
        exit(-1);
    }

    std::vector<glm::vec3> vertexPosition, vertexColor; // off模型顶点位置和颜色
    //std::vector<glm::vec3> normal1;
    fin >> line;    // OFF字符串
    // 读取顶点位置
    //int vertexNum, faceNum, edgeNum;
    fin >> vertexNum >> faceNum >> edgeNum;
    double maxX = -1;
    double minX = 1;
    double maxY = -1;
    double minY = 1;
    double maxZ = -1;
    double minZ = 1;
    for (int i = 0; i < vertexNum; i++)
    {
    
    
        float p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        if(p1>maxX){
            maxX = p1;
        }
        if(p1<minX){
            minX = p1;
        }
        if(p2>maxY){
            maxY = p2;
        }
        if(p2<minY){
            minY = p2;
        }
        if(p3>maxZ){
            maxZ = p3;
        }
        if(p3<minZ){
            minZ = p3;
        }
        vertexPosition.push_back(glm::vec3(p1, p2, p3));
        vertexColor.push_back(glm::vec3(p1 + 0.5, p2 + 0.5, p3 + 0.5)); // 用位置做颜色
    }
    double scaleX = 0.1/(maxX-minX);
    double transX = 0-(maxX+minX)/2;
    double scaleY = 0.1/(maxY-minY);
    double transY = 0-(maxY+minY)/2;
    double scaleZ = 0.1/(maxZ-minZ);
    double transZ = 0-(maxZ+minZ)/2;

    for(int i=0;i<vertexPosition.size();i++){
        vertexPosition[i].x = (vertexPosition[i].x + transX)*scaleX;
        vertexPosition[i].y = (vertexPosition[i].y + transY)*scaleY;
        vertexPosition[i].z = (vertexPosition[i].z + transZ)*scaleZ;
    }

    /*glm::vec3 dir(transX,transY,transZ);
    glm::mat4 trans1 = glm::mat4(1.0f);
    trans1 = glm::translate(trans1,dir);
    glm::mat4 scale1 = glm::mat4(1.0f);
    scale1 = glm::scale(scale1, glm::vec3(scaleX, scaleY, scaleZ));
    glm::mat4 conv = scale1*trans1;*/
    // 根据面信息生成实际顶点
    points1.clear();
    colors.clear();
    bcenter2 = glm::vec3(0.0f,0.0f,0.0f);
    for(int i=0;i<vertexPosition.size();i++){
        bcenter2.x += vertexPosition[i].x;
        bcenter2.y += vertexPosition[i].y;
        bcenter2.z += vertexPosition[i].z;
    }
    bcenter2.x /= vertexPosition.size();
    bcenter2.y /= vertexPosition.size();
    bcenter2.z /= vertexPosition.size();

    for(int i=0;i<vertexNum;i++){
        normalV2.push_back(glm::vec3(0.0f,0.0f,0.0f));
    }

    for (int i = 0; i < faceNum; i++)
    {
    
    
        int n, index1, index2, index3;
        fin >> n >> index1 >> index2 >> index3;
        //vertexPosition[index1] = conv * glm::vec4(vertexPosition[index1],1.0f);
        points1.push_back(vertexPosition[index1]);
        points1.push_back(vertexPosition[index2]);
        points1.push_back(vertexPosition[index3]);
        colors.push_back(vertexColor[index1]);
        colors.push_back(vertexColor[index2]);
        colors.push_back(vertexColor[index3]);
        glm::vec3 nm = glm::normalize(glm::cross((vertexPosition[index2]-vertexPosition[index1]),(vertexPosition[index3]-vertexPosition[index1])));
        normal2.push_back(nm);
        normal2.push_back(nm);
        normal2.push_back(nm);
        normalV2[index1] = nm + normalV2[index1];
        normalV2[index2] = nm + normalV2[index2];
        normalV2[index3] = nm + normalV2[index3];
        indexList.push_back(index1);
        indexList.push_back(index2);
        indexList.push_back(index3);
    }
    for(int i=0;i<vertexNum;i++){
        normalV2[i] = glm::normalize(normalV2[i]);
    }
    for(int i=0;i<faceNum*3;i++){
        normal22.push_back(normalV2[indexList[i]]);
    }
    printf("Pos 2\n");
    std::string line2;
    std::ifstream fin2("../data/bumpy_cube.off");
    if (!fin2.is_open())
    {
    
    
        std::cout << "Open file failed" << std::endl;
        exit(-1);
    }

    std::vector<glm::vec3> vertexPosition2, vertexColor2; // off模型顶点位置和颜色
    fin2 >> line2;    // OFF字符串
    // 读取顶点位置
    //int vertexNum2, faceNum2, edgeNum2;
    fin2 >> vertexNum2 >> faceNum2 >> edgeNum2;
    maxX = -1;
    minX = 1;
    maxY = -1;
    minY = 1;
    maxZ = -1;
    minZ = 1;
    for (int i = 0; i < vertexNum2; i++)
    {
    
    
        float p1, p2, p3;
        fin2 >> p1 >> p2 >> p3;
        if(p1>maxX){
            maxX = p1;
        }
        if(p1<minX){
            minX = p1;
        }
        if(p2>maxY){
            maxY = p2;
        }
        if(p2<minY){
            minY = p2;
        }
        if(p3>maxZ){
            maxZ = p3;
        }
        if(p3<minZ){
            minZ = p3;
        }
        vertexPosition2.push_back(glm::vec3(p1, p2, p3));
        vertexColor2.push_back(glm::vec3(p1 + 0.5, p2 + 0.5, p3 + 0.5)); // 用位置做颜色
    }
    scaleX = 0.1/(maxX-minX);
    transX = 0-(maxX+minX)/2;
    scaleY = 0.1/(maxY-minY);
    transY = 0-(maxY+minY)/2;
    scaleZ = 0.1/(maxZ-minZ);
    transZ = 0-(maxZ+minZ)/2;

    for(int i=0;i<vertexPosition2.size();i++){
        vertexPosition2[i].x = (vertexPosition2[i].x + transX)*scaleX;
        vertexPosition2[i].y = (vertexPosition2[i].y + transY)*scaleY;
        vertexPosition2[i].z = (vertexPosition2[i].z + transZ)*scaleZ;
    }

    /*glm::vec3 dir(transX,transY,transZ);
    glm::mat4 trans1 = glm::mat4(1.0f);
    trans1 = glm::translate(trans1,dir);
    glm::mat4 scale1 = glm::mat4(1.0f);
    scale1 = glm::scale(scale1, glm::vec3(scaleX, scaleY, scaleZ));
    glm::mat4 conv = scale1*trans1;*/
    // 根据面信息生成实际顶点
    //points.clear();
    //colors.clear();
    bcenter3 = glm::vec3(0.0f,0.0f,0.0f);
    for(int i=0;i<vertexPosition2.size();i++){
        bcenter3.x += vertexPosition2[i].x;
        bcenter3.y += vertexPosition2[i].y;
        bcenter3.z += vertexPosition2[i].z;
    }
    bcenter3.x /= vertexPosition2.size();
    bcenter3.y /= vertexPosition2.size();
    bcenter3.z /= vertexPosition2.size();

    for(int i=0;i<vertexNum2;i++){
        normalV3.push_back(glm::vec3(0.0f,0.0f,0.0f));
    }
    indexList.clear();
    for (int i = 0; i < faceNum2; i++)
    {
    
    
        int n, index1, index2, index3;
        fin2 >> n >> index1 >> index2 >> index3;
        //vertexPosition[index1] = conv * glm::vec4(vertexPosition[index1],1.0f);
        points2.push_back(vertexPosition2[index1]);
        points2.push_back(vertexPosition2[index2]);
        points2.push_back(vertexPosition2[index3]);
        colors.push_back(vertexColor2[index1]);
        colors.push_back(vertexColor2[index2]);
        colors.push_back(vertexColor2[index3]);
        glm::vec3 nm = glm::normalize(glm::cross((vertexPosition2[index2]-vertexPosition2[index1]),(vertexPosition2[index3]-vertexPosition2[index1])));
        normal3.push_back(nm);
        normal3.push_back(nm);
        normal3.push_back(nm);
        normalV3[index1] = nm + normalV3[index1];
        normalV3[index2] = nm + normalV3[index2];
        normalV3[index3] = nm + normalV3[index3];
        indexList.push_back(index1);
        indexList.push_back(index2);
        indexList.push_back(index3);
    }
    for(int i=0;i<vertexNum2;i++){
        normalV3[i] = glm::normalize(normalV3[i]);
    }
    for(int i=0;i<faceNum2*3;i++){
        normal33.push_back(normalV3[indexList[i]]);
    }
    printf("Pos 3\n");
    V.resize(36);
    V[0] = glm::vec3(-0.05, -0.05, -0.05);
    V[1] = glm::vec3(-0.05, -0.05, 0.05);
    V[2] = glm::vec3(-0.05, 0.05, 0.05);

    V[3] = glm::vec3(0.05,  0.05, -0.05);
    V[4] = glm::vec3(-0.05, -0.05, -0.05);
    V[5] = glm::vec3(-0.05, 0.05, -0.05);

    V[6] = glm::vec3(0.05, -0.05, 0.05);
    V[7] = glm::vec3(-0.05, -0.05, -0.05);
    V[8] = glm::vec3(0.05, -0.05, -0.05);

    V[9] = glm::vec3(0.05,  0.05, -0.05);
    V[10] = glm::vec3(0.05, -0.05, -0.05);
    V[11] = glm::vec3(-0.05, -0.05, -0.05);

    V[12] = glm::vec3(-0.05, -0.05, -0.05);
    V[13] = glm::vec3(-0.05, 0.05, 0.05);
    V[14] = glm::vec3(-0.05, 0.05, -0.05);

    V[15] = glm::vec3(0.05, -0.05, 0.05);
    V[16] = glm::vec3(-0.05, -0.05, 0.05);
    V[17] = glm::vec3(-0.05, -0.05, -0.05);

    V[18] = glm::vec3(-0.05,  0.05, 0.05);
    V[19] = glm::vec3(-0.05, -0.05, 0.05);
    V[20] = glm::vec3(0.05, -0.05, 0.05);

    V[21] = glm::vec3(0.05, 0.05, 0.05);
    V[22] = glm::vec3(0.05, -0.05, -0.05);
    V[23] = glm::vec3(0.05, 0.05, -0.05);

    V[24] = glm::vec3(0.05, -0.05, -0.05);
    V[25] = glm::vec3(0.05, 0.05, 0.05);
    V[26] = glm::vec3(0.05, -0.05, 0.05);

    V[27] = glm::vec3(0.05, 0.05, 0.05);
    V[28] = glm::vec3(0.05, 0.05, -0.05);
    V[29] = glm::vec3(-0.05, 0.05, -0.05);

    V[30] = glm::vec3(0.05, 0.05, 0.05);
    V[31] = glm::vec3(-0.05, 0.05, -0.05);
    V[32] = glm::vec3(-0.05, 0.05, 0.05);

    V[33] = glm::vec3(0.05, 0.05, 0.05);
    V[34] = glm::vec3(-0.05, 0.05, 0.05);
    V[35] = glm::vec3(0.05, -0.05, 0.05);

    bcenter1 = glm::vec3(0.0f,0.0f,0.0f);
    indexList.clear();
    for(int i=0;i<8;i++){
        normalV1.push_back(glm::vec3(0.0f,0.0f,0.0f));
    }
    printf("Pos 4\n");
    for(int i=0;i<V.size();i+=3){
        glm::vec3 nm = glm::normalize(glm::cross((V[i+1]-V[i]),(V[i+2]-V[i])));
        normal1.push_back(nm);
        normal1.push_back(nm);
        normal1.push_back(nm);
        //normalV1[i] = nm + normalV1[i];
        //normalV1[i+1] = nm + normalV1[i+1];
        //normalV3[i+2] = nm + normalV3[i+2];
        int index[3]={0,0,0};
        int nb[3][3]={0,0,0,0,0,0,0,0,0};
        for(int j=0;j<9;j++){
            if(V[i+j/3][j%3]<0){
                nb[j/3][j%3] = 0;
            }
            else{
                nb[j/3][j%3] = 1;
            }
        }
        for(int j=0;j<3;j++){
            index[j] = nb[j][0]*4 + nb[j][1]*2 + nb[j][2];
        }
        printf("Pos 5\n");
        printf("index: %d, %d, %d\n",index[0],index[1],index[2]);
        printf("normalV1: %d\n",normalV1.size());
        normalV1[index[0]] = nm + normalV1[index[0]];
        printf("Pos 6\n");
        normalV1[index[1]] = nm + normalV1[index[1]];
        printf("Pos 7\n");
        normalV1[index[2]] = nm + normalV1[index[2]];
        printf("Pos 8\n");
        indexList.push_back(index[0]);
        indexList.push_back(index[1]);
        indexList.push_back(index[2]);
        //printf("Pos 7\n");
    }
    //printf("Pos 4\n");
    for(int i=0;i<8;i++){
        normalV1[i] = glm::normalize(normalV1[i]);
    }
    for(int i=0;i<36;i++){
        normal11.push_back(normalV1[indexList[i]]);
    }

    Vf.resize(6);
    Vf[0] = glm::vec3(0.3, 0.3, -0.1);
    Vf[1] = glm::vec3(0.3, -0.3, -0.1);
    Vf[2] = glm::vec3(-0.3, 0.3, -0.1);
    Vf[3] = glm::vec3(-0.3, 0.3, -0.1);
    Vf[4] = glm::vec3(0.3, -0.3, -0.1);
    Vf[5] = glm::vec3(-0.3, -0.3, -0.1);
    
    /*Vf[0] = glm::vec3(0.0, 0.0, -0.1);
    Vf[1] = glm::vec3(0.0, 0.0, -0.1);
    Vf[2] = glm::vec3(0.0, 0.0, -0.1);
    Vf[3] = glm::vec3(0.0, 0.0, -0.1);
    Vf[4] = glm::vec3(0.0, 0.0, -0.1);
    Vf[5] = glm::vec3(0.0, 0.0, -0.1);
    */
    /*Vf[0] = glm::vec3(0.1, 0.1, 0.1);
    Vf[1] = glm::vec3(0.1, -0.1, 0.1);
    Vf[2] = glm::vec3(-0.1, 0.1, 0.1);
    Vf[3] = glm::vec3(-0.1, 0.1, 0.1);
    Vf[4] = glm::vec3(0.1, -0.1, 0.1);
    Vf[5] = glm::vec3(-0.1, -0.1, 0.1);
    */
    for(int i=0;i<6;i++){
        normal00.push_back(glm::vec3(0.0 ,0.0, 1.0));
    }
    VertexBufferObject VB;
    VB.init();
    VB.vnum = Vf.size();
    VB.type = 0;
    VB.center = glm::vec3(0.0, 0.0, -0.1);
    VB.update(Vf);
    VBOList.push_back(VB);

    debugV.resize(6);
    debugV[0] = glm::vec3(0.1, 0.1, 0.0);
    debugV[1] = glm::vec3(0.1, -0.1, 0.0);
    debugV[2] = glm::vec3(-0.1, 0.1, 0.0);
    debugV[3] = glm::vec3(-0.1, 0.1, 0.0);
    debugV[4] = glm::vec3(0.1, -0.1, 0.0);
    debugV[5] = glm::vec3(-0.1, -0.1, 0.0);

    VertexBufferObject VBDebug;
    VBDebug.init();
    VBDebug.vnum = Vf.size();
    VBDebug.type = 0;
    VBDebug.center = glm::vec3(0.0, 0.0, 0.0);
    VBDebug.update(debugV);
    //printf("Pos 4\n");
    //std::vector<glm::vec3> points1;

    VSky.resize(36);
    VSky[0] = glm::vec3(-1.0f,  1.0f, -1.0f);
    VSky[1] = glm::vec3(-1.0f, -1.0f, -1.0f);
    VSky[2] = glm::vec3(1.0f, -1.0f, -1.0f);
    VSky[3] = glm::vec3(1.0f, -1.0f, -1.0f);
    VSky[4] = glm::vec3(1.0f,  1.0f, -1.0f);
    VSky[5] = glm::vec3(-1.0f,  1.0f, -1.0f);

    VSky[6] = glm::vec3(-1.0f, -1.0f,  1.0f);
    VSky[7] = glm::vec3(-1.0f, -1.0f, -1.0f);
    VSky[8] = glm::vec3(-1.0f,  1.0f, -1.0f);
    VSky[9] = glm::vec3(-1.0f,  1.0f, -1.0f);
    VSky[10] = glm::vec3(-1.0f,  1.0f,  1.0f);
    VSky[11] = glm::vec3(-1.0f, -1.0f,  1.0f);

    VSky[12] = glm::vec3(1.0f, -1.0f, -1.0f);
    VSky[13] = glm::vec3(1.0f, -1.0f,  1.0f);
    VSky[14] = glm::vec3(1.0f,  1.0f,  1.0f);
    VSky[15] = glm::vec3(1.0f,  1.0f,  1.0f);
    VSky[16] = glm::vec3(1.0f,  1.0f, -1.0f);
    VSky[17] = glm::vec3(1.0f, -1.0f, -1.0f);

    VSky[18] = glm::vec3(-1.0f, -1.0f,  1.0f);
    VSky[19] = glm::vec3(-1.0f,  1.0f,  1.0f);
    VSky[20] = glm::vec3(1.0f,  1.0f,  1.0f);
    VSky[21] = glm::vec3(1.0f,  1.0f,  1.0f);
    VSky[22] = glm::vec3(1.0f, -1.0f,  1.0f);
    VSky[23] = glm::vec3(-1.0f, -1.0f,  1.0f);

    VSky[24] = glm::vec3(-1.0f,  1.0f, -1.0f);
    VSky[25] = glm::vec3(1.0f,  1.0f, -1.0f);
    VSky[26] = glm::vec3(1.0f,  1.0f,  1.0f);
    VSky[27] = glm::vec3(1.0f,  1.0f,  1.0f);
    VSky[28] = glm::vec3(-1.0f,  1.0f,  1.0f);
    VSky[29] = glm::vec3(-1.0f,  1.0f, -1.0f);

    VSky[30] = glm::vec3(-1.0f, -1.0f, -1.0f);
    VSky[31] = glm::vec3(-1.0f, -1.0f,  1.0f);
    VSky[32] = glm::vec3(1.0f, -1.0f, -1.0f);
    VSky[33] = glm::vec3(1.0f, -1.0f, -1.0f);
    VSky[34] = glm::vec3(-1.0f, -1.0f,  1.0f);
    VSky[35] = glm::vec3(1.0f, -1.0f,  1.0f);

    VertexBufferObject VBSky;
    VBSky.init();
    VBSky.vnum = VSky.size();
    VBSky.type = -1;
    VBSky.center = glm::vec3(0.0, 0.0, 0.0);
    VBSky.update(VSky);

    std::vector<std::string> faces(6);
    faces[0] = "../data/night_posx.png";
    faces[1] = "../data/night_negx.png";
    faces[2] = "../data/night_posy.png";
    faces[3] = "../data/night_negy.png";
    faces[4] = "../data/night_posz.png";
    faces[5] = "../data/night_negz.png";

    unsigned int cubemapTexture = loadCubemap(faces);

    //glm::mat4 view;
    cameraPos = glm::vec3(0.0f, 0.0f, 0.2f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    up = glm::vec3(0.0f, 1.0f, 0.0f); 
    cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);
    //view = glm::lookAt(cameraPos, cameraTarget, up);

    //points.insert(points.begin(), V.begin(), V.end());
    points.push_back(glm::vec3(0, 0, 0));
    points.push_back(glm::vec3(0, 0, 0));
    points.push_back(glm::vec3(0, 0, 0));
    //VertexBufferObject VBTest;
    //VBTest.init();
    //VBTest2.init();
    VertexBufferObject VBN0;
    VertexBufferObject VBN1;
    VertexBufferObject VBN2;
    VertexBufferObject VBN3;
    VBN0.init();
    VBN1.init();
    VBN2.init();
    VBN3.init();
    printf("normal num:%d,%d,%d\n",normal11.size(),normal22.size(),normal33.size());
    VBN0.update(normal00);
    VBN1.update(normal11);
    VBN2.update(normal22);
    VBN3.update(normal33);
    VBONList.push_back(VBN0);
    VBONList.push_back(VBN1);
    VBONList.push_back(VBN2);
    VBONList.push_back(VBN3);

    VertexBufferObject VBNs0;
    VertexBufferObject VBNs1;
    VertexBufferObject VBNs2;
    VertexBufferObject VBNs3;
    VBNs0.init();
    VBNs1.init();
    VBNs2.init();
    VBNs3.init();
    printf("normal num:%d,%d,%d\n",normal1.size(),normal2.size(),normal3.size());
    VBNs0.update(normal00);
    VBNs1.update(normal1);
    VBNs2.update(normal2);
    VBNs3.update(normal3);
    VBONList2.push_back(VBNs0);
    VBONList2.push_back(VBNs1);
    VBONList2.push_back(VBNs2);
    VBONList2.push_back(VBNs3);

    //VBTest.vnum = V.size();
    //VBTest.type = 1;
    //VBTest.update(points);
    //VBOList.push_back(VB);
    //VBO.update(points);

    // Initialize the OpenGL Program
    // A program controls the OpenGL pipeline and it must contains
    // at least a vertex shader and a fragment shader to be valid
    //Program program;

    const GLchar* shadowmap_vert =
            "#version 150 core\n"
                    "in vec3 position;"
                    "uniform mat4 LSM;"
                    "uniform mat4 model;"
                    "void main()"
                    "{"
                    "    gl_Position = LSM * model * vec4(position, 1.0);"
                    "}";
    
    const GLchar* shadowmap_frag =
            "#version 150 core\n"
                    "void main()"
                    "{"
                    "   gl_FragDepth = gl_FragCoord.z;"
                    "}";
    
    const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec3 position;"
                    "in vec3 norm;"
                    "out vec3 fragpos;"
                    "out vec3 normal;"
                    "out vec4 fragPosLightSpace;"
                    "uniform mat4 rotat;"
                    "uniform mat4 transform;"
                    "uniform mat4 view;"
                    "uniform mat4 proj;"
                    "uniform mat4 LSM;"
                    "void main()"
                    "{"
                    "    fragpos = vec3(transform * vec4(position,1.0));"
                    "    normal = vec3(rotat * vec4(norm,1.0));"
                    "    fragPosLightSpace = LSM * vec4(fragpos, 1.0);"
                    "    gl_Position = proj * view * transform * vec4(position, 1.0);"
                    "}";
    const GLchar* fragment_shader =
            "#version 150 core\n"
                    "out vec4 outColor;"
                    "in vec3 normal;"
                    "in vec3 fragpos;"
                    "in vec4 fragPosLightSpace;"
                    "uniform sampler2D depthMap;"
                    "uniform vec3 triangleColor;"
                    "uniform vec3 lightpos;"
                    "uniform vec3 shadowColor;"
                    "float ShadowCalculation(vec4 fragPosLightSpace){"
                    "   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;"
                    "   projCoords = (projCoords + 1) * 0.5;"
                    "   float closestDepth = texture(depthMap, projCoords.xy).r;"
                    "   float currentDepth = projCoords.z;"
                    "   float bias = 0.005;"
                    "   float shadow = currentDepth - bias > (closestDepth)? 1.0 : 0.0;"
                    "   return shadow;"
                    "}"
                    "void main()"
                    "{"
                    "    float ambientStrength = 0.2;"
                    "    vec3 ambient = ambientStrength * vec3(1.0,1.0,1.0);"
                    "    vec3 nm = normalize(normal);"
                    "    vec3 lightDir = normalize(lightpos - fragpos);"
                    "    float diff = max(dot(nm, lightDir), 0.0);"
                    "    vec3 diffuse = diff * vec3(1.0,1.0,1.0);"
                    "    float shadow = ShadowCalculation(fragPosLightSpace);"
                    "    vec3 result = (ambient + (1.0-shadow)*diffuse) * triangleColor + shadow * shadowColor;"
                    "    outColor = vec4(result, 1.0);"
                    "}";
    const GLchar* vertex_shader_rf =
            "#version 150 core\n"
                    "in vec3 position;"
                    "in vec3 norm;"
                    "out vec3 fragpos;"
                    "out vec3 normal;"
                    "uniform mat4 rotat;"
                    "uniform mat4 transform;"
                    "uniform mat4 view;"
                    "uniform mat4 proj;"
                    "void main()"
                    "{"
                    "    fragpos = vec3(transform * vec4(position,1.0));"
                    "    normal = vec3(rotat * vec4(norm,1.0));"
                    "    gl_Position = proj * view * transform * vec4(position, 1.0);"
                    "}";
    const GLchar* fragment_shader_rf = 
            "#version 150 core\n"
                    "out vec4 outColor;"
                    "in vec3 normal;"
                    "in vec3 fragpos;"
                    "uniform vec3 cameraPos;"
                    "uniform samplerCube skybox;"
                    "void main()"
                    "{"
                    "   vec3 I = normalize(fragpos - cameraPos);"
                    "   vec3 R = reflect(I, normalize(normal));"
                    "   outColor = vec4(texture(skybox,R).rgb,1.0);"
                    "}";
    const GLchar* skybox_vs = 
            "#version 150 core\n"
                    "in vec3 position;"
                    "out vec3 texCoord;"
                    "uniform mat4 proj;"
                    "uniform mat4 view;"
                    "void main()"
                    "{"
                    "   texCoord = position;"
                    "   vec4 pos = proj * view * vec4(position,1.0);"
                    "   gl_Position = pos.xyww;"
                    "}";
    const GLchar* skybox_fs = 
            "#version 150 core\n"
                    "out vec4 outColor;"
                    "in vec3 texCoord;"
                    "uniform samplerCube skybox;"
                    "void main()"
                    "{"
                    "   outColor = texture(skybox,texCoord);"
                    "}";
    const GLchar* debug_vs =
            "#version 150 core\n"
                    "in vec3 position;"
                    "out vec3 fragpos;"
                    "uniform mat4 rotat;"
                    "uniform mat4 transform;"
                    "uniform mat4 view;"
                    "uniform mat4 proj;"
                    "uniform mat4 LSM;"
                    "void main()"
                    "{"
                    "    fragpos = vec3(transform * vec4(position,1.0));"
                    "    gl_Position = transform * vec4(position, 1.0);"
                    "}";
    const GLchar* debug_fs =
            "#version 150 core\n"
                    "out vec4 outColor;"
                    "in vec3 fragpos;"
                    "uniform sampler2D depthMap;"
                    "uniform vec3 triangleColor;"
                    "uniform vec3 lightpos;"
                    "void main()"
                    "{"
                    "    vec2 TexCoord = vec2((fragpos.x+1.0f)/2.0f, (fragpos.y+1.0f)/2.0f);"
                    "    float depthValue = texture(depthMap, TexCoord).r;"
                    "    outColor = vec4(vec3(depthValue), 1.0);"
                    "}";
   /*
    const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec3 position;"
                    "uniform mat4 transform;"
                    "void main()"
                    "{"
                    "    gl_Position = transform * vec4(position, 1.0);"
                    "}";
    const GLchar* fragment_shader =
            "#version 150 core\n"
                    "out vec4 outColor;"
                    "uniform vec3 triangleColor;"
                    "void main()"
                    "{"
                    "    outColor = vec4(triangleColor, 1.0);"
                    "}";
    */

    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    //shadowProgram.init(shadowmap_vert,shadowmap_frag)
    
    /*unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shadowmap_vert, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shadowmap_frag, NULL);
    glCompileShader(fragmentShader);

    unsigned int shadowProgram;
    shadowProgram = glCreateProgram();

    glAttachShader(shadowProgram, vertexShader);
    glAttachShader(shadowProgram, fragmentShader);
    glLinkProgram(shadowProgram);
    */
    shadowProgram.init(shadowmap_vert,shadowmap_frag,"null");

    program.init(vertex_shader,fragment_shader,"outColor");

    skyboxProgram.init(skybox_vs,skybox_fs,"outColor");

    rfProgram.init(vertex_shader_rf, fragment_shader_rf, "outColor");
    //program.init(debug_vs,debug_fs,"outColor");
    //program.bind();

    // The vertex shader wants the position of the vertices as an input.
    // The following line connects the VBO we defined above with the position "slot"
    // in the vertex shader
    //program.bindVertexAttribArray("position",VBO);

    // Save the current time --- it will be used to dynamically change the triangle color
    auto t_start = std::chrono::high_resolution_clock::now();

    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);

    // Register the mouse callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    // Loop until the user closes the window
    //check_gl_error();
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glUniform1i(program.uniform("shadowMap"), 0);
    //check_gl_error();
    /*
    // 1. first render to depth map
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        ConfigureShaderAndMatrices();
        RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 2. then render scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ConfigureShaderAndMatrices();
    glBindTexture(GL_TEXTURE_2D, depthMap);
    RenderScene();
    */
    VertexBufferObject VB0;
    VB0.init();
    VB0.vnum = points1.size();
    VB0.type = 2;
    VB0.center = bcenter2;
    VB0.update(points1);
    VBOList.push_back(VB0);

    //VBOList.push_back(VBDebug);
    while (!glfwWindowShouldClose(window))
    {
        // Bind your VAO (not necessary if you have only one)
        //check_gl_error();

        /*unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);

        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                    SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        VAO.bind();
        //check_gl_error();
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        float near_plane = 0.001f, far_plane = 2.0f;
        glm::mat4 lightOrthoPoojection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, near_plane, far_plane);

 
        glm::mat4 lightView = glm::lookAt(glm::vec3((float)sin(time * 2.0f), (float)cos(time * 2.0f), 0.5),glm::vec3(0.0f),glm::vec3(0.0f, 1.0f, 0.0f));


        glm::mat4 lightSpaceMatrix = lightOrthoPoojection * lightView;
        //check_gl_error();
        //simpleDepthShader.use();
        //glUseProgram(shadowProgram);
        shadowProgram.bind();
        //check_gl_error();
        //simpleDepthShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //ConfigureShaderAndMatrices();
        glUniformMatrix4fv(shadowProgram.uniform("LSM"),1,GL_FALSE,glm::value_ptr(lightSpaceMatrix));
        
        //RenderScene();
        for(int i=0;i<VBOList.size();i++){
            //VBOList[i].init();
            //printf("Render depth map\n");
            shadowProgram.bindVertexAttribArray("position",VBOList[i]);

            glUniformMatrix4fv(shadowProgram.uniform("model"), 1, GL_FALSE, glm::value_ptr(VBOList[i].trans));
            
            glDrawArrays(GL_TRIANGLES, 0, VBOList[i].vnum);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        //check_gl_error();
        // Bind your program
        
        //program.bind();
        //check_gl_error();
        // Set the uniform value depending on the time difference
        //auto t_now = std::chrono::high_resolution_clock::now();
        //float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        int SCR_WIDTH, SCR_HEIGHT;
        glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(program.uniform("lightpos"), (float)sin(time * 2.0f), (float)cos(time * 2.0f), 0.5);
        glUniformMatrix4fv(program.uniform("LSM"),1,GL_FALSE,glm::value_ptr(lightSpaceMatrix));
        glUniform1i(program.uniform("depthMap"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        */
        //glUniform1i(program.uniform("shadowMap"), 0);
        //glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

        // Clear the framebuffer
        //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle

        //Turn on wireframe mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //view = glm::lookAt(cameraPos, cameraTarget, up);
        //glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

        //Turn off wireframe mode
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //cameraPos = glm::vec3(0.0f, 0.0f, 0.2f);
        //cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraDirection = glm::normalize(cameraPos - cameraTarget);
        up = glm::vec3(0.0f, 1.0f, 0.0f); 
        cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        cameraUp = glm::cross(cameraDirection, cameraRight);
        //VBOList.push_back(VBDebug);
        for(int i=0;i<VBOList.size();i++){
            //VBOList[i].init();
            if(VBOList[i].rf == 0){
                program.bind();
                
                glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 1.0f);
                glUniform3f(program.uniform("lightpos"), (float)sin(time * 2.0f), (float)cos(time * 2.0f), 0.5);
                glUniformMatrix4fv(program.uniform("LSM"),1,GL_FALSE,glm::value_ptr(lightSpaceMatrix));
                glUniform1i(program.uniform("depthMap"), 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, depthMap);
                if(sc == 0){
                    glUniform3f(program.uniform("shadowColor"),0.0f,0.0f,0.0f);
                }
                else{
                    glUniform3f(program.uniform("shadowColor"),1.0f,0.0f,0.0f);
                }
                if(selected == i){
                    glUniform3f(program.uniform("triangleColor"), 0.0f, 0.0f, 1.0f);
                }
                else{
                    glUniform3f(program.uniform("triangleColor"), 1.0f, 1.0f, 1.0f);
                }
                program.bindVertexAttribArray("position",VBOList[i]);
                if(smod==0){
                    program.bindVertexAttribArray("norm",VBONList[VBOList[i].type]);
                }
                else {
                    program.bindVertexAttribArray("norm",VBONList2[VBOList[i].type]);
                }
                glUniformMatrix4fv(program.uniform("transform"), 1, GL_FALSE, glm::value_ptr(VBOList[i].trans));
                glUniformMatrix4fv(program.uniform("rotat"), 1, GL_FALSE, glm::value_ptr(VBOList[i].rotat));
                view = glm::lookAt(cameraPos, cameraTarget, up);
                if(pmod == 0){
                    int width, height;
                    glfwGetWindowSize(window, &width, &height);
                    proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
                    glUniformMatrix4fv(program.uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
                    //glUniformMatrix4fv(program.uniform("proj"), 1, GL_FALSE, glm::value_ptr(lightOrthoPoojection));
                }
                else{
                    proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
                    int width, height;
                    glfwGetWindowSize(window, &width, &height);
                    float aspect_ratio = float(height)/float(width);
                    glm::mat4 dist = glm::scale(glm::mat4(1.f), glm::vec3(aspect_ratio, 1.f, 1.f));
                    proj = dist * proj;
                    //proj = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f);
                    glUniformMatrix4fv(program.uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
                    //glUniformMatrix4fv(program.uniform("proj"), 1, GL_FALSE, glm::value_ptr(lightOrthoPoojection));
                }
                glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
                //glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, glm::value_ptr(lightView));
                if(smod==1){
                    glDrawArrays(GL_TRIANGLES, 0, VBOList[i].vnum);
                }
                if(smod==2 || smod == 1){
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    if(selected!=i){
                        glUniform3f(program.uniform("triangleColor"), 0.0f, 0.0f, 0.0f);
                    }
                }
                glDrawArrays(GL_TRIANGLES, 0, VBOList[i].vnum);
                if(smod==2 || smod==1){
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
            else{
                rfProgram.bind();
                glUniform1i(rfProgram.uniform("skybox"), 0);
                
                glUniform3f(rfProgram.uniform("cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
                glUniformMatrix4fv(rfProgram.uniform("view"),1,GL_FALSE,glm::value_ptr(view));
                glUniformMatrix4fv(rfProgram.uniform("proj"),1,GL_FALSE,glm::value_ptr(proj));
                glUniformMatrix4fv(rfProgram.uniform("transform"),1,GL_FALSE,glm::value_ptr(VBOList[i].trans));
                glUniformMatrix4fv(rfProgram.uniform("rotat"),1,GL_FALSE,glm::value_ptr(VBOList[i].rotat));
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
                rfProgram.bindVertexAttribArray("position", VBOList[i]);
                rfProgram.bindVertexAttribArray("norm", VBONList[VBOList[i].type]);
                glDrawArrays(GL_TRIANGLES, 0, VBOList[i].vnum);
                
            }

        }
        glDepthFunc(GL_LEQUAL);
        skyboxProgram.bind();
        glUniform1i(program.uniform("skybox"), 0);
        glUniformMatrix4fv(skyboxProgram.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(skyboxProgram.uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        program.bindVertexAttribArray("position",VBSky);
        glDrawArrays(GL_TRIANGLES, 0, VBSky.vnum);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        /*program.bindVertexAttribArray("position",VBTest);
        glUniformMatrix4fv(program.uniform("transform"), 1, GL_FALSE, glm::value_ptr(VBTest.trans));
        view = glm::lookAt(cameraPos, cameraTarget, up);
        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(program.uniform("triangleColor"), 1.0f, 0.0f, 0.0f);
        //glDrawArrays(GL_LINES, 0, 2);
        glDrawArrays(GL_TRIANGLES,0,3);*/
        /*if(points.size()>1){
            glDrawArrays(GL_TRIANGLES, 1, points.size()-1);
        }*/

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //glDrawArrays(GL_TRIANGLES, 0, points.size());
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Deallocate opengl memory
    program.free();
    VAO.free();
    //VBO.free();
    for(int i=0;i<VBOList.size();i++){
        VBOList[i].free();
    }

    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}
