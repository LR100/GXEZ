#pragma once

// GXEZ
#include "GXEZ/Event/IEventHandler.h"
#include "GXEZ/Graphic/IRenderer.h"
#include "GXEZ/GUI/GUIItem.h"

// STD
#include <unordered_map>
#include <vector>
#include <list>

// MXEZ
#include "MXEZ/AABB/QuadTree.h"

namespace GXEZ
{
	class GUIManager
	{
	public:
		GUIManager(const unsigned int& width, const unsigned int& height);

		// Basics
		void			Resize(const unsigned int& width, const unsigned int& height);
		void			Draw();

		// Link/Init
		void			LinkEventHandler(IEventHandler* eventHandler);
		void			LinkRenderer(IRenderer* renderer);
		unsigned int	LinkItem(GUIItem* item);

		// Items
		GUIItem* GetItemHovered() const;
		GUIButton* CreateButton(const GUIButton::Definition& definition, GUIItem* parent = NULL);

	private:

		class GUIScreen : public GUIItem
		{
		public:
			GUIScreen(GUIManager* manager, IRenderer* renderer, const GUIItem::Definition& definition);
		private:
			virtual void	OnDraw(IRenderer* renderer) override;
			virtual void	CreateTexture() override;
		};
		 
		// Event Handling //
		void			EventMouseMoved();
		void			EventMouseLeftPress();
		void			EventMouseLeftRelease();

		// Event Trigger
		void			UpdateItemsHoveredByMouse();
		void			InsertItemsClickedByMouse();
		void			RemoveItemsClickedByMouse();


		// Items
		std::vector<GUIItem*>						_items;
		std::unordered_map<unsigned int, GUIItem*>	_itemsByID;
		std::vector<unsigned int>					_itemsAvailablesIDs;
		std::list<GUIItem*>							_itemsHovered;
		std::vector<GUIItem*>						_itemsClicked;
		GUIItem* _itemHovered; // Top Foreground
		// Root Item
		GUIItem* _itemScreen;
		GUIItem::Definition _screenDef;

		QuadTreeAABB2T<int, GUIItem*>* _tree;

		// Basics
		unsigned int			_width, _height;
		IEventHandler*			_eventHandler;
		IRenderer*				_renderer;
		//IDrawer2D*	 _drawer2D;
	};

}