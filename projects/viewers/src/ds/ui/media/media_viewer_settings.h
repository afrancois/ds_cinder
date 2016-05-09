#pragma once
#ifndef VIEWERS_UI_MEDIA_MEDIA_VIEWER_SETTINGS
#define VIEWERS_UI_MEDIA_MEDIA_VIEWER_SETTINGS

#include <string>
#include <cinder/Vector.h>
#include <cinder/Color.h>

namespace ds {
namespace ui { 

/// MediaViewerSettings
/// A set of things you can change about MediaViewers
/// Most of these need to be set before loading the media
/// This is a struct cause everything is public
struct MediaViewerSettings {

	MediaViewerSettings()
		: mDefaultBounds(0.0f, 0.0f)
		, mWebDefaultSize(0.0f, 0.0f)
		, mWebKeyboardPanelSize(900.0f, 450.0f)
		, mWebKeyboardKeyScale(1.0f)
		, mWebAllowKeyboard(true)
		, mWebAllowTouchToggle(true)
		, mCacheImages(false)
		, mVideoPanning(0.0f)
		, mVideoAutoSync(true)
		, mVideoAutoPlayFirstFrame(true)
		, mVideoAllowOutOfBoundsMuting(true)
		, mVideoLoop(true)
	{}

	//--------------------Overall Settings -----------------------------------------//
	/// The size to be calculated to fit inside when initially loading content
	ci::Vec2f					mDefaultBounds;

	//--------------------Web Settings ---------------------------------------------//

	/// The size to load the web page, which will then get scaled up or down when the viewer changes size
	ci::Vec2f					mWebDefaultSize;		
	// The size of the panel behind the keyboard
	ci::Vec2f					mWebKeyboardPanelSize;
	// the scale of the keyboard
	float						mWebKeyboardKeyScale;
	// If we should show the keyboard button at all. Default = true
	bool						mWebAllowKeyboard;
	// If we should show the button to allowing touching web sites. Default=true
	bool						mWebAllowTouchToggle;

	//--------------------Image Settings -------------------------------------------//

	/// whether to cache the primary and thumb images. Default = false
	bool						mCacheImages;

	//--------------------Video Settings -------------------------------------------//

	/// From -1.0f (left) to 1.0f (right). Default = 0.0f (center)
	float						mVideoPanning;

	/// A list of clients to not load the video on
	/// Default is empty, which loads everywhere
	/// See GstVideo header for more info
	std::vector<std::string>	mVideoPlayableInstances;

	/// Whether to auto-synchronize video across clients (no effect in standalone mode)
	/// Default = true
	bool						mVideoAutoSync;

	/// Set this to false in a some-instances-only environment, otherwise clients might never get audio
	/// Automatically displays the first frame of the video
	bool						mVideoAutoPlayFirstFrame;

	/// If the video goes out of the current bounds of the window, mute the video automatically (or don't have that functionality)
	bool						mVideoAllowOutOfBoundsMuting;

	/// Whether the video should loop or not by default
	bool						mVideoLoop;



};

} // namespace ui
} // namespace ds

#endif