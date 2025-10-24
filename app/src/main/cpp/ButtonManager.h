#pragma once
#include <vector>
#include "Button.h"

class ButtonManager
{
public:
    ButtonManager() {};

    void OnTouchDown(glm::vec2 position, int pointerId);
    void OnTouchUp(glm::vec2 position, int pointerId);
    void OnTouchMove(glm::vec2 position, int pointerId);
    void UpdateButtons();

    void AddButton(Button* button);

    std::vector<Button*>& GetButtons();

    enum TouchState
    {
        Inactive,
        Pressed,
        Held,
        Released,
    };

    struct TouchAction
    {
        TouchState State;
        glm::vec2 Position;
    };

    std::vector<Button*> m_buttons;
    std::unordered_map<int, TouchAction> m_touchStates;
};
