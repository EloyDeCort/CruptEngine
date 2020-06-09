//General Design And Idea: Matthieu Delaere
//Adjusted to work with command pattern.
#pragma once
#include <XInput.h>
#include <unordered_map>
#include "Singleton.h"
#include "ECSTypes.h"


namespace crupt
{
	enum class GamepadIndex : DWORD
	{
		PlayerOne = 0,
		PlayerTwo = 1,
		PlayerThree = 2,
		PlayerFour = 3
	};	

	enum class ControllerButton
	{
		//Assign the actual inputs to the enum
		ButtonDPADUp = XINPUT_GAMEPAD_DPAD_UP,       
		ButtonDPADDown = XINPUT_GAMEPAD_DPAD_DOWN,     
		ButtonDPADLeft = XINPUT_GAMEPAD_DPAD_LEFT,   
		ButtonDPADRight = XINPUT_GAMEPAD_DPAD_RIGHT,    
		ButtonStart = XINPUT_GAMEPAD_START,         
		ButtonBack = XINPUT_GAMEPAD_BACK,   
		ButtonLeftThumb = XINPUT_GAMEPAD_LEFT_THUMB, 
		ButtonRightThumb = XINPUT_GAMEPAD_RIGHT_THUMB, 
		ButtonLeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER, 
		ButtonRightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y
	};

	struct Binding
	{
		ControllerButton GamePadButton;
		int KeyboardKey;
	};

	class ICommand;
	class Invoker;

	class InputManager : public Singleton<InputManager>
	{
	public:
		InputManager() = default;
		virtual ~InputManager();


		void Init();
		bool ProcessInput();
		bool ProcessQuit();
		void AddCommand(const std::string& eventName, ICommand* pCommand);
		void HandleCommand(const std::string& eventName);
		bool IsActivated(const std::string& eventName);
		bool IsPressed(Binding button) const;
		bool FindBinding(const std::string& key) const;
		bool FindCommand(const std::string& key) const;

		void AddBinding(const std::string& eventName, Binding newBinding);
		void Rebind(const std::string& eventName, Binding newBinding);

	private:
		Invoker* m_pInvoker;
		Entity m_pPlayer;


		XINPUT_STATE m_CurrentState;
		XINPUT_STATE m_PreviousState;
		
		XINPUT_GAMEPAD m_Gamepads;
		SHORT m_CurrentKBState[256];
		SHORT m_PreviousKBState[256];

		std::unordered_map<std::string, Binding> m_BindingMap;
		std::unordered_map<std::string, ICommand*> m_CommandMap;
	};

}
