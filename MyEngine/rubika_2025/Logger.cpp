#include "Logger.h"

#include <SFML/System/Vector3.hpp>

#include "Imgui/imgui.h"
#include "Debugs.h"
#include "Entity.h"
#include "GameMgr.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

void Logger::Info(const std::string& message)
{
	sMessageInfo info;
	info.Type = eMessageType::Info;
	info.Message = message;
	Debugs::GetLogger().PushNewMessage(info);
}

void Logger::Warning(const std::string& message)
{
	sMessageInfo info;
	info.Type = eMessageType::Warning;
	info.Message = message;
	Debugs::GetLogger().PushNewMessage(info);
}

void Logger::Error(const std::string& message)
{
	sMessageInfo info;
	info.Type = eMessageType::Error;
	info.Message = message;
	Debugs::GetLogger().PushNewMessage(info);
}

void Logger::DrawLogger()
{
	Debugs::GetLogger().Draw();
}

void Logger::PushNewMessage(const sMessageInfo& message)
{
	Messages.push_back(message);
}

void Logger::Draw()
{
	/*if (ImGui::Begin("Console"))
	{
		if (ImGui::Checkbox("Info", &DisplayInfoMessage))
		{
		}

		if (ImGui::Checkbox("Info", &DisplayInfoMessage))
		{
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Warning", &DisplayWarningMessage))
		{
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Error", &DisplayErrorMessage))
		{
		}
		ImGui::SameLine();

		if (ImGui::Button("Clear"))
		{
			Messages.clear();
		}

		if (ImGui::BeginTable("Logs##LogConsole", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Message");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			for (const sMessageInfo& info : Messages)
			{
				ImVec4 color = ImVec4(1, 1, 1, 1);
				const char* text = nullptr;
				switch (info.Type)
				{
				case eMessageType::Info:
				{
					if (!DisplayInfoMessage)
					{
						continue;
					}

					color = ImVec4(1, 1, 1, 1);
					text = "Info";
				}
				break;

				case eMessageType::Warning:
				{
					if (!DisplayWarningMessage)
					{
						continue;
					}

					color = ImVec4(1, 1, 0, 1);
					text = "Warning";
				}
				break;

				case eMessageType::Error:
				{
					if (!DisplayErrorMessage)
					{
						continue;
					}

					color = ImVec4(1, 0, 0, 1);
					text = "Error";
				}
				break;

				default:
					continue;
				}

				ImGui::TableSetColumnIndex(0);
				ImGui::TextColored(color, text);
				ImGui::TableSetColumnIndex(1);
				ImGui::Text(info.Message.c_str());
				ImGui::TableNextRow();
			}

			ImGui::EndTable();
		}

	}
	ImGui::End();
*/
	
	if (ImGui::Begin("Debug"))
	{
		if (ImGui::Button("Spawn"))
		{
			Entity* test = new Entity();
			 sf::Vector2f vec  (rand() % 200, rand() % 200); 
			test->GetComponent<TransformComponent>()->SetPosition(vec);
			test->GetComponent<SpriteComponent>()->SetTexture("C:/Users/p.farin/Documents/GitHub/rubika_25_26_public/MyEngine/Ressources/IsaacSprite.png");
			test->GetComponent<SpriteComponent>()->SetAnimation("Body_Vertical");
			Globals::Instance().GameMgr->AddEntity(test);	
		}
		if (ImGui::Button("Delete"))
		{
			Globals::Instance().GameMgr->DeleteEntity();
		}
	}
	ImGui::End();
}