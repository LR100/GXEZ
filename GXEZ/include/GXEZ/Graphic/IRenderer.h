#ifndef IRENDERER_H_
#define IRENDERER_H_

#include "GXEZ/Graphic/IDrawer2D.h"
#include "GXEZ/Graphic/IWindow.h"

namespace GXEZ
{
	class IRenderer : public IDrawer2D
	{
	public:
		IRenderer(IWindow* window) {};
		virtual ~IRenderer() {};

		virtual void LinkWindow(IWindow* window) = 0;
		virtual void UseWindow(IWindow* window) = 0;
		
		virtual void PrepareScene() = 0;
		virtual void PresentScene() = 0;
	};

}

#endif // !IRENDERER_H_

//#ifndef IRENDERER_H_
//#define IRENDERER_H_
//#endif // !IRENDERER_H_