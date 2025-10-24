#include "Button.h"

Button::Button(const Texture &texture, const Texture& pressedTexture) : Sprite(texture)
    , m_texture(texture)
    , m_pressedTexture(pressedTexture)
{

}

void Button::UpdateTexture()
{
    SetTexture(IsHeld() ? m_pressedTexture : m_texture);
}

bool Button::IsClicked() const
{
    return m_isClicked;
}

bool Button::IsHeld() const
{
    return m_isHeld || m_isClicked;
}

bool Button::IsPositionInButtonBounds(glm::vec2 pos)
{
    float w_half = GetScale() * GetTexture()->Width * 0.5f;
    float h_half = GetScale() * GetTexture()->Height * 0.5f;

    if (pos.x > GetPosition().x - w_half && pos.x < GetPosition().x + w_half &&
        pos.y > GetPosition().y - h_half && pos.y < GetPosition().y + h_half)
    {
        return true;
    }
    return false;
}

