#include "entry_field.h"

#include <ds/ui/sprite/sprite_engine.h>
#include <ds/app/engine/engine_cfg.h>
#include <ds/ui/sprite/image.h>
#include <ds/app/environment.h>
#include <ds/debug/logger.h>

#include "ds/ui/soft_keyboard/soft_keyboard_button.h"

namespace ds{
namespace ui{

EntryField::EntryField(ds::ui::SpriteEngine& engine, EntryFieldSettings& settings)
	: ds::ui::Sprite(engine)
	, mCursor(nullptr)
	, mTextSprite(nullptr)
	, mInFocus(false)
	, mCursorIndex(0)
{
	mTextSprite = new ds::ui::MultilineText(engine);
	addChildPtr(mTextSprite);

	mCursor = new ds::ui::Sprite(engine);
	mCursor->setTransparent(false);
	addChildPtr(mCursor);

	// In case you wanna see the size of this sucka
	//setTransparent(false);
	//setColor(ci::Color(0.3f, 0.0f, 0.0f));

	enableMultiTouch(ds::ui::MULTITOUCH_INFO_ONLY);
	setProcessTouchCallback([this](ds::ui::Sprite* bs, const ds::ui::TouchInfo& ti){
		handleTouchInput(bs, ti);
	});

	setEntryFieldSettings(settings);
}

void EntryField::setEntryFieldSettings(EntryFieldSettings& newSettings){
	mEntryFieldSettings = newSettings;

	setSize(mEntryFieldSettings.mFieldSize.x, mEntryFieldSettings.mFieldSize.y);

	if(mTextSprite){
		mTextSprite->setResizeLimit(mEntryFieldSettings.mFieldSize.x, mEntryFieldSettings.mFieldSize.y);
		if(!newSettings.mTextConfig.empty()){
			mEngine.getEngineCfg().getText(newSettings.mTextConfig).configure(*mTextSprite);
		}
	}

	if(mCursor){
		mCursor->setColor(newSettings.mCursorColor);
		mCursor->setSize(newSettings.mCursorSize);
	}

	textUpdated();
}

const std::wstring EntryField::getCurrentText(){
	if(mTextSprite){
		return mTextSprite->getText();
	}
	return L"";
}

void EntryField::setCurrentText(const std::wstring& crTxStr){
	if(mTextSprite){
		mTextSprite->setText(crTxStr);
	}
	
	mCursorIndex = crTxStr.size();

	textUpdated();
}

void EntryField::keyPressed(const std::wstring& keyCharacter, const ds::ui::SoftKeyboardDefs::KeyType keyType){
	std::wstring currentCharacter = keyCharacter;
	std::wstring currentFullText = getCurrentText();

	if(mCursorIndex == currentFullText.size()){
		handleKeyPressGeneric(keyType, currentCharacter, currentFullText);

		setCurrentText(currentFullText);
	} else {

		std::wstring preString = currentFullText.substr(0, mCursorIndex);
		std::wstring posString = currentFullText.substr(mCursorIndex);
		handleKeyPressGeneric(keyType, currentCharacter, preString);
		std::wstringstream wss;
		wss << preString << posString;

		if(mTextSprite){
			mTextSprite->setText(wss.str());
		}

		if(keyType == ds::ui::SoftKeyboardDefs::kDelete){
			mCursorIndex--;
		} else if(keyType == ds::ui::SoftKeyboardDefs::kShift){
			// nothin!
		} else {
			mCursorIndex++;
		}

		textUpdated();
	}


}


void EntryField::resetCurrentText() {
	setCurrentText(L"");
}

void EntryField::focus(){
	if(mInFocus) return;
	mInFocus = true;

	enable(true);

	if(mCursor){
		mCursor->show();
		mCursor->setOpacity(0.0f);
		blinkCursor();
	}

	onFocus();
}

void EntryField::unfocus(){
	if(!mInFocus) return;
	mInFocus = false;

	enable(false);

	if(mCursor){
		mCursor->tweenOpacity(0.0f, mEntryFieldSettings.mAnimationRate, 0.0f, ci::easeNone, [this]{
			mCursor->hide();
		});
	}
}

void EntryField::textUpdated(){
	cursorUpdated();
	onTextUpdated();
}

void EntryField::cursorUpdated(){
	if(mTextSprite && mCursor){
		if(mCursorIndex > getCurrentText().size()){
			mCursorIndex = getCurrentText().size();
		}
		ci::Vec2f cursorPos = mTextSprite->getPositionForCharacterIndex(mCursorIndex);
		mCursor->setPosition(cursorPos.x + mEntryFieldSettings.mCursorOffset.x, cursorPos.y + mEntryFieldSettings.mCursorOffset.y);
	}
}


void EntryField::handleTouchInput(ds::ui::Sprite* bs, const ds::ui::TouchInfo& ti){

	if(ti.mPhase != ds::ui::TouchInfo::Removed && mTextSprite){
		ci::Vec3f loccy = mTextSprite->globalToLocal(ti.mCurrentGlobalPoint);
		mCursorIndex = mTextSprite->getCharacterIndexForPosition(loccy.xy());

		cursorUpdated();
	}
}

void EntryField::blinkCursor(){
	if(!mCursor) return;
	mCursor->tweenOpacity(1.0f, mEntryFieldSettings.mAnimationRate, 0.0f, ci::easeNone, [this]{
		mCursor->tweenOpacity(1.0f, mEntryFieldSettings.mBlinkRate, 0.0f, ci::easeNone, [this]{
			mCursor->tweenOpacity(0.0f, mEntryFieldSettings.mAnimationRate, 0.0f, ci::easeNone, [this]{
				blinkCursor();
			});
		});
	});

}
}
}