#include "media_viewer.h"


#include <ds/app/environment.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/ui/sprite/image.h>
#include <ds/debug/logger.h>
#include <ds/util/string_util.h>
#include <ds/ui/util/ui_utils.h>

#include <ds/data/resource.h>

#include "ds/ui/media/player/video_player.h"
#include "ds/ui/media/player/pdf_player.h"
#include "ds/ui/media/player/stream_player.h"
#include "ds/ui/media/player/web_player.h"

#include "ds/ui/media/media_interface.h"

#include "ds/ui/sprite/web.h"
#include "ds/ui/sprite/gst_video.h"

namespace ds {
namespace ui {

MediaViewer::MediaViewer(ds::ui::SpriteEngine& eng, const bool embedInterface)
	: BasePanel(eng)
	, mInitialized(false)
	, mVideoPlayer(nullptr)
	, mStreamPlayer(nullptr)
	, mPDFPlayer(nullptr)
	, mWebPlayer(nullptr)
	, mThumbnailImage(nullptr)
	, mPrimaryImage(nullptr)
	, mEmbedInterface(embedInterface)
{
	setDefaultProperties();
}

MediaViewer::MediaViewer(ds::ui::SpriteEngine& eng, const std::string& mediaPath, const bool embedInterface)
	: BasePanel(eng)
	, mInitialized(false)
	, mResource(mediaPath, ds::Resource::parseTypeFromFilename(mediaPath))
	, mVideoPlayer(nullptr)
	, mPDFPlayer(nullptr)
	, mStreamPlayer(nullptr)
	, mWebPlayer(nullptr)
	, mThumbnailImage(nullptr)
	, mPrimaryImage(nullptr)
	, mEmbedInterface(embedInterface)
{
	setDefaultProperties();
}

MediaViewer::MediaViewer(ds::ui::SpriteEngine& eng, const ds::Resource& resource, const bool embedInterface)
	: BasePanel(eng)
	, mInitialized(false)
	, mResource(resource)
	, mVideoPlayer(nullptr)
	, mPDFPlayer(nullptr)
	, mStreamPlayer(nullptr)
	, mWebPlayer(nullptr)
	, mThumbnailImage(nullptr)
	, mPrimaryImage(nullptr)
	, mEmbedInterface(embedInterface)
{
	setDefaultProperties();
}


void MediaViewer::setSettings(const MediaViewerSettings& newSettings){
	mMediaViewerSettings = newSettings;
}

void MediaViewer::setDefaultProperties(){
	setDefaultBounds(mEngine.getWorldWidth(), mEngine.getWorldHeight());
	setWebViewSize(ci::Vec2f::zero());
	setCacheImages(false);
}

void MediaViewer::loadMedia(const std::string& mediaPath, const bool initializeImmediately) {
	if(mInitialized){
		uninitialize();
	}

	mResource = ds::Resource(mediaPath, ds::Resource::parseTypeFromFilename(mediaPath));

	if(initializeImmediately){
		initialize();
	}
}

void MediaViewer::loadMedia(const ds::Resource& reccy, const bool initializeImmediately) {
	if(mInitialized){
		uninitialize();
	}

	mResource = reccy;

	if(initializeImmediately){
		initialize();
	}
}


void MediaViewer::setDefaultBounds(const float defaultWidth, const float defaultHeight){
	mMediaViewerSettings.mDefaultBounds.x = defaultWidth;
	mMediaViewerSettings.mDefaultBounds.y = defaultHeight;
}

void MediaViewer::setWebViewSize(const ci::Vec2f webSize){
	mMediaViewerSettings.mWebDefaultSize = webSize;
}	

void MediaViewer::initialize(){
	if(mInitialized) return;

	// do this first to avoid recursion problems
	mInitialized = true;

	const int mediaType = mResource.getType();
	if(mediaType == ds::Resource::ERROR_TYPE || mediaType == ds::Resource::FONT_TYPE){
		DS_LOG_WARNING("Whoopsies - tried to open a media player on an invalid file type. " << mResource.getAbsoluteFilePath());
		return;
	}

	float contentWidth = 1.0f;
	float contentHeight = 1.0f;
	mContentAspectRatio = 1.0f;

	bool showThumbnail = true;

	if(mediaType == ds::Resource::IMAGE_TYPE){
		int flags = 0;
		if(mMediaViewerSettings.mCacheImages){
			flags |= Image::IMG_CACHE_F;
		}
		mPrimaryImage = new ds::ui::Image(mEngine, mResource, flags);
		addChildPtr(mPrimaryImage);
		mPrimaryImage->checkStatus();
		if(mPrimaryImage->isLoaded()){
			showThumbnail = false;
		} else {
			mPrimaryImage->setOpacity(0.0f);
			mPrimaryImage->setStatusCallback([this](ds::ui::Image::Status status){
				if(status.mCode == status.STATUS_LOADED && mPrimaryImage){
					mPrimaryImage->tweenOpacity(1.0f, mAnimDuration);
					if(mStatusCallback){
						mStatusCallback(true);
					}
				}
			});
		}

		mContentAspectRatio = mPrimaryImage->getWidth() / mPrimaryImage->getHeight();
		contentWidth = mPrimaryImage->getWidth();
		contentHeight = mPrimaryImage->getHeight();

	} else if(mediaType == ds::Resource::VIDEO_TYPE){
		mVideoPlayer = new VideoPlayer(mEngine, mEmbedInterface);
		addChildPtr(mVideoPlayer);

		mVideoPlayer->setErrorCallback([this](const std::string& msg){
			if(mErrorCallback) mErrorCallback(msg);
		});

		mVideoPlayer->setGoodStatusCallback([this]{ 
			if(mStatusCallback) mStatusCallback(true); 
		});

		mVideoPlayer->setPan(mMediaViewerSettings.mVideoPanning);
		mVideoPlayer->setAutoSynchronize(mMediaViewerSettings.mVideoAutoSync);
		mVideoPlayer->setPlayableInstances(mMediaViewerSettings.mVideoPlayableInstances);

		mVideoPlayer->setMedia(mResource.getAbsoluteFilePath());


		mContentAspectRatio = mVideoPlayer->getWidth() / mVideoPlayer->getHeight();
		contentWidth = mVideoPlayer->getWidth();
		contentHeight = mVideoPlayer->getHeight();

	} else if( mediaType == ds::Resource::VIDEO_STREAM_TYPE ){
		
		mStreamPlayer = new StreamPlayer(mEngine, mEmbedInterface);
		addChildPtr(mStreamPlayer);


		mStreamPlayer->setErrorCallback([this](const std::string& msg){
			if(mErrorCallback) mErrorCallback(msg);
		});

		mStreamPlayer->setGoodStatusCallback([this]{
			if(mStatusCallback) mStatusCallback(true);
		});

		mStreamPlayer->setResource(mResource);

		mContentAspectRatio = mStreamPlayer->getWidth() / mStreamPlayer->getHeight();
		contentWidth = mStreamPlayer->getWidth();
		contentHeight = mStreamPlayer->getHeight();
		
	} else if(mediaType == ds::Resource::PDF_TYPE){
		mPDFPlayer = new PDFPlayer(mEngine, mEmbedInterface);
		addChildPtr(mPDFPlayer);
		mPDFPlayer->setMedia(mResource.getAbsoluteFilePath());

		mPDFPlayer->setErrorCallback([this](const std::string& msg){
			if(mErrorCallback) mErrorCallback(msg);
		});

		mPDFPlayer->setGoodStatusCallback([this]{
			if(mStatusCallback){
				mStatusCallback(true);
			}
		});

		mContentAspectRatio = mPDFPlayer->getWidth() / mPDFPlayer->getHeight();
		contentWidth = mPDFPlayer->getWidth();
		contentHeight = mPDFPlayer->getHeight();

	} else if(mediaType == ds::Resource::WEB_TYPE){
		mWebPlayer = new WebPlayer(mEngine, mEmbedInterface);
		addChildPtr(mWebPlayer);
		mWebPlayer->setWebViewSize(mMediaViewerSettings.mWebDefaultSize);
		mWebPlayer->setKeyboardParams(mMediaViewerSettings.mWebKeyboardPanelSize, mMediaViewerSettings.mWebKeyboardKeyScale);
		mWebPlayer->setMedia(mResource.getAbsoluteFilePath());

		if(mWebPlayer->getWeb()){
			mWebPlayer->getWeb()->setDocumentReadyFn([this]{
				if(mStatusCallback) mStatusCallback(true);
			});
			mWebPlayer->getWeb()->setErrorCallback([this](const std::string& errorMsg){
				if(mErrorCallback) mErrorCallback(errorMsg);
			});
		}

		mContentAspectRatio = mWebPlayer->getWidth() / mWebPlayer->getHeight();
		contentWidth = mWebPlayer->getWidth();
		contentHeight = mWebPlayer->getHeight();


	} else {
		DS_LOG_WARNING("Whoopsies - tried to open a media player on an invalid file type. " << mResource.getAbsoluteFilePath() << " " << ds::utf8_from_wstr(mResource.getTypeName()));
	}


	if(showThumbnail && (mResource.getThumbnailId() > 0 || !mResource.getThumbnailFilePath().empty())){
		int flags = 0;
		if(mMediaViewerSettings.mCacheImages){
			flags |= Image::IMG_CACHE_F;
		}
		mThumbnailImage = new ds::ui::Image(mEngine);
		addChildPtr(mThumbnailImage);
		mThumbnailImage->sendToBack();
		if(mResource.getThumbnailId() > 0){
			mThumbnailImage->setImageResource(mResource.getThumbnailId(), flags);
		} else {
			mThumbnailImage->setImageFile(mResource.getThumbnailFilePath(), flags);
		}
		mThumbnailImage->setOpacity(0.0f);
		mThumbnailImage->setStatusCallback([this](ds::ui::Image::Status status){
			if(status.mCode == status.STATUS_LOADED && mThumbnailImage){
				mThumbnailImage->tweenOpacity(1.0f, mAnimDuration);
			}
		});
	}

	// calculate a default size that maximizes size
	float settingsAspect = 1.0f;
	const float settingsWidth = mMediaViewerSettings.mDefaultBounds.x;
	const float settingsHeight = mMediaViewerSettings.mDefaultBounds.y;
	if(settingsHeight > 0.0f){
		settingsAspect = settingsWidth / settingsHeight;
	}
	
	// calculate a width to make the player fit maximally
	float scaleFactor = 1.0f;
	float idealWidth = settingsWidth;
	float idealHeight = settingsHeight;
	if(mContentAspectRatio < settingsAspect){
		scaleFactor = settingsHeight / contentHeight;
		idealWidth = contentWidth * scaleFactor;
	} else if(mContentAspectRatio > settingsAspect){
		scaleFactor = settingsWidth / contentWidth;
		idealHeight = contentHeight * scaleFactor;
	}

	mDefaultSize = ci::Vec2f(idealWidth, idealHeight);
	// setting size is necessary to get size limits to work
	setSize(idealWidth, idealHeight);
	setSizeLimits();
	setViewerSize(mDefaultSize.x, mDefaultSize.y);
}

void MediaViewer::uninitialize() {
	if(!mInitialized) return;
	if(mThumbnailImage){
		mThumbnailImage->release();
	}
	if(mVideoPlayer){
		mVideoPlayer->release();
	}
	if(mStreamPlayer){
		mStreamPlayer->release();
	}
	if(mPDFPlayer){
		mPDFPlayer->release();
	}
	if(mPrimaryImage){
		mPrimaryImage->release();
	}
	if(mWebPlayer){
		mWebPlayer->release();
	}

	mThumbnailImage = nullptr;
	mVideoPlayer = nullptr;
	mPDFPlayer = nullptr;
	mPrimaryImage = nullptr;
	mWebPlayer = nullptr;

	mInitialized = false;
}


void MediaViewer::onLayout(){
	initialize();

	if(mVideoPlayer){
		mVideoPlayer->setSize(getWidth(), getHeight());
	}

	if(mStreamPlayer){
		mStreamPlayer->setSize(getWidth(), getHeight());
	}

	if(mPDFPlayer){
		mPDFPlayer->setSize(getWidth(), getHeight());
	}

	if(mThumbnailImage){
		fitInside(mThumbnailImage, ci::Rectf(0.0f, 0.0f, getWidth(), getHeight()), true);
	}

	if(mPrimaryImage){
		fitInside(mPrimaryImage, ci::Rectf(0.0f, 0.0f, getWidth(), getHeight()), true);
	}

	if(mWebPlayer){
		mWebPlayer->setSize(getWidth(), getHeight());
	}
}

void MediaViewer::enter(){
	initialize();

	if(mVideoPlayer){
		mVideoPlayer->play();
	}
}

void MediaViewer::exit(){
	if(mVideoPlayer){
		mVideoPlayer->pause();
	}
}

void MediaViewer::userInputReceived(){
	BasePanel::userInputReceived();

	showInterface();
}

void MediaViewer::showInterface(){
	if(mVideoPlayer){
		mVideoPlayer->showInterface();
	}
	if(mPDFPlayer){
		mPDFPlayer->showInterface();
	}
	if(mWebPlayer){
		mWebPlayer->showInterface();
	}
}

void MediaViewer::stopContent(){
	if(mVideoPlayer){
		mVideoPlayer->stop();
	}

	if(mStreamPlayer){
		mStreamPlayer->stop();
	}
}

ds::ui::Sprite* MediaViewer::getPlayer(){
	if(mVideoPlayer){
		return mVideoPlayer;
	}

	if(mPDFPlayer){
		return mPDFPlayer;
	}


	if(mStreamPlayer){
		return mStreamPlayer;
	}

	if(mWebPlayer){
		return mWebPlayer;
	}

	if(mPrimaryImage){
		return mPrimaryImage;
	}

	return nullptr;
}

void MediaViewer::setErrorCallback(std::function<void(const std::string& msg)> func){
	mErrorCallback = func; 
}

void MediaViewer::setStatusCallback(std::function<void(const bool isGood)> func){
	mStatusCallback = func;
}

void MediaViewer::handleStandardClick(const ci::Vec3f& globalPos){
	if(mWebPlayer){
		mWebPlayer->sendClick(globalPos);
	}
	if(mPDFPlayer){
		mPDFPlayer->nextPage();
	}
	if(mVideoPlayer){
		mVideoPlayer->togglePlayPause();
	}
}

void MediaViewer::enableStandardClick(){
	setTapCallback([this](ds::ui::Sprite* bs, const ci::Vec3f& pos){
		handleStandardClick(pos);
	});
}

} // namespace ui
} // namespace ds
