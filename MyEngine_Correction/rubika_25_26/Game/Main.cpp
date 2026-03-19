#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <Engine/Render/Ressource/TextureMgr.h>
#include <Engine/Console/LogConsole.h>
#include <Engine/Globals.h>
#include <Engine/Gameplay/GameMgr.h>

#include <Engine/Gameplay/Entity/Entity.h>
#include <Engine/Gameplay/Component/Transform/Transform.h>
#include <Engine/Gameplay/Component/Renderer/Renderer.h>
#include <Engine/Render/Drawable/Sprite/Sprite.h>
#include <Engine/Task/TaskMgr.h>
#include <Engine/Input/InputMgr.h>

#ifdef _USE_IMGUI
#include "Imgui/imgui.h"
#include "Imgui/imgui-SFML.h"
#endif

#include <Engine/Profiler.h>

void PopulateUpdate()
{
    for (int i = 0; i < 5; ++i)
    {
        gData.TaskMgr->RegisterTask([i]()
            {
                PROFILER_EVENT_BEGIN(PROFILER_COLOR_DARK_BLUE, "Update %d", i);

                Sleep(5);

                PROFILER_EVENT_END();
            },
            TaskMgr::ePhase::Update);
    }
}

void PopulateDraw()
{
    for (int i = 0; i < 5; ++i)
    {
        gData.TaskMgr->RegisterTask([i]()
            {
                PROFILER_EVENT_BEGIN(PROFILER_COLOR_DARK_BLUE, "Draw %d", i);

                Sleep(5);

                PROFILER_EVENT_END();
            },
            TaskMgr::ePhase::Draw);
    }
}

Entity* CreateEntity(sf::Vector2f pos)
{
    Entity* e = new Entity();

    Transform* TransformComp = e->AddComponent<Transform>();
    Renderer* RendererComp = e->AddComponent<Renderer>();

    Sprite* Body = RendererComp->AddNewDrawable<Sprite>("Body", sf::Vector2f(2, 0), 0, sf::Vector2f(1, 1));
    Body->SetVisibility(true);
    Body->SetTexture("../Ressources/IsaacSprite.png");
    Body->SetAnimation("Body_Vertical");

    Sprite* Head = RendererComp->AddNewDrawable<Sprite>("Head", sf::Vector2f(-2.5f, -20), 0, sf::Vector2f(1, 1));
    Head->SetVisibility(true);
    Head->SetTexture("../Ressources/IsaacSprite.png");
    Head->SetAnimation("Head_Down");

    TransformComp->SetWorldPosition(pos);

    return e;
}

unsigned SIZE_X = 1280;
unsigned SIZE_Y = 720;

int main()
{
    gData.Init();

    sf::RenderWindow window(sf::VideoMode({ SIZE_X, SIZE_Y }), "MyEngine - Rubika 2025 2026");
    
#ifdef _USE_IMGUI
    if (!ImGui::SFML::Init(window))
    {
        return -1;
    }
#endif

    if (!gData.TextureMgr->LoadTexture("../Ressources/IsaacSprite.png"))
    {
        return -1;
    }
    
    if (!gData.TextureMgr->LoadTexture("../Ressources/Basement.png"))
    {
        return -2;
    }

    if (!gData.TextureMgr->LoadTexture("../Ressources/Tear.png"))
    {
        return -2;
    }

    if (!gData.TextureMgr->LoadTexture("../Ressources/Rocks.png"))
    {
        return -3;
    }

    if (!gData.TextureMgr->LoadTexture("../Ressources/Doors.png"))
    {
        return -4;
    }

    for (float x = 0; x < SIZE_X; x += 50)
    {
        for (float y = 0; y < SIZE_Y; y += 50)
        {
            gData.GameMgr->AddEntity(CreateEntity(sf::Vector2f(x, y)));

        }
    }

    sf::Clock clock;
    clock.restart();

    while (window.isOpen() && !gData.ExitApp)
    {
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", gData.FrameCount);
        {
            int deltaTimeMS = clock.getElapsedTime().asMilliseconds();
            float fDeltaTimeS = (float)deltaTimeMS / 1000.f;
            sf::Time imGuiTime = clock.restart();

            PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");
            {
                while (const std::optional event = window.pollEvent())
                {

                    if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                    {
                        window.close();
                        gData.ExitApp = true;
                    }

#ifdef _USE_IMGUI
                    ImGui::SFML::ProcessEvent(window, *event);
#endif
                }

                gData.InputMgr->Update(fDeltaTimeS);
            }
            PROFILER_EVENT_END();

            PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
            {
                //PopulateUpdate();

                gData.TaskMgr->StartPhase(TaskMgr::ePhase::Update);
#ifdef _USE_IMGUI
                ImGui::SFML::Update(window, imGuiTime);
#endif

                gData.GameMgr->Update(fDeltaTimeS);

                gData.TaskMgr->WaitPhase();
            }
            PROFILER_EVENT_END();

            PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
            {
                //PopulateDraw();

                gData.TaskMgr->StartPhase(TaskMgr::ePhase::Draw);

                PROFILER_EVENT_BEGIN(PROFILER_COLOR_BROWN, "Debug Draw");
                gData.DebugMgr->Draw();
                PROFILER_EVENT_END();

                window.clear();

                PROFILER_EVENT_BEGIN(PROFILER_COLOR_PURPLE, "Game Draw");
                gData.GameMgr->Draw(window);
                PROFILER_EVENT_END();

#ifdef _USE_IMGUI
                PROFILER_EVENT_BEGIN(PROFILER_COLOR_ORANGE, "ImGui Draw");
                ImGui::SFML::Render(window);
                PROFILER_EVENT_END();
#endif
                gData.TaskMgr->WaitPhase();

                PROFILER_EVENT_BEGIN(PROFILER_COLOR_CYAN, "Window Display");
                window.display();
                PROFILER_EVENT_END();

            }
            PROFILER_EVENT_END();
        }
        PROFILER_EVENT_END();
        ++gData.FrameCount;
    }

    gData.ExitApp = true;

    gData.Shut();
    gData.Destroy();
#ifdef _USE_IMGUI
    ImGui::SFML::Shutdown();
#endif

    return 0;
}