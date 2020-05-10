#include "GXEZ/Graphic/ATexture.h"

#include "GXEZ/Graphic/IRenderer.h"

namespace GXEZ
{
	ATexture::ATexture(const Definition& definition, IRenderer* renderer) : _definition(definition)
	{
		renderer->CreateTexture(_definition, this);
		SetRenderer(renderer);
	}

	void ATexture::SetRenderer(IRenderer* renderer)
	{
		throw ("ATexture::SetRenderer() not implemented");
	}
}
