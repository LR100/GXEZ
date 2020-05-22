#ifndef IRENDERER_H_
#define IRENDERER_H_

#include "GXEZ/Graphic/IDrawer2D.h"
#include "GXEZ/Graphic/IWindow.h"
#include "GXEZ/Graphic/ATexture2D.h"

namespace GXEZ
{
	class IRenderer : public IDrawer2D
	{
	public:
		IRenderer(IWindow* window) {};
		virtual ~IRenderer() {};

		virtual void LinkWindow(IWindow* window) = 0;

		// Renders targets can be or a window or a texture

		virtual void		SetRenderTarget(IWindow* window) = 0;
		virtual void		SetRenderTarget(ATexture2D* texture) = 0;

		// Textures
		virtual ATexture2D* CreateTexture2D(const ATexture2D::Definition& definition, ATexture2D* texture = NULL) = 0;

		virtual void		DrawTexture(const Vec2i& pos, ATexture2D* texture, float rotation = 0) = 0;

		/// Scenes
		virtual void		PrepareScene() = 0;
		virtual void		PresentScene() = 0;
	};
}

#endif // !IRENDERER_H_

//#ifndef IRENDERER_H_
//#define IRENDERER_H_
//#endif // !IRENDERER_H_