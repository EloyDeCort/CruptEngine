#include "CruptEnginePCH.h"
#include "InputManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "ImGui/imgui_impl_sdl.h"
#include "Invoker.h"
#include "ICommand.h"
#include <SDL.h>


void crupt::InputManager::Init()
{
	m_pInvoker = &Invoker::GetInstance();
}

crupt::InputManager::~InputManager()
{
	for(std::pair<std::string, ICommand*> command : m_CommandMap)
	{
		delete command.second;
	}

	m_BindingMap.clear();
	m_CommandMap.clear();
}

void crupt::InputManager::AddBinding(const std::string& eventName, Binding newBinding)
{
	m_BindingMap[eventName] = newBinding;
}

void crupt::InputManager::Rebind(const std::string& eventName, Binding newBinding)
{
	m_BindingMap[eventName] = newBinding;
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
	for(int i{}; i < 256; ++i)
	{
		m_PreviousKBState[i] = m_CurrentKBState[i];
	}

	m_PreviousState = m_CurrentState;

	int playerID{-1};
	//Clear/Zero out the memory on the state location
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	for(DWORD i{}; i < XUSER_MAX_COUNT && playerID == -1; ++i)
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		if(XInputGetState(i, &m_CurrentState) == ERROR_SUCCESS)
			playerID = i;
	}

	for(int i = 0; i < 256; i++) 
	{
        m_CurrentKBState[i] = GetAsyncKeyState(i);
    }


	std::unordered_map<std::string, ICommand*>::iterator it;
	for(it = m_CommandMap.begin(); it != m_CommandMap.end(); ++it)
	{
		HandleCommand(it->first);
	}

	m_pInvoker->Update();

	return ProcessQuit();

	//if(playerID != -1)
	//{
	//	//std::cout << "Controller Found!" << std::endl;
	//	return true;
	//}
	//else
	//{
	//	//std::cout << "No Controller Found!" << std::endl;
	//	return false;
	//}
	
}

void crupt::InputManager::HandleCommand(const std::string& eventName)
{
	if(IsActivated(eventName))
	{
		m_CommandMap[eventName]->Execute();
	}
}

bool crupt::InputManager::IsActivated(const std::string& eventName)
{
	return IsPressed(m_BindingMap[eventName]);
}

bool crupt::InputManager::IsPressed(Binding button) const
{
	// todo: return whether the given button is pressed or not.
	DWORD xButton{DWORD(button.GamePadButton)};

	if ((m_CurrentState.Gamepad.wButtons & xButton) != 0 && (m_PreviousState.Gamepad.wButtons & xButton) == 0)
	{
		return true;
	}


	if((m_CurrentKBState[button.KeyboardKey] != 0) && (m_PreviousKBState[button.KeyboardKey] == 0))
	{
		return true;
	}

	return false;
}


bool crupt::InputManager::ProcessQuit()
{
	ImGuiIO& io = ImGui::GetIO();
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
	int wheel = 0;
	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

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
