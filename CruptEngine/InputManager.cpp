#include "CruptEnginePCH.h"
#include "InputManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "ImGui/imgui_impl_sdl.h"
#include "Invoker.h"
#include "ICommand.h"
#include <SDL.h>

PBYTE crupt::InputManager::m_pCurrKeyboardState = nullptr;
PBYTE crupt::InputManager::m_pOldKeyboardState = nullptr;
PBYTE crupt::InputManager::m_pKeyboardState0 = nullptr;
PBYTE crupt::InputManager::m_pKeyboardState1 = nullptr;
bool crupt::InputManager::m_KeyboardState0Active = true;
XINPUT_STATE crupt::InputManager::m_OldGamepadState[XUSER_MAX_COUNT];
XINPUT_STATE crupt::InputManager::m_CurrGamepadState[XUSER_MAX_COUNT];
bool crupt::InputManager::m_ConnectedGamepads[XUSER_MAX_COUNT];
bool crupt::InputManager::m_Enabled = true;
bool crupt::InputManager::m_ForceToCenter = false;

void crupt::InputManager::Init()
{
	m_pInvoker = &Invoker::GetInstance();

	m_pKeyboardState0 = new BYTE[256];
	m_pKeyboardState1 = new BYTE[256];

	bool gotKeyboardState = false;
	gotKeyboardState = GetKeyboardState(m_pKeyboardState0);
	gotKeyboardState = GetKeyboardState(m_pKeyboardState1);
	RefreshControllerConnections();

	UpdateKeyboardStates();
	UpdateGamepadStates();
}

crupt::InputManager::~InputManager()
{
	for(std::pair<std::string, ICommand*> command : m_CommandMap)
	{
		delete command.second;
	}

	m_BindingMap.clear();
	m_CommandMap.clear();


	if (m_pKeyboardState0 != nullptr)
	{
		delete[] m_pKeyboardState0;
		delete[] m_pKeyboardState1;

		m_pKeyboardState0 = nullptr;
		m_pKeyboardState1 = nullptr;
		m_pCurrKeyboardState = nullptr;
		m_pOldKeyboardState = nullptr;
	}
}

void crupt::InputManager::AddBinding(const std::string& eventName, Binding newBinding)
{
	m_BindingMap[eventName] = newBinding;
}

void crupt::InputManager::Rebind(const std::string& eventName, Binding newBinding)
{
	m_BindingMap[eventName] = newBinding;
}

void crupt::InputManager::RefreshControllerConnections()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		const DWORD dwResult = XInputGetState(i, &state);
		m_ConnectedGamepads[i] = (dwResult == ERROR_SUCCESS);
	}
}

bool crupt::InputManager::UpdateKeyboardStates()
{
	//Get Current KeyboardState and set Old KeyboardState
	BOOL getKeyboardResult;
	if (m_KeyboardState0Active)
	{
		getKeyboardResult = GetKeyboardState(m_pKeyboardState1);
		m_pOldKeyboardState = m_pKeyboardState0;
		m_pCurrKeyboardState = m_pKeyboardState1;
	}
	else
	{
		getKeyboardResult = GetKeyboardState(m_pKeyboardState0);
		m_pOldKeyboardState = m_pKeyboardState1;
		m_pCurrKeyboardState = m_pKeyboardState0;
	}

	m_KeyboardState0Active = !m_KeyboardState0Active;

	return getKeyboardResult > 0;
}

void crupt::InputManager::UpdateGamepadStates()
{
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (!m_ConnectedGamepads[i])
			return;

		m_OldGamepadState[i] = m_CurrGamepadState[i];

		const DWORD dwResult = XInputGetState(i, &m_CurrGamepadState[i]);
		m_ConnectedGamepads[i] = (dwResult == ERROR_SUCCESS);
	}
}

void crupt::InputManager::AddCommand(const std::string& eventName, ICommand* pCommand)
{
	m_CommandMap[eventName] = pCommand;
}

bool crupt::InputManager::FindBinding(const std::string& key) const
{
	std::unordered_map<std::string, Binding>::const_iterator it = m_BindingMap.find(key);
	if(it != m_BindingMap.end())
	{
		return true;
	}
	return false;
}
bool crupt::InputManager::FindCommand(const std::string& key) const
{
	std::unordered_map<std::string, ICommand*>::const_iterator it = m_CommandMap.find(key);
	if(it != m_CommandMap.end())
	{
		return true;
	}
	return false;
}


bool crupt::InputManager::ProcessInput()
{
	UpdateKeyboardStates();
	UpdateGamepadStates();

	//Handling all the commands, check if theres an input.
	std::unordered_map<std::string, ICommand*>::iterator it;
	for(it = m_CommandMap.begin(); it != m_CommandMap.end(); ++it)
	{
		HandleCommand(it->first);
	}

	//Running the handling of the command.
	m_pInvoker->Update();

	return ProcessQuit();
	
}

void crupt::InputManager::HandleCommand(const std::string& eventName)
{
	if(IsActivated(eventName))
	{
		m_CommandMap.at(eventName)->Execute();
	}
}

bool crupt::InputManager::IsActivated(const std::string& eventName)
{
	switch(m_BindingMap.at(eventName).TriggerState)
	{
		case InputTriggerState::Pressed:
			return IsPressed(m_BindingMap.at(eventName));
			break;

		case InputTriggerState::Released:
			return IsReleased(m_BindingMap.at(eventName));
			break;

		case InputTriggerState::Down:
			return IsDown(m_BindingMap.at(eventName));
			break;
	}
	return false;
}

bool crupt::InputManager::IsPressed(Binding button) const
{
	UINT pi = static_cast<UINT>(button.PlayerIndex);
	if (m_ConnectedGamepads[pi])
	{
		DWORD xButton{DWORD(button.GamePadButton)};

		if ((m_CurrGamepadState->Gamepad.wButtons & xButton) != 0 && (m_OldGamepadState->Gamepad.wButtons & xButton) == 0)
		{
			return true;
		}
	}

	if((m_pCurrKeyboardState[button.KeyboardKey] & 0xF0) != 0 && (m_pOldKeyboardState[button.KeyboardKey] & 0xF0) == 0)
	{
		return true;
	}

	return false;
}

bool crupt::InputManager::IsDown(Binding button) const
{
	UINT pi = static_cast<UINT>(button.PlayerIndex);
	if (m_ConnectedGamepads[pi])
	{
		DWORD xButton{DWORD(button.GamePadButton)};

		if ((m_CurrGamepadState->Gamepad.wButtons & xButton) != 0 && (m_OldGamepadState->Gamepad.wButtons & xButton) != 0)
		{
			return true;
		}
	}

	if((m_pCurrKeyboardState[button.KeyboardKey] & 0xF0) != 0 && (m_pOldKeyboardState[button.KeyboardKey] & 0xF0) != 0)
	{
		return true;
	}

	return false;
}

bool crupt::InputManager::IsReleased(Binding button) const
{
	UINT pi = static_cast<UINT>(button.PlayerIndex);
	if (m_ConnectedGamepads[pi])
	{
		DWORD xButton{DWORD(button.GamePadButton)};

		if ((m_CurrGamepadState->Gamepad.wButtons & xButton) == 0 && (m_OldGamepadState->Gamepad.wButtons & xButton) != 0)
		{
			return true;
		}
	}

	if((m_pCurrKeyboardState[button.KeyboardKey] & 0xF0) == 0 && (m_pOldKeyboardState[button.KeyboardKey] & 0xF0) != 0)
	{
		return true;
	}

	return false;
}


bool crupt::InputManager::ProcessQuit()
{
	/*ImGuiIO& io = ImGui::GetIO();
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	int wheel = 0;
	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);*/

	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		//ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_QUIT)
		{
			return false;
		}
	}

	return true;
}
