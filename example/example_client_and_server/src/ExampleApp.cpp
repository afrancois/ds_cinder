#include <cinder/app/AppBasic.h>
#include <cinder/Timeline.h>
#include <Poco/Path.h>

#include <ds/app/app.h>
#include <ds/app/environment.h>
#include <ds/cfg/settings.h>
#include <ds/data/resource_list.h>
#include <ds/ui/touch/touch_info.h>
#include <ds/ui/sprite/image.h>
#include <ds/ui/sprite/multiline_text.h>

using namespace std;
using namespace ci;
using namespace ci::app;

namespace {
const std::string               FONT_NAME("din-medium");
const char                      EXAMPLE_DB_TYPE = ds::Resource::Id::CUSTOM_TYPE;
// A hardcoded resource from the example database
const ds::Resource::Id          KITTY_RES_ID(EXAMPLE_DB_TYPE, 44);

// Custom database info
std::string                     CUSTOM_RESOURCE_PATH;
std::string                     CUSTOM_DB_PATH;
std::string                     EMPTY_CUSTOM_PATH("");
}

class CsApp : public ds::App {
  public:
    CsApp();

    void				      setupServer();
    void              keyDown( KeyEvent event );

  private:
    typedef ds::App   inherited;
};

CsApp::CsApp() {
	try {
		// Setup my custom database info
		CUSTOM_RESOURCE_PATH = ds::Environment::getAppFolder("data", "resources");
		CUSTOM_DB_PATH = ds::Environment::getAppFolder("data", "resources/db/db.sqlite");
		ds::Resource::Id::setupCustomPaths(	[](const ds::Resource::Id& id)->const std::string&{ if (id.mType == EXAMPLE_DB_TYPE) return CUSTOM_RESOURCE_PATH; return EMPTY_CUSTOM_PATH; },
											[](const ds::Resource::Id& id)->const std::string&{ if (id.mType == EXAMPLE_DB_TYPE) return CUSTOM_DB_PATH; return EMPTY_CUSTOM_PATH; });
		// Setup my custom fonts. This is purely optional, but is used to optimize
		// server->client communication. If the client and server have different font
		// folders, that's fine. You'll need to construct each on its own, and in the
		// same order.
		// It's maybe worth nothing that if the client and server do have different
		// font folders, then you HAVE to do this, to abstract that info.
		mEngine.editFonts().install(ds::Environment::getAppFolder("data/fonts", "DIN-Medium.otf"), FONT_NAME);
	} catch (std::exception const& ex) {
		cout << "ERROR in app constructor=" << ex.what() << endl;
	}
}

void CsApp::setupServer() {
	ds::ui::Sprite &rootSprite = mEngine.getRootSprite();
	ds::ui::Image   *imgSprite;
 
	// Example image sprite from a hardcoded filename.
	imgSprite = new ds::ui::Image(mEngine, ds::Environment::getAppFolder("data", "lorem_kicksum.w_1170.h_1146.png"));
	imgSprite->setScale(0.25f, 0.25f);
	imgSprite->enable(true);
	imgSprite->enableMultiTouch(ds::ui::MULTITOUCH_NO_CONSTRAINTS);
	rootSprite.addChild(*imgSprite);

	// Example image sprite from a resource.
	imgSprite = new ds::ui::Image(mEngine, KITTY_RES_ID);
	imgSprite->setScale(0.5f, 0.5f);
	imgSprite->setPosition(200, 200);
	imgSprite->enable(true);
	imgSprite->enableMultiTouch(ds::ui::MULTITOUCH_NO_CONSTRAINTS);
	rootSprite.addChild(*imgSprite);
 
	// Example sprite
	ds::ui::Sprite *child = new ds::ui::Sprite(mEngine, 100.0f, 100.0f);
	child->setPosition(getWindowWidth() / 4.0f, getWindowHeight() / 4.0f);
	child->setCenter(0.5f, 0.5f);
	child->setColor(1.0f, 1.0f, 0.0f);
	child->setTransparent(false);
	child->enable(true);
	child->enableMultiTouch(ds::ui::MULTITOUCH_NO_CONSTRAINTS);
	rootSprite.addChild(*child);

	// Example text sprite
	ds::ui::MultilineText*  text = new ds::ui::MultilineText(mEngine);
	text->setFont(FONT_NAME, 12.0f);
	text->setText("beavers and ducks");
	text->setPosition(getWindowWidth() * 0.25f, getWindowHeight() * 0.75f);
	text->enable(true);
	rootSprite.addChild(*text);
}

void CsApp::keyDown( KeyEvent event )
{
  inherited::keyDown(event);

  if ( event.getCode() == KeyEvent::KEY_ESCAPE )
  {
    quit();
  }
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( CsApp, RendererGl )
