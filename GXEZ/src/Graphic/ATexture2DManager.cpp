#include "GXEZ/Graphic/ATexture2DManager.h"

// GXEZ
#include "GXEZ/Graphic/IRenderer.h"

namespace GXEZ
{
	ATexture2DManager::ATexture2DManager(IRenderer* renderer)
	{
		_renderer = renderer;
	}

	ATexture2DManager::~ATexture2DManager()
	{
	}

	IRenderer* ATexture2DManager::GetRenderer() const
	{
		return (_renderer);
	}
}

