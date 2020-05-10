#ifndef IRENDERER_H_
#define IRENDERER_H_

#include "GXEZ/Graphic/IDrawer2D.h"
#include "GXEZ/Graphic/IWindow.h"
#include "GXEZ/Graphic/ATexture.h"

namespace GXEZ
{
	class IRenderer : public IDrawer2D
	{
	public:
		IRenderer(IWindow* window) {};
		virtual ~IRenderer() {};

		virtual void LinkWindow(IWindow* window) = 0;

		// Renders targets can be or a window or a texture

		virtual void SetRenderTarget(IWindow* window) = 0;
		virtual void SetRenderTarget(ATexture* texture) = 0;

		// Textures
		virtual ATexture* CreateTexture(const ATexture::Definition& definition, ATexture* texture = NULL) = 0;

		/// Scenes
		virtual void PrepareScene() = 0;
		virtual void PresentScene() = 0;
	};

}

#endif // !IRENDERER_H_

//#ifndef IRENDERER_H_
//#define IRENDERER_H_
//#endif // !IRENDERER_H_