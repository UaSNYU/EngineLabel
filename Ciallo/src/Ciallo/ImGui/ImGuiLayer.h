#pragma once

#include "Ciallo//Layer.h"

#include "Ciallo/Events/KeyEvent.h"
#include "Ciallo/Events/MouseEvent.h"
#include "Ciallo/Events/ApplicationEvent.h"

namespace Ciallo
{
	class CIALLO_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnDetach() override;
		void OnAttach() override;
		void OnImGuiRender() override;
		void Begin();
		void End();
	private:
		float m_time=0.0f;
	};


}