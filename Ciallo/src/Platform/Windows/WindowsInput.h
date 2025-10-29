#pragma once

#include "Ciallo/Input.h"

namespace Ciallo {

	class WindowsInput : public Input
	{


	protected:
		virtual bool IsKeyPressedImpl(int keycode)override;
		virtual bool IsMouseButtonImpl(int button)override;
		virtual float GetMouseXImpl()override;
		virtual float GetMouseYImpl()override;
		virtual std::pair<float, float> GetMousePosImpl()override;
	};
}