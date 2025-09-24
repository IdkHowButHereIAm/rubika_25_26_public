#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#include "Profiler.h"
#include "Globals.h"

#include "Logger.h"
#include "TextureMgr.h"

unsigned long long uFrameCount = 0;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);

    Logger::Info("This is an info message. That is useful");
    Logger::Warning("This is a warning message. Should I worry?");
    Logger::Error("This is an error message. Oh sh*t!!");

    Globals::Instance().Init();

    sf::Clock clock;
    clock.restart();

    while (window.isOpen())
    {
        Globals::Instance().Update();
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", uFrameCount);

        int deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            ImGui::SFML::ProcessEvent(window, event.value());
        }

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
        ImGui::SFML::Update(window, imGuiTime);

        // sample
        //ImGui::ShowDemoWindow();
        Logger::DrawLogger();

        PROFILER_EVENT_END();
        
        
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();
      

        Globals::Instance().TextureMgr->LoadTexture("C:/Users/p.farin/Documents/GitHub/rubika_25_26_public/MyEngine/Ressources/enemy_texture_1.xml");

        sf::Sprite sprite(Globals::Instance().TextureMgr->GetTextureData("vroom").texture);
        sprite.setScale(sf::Vector2f(0.25f, 0.25f));
        
        window.draw(sprite);

        ImGui::SFML::Render(window);
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
        ++uFrameCount;
        Globals::Instance().Draw();
    }

    ImGui::SFML::Shutdown();

    return 0;
}