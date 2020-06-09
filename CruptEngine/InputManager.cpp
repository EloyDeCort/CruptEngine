#include "CruptEnginePCH.h"
#include "InputManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_sdl.h"
#include "ImGui/imgui_impl_sdl.h"
#include "Invoker.h"
#include "ICommand.h"
#include <SDL.h>

crupt::InputManager::InputManager()
{
	m_pInvoker = &Invoker::GetInstance();
}

crupt::InputManager::~InputManager()
{
}

bool crupt::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);
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
		ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			if(e.key.keysym.sym == SDLK_SPACE)
			{
				m_pInvoker->AddCommand(new JumpCommand(m_pPlayer));
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	m_pInvoker->Update();

	return true;
}

bool crupt::InputManager::IsPressed(ControllerButton button) const
{
	switch (button)
	{
	case ControllerButton::ButtonA:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	case ControllerButton::ButtonB:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	case ControllerButton::ButtonX:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	case ControllerButton::ButtonY:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	default: return false;
	}
}

void crupt::InputManager::SetPlayer(Entity entity)
{
	m_pPlayer = entity;
}

