#include "Globals.h"

#include <ImGui/imgui_internal.h>

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
}

void Globals::Destroy()
{
}
