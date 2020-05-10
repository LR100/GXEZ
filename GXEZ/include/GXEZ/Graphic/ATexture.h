#pragma once

#include "MXEZ/Vec/Vec.h"

using namespace MXEZ;

namespace GXEZ
{
	class IRenderer;

	class ATexture
	{
	public:

		struct Definition
		{
			Definition();
			const Vec2i& size;
		};
		
		ATexture(const Definition& definition, IRenderer* renderer);

	protected:

		virtual void			SetRenderer(IRenderer* renderer);

	private:

		const Definition		_definition;
	};
}
