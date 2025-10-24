#pragma once
#include "Sprite.h"

class Button : public Sprite
{
public:
    Button(const Texture& texture, const Texture& pressedTexture);
    bool IsClicked() const;
    bool IsHeld() const;

    void UpdateTexture();

    bool IsPositionInButtonBounds(glm::vec2 pos);

    bool m_isClicked = false;
    bool m_isHeld = false;
private:
    Texture m_texture;
    Texture m_pressedTexture;
};
