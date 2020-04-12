#pragma once
#include <XInput.h>
#include "Singleton.h"
#include "ECSTypes.h"
namespace crupt
{
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
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
		void SetPlayer(Entity entity);
	private:
		XINPUT_STATE m_CurrentState{};
		Entity m_pPlayer;
		Invoker* m_pInvoker;
	};

}
