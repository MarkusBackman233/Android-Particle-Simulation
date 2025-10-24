#pragma once
#include <GLES3/gl3.h>
#include "glm/vec2.hpp"
#include "ResourceManager.h"
class Sprite
{
public:
    Sprite(const Texture& texture);

    void SetPosition(glm::vec2 pos);
    void SetScale(float scale);
    void SetUvScale(float scale);

    Texture* GetTexture();
    glm::vec2 GetPosition();
    float GetUvScale();
    float GetScale();


    void SetTexture(Texture texture);
private:
    float m_scale = 1.0f;
    float m_uvScale = 1.0f;
    Texture m_texture;
    glm::vec2 m_position;
};
