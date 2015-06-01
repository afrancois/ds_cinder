#pragma once
#ifndef DS_UI_SCROLL_SCROLL_LIST
#define DS_UI_SCROLL_SCROLL_LIST

#include <ds/ui/sprite/sprite.h>

namespace ds{
namespace ui{
class ScrollArea;



	/*
	* ScrollList is an advanced Scroll Area that handles an arbitrarily large set of items.
	* ScrollList handles keeping a cache of placeholders and assigning sprites to onscreen items.
	* This assumes that you can refer to your content by integers only, so you may have to keep a map in your super class.
	*
	*/

	class ScrollList : public ds::ui::Sprite {
	public:
		ScrollList(ds::ui::SpriteEngine& engine, const bool verticalScroll = true);

		void						setContent(const std::vector<int>& dbIds);

		// OPTIONAL (but highly recommended): A row has been tapped, set this function to handle it
		void						setItemTappedCallback(const std::function<void(ds::ui::Sprite* bs, const ci::Vec3f& cent)> &func);

		// REQUIRED: When we need to create a new sprite, respond with a new sprite of your custom type
		void						setCreateItemCallback(const std::function<ds::ui::Sprite*() > &func);

		// REQUIRED: When a sprite needs data assigned (coming onscreen for the first time for example). May need to cast the sprite to your custom type
		void						setDataCallback(const std::function<void(ds::ui::Sprite*, const int dbId) > &func);

		// OPTIONAL: During animate on, you can call custom animation code here with delay (set member properties for delay if desired, defaults=0.0 seconds)
		void						setAnimateOnCallback(const std::function<void(ds::ui::Sprite*, const float delay)>&func);

		// OPTIONAL: If you want to show highlighted states you can react here
		void						setStateChangeCallback(const std::function<void(ds::ui::Sprite*, const bool highlighted)>&func);

		void						animateItemsOn();

	protected:

		// We only create enough sprites that are onscreen at one time.
		// Here's how this crap works (roughly in order):
		//		- Set Data: clear out old data, and build a list of placeholders. These are just the data (product), a position and a pointer to a sprite
		//		- layoutItems: place the placeholders on a virtual grid.
		//		- assignItems: figure out which placeholders should be onscreen and associate sprites with them. New sprites can be created here if needeed.
		//						extra sprites are kept in mReserveI tems.

		// A helper so we only have to show the visible results at one time (instead of creating a zillion sprites)
		struct ItemPlaceHolder	{

			ItemPlaceHolder(const int dbId, float x = 0.0f, float y = 0.0f, ds::ui::Sprite *associatedSprite = nullptr)
				: mDbId(dbId)
				, mX(x)
				, mY(y)
				, mAssociatedSprite(associatedSprite)
			{
			};

			int							mDbId;
			float						mX;
			float						mY;
			ds::ui::Sprite*				mAssociatedSprite;
		};


		ds::ui::ScrollArea*					getScrollArea(){ return mScrollArea; }
		virtual void						onSizeChanged();
		virtual void						layout();
		virtual void						layoutItems();

		virtual void						clearItems();
		virtual void						assignItems();

		void								handleItemTouchInfo(ds::ui::Sprite* bs, const TouchInfo& ti);

		std::vector<ItemPlaceHolder>		mItemPlaceHolders;
		std::vector<ds::ui::Sprite*>		mReserveItems;

		ds::ui::ScrollArea*					mScrollArea;
		ds::ui::Sprite*						mScrollableHolder;
		const bool							mVerticalScrolling;

		// for laying out items
		float								mStartPositionY;
		float								mStartPositionX;
		float								mIncrementAmount;

		// for animate on
		float								mAnimateOnDeltaDelay;
		float								mAnimateOnStartDelay;

		std::function<void(ds::ui::Sprite*, const ci::Vec3f& cent)>	mItemTappedCallback;
		std::function<ds::ui::Sprite* ()>							mCreateItemCallback;
		std::function<void(ds::ui::Sprite*, const int dbId)>		mSetDataCallback;
		std::function<void(ds::ui::Sprite*, const float delay)>	mAnimateOnCallback;
		std::function<void(ds::ui::Sprite*, const bool highli)>	mStateChangeCallback;
	};
}
}

#endif