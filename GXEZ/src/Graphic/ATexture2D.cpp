#include "GXEZ/Graphic/ATexture2D.h"

// Texture Manager
#include "GXEZ/Graphic/ATexture2DManager.h"
// Renderer
#include "GXEZ/Graphic/IRenderer.h"

namespace GXEZ
{

	ATexture2D::ATexture2D(const Definition& definition, ATexture2DManager* manager) : _definition(definition)
	{

		if (manager == NULL) {
			throw ("ATexture2D::ATexture2D : ERROR : ATextureManager cannot be NULL");
		}
		_manager = manager;
		_manager->RegisterTexture2D(this);
	}

	ATexture2D::~ATexture2D()
	{
		_manager->UnRegisterTexture2D(this);
	}

	void ATexture2D::UseAsRenderTarget(const bool& state)
	{
		if (state)
		{
			_manager->GetRenderer()->SetRenderTarget(this);
		}
		else
		{
			// Do Nothing
		}
	}

	void ATexture2D::Save()
	{
	}

	void ATexture2D::Restore()
	{
	}

	void ATexture2D::DrawPoint(const Vec2i& pos, const Color& color)
	{
		_manager->GetRenderer()->DrawPoint(pos, color);
	}

	void ATexture2D::DrawLine(const Vec2i& a, const Vec2i& b, const Color& color)
	{
		_manager->GetRenderer()->DrawLine(a, b, color);
	}

	void ATexture2D::DrawRectBorder(const Vec2i& pos, const IDrawer2D::RectBorder& borderdef)
	{
		_manager->GetRenderer()->DrawRectBorder(pos, borderdef);
	}

	void ATexture2D::DrawRect(const Vec2i& pos, const IDrawer2D::Rect& rect)
	{
		_manager->GetRenderer()->DrawRect(pos, rect);
	}

	void ATexture2D::DrawCircle(const Vec2i& pos, const IDrawer2D::Circle& circle)
	{
		_manager->GetRenderer()->DrawCircle(pos, circle);
	}

	void ATexture2D::DrawText(const Vec2i& pos, const IDrawer2D::Text& text)
	{
		_manager->GetRenderer()->DrawText(pos, text);
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

	ATexture2D::Definition::Definition(const std::string& _file)
	{
		file = _file;
	}

	ATexture2D::Definition::Definition(const Vec2i& _size)
	{
		size = _size;
	}

}
