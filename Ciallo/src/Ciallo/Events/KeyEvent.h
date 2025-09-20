#pragma once

#include "clpch.h"
#include "Event.h"

namespace Ciallo 
{

	class CIALLO_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategorykeyboard| EventCategoryInput)
	protected:
		KeyEvent(int keycode): m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class CIALLO_API KeyTypedEvent :public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			:KeyEvent(keycode) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_KeyCode ;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	class CIALLO_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode,int repeatCount)
            		:KeyEvent(keycode),m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
            std::stringstream ss;
			ss<<"KeyPressedEvent:"<<m_KeyCode<<"("<<m_RepeatCount<<")";
		    return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class CIALLO_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode){}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_KeyCode ;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}