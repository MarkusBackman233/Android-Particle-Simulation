#pragma once
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cmath>
#include <cstdlib>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Sprite.h"
#include <vector>
class Renderer {
public:
    Renderer(AAssetManager* assetManager,
             int screenWidth,
             int screenHeight);
    void Resize(float x, float y);

    void Draw(Sprite* sprite);

    void Render();


    void CircleRenderSetup(AAssetManager* assetManager);
    void RenderCircles(const std::vector<glm::vec2>& positions);

    float GetScreenWidth() { return m_screenwidth; }
    float GetScreenHeight() { return m_screenheight; }

private:
    GLuint m_quadVBO;
    GLuint m_instanceVBO;
    GLuint m_VAO;
    GLuint m_EBOCircle;
    GLuint m_maxDroplets;
    GLuint m_circleProgram;
    GLuint m_mvpMatrixHandleCricle;


    GLuint m_program;
    GLuint m_positionHandle;
    GLuint m_uvHandle;
    GLuint m_offsetXHandle;
    GLuint m_offsetYHandle;
    GLuint m_scaleHandle;
    GLuint m_uvScaleHandle;
    GLuint m_mvpMatrixHandle;
    GLuint m_textureSamplerHandle;
    GLuint m_VBO;
    GLuint m_EBO;

    float quadVertices[20]  {
            // X, Y, Z, U, V
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, // Top-left
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-left
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f  // Bottom-right
    };
    unsigned short quadIndices[6] = { 0, 1, 2, 2, 1, 3 };
    float m_screenwidth = 0.0f;
    float m_screenheight = 0.0f;

    std::vector<Sprite*> m_renderList;

    glm::mat4 m_orthoProjection;
};
