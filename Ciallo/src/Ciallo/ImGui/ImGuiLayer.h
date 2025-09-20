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
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);

		float m_time;
	};


}