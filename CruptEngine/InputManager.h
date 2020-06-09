//General Design And Idea: Matthieu Delaere
//Adjusted to work with command pattern.
#pragma once
#include <XInput.h>
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

	enum class InputTriggerState
	{
		Pressed,
		Released,
		Down
	};

	class Invoker;
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
		void SetPlayer(Entity entity);
	private:
		XINPUT_STATE m_CurrentState{};
		Entity m_pPlayer;
		Invoker* m_pInvoker;
	};

}
