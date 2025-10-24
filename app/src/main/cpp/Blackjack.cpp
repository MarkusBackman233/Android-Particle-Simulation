
#include "Blackjack.h"
#include <random>
Blackjack::Blackjack(AAssetManager* assetManager,
    Renderer* renderer,
    ButtonManager* buttonManager
    )
    : m_renderer(renderer)
    , m_resourceManager(assetManager)
    , m_buttonManager(buttonManager)
    , m_phoneAtVector(0.0f,0.0f,0.0f)
{
    //sprite1 = new Sprite(m_resourceManager.GetTexture("grass.png"));
    //sprite1->SetPosition({1000.00,  500});
    //sprite1->SetUvScale(20.0f);
    //sprite1->SetScale(10);
//
    //m_player = new Sprite(m_resourceManager.GetTexture("Attack.png"));
    //m_player->SetPosition({500,1000});
    //m_player->SetScale(8.0f);
//
    //{
    //    m_spawn_goblin_button = new Button(m_resourceManager.GetTexture("UI_Flat_Button02a_4.png"), m_resourceManager.GetTexture("UI_Flat_Button02a_1.png"));
    //    m_spawn_goblin_button->SetPosition({300,2000});
    //    m_spawn_goblin_button->SetScale(9);
    //    buttonManager->AddButton(m_spawn_goblin_button);
    //}
    //{
    //    m_spawn_goblin_button2 = new Button(m_resourceManager.GetTexture("UI_Flat_Button02a_4.png"), m_resourceManager.GetTexture("UI_Flat_Button02a_1.png"));
    //    m_spawn_goblin_button2->SetPosition({800,2000});
    //    m_spawn_goblin_button2->SetScale(9);
    //    buttonManager->AddButton(m_spawn_goblin_button2);
    //}

    //m_resourceManager.GetTexture("Attack.png");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0.0f, renderer->GetScreenWidth());
    std::uniform_real_distribution<float> yDist(0.0f, renderer->GetScreenHeight());

    m_positions.resize(200);
    m_velocities.resize(m_positions.size());
    for (int i = 0; i < m_positions.size(); ++i) {
        float x = xDist(gen);
        float y = yDist(gen);

        m_positions[i] = glm::vec2 (x,y);
    }
}



void Blackjack::Update()
{
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    if(glm::length(m_phoneAtVector) < 0.1f)
        return;
    glm::vec2 gravity(m_phoneAtVector.x, -m_phoneAtVector.z); // flip sign so "down" points to bottom of phone
    size_t size = m_positions.size();


    float sWidth = m_renderer->GetScreenWidth();
    float sHeight = m_renderer->GetScreenHeight();


    float restitution = 0.5f;
    const float restitutionHalf = restitution * 0.5f;
    float dt = 1.0f / 120.0f * 0.5;
    const int iterations = 55;  // multiple passes per frame
    float dt_sub = dt / iterations;  // smaller substep
    float pixelToWorldSpaceTime = 163.0f * 100.0f * dt_sub;



    glm::vec2 gravityVec = glm::normalize(gravity) * 9.81f * pixelToWorldSpaceTime;
    gravityVec *= 1.0f - abs(m_phoneAtVector.y);



    float dampingCoefficient = 0.2f; // adjust for desired damping
    float dampingFactor = exp(-dampingCoefficient * dt_sub);

    float friction = 2.0f * dt_sub;
    float radius = 30.0f;

    float radius2 = radius + radius;
    float minDist2 = radius2 * radius2;

    float cellSize = radius2;
    float invCellSize = 1.0f / cellSize;
    int cellsCountX = static_cast<int>(sWidth * invCellSize);
    int cellsCountY = static_cast<int>(sHeight * invCellSize);
    int numCells = cellsCountX * cellsCountY;

    const uint32_t unasignedIndex= 9999999;
    static std::vector<uint32_t> cellParticleCount(numCells);      // number of particles per cell
    static std::vector<uint32_t> cellParticleStartIndex(numCells);     // prefix sums
    static std::vector<uint32_t> particleIndices(m_positions.size(),unasignedIndex); // flattened array of particle indices

    float h = m_renderer->GetScreenHeight() - radius;
    float w = m_renderer->GetScreenWidth() - radius;

    glm::vec2 acceleration = glm::vec2 (-m_accelerationAccumulator.x  , m_accelerationAccumulator.y) * pixelToWorldSpaceTime * 0.4f;
    m_accelerationAccumulator = glm::vec3 (0.0f);

    for (int iter = 0; iter < iterations; ++iter) {


        std::fill(cellParticleCount.begin(), cellParticleCount.end(), 0);
        std::fill(particleIndices.begin(), particleIndices.end(), unasignedIndex);

        for (int i = 0; i < size; ++i) {
            glm::vec2 &pos = m_positions[i];

            int cellX = static_cast<int>(pos.x * invCellSize);
            int cellY = static_cast<int>(pos.y * invCellSize);

            uint32_t cellIndex = cellX + cellY * cellsCountX;
            cellParticleCount[cellIndex]++;
        }

        uint32_t currentCount = 0;
        for (uint32_t cellIndex = 0; cellIndex < numCells; ++cellIndex)
        {
            cellParticleStartIndex[cellIndex] = currentCount;
            currentCount += cellParticleCount[cellIndex];
        }


        for (int i = 0; i < size; ++i) {
            glm::vec2 &pos = m_positions[i];

            int cellX = static_cast<int>(pos.x * invCellSize);
            int cellY = static_cast<int>(pos.y * invCellSize);

            uint32_t cellIndex = cellX + cellY * cellsCountX;

            uint32_t startIndex = cellParticleStartIndex[cellIndex];

            uint32_t particleIndexInCell = startIndex;

            while(particleIndices[particleIndexInCell] != unasignedIndex)
            {
                particleIndexInCell++;
            }

            particleIndices[particleIndexInCell] = i;
        }


        for (int i = 0; i < size; ++i) {
            glm::vec2 &pos = m_positions[i];
            glm::vec2 &vel = m_velocities[i];
            int cellX = static_cast<int>(pos.x * invCellSize);
            int cellY = static_cast<int>(pos.y * invCellSize);
            vel += gravityVec;
            vel += acceleration;
            vel *= dampingFactor;
            pos += vel * dt_sub;

            for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                    int nx = cellX + offsetX;
                    int ny = cellY + offsetY;


                    if (nx < 0 || ny < 0 || nx >= cellsCountX || ny >= cellsCountY)
                        continue;

                    uint32_t neighborIndex = nx + ny * cellsCountX;
                    uint32_t start = cellParticleStartIndex[neighborIndex];
                    uint32_t end = start + cellParticleCount[neighborIndex];

                    for (uint32_t j = start; j < end; ++j) {
                        uint32_t otherIndex = particleIndices[j];
                        if (otherIndex <= i) continue; // process each pair once

                        glm::vec2& otherPos = m_positions[otherIndex];

                        glm::vec2 delta = pos - otherPos;
                        float dist2 = glm::dot(delta, delta);
                        if (dist2 >= minDist2)
                        {
                            continue;
                        }

                        float dist = std::sqrt(dist2);
                        glm::vec2 normal = delta / dist;
                        float penetration = radius2 - dist;
                        auto correction = normal * (penetration * 0.5f);
                        pos += correction;
                        otherPos -= correction;


                        glm::vec2& otherVel = m_velocities[otherIndex];
                        glm::vec2 relativeVel = vel - otherVel;
                        float separatingVel = glm::dot(relativeVel, normal);
                        if (separatingVel < 0.0f) { // only if moving towards each other
                            auto impulse = -separatingVel * normal * restitutionHalf;
                            vel += impulse;
                            otherVel -= impulse;
                        }
                    }
                }
            }

            // Bottom
            if (pos.y < radius) {
                pos.y = radius;
                if (vel.y < 0.0f) vel.y *= -restitution;
            }
            // Top
            if (pos.y > h) {
                pos.y = h;
                if (vel.y > 0.0f) vel.y *= -restitution;
            }
            // Left
            if (pos.x < radius) {
                pos.x = radius;
                if (vel.x < 0.0f) vel.x *= -restitution;
            }
            // Right
            if (pos.x > w) {
                pos.x = w;
                if (vel.x > 0.0f) vel.x *= -restitution;
            }
        }
    }



    m_renderer->RenderCircles(m_positions);



    for (Button* button : m_buttonManager->GetButtons())
    {
        m_renderer->Draw(button);
    }
}

void Blackjack::UpdatePhoneOrientation(const glm::vec3& at)
{
    m_phoneAtVector = at;
}

void Blackjack::UpdateAcceleration(const glm::vec3 &acceleration)
{
    m_accelerationAccumulator += acceleration;
}
