#pragma once

// STD
#include <future> // for handlers (like onClick, ....)
#include <functional>
#include <optional>
#include <stdint.h> 
#include <vector>


// MXEZ
#include "MXEZ/AABB/AABB.h"
#include "MXEZ/Vec/Vec.h"

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

				std::optional<float>				width;
				std::optional<float>				height;
				std::optional<GUIItem::UnitType>	type;
			};

			struct Border
			{
				Border()
				{
					sizeType = UnitType::ABSOLUTE_PX;
				}

			//private:
				std::optional<Color>				color;
				std::optional<float>				size;
				std::optional<GUIItem::UnitType>	sizeType;
				std::optional<float>				radius;
			};

			struct Position
			{
				Position()
				{
					x = 0;
					y = 0;
					type = UnitType::ABSOLUTE_PX;
				}
			//private:
				std::optional<float>				x;
				std::optional<float>				y;
				std::optional<GUIItem::UnitType>	type;
			};

			struct Text
			{
				Text()
				{

				}
			//private:
				std::optional<std::string>		content;
				std::optional<std::string>		fontFamily;
				std::optional<int>				fontSize;
			};

			struct Background
			{
				std::optional<Color>			color;
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

			// Load Default Needed values from parameters() ...
			virtual void	LoadDefault();

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

		// Modifications //
		void				Resize(const Definition::Size& size);


		// Draw Textures Into The Render Target
		void				Draw(IRenderer* renderer);
		void				DrawTree(IRenderer* renderer);

		// Properties Getters
		const unsigned int& GetID() const;
		const ItemType&		GetType() const;
		const AABB2i&		GetAABB() const;
		const Definition&	GetDefinition() const;

		// Tree Methods
		void				SetParent(GUIItem* item);
		GUIItem*			GetParent() const;
		void				AddChild(GUIItem* item);
		void				RemoveChildren(GUIItem* item);

		////////////////////////
		// Graphic Properties // 
		////////////////////////
		void				SetHovered(bool state);
		const bool&			IsHovered() const;

		void				SetSelected (bool state);
		const bool&			IsSelected() const;

		void				SetClicked(bool state);
		const bool&			IsClicked() const;

		void				SetVisible(bool state);
		const bool&			IsVisible() const;

		void				SetZIndex(const int& zindex);
		const int&			GetZIndex() const;

	protected:

		void				SetDefinition(const GUIItem::Definition& definition);

		// OVERRIDABLE //
		virtual void		OnDraw(IRenderer* renderer) = 0;
		virtual void		OnClick(bool state) {};
		virtual void		OnHovered(bool state) {};
		virtual void		OnVisible(bool state) {};
		

		virtual void		CreateTexture() = 0;
		void				ClearTextures();

		const Vec2i&		GetRealPosition() const;
		const Vec2i&		GetRealSize() const;

		// Drawing/Design
		GUIItem::Definition			_definition;
		ATexture2D*					_texture;
		IRenderer*					_renderer;



	private:

		virtual void			OnResizeParent();


		// Events
		void					OnChanged();

		// Compute Changes
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
		Vec2i					_realPosition; // Can Be Negative
		Vec2i					_realSize;
		AABB2i					_aabb;

		// Drawing/Design Properties
		bool					_isSelected;
		bool					_isHovered;
		bool					_isClicked;
		bool					_isVisible;
	};

	class GUIButton : public GUIItem
	{
	public:

		enum ModificatorID
		{
			GUIBUTTONMODIFICATOR_NONE_ID = 0,
			GUIBUTTONMODIFICATOR_HOVERED_ID = 1,
			GUIBUTTONMODIFICATOR_CLICKED_ID = 2,
			GUIBUTTONMODIFICATOR_SELECTED_ID = 3,
		};

		struct DefinitionModificator
		{
			GUIItem::Definition::Border			border;
			GUIItem::Definition::Background		background;
		};

		struct Definition : public GUIItem::Definition
		{
			Definition();

			virtual void			LoadDefault() override;

			DefinitionModificator	hovered;
			DefinitionModificator	clicked;
			DefinitionModificator	selected;

		private:

			void					LoadDefaultModificator(GUIButton::DefinitionModificator& modificator, const GUIButton::ModificatorID& id);
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
		virtual void	OnResize() {};


		// Draw
		virtual void	CreateTexture() override;
		ATexture2D*		CreateTextureBase(const GUIItem::Definition::Border& border, const GUIItem::Definition::Background& background);
		void			CreateTextureNormal();
		void			CreateTextureHovered();
		void			CreateTextureClicked();
		void			CreateTextureSelected();

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
		virtual void	CreateTexture() override;
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
	//	virtual void	CreateTexture(IImageDrawer2D* drawer) override;
	//
	//	GUIImage::Definition		_definition;
	//	GUIImage::Definition		_definitionHovered;
	//	// Texture 0 is Classic // Texture 1 is Hovered
	//	bool		_isHovered;
	//};

}

