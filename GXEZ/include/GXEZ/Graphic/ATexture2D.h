#pragma once

// GXEZ
#include "IDrawer2D.h"

// MXEZ
#include "MXEZ/Vec/Vec.h"
using namespace MXEZ;

// STD
#include <string>

namespace GXEZ
{
	class IRenderer;

	class ATexture2D : public IDrawer2D
	{
	public:

		struct Definition
		{
			Definition();
			Definition(const std::string& _file);
			Definition(const Vec2i& _size);

			Vec2i			size;
			std::string		file;
		};
		
		ATexture2D(const Definition& definition, IRenderer* renderer);
		virtual	~ATexture2D();

		// Must Be Called before any draw call
		virtual void		UseAsRenderTarget();

		// Inherited via IDrawer2D
		virtual void		DrawPoint(const Vec2i& pos, const Color& color) override;
		virtual void		DrawLine(const Vec2i& a, const Vec2i& b, const Color& color) override;
		virtual void		DrawRectBorder(const Vec2i& pos, const RectBorder& borderdef) override;
		virtual void		DrawRect(const Vec2i& pos, const Rect& rect) override;
		virtual void		DrawCircle(const Vec2i& pos, const Circle& circle) override;
		virtual void		DrawText(const Vec2i& pos, const Text& text) override;

		const Definition&	GetDefinition() const;
		const Vec2i&		GetCenter() const;
		void				SetCenter(const Vec2i& center);
		
	protected:

		Definition				_definition;
		Vec2i					_center;

	private:

		IRenderer*				_renderer;
	};
}
