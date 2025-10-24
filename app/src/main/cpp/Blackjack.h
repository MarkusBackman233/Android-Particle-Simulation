#pragma once
#include "Renderer.h"
#include "ResourceManager.h"
#include "Button.h"
#include "ButtonManager.h"

class Blackjack
{
public:
    Blackjack(AAssetManager* assetManager,
              Renderer* renderer,
              ButtonManager* buttonManager
              );

    void Update();

    void UpdatePhoneOrientation(const glm::vec3& at);
    void UpdateAcceleration(const glm::vec3& acceleration);

private:

    glm::vec3 m_phoneAtVector;
    glm::vec3 m_accelerationAccumulator;

    std::vector<glm::vec2> m_positions;
    std::vector<glm::vec2> m_velocities;


    Renderer* m_renderer;
    ButtonManager* m_buttonManager;
    Button* m_spawn_goblin_button;
    Button* m_spawn_goblin_button2;


    //Sprite* sprite1;
    //Sprite* m_player;
    ResourceManager m_resourceManager;
};
