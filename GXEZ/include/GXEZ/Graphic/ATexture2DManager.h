#pragma once

// GXEZ
#include "GXEZ/Graphic/ATexture2D.h"

namespace GXEZ
{
	class IRenderer;

	class ATexture2DManager
	{
	public:
		ATexture2DManager(IRenderer* renderer);
		~ATexture2DManager();

		virtual ATexture2D* CreateTexture2D(const ATexture2D::Definition& definition) = 0;
		virtual void		RemoveTexture2D(ATexture2D* texture) = 0;
		virtual void		RegisterTexture2D(ATexture2D* texture) = 0;
		virtual void		UnRegisterTexture2D(ATexture2D* texture) = 0;

		// Renderer
		IRenderer*			GetRenderer() const;

	private:
		IRenderer*			_renderer;
	};

}