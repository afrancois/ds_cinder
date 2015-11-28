#ifndef _MEDIA_TESTER_APP_H_
#define _MEDIA_TESTER_APP_H_

#include <cinder/app/AppBasic.h>
#include <ds/ui/sprite/text.h>
#include <ds/app/app.h>
#include "ui/video_360_sprite/drone_video_sprite.h"

namespace test {
class AllData;
//class dlpr::view::DroneVideoSprite;
class media_tester : public ds::App {
public:
	media_tester();

	virtual void		fileDrop(ci::app::FileDropEvent event);
	virtual void		keyDown(ci::app::KeyEvent event);
	void				setupServer();
	void				update();
private:
	typedef ds::App		inherited;

	void				loadMedia(const std::string& newMedia);
	void				fitSpriteInArea(ci::Rectf area, ds::ui::Sprite* spriddy);
	ds::ui::DroneVideoSprite* mDroneVideo;
	ds::ui::Video*		mBaseVideo;
	ds::ui::Sprite*		mMedia;
	ds::ui::Text*		mLabelText;
	ds::ui::Sprite*		mHeader;

};

} // !namespace test

#endif // !_MEDIA_TESTER_APP_H_