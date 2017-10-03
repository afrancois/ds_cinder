#include "video_player.h"


#include <ds/app/environment.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/debug/logger.h>
#include <ds/util/string_util.h>

#include <ds/ui/sprite/video.h>
#include <ds/ui/button/image_button.h>

#include "ds/ui/media/interface/video_interface.h"
#include "ds/ui/media/media_interface_builder.h"

namespace ds {
namespace ui {

VideoPlayer::VideoPlayer(ds::ui::SpriteEngine& eng, const bool embedInterface)
	: ds::ui::Sprite(eng)
	, mVideo(nullptr)
	, mVideoInterface(nullptr)
	, mEmbedInterface(embedInterface)
	, mShowInterfaceAtStart(true)
	, mAutoSyncronize(true)
	, mAutoPlayFirstFrame(true)
	, mAllowOutOfBoundsMuted(true)
	, mPanning(0.0f)
	, mLooping(true)
{
	mLayoutFixedAspect = true;
}

void VideoPlayer::setMedia(const std::string mediaPath){
	clear();

	mVideo = new ds::ui::GstVideo(mEngine);
	mVideo->generateAudioBuffer(true);
	mVideo->setLooping(true);

	mVideo->setVideoCompleteCallback([this]{


		mVideo->seekPosition(0);

		if(!mLooping){
			mVideo->pause();
		}

		// show the interface if we have one
		if(mVideoInterface){
			mVideoInterface->userInputReceived();
		}
	});

	setPan(mPanning);
	setAutoSynchronize(mAutoSyncronize);
	setPlayableInstances(mPlayableInstances);
	allowOutOfBoundsMuted(mAllowOutOfBoundsMuted);
	setVideoLoop(mLooping);
	setAudioDevices(mAudioDevices);

	mVideo->setErrorCallback([this](const std::string& msg){
		if(mErrorMsgCallback) mErrorMsgCallback(msg);
	});

	mVideo->setStatusCallback([this](const ds::ui::GstVideo::Status& status){
		bool isGood = status == ds::ui::GstVideo::Status::STATUS_PLAYING;
		if(mGoodStatusCallback){
			mGoodStatusCallback();
		}
	});

	if(mAutoPlayFirstFrame){
		mVideo->setMute(true);
		mVideo->setAutoStart(false);
	} else {
		mVideo->setAutoStart(true);
	}

	mVideo->loadVideo(mediaPath);
	if(mAutoPlayFirstFrame){
		mVideo->playAFrame(-1.0, [this](){
			mVideo->setMute(false);
		});
	}
	addChildPtr(mVideo);

	if(mVideoInterface){
		mVideoInterface->release();
		mVideoInterface = nullptr;
	}

	if(mEmbedInterface){
		mVideoInterface = dynamic_cast<VideoInterface*>(MediaInterfaceBuilder::buildMediaInterface(mEngine, this, this));
		if(mVideoInterface){
			mVideoInterface->sendToFront();
		}
	}

	if(mVideoInterface){
		if(mShowInterfaceAtStart){
			mVideoInterface->show();
		} else {
			mVideoInterface->setOpacity(0.0f);
			mVideoInterface->hide();
		}
	}

	if(mVideo->getWidth() < 1.0f || mVideo->getHeight() < 1.0f){
		// make this a setting? This is mostly for when the "video" is just an audio track
		// Probably should detect this properly from GstVideo and expose it to the outside world. Users may want to know that this only has audio
		setSize(600.0f, 100.0f);
	} else {
		setSize(mVideo->getWidth(), mVideo->getHeight());
	}
}

void VideoPlayer::clear(){
	if(mVideo){
		mVideo->release();
		mVideo = nullptr;
		if(mVideoInterface){
			mVideoInterface->linkVideo(nullptr);
		}
	}
}

void VideoPlayer::onSizeChanged(){
	layout();
}

void VideoPlayer::layout(){
	if(mVideo){
		if(mVideo->getWidth() > 0.0f){
			mVideo->setScale(getWidth() / mVideo->getWidth());
		}
	}

	if(mVideoInterface && mEmbedInterface){
		mVideoInterface->setSize(getWidth() / 2.0f, mVideoInterface->getHeight());
		float yPos = getHeight() - mVideoInterface->getHeight() - 50.0f;
		if(yPos < getHeight() / 2.0f) yPos = getHeight() / 2.0f;
		if(yPos + mVideoInterface->getHeight() > getHeight()) yPos = getHeight() - mVideoInterface->getHeight();
		mVideoInterface->setPosition(getWidth() / 2.0f - mVideoInterface->getWidth() / 2.0f, yPos);
	}
}

void VideoPlayer::showInterface(){
	if(mVideoInterface){
		mVideoInterface->animateOn();
	}
}

void VideoPlayer::hideInterface(){
	if(mVideoInterface){
		mVideoInterface->startIdling();
	}
}

void VideoPlayer::setShowInterfaceAtStart(bool showInterfaceAtStart){
	mShowInterfaceAtStart = showInterfaceAtStart;
}

void VideoPlayer::play(){
	if(mVideo){
		if(mVideo->isPlayingAFrame()){
			mVideo->enablePlayingAFrame(false);
			mVideo->setMute(false);
		}
		mVideo->play();
	}
}

void VideoPlayer::pause(){
	if(mVideo){
		mVideo->pause();
	}
}

void VideoPlayer::stop(){
	if(mVideo){
		mVideo->stop();
	}
}

ds::ui::GstVideo* VideoPlayer::getVideo(){
	return mVideo;
}

void VideoPlayer::togglePlayPause(){
	if(mVideo){
		if(mVideo->getIsPlaying()){
			mVideo->pause();
		} else {
			mVideo->play();
		}
	}
}

void VideoPlayer::toggleMute(){
	if(mVideo){
		if(mVideo->getIsMuted()){
			mVideo->setMute(false);
		} else {
			mVideo->setMute(true);
		}
	}
}

void VideoPlayer::setPan(const float newPan){
	if(mVideo){
		mVideo->setPan(newPan);
	}

	mPanning = newPan;
}

void VideoPlayer::setAutoSynchronize(const bool doSync){
	if(mVideo){
		mVideo->setAutoSynchronize(doSync);
	}

	mAutoSyncronize = doSync;
}

void VideoPlayer::setAutoPlayFirstFrame(const bool playFirstFrame){
	mAutoPlayFirstFrame = playFirstFrame;
}

void VideoPlayer::setPlayableInstances(const std::vector<std::string> instanceNames){
	if(mVideo){
		mVideo->setPlayableInstances(instanceNames);
	}

	mPlayableInstances = instanceNames;
}

void VideoPlayer::allowOutOfBoundsMuted(const bool allowMuting) {
	mAllowOutOfBoundsMuted = allowMuting;
	if(mVideo){
		mVideo->setAllowOutOfBoundsMuted(mAllowOutOfBoundsMuted);
	}
}

void VideoPlayer::setVideoLoop(const bool doLoop){
	mLooping = doLoop;
	if(mVideo){
		mVideo->setLooping(mLooping);
	}
}

void VideoPlayer::setAudioDevices(std::vector<GstAudioDevice>& audioDevices){
	mAudioDevices = audioDevices;
	if(mVideo){
		mVideo->setAudioDevices(audioDevices);
	}
}

} // namespace ui
} // namespace ds
