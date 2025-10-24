#include "Sprite.h"

Sprite::Sprite(const Texture& texture)
    : m_texture(texture)
{

}

void Sprite::SetPosition(glm::vec2 pos)
{
    m_position = pos;
}

void Sprite::SetScale(float scale)
{
    m_scale = scale;
}
void Sprite::SetUvScale(float scale)
{
    m_uvScale = scale;
}

glm::vec2 Sprite::GetPosition()
{
    return m_position;
}

float Sprite::GetUvScale()
{
    return m_uvScale;
}

Texture* Sprite::GetTexture() {
    return &m_texture;
}

float Sprite::GetScale() {
    return m_scale;
}

void Sprite::SetTexture(Texture texture)
{
    m_texture = texture;
}






