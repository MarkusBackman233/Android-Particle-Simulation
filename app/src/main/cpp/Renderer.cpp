#include "Renderer.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

std::string LoadAsset(AAssetManager* mgr, const char* filename) {
    AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
    if (!asset) return "";
    size_t size = AAsset_getLength(asset);
    std::string result(size, '\0');
    AAsset_read(asset, result.data(), size);
    AAsset_close(asset);
    return result;
}

GLuint loadShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            std::vector<char> log(len);
            glGetShaderInfoLog(shader, len, nullptr, log.data());
            __android_log_print(ANDROID_LOG_ERROR, "Shader", "%s", log.data());
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

Renderer::Renderer(AAssetManager* assetManager,
                   int screenWidth,
                   int screenHeight)
{
    m_screenwidth = static_cast<float>(screenWidth);
    m_screenheight = static_cast<float>(screenHeight);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    CircleRenderSetup(assetManager);

    return;

    std::string vsSrc = LoadAsset(assetManager, "shaders/Default_VS.glsl");
    std::string fsSrc = LoadAsset(assetManager, "shaders/Default_PS.glsl");
    GLuint vs = loadShader(GL_VERTEX_SHADER, vsSrc.c_str());
    GLuint fs = loadShader(GL_FRAGMENT_SHADER, fsSrc.c_str());

    m_program = glCreateProgram();
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);
    glLinkProgram(m_program);


    GLint linked;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint len;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            std::vector<char> log(len);
            glGetProgramInfoLog(m_program, len, nullptr, log.data());
            __android_log_print(ANDROID_LOG_ERROR, "Program", "%s", log.data());
        }
        glDeleteProgram(m_program);
        m_program = 0;
    }

    // Attribute & uniform locations
    m_offsetXHandle = glGetUniformLocation(m_program, "offsetX");
    m_offsetYHandle = glGetUniformLocation(m_program, "offsetY");
    m_scaleHandle = glGetUniformLocation(m_program, "scale");
    m_uvScaleHandle = glGetUniformLocation(m_program, "uvScale");
    m_mvpMatrixHandle = glGetUniformLocation(m_program, "uMVPMatrix");
    m_textureSamplerHandle = glGetUniformLocation(m_program, "u_Texture");



    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glUseProgram(m_program);

    m_positionHandle = glGetAttribLocation(m_program, "vPosition");
    glEnableVertexAttribArray(m_positionHandle);
    glVertexAttribPointer(m_positionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    m_uvHandle = glGetAttribLocation(m_program, "a_TexCoord");
    glEnableVertexAttribArray(m_uvHandle);
    glVertexAttribPointer(m_uvHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {   // Render Binds
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        glEnableVertexAttribArray(m_positionHandle);
        glVertexAttribPointer(m_positionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(m_uvHandle);
        glVertexAttribPointer(m_uvHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glActiveTexture(GL_TEXTURE0);

        { // clamp sampling
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        glUseProgram(m_program);
        glUniform1i(m_textureSamplerHandle, 0);
    }
}

void Renderer::Resize(float x, float y)
{
    m_screenwidth = x;
    m_screenheight = y;
    glViewport(0, 0, static_cast<int>(x), static_cast<int>(y));

    //float aspectRatio = m_screenwidth / m_screenheight;
    m_orthoProjection = glm::ortho(
            0.0f,                 // left
            m_screenwidth,        // right
            m_screenheight,       // bottom
            0.0f,                 // top (flipped for top-left origin)
            -1.0f,                // near
            1.0f                  // far
    );
    glUniformMatrix4fv(m_mvpMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_orthoProjection));

}

void Renderer::Draw(Sprite* sprite)
{
    m_renderList.push_back(sprite);
}


void Renderer::Render()
{
    return;
    glClear(GL_COLOR_BUFFER_BIT); // this clears circles


    for (Sprite* sprite : m_renderList)
    {
        { // clamp sampling
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        Texture* texture = sprite->GetTexture();


        glUniform1f(m_offsetXHandle, sprite->GetPosition().x);
        glUniform1f(m_offsetYHandle, sprite->GetPosition().y);
        glUniform2f(m_scaleHandle,
                    static_cast<float>(texture->Width) * sprite->GetScale(),
                    static_cast<float>(texture->Height) * sprite->GetScale()
        );
        glUniform1f(m_uvScaleHandle, sprite->GetUvScale());
        glBindTexture(GL_TEXTURE_2D, texture->TextureHandle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    }
    m_renderList.clear();
}

void Renderer::CircleRenderSetup(AAssetManager* assetManager)
{
    std::string vsSrc = LoadAsset(assetManager, "shaders/Circle_VS.glsl");
    std::string fsSrc = LoadAsset(assetManager, "shaders/Circle_PS.glsl");
    GLuint vs = loadShader(GL_VERTEX_SHADER, vsSrc.c_str());
    GLuint fs = loadShader(GL_FRAGMENT_SHADER, fsSrc.c_str());

    m_circleProgram = glCreateProgram();
    glAttachShader(m_circleProgram, vs);
    glAttachShader(m_circleProgram, fs);
    glLinkProgram(m_circleProgram);
    GLint linked;
    glGetProgramiv(m_circleProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint len;
        glGetProgramiv(m_circleProgram, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            std::vector<char> log(len);
            glGetProgramInfoLog(m_circleProgram, len, nullptr, log.data());
            __android_log_print(ANDROID_LOG_ERROR, "Program", "%s", log.data());
        }
        glDeleteProgram(m_circleProgram);
        m_circleProgram = 0;
    }
    glUseProgram(m_circleProgram);
    m_mvpMatrixHandleCricle = glGetUniformLocation(m_circleProgram, "uMVPMatrix");
    m_maxDroplets = 1000;
    float scale = 1.0f;
    float quadVertices[] = {
            -1.0f*scale, -1.0f*scale,
            1.0f*scale, -1.0f*scale,
            1.0f*scale,  1.0f*scale,
            -1.0f*scale,  1.0f*scale
    };
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    GLuint quadIndices[] = { 0, 1, 2, 2, 3, 0 };
    glGenBuffers(1, &m_EBOCircle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOCircle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);



    glGenBuffers(1, &m_quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glGenBuffers(1, &m_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxDroplets * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glVertexAttribDivisor(1, 1); // one per instance

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(2 * sizeof(float)));
    glVertexAttribDivisor(2, 1);


}

void Renderer::RenderCircles(const std::vector<glm::vec2>& positions)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_circleProgram);

    glUniformMatrix4fv(m_mvpMatrixHandleCricle, 1, GL_FALSE, glm::value_ptr(m_orthoProjection));


    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec2), positions.data());
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOCircle); // make sure m_EBO points to the quad indices

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, positions.size());
}

