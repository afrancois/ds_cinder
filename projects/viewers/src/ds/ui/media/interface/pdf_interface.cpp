#include "pdf_interface.h"

#include <ds/app/environment.h>
#include <ds/app/engine/engine_cfg.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/ui/sprite/image.h>
#include <ds/debug/logger.h>
#include <ds/util/string_util.h>

#include <ds/ui/sprite/pdf.h>
#include <ds/ui/button/image_button.h>
#include <ds/ui/sprite/text.h>

namespace ds {
namespace ui {

PDFInterface::PDFInterface(ds::ui::SpriteEngine& eng, const ci::Vec2f& sizey, const float buttonHeight, const ci::Color buttonColor, const ci::Color backgroundColor)
	: MediaInterface(eng, sizey, backgroundColor)
	, mLinkedPDF(nullptr)
	, mUpButton(nullptr)
	, mDownButton(nullptr)
	, mPageCounter(nullptr)
	, mTouchToggle(nullptr)
{
	mUpButton = new ds::ui::ImageButton(mEngine, "%APP%/data/images/media_interface/prev.png", "%APP%/data/images/media_interface/prev.png", (sizey.y - buttonHeight) / 2.0f);
	addChildPtr(mUpButton);
	mUpButton->setClickFn([this](){
		if(mLinkedPDF){
			if(mLinkedPDF->getPageCount() > 1){
				mLinkedPDF->goToPreviousPage();
				updateWidgets();
			}
		}
	});

	mUpButton->getNormalImage().setColor(buttonColor);
	mUpButton->getHighImage().setColor(buttonColor / 2.0f);
	mUpButton->setScale(sizey.y / mUpButton->getHeight());

	mDownButton = new ds::ui::ImageButton(mEngine, "%APP%/data/images/media_interface/next.png", "%APP%/data/images/media_interface/next.png", (sizey.y - buttonHeight) / 2.0f);
	addChildPtr(mDownButton);
	mDownButton->setClickFn([this](){
		if(mLinkedPDF){
			if(mLinkedPDF->getPageCount() > 1){
				mLinkedPDF->goToNextPage();
				updateWidgets();
			}
		}
	});

	mDownButton->getNormalImage().setColor(buttonColor);
	mDownButton->getHighImage().setColor(buttonColor / 2.0f);
	mDownButton->setScale(sizey.y / mDownButton->getHeight());

	mPageCounter = mEngine.getEngineCfg().getText("viewer:widget").create(mEngine, this);
	if(mPageCounter){
		mPageCounter->setResizeToText(true);
		mPageCounter->enable(false);
	}

	mTouchToggle = new ds::ui::ImageButton(mEngine, "%APP%/data/images/media_interface/touch_unlocked.png", "%APP%/data/images/media_interface/touch_unlocked.png", (sizey.y - buttonHeight) / 2.0f);
	addChildPtr(mTouchToggle);
	mTouchToggle->setClickFn([this](){
		if(mLinkedPDF){
			if(mLinkedPDF->isEnabled()){
				mLinkedPDF->enable(false);
			} else {
				mLinkedPDF->enable(true);
			}
			updateWidgets();
		}
	});

	mTouchToggle->getNormalImage().setColor(buttonColor);
	mTouchToggle->getHighImage().setColor(buttonColor / 2.0f);
	mTouchToggle->setScale(sizey.y / mTouchToggle->getHeight());

	updateWidgets();
	const float padding = sizey.y / 4.0f;

	mMinWidth = (
		mUpButton->getScaleWidth() + padding +
		mPageCounter->getScaleWidth() + padding +
		mDownButton->getScaleWidth() + padding +
		mTouchToggle->getScaleWidth() + padding * 4.0f
		);

	mMaxWidth = mMinWidth;

}

void PDFInterface::linkPDF(ds::ui::Pdf* linkedPDF){
	mLinkedPDF = linkedPDF;
	if(mLinkedPDF){
		mLinkedPDF->setPageChangeCallback([this]{
			updateWidgets();
		});
	}
	updateWidgets();
}


// Layout is called when the size is changed, so don't change the size in the layout
void PDFInterface::onLayout(){
	if(mUpButton && mDownButton && mPageCounter){
		const float w = getWidth();
		const float h = getHeight();
		const float padding = h / 4.0f;

		float componentsWidth = (
			mUpButton->getScaleWidth() + padding +
			mPageCounter->getScaleWidth() + padding +
			mDownButton->getScaleWidth() + padding +
			mTouchToggle->getScaleWidth()
		);

		float margin = ((w - componentsWidth) * 0.5f);
		float xp = margin;

		mUpButton->setPosition(xp, (h * 0.5f) - (mUpButton->getScaleHeight() * 0.5f));
		xp += mUpButton->getScaleWidth() + padding;

		mPageCounter->setPosition(xp, (h * 0.5f) - (mPageCounter->getHeight() * 0.6f));
		xp += mPageCounter->getScaleWidth() + padding;

		mDownButton->setPosition(xp, (h * 0.5f) - (mDownButton->getScaleHeight() * 0.5f));
		xp += mDownButton->getScaleWidth() + padding;

		mTouchToggle->setPosition(xp, (h * 0.5f) - (mTouchToggle->getScaleHeight() * 0.5f));
		xp += mTouchToggle->getScaleWidth() + padding;
	}
}

void PDFInterface::updateWidgets(){
	if(mPageCounter){
		std::wstringstream wss;
		if(mLinkedPDF){
			wss << mLinkedPDF->getPageNum() << " / " << mLinkedPDF->getPageCount();
		}
		mPageCounter->setText(wss.str());
	}

	if(mLinkedPDF){
		if(mLinkedPDF->isEnabled()){
			mTouchToggle->getHighImage().setImageFile("%APP%/data/images/media_interface/touch_locked.png", ds::ui::Image::IMG_CACHE_F);
			mTouchToggle->getNormalImage().setImageFile("%APP%/data/images/media_interface/touch_locked.png", ds::ui::Image::IMG_CACHE_F);
		} else {
			mTouchToggle->getHighImage().setImageFile("%APP%/data/images/media_interface/touch_unlocked.png", ds::ui::Image::IMG_CACHE_F);
			mTouchToggle->getNormalImage().setImageFile("%APP%/data/images/media_interface/touch_unlocked.png", ds::ui::Image::IMG_CACHE_F);
		}
	}
	layout();
}
} // namespace ui
} // namespace ds
