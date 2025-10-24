#include "ButtonManager.h"

std::vector<Button*>& ButtonManager::GetButtons()
{
    return m_buttons;
}

void ButtonManager::AddButton(Button* button)
{
    m_buttons.push_back(button);
}

void ButtonManager::UpdateButtons()
{
    for(Button* button : m_buttons)
    {
        bool found_touch = false;
        for (auto& [touchId, touchAction] : m_touchStates)
        {
            bool is_touch_in_bounds = button->IsPositionInButtonBounds(touchAction.Position);
            if(is_touch_in_bounds == false)
            {
                continue;
            }
            found_touch = true;
            switch (touchAction.State)
            {
                case Inactive:
                    break;
                case Pressed:
                    button->m_isClicked = true;
                    break;
                case Held:
                    if (button->m_isClicked)
                        button->m_isHeld = true;
                    break;
                case Released:
                    break;
            }
            break;
        }
        if(found_touch == false)
        {
            button->m_isHeld = false;
            button->m_isClicked = false;
        }
        button->UpdateTexture();

    }

    for (auto& [touchId, touchAction] : m_touchStates)
    {
        if (touchAction.State == TouchState::Pressed)
        {
            touchAction.State = TouchState::Held;
        }
        if (touchAction.State == TouchState::Released)
        {
            m_touchStates.erase(touchId);
        }
    }
}


void ButtonManager::OnTouchDown(glm::vec2 position, int pointerId)
{
    m_touchStates.emplace(pointerId, TouchAction{TouchState::Pressed, position});
}

void ButtonManager::OnTouchUp(glm::vec2 position, int pointerId)
{
    m_touchStates.insert_or_assign(pointerId, TouchAction{TouchState::Released, position});
}
void ButtonManager::OnTouchMove(glm::vec2 position, int pointerId)
{
    auto it = m_touchStates.find(pointerId);
    if (it != m_touchStates.end())
        it->second.Position = position;
}