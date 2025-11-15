#include <Ciallo.h>
#include "CialloEditor2D.h"
#include "Ciallo/Core/EntryPoint.h"

namespace Ciallo
{
	class CialloEditor : public Application
	{
	public:
		CialloEditor()
			:Application("Ciallo Editor")
		{
			pushLayer(new CialloEditor2D());
			//pushLayer(new GameLayer());
		}

		~CialloEditor() {}

	};

	Application* CreateApplication()
	{
		return new CialloEditor();
	}
}

