#pragma once

// STD
#include <future> // for handlers (like onClick, ....)
#include <functional>
#include <stdint.h> 
#include <vector>

// MXEZ
#include "MXEZ/AABB/AABB.h"

using namespace MXEZ;

// GXEZ
#include "GXEZ/Graphic/IRenderer.h"
#include "GXEZ/Graphic/Color.h"

///////////////////
// FULL RELATIVE //

namespace GXEZ
{

	class GUIManager;

	class GUIItem
	{
	public:

		struct Handler
		{
			Handler(std::function<void()> f);
			std::function<void()>				_function;
		};

		enum ItemType
		{
			NONE,
			BUTTON,
			IMAGE,
			CANVAS,
		};

		// Unit Types for each Sizes (px, cm, %...)
		enum class UnitType : uint8_t {
			ABSOLUTE_PX,
			ABSOLUTE_CM,
			RELATIVE_TO_PARENT,
		};

		// Basic Definition common to every items
		struct Definition
		{
			struct Size
			{
				Size()
				{
					width = 0;
					height = 0;
					type = UnitType::ABSOLUTE_PX;
				}
				float				width;
				float				height;
				GUIItem::UnitType	type;
			};

			struct Border
			{
				Border()
				{
					radius = 0;
					size = 0;
					sizeType = UnitType::ABSOLUTE_PX;
				}
				Color					color;
				float					size;
				GUIItem::UnitType		sizeType;
				float					radius;
			};

			struct Position
			{
				Position()
				{
					x = 0;
					y = 0;
					type = UnitType::ABSOLUTE_PX;
				}
				float				x;
				float				y;
				GUIItem::UnitType	type;
			};

			struct Text
			{
				std::string			content;
				std::string			fontFamily;
				int					fontSize;
			};

			struct Background
			{
				Color		color;
				// Should be able to load itself an image from a file via a specific method
			};

			Definition() {

				size.width = 1;
				size.height = 1;
				size.type = GUIItem::UnitType::ABSOLUTE_PX;
				position.x = 0;
				position.y = 0;
				position.type = GUIItem::UnitType::ABSOLUTE_PX;
				zIndex = 0;
			}

			// Text
			Text		text;
			// Size
			Size		size;
			// Position
			Position	position;
			// Z-Index
			int			zIndex;
			// Border
			Border		border;
			// BackGround
			Background	background;
		};

		GUIItem(GUIManager* manager, GUIItem* parent, IRenderer* renderer, ItemType type, const GUIItem::Definition& definition);

		// Draw Textures Into The Drawer
		void				Draw(IRenderer* renderer);
		void				DrawTree(IRenderer* renderer);

		const unsigned int& GetID() const;
		const ItemType& GetType() const;
		const AABB2i& GetAABB() const;


		const Definition& GetDefinition() const;

		void				SetParent(GUIItem* item);
		GUIItem* GetParent() const;
		void				AddChild(GUIItem* item);
		void				RemoveChildren(GUIItem* item);




		////////////////
		// Properties // // Manually Get/Set Properties
		////////////////
		void				SetHovered(bool state);
		const bool&			IsHovered() const;

		void				SetClicked(bool state);
		const bool&			IsClicked() const;

		void				SetVisibile(bool state);
		const bool&			IsVisibile() const;

		void				SetZIndex(const int& zindex);
		const int&			GetZIndex() const;

	protected:

		void				SetDefinition(const GUIItem::Definition& definition);

		// OVERRIDABLE //
		virtual void		OnDraw(IRenderer* renderer) = 0;
		virtual void		OnClick(bool state) {};
		virtual void		OnHovered(bool state) {};
		virtual void		OnVisible(bool state) {};

		virtual void		CreateTextures() = 0;
		void				ClearTextures();

		const int& GetRealPositionX() const;
		const int& GetRealPositionY() const;

		const int& GetRealSizeWidth() const;
		const int& GetRealSizeHeight() const;

		// Drawing/Design
		GUIItem::Definition		_definition;
		std::vector<ATexture*>	_textures;
		IRenderer*				_renderer;



	private:

		void					ComputeRealPosition();
		void					ComputeRealSize();
		void					ComputeAABB();

		// The ID
		unsigned int			_id;
		// Basics
		GUIManager* _manager;
		ItemType				_type;
		// Tree
		GUIItem* _parent;
		std::vector<GUIItem*>	_children;
		// Real Properties
		int						_realPositionX, _realPositionY; // Can Be Negative
		unsigned int			_realSizeWidth, _realSizeHeight;
		AABB2i					_aabb;

		// Drawing/Design Properties
		bool					_isHovered;
		bool					_isClicked;
		bool					_isVisible;
	};

	class GUIButton : public GUIItem
	{
	public:

		struct DefinitionHovered
		{
			GUIItem::Definition::Border			border;
			GUIItem::Definition::Background		background;
		};

		struct Definition : public GUIItem::Definition
		{
			Definition()
			{
				size.width = 60;
				size.height = 20;

				background.color = Color(200, 200, 200);
				border.size = 1;
				border.radius = 10;
				border.color = Color(170, 170, 170);

				// Hovered 
				hovered.background.color = Color(215, 215, 240);
				hovered.border.size = 1;
				hovered.border.radius = border.radius;
				hovered.border.color = Color(190, 190, 220);
				// Clicked
				clicked.background.color = Color(235, 235, 235);
				clicked.border.size = 1;
				clicked.border.radius = border.radius;
				clicked.border.color = Color(230, 230, 245);
			}
			DefinitionHovered hovered;
			DefinitionHovered clicked;
		};

		GUIButton(GUIManager* manager, GUIItem* parent, IRenderer* renderer, const GUIButton::Definition& definition);


		void			SetDefinitionButton(const GUIButton::Definition& definition);

		template <class Function, class ...Args>
		void			SetOnClickHandler(Function&& f, Args&& ...args)
		{
			if (_onClickHandler) {
				delete (_onClickHandler);
			}
			_onClickHandler = new Handler(std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
		}

	private:

		// Overridable
		virtual void	OnDraw(IRenderer* renderer) override;
		virtual void	OnClick(bool state) override;

		// Draw
		virtual void	CreateTextures() override;
		ATexture*		CreateTextureBase(const GUIItem::Definition::Border& border, const GUIItem::Definition::Background& background);
		void			CreateTextureNormal();
		void			CreateTextureHovered();
		void			CreateTextureClicked();

		GUIButton::Definition		_definitionButton;
		// Texture 0 is Classic - Texture 1 is Hovered

		Handler* _onClickHandler;
	};


	class GUICanvas : public GUIItem
	{
	public:
		GUICanvas(GUIManager* manager, GUIItem* parent, IRenderer* renderer, const GUIItem::Definition& definition);
	private:
		virtual void	OnDraw(IRenderer* renderer) override;
		virtual void	CreateTextures() override;
	};
	//
	//class GUIImage : public GUIItem
	//{
	//public:
	//	struct Definition
	//	{
	//		Color					borderColor;
	//		float					borderSize;
	//		GUIItem::UnitType		borderSizeType;
	//		float					borderRadius;
	//		GUIItem::Definition		itemDefinition;
	//	};
	//
	//	GUIImage(GUIManager* manager, const Definition& definition);
	//
	//	virtual void	Draw(IImageDrawer2D* drawer) override;
	//	void			SetDefinition(const GUIImage::Definition& definition);
	//	void			SetDefinitionHovered(const GUIImage::Definition& definition);
	//	void			SetHovered(bool hovered);
	//
	//private:
	//
	//	virtual void	CreateTextures(IImageDrawer2D* drawer) override;
	//
	//	GUIImage::Definition		_definition;
	//	GUIImage::Definition		_definitionHovered;
	//	// Texture 0 is Classic // Texture 1 is Hovered
	//	bool		_isHovered;
	//};

}

