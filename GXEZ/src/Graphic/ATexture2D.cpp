#include "GXEZ/Graphic/ATexture2D.h"

#include "GXEZ/Graphic/IRenderer.h"

namespace GXEZ
{
	ATexture2D::ATexture2D(const Definition& definition, IRenderer* renderer) : _definition(definition)
	{
		if (renderer == NULL) {
			throw ("ATexture2D::ATexture2D : ERROR : IRenderer cannot be NULL");
		}
		_renderer = renderer;
		_renderer->CreateTexture2D(_definition, this);
		// Must Create Basics Now
	}

	ATexture2D::~ATexture2D()
	{
	}

	inline void ATexture2D::UseAsRenderTarget()
	{
		_renderer->SetRenderTarget(this);
	}

	inline void ATexture2D::DrawPoint(const Vec2i& pos, const Color& color)
	{
		_renderer->DrawPoint(pos, color);
	}

	inline void ATexture2D::DrawLine(const Vec2i& a, const Vec2i& b, const Color& color)
	{
		_renderer->DrawLine(a, b, color);
	}

	inline void ATexture2D::DrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& borderdef)
	{
		_renderer->DrawRectBorder(pos, borderdef);
	}

	inline void ATexture2D::DrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect)
	{
		_renderer->DrawRect(pos, rect);
	}

	inline void ATexture2D::DrawCircle(const Vec2i& pos, const IDrawer2D::Circle& circle)
	{
		_renderer->DrawCircle(pos, circle);
	}

	inline void ATexture2D::DrawText(const Vec2i& pos, const IDrawer2D::Text& text)
	{
		_renderer->DrawText(pos, text);
	}

	const ATexture2D::Definition& ATexture2D::GetDefinition() const
	{
		return (_definition);
	}

	const Vec2i& ATexture2D::GetCenter() const
	{
		return (_center);
	}

	void ATexture2D::SetCenter(const Vec2i& center)
	{
		_center = center;
	}

	ATexture2D::Definition::Definition()
	{
	}

	ATexture2D::Definition::Definition(const std::string& _file)
	{
		file = _file;
	}

	ATexture2D::Definition::Definition(const Vec2i& _size)
	{
		size = _size;
	}

}
