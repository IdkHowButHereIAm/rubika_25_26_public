#include "Globals.h"

#include <ImGui/imgui_internal.h>

#include "GameMgr.h"
#include "Logger.h"
#include "TextureMgr.h"

Globals& Globals::Instance()
{
    if (m_instance == nullptr)
        m_instance = new Globals();
    
    return *m_instance;
}

Globals::Globals()
{
}

void Globals::Init()
{
    TextureMgr = new ::TextureMgr();
    GameMgr = new ::GameMgr();
}

void Globals::Destroy()
{
}

void Globals::Update()
{
}

void Globals::Draw()
{
}
