#ifndef _GENERIC_DATA_MODEL_APP_GLOBALS_
#define _GENERIC_DATA_MODEL_APP_GLOBALS_

#include <ds/cfg/cfg_text.h>
#include <ds/cfg/settings.h>
#include <ds/ui/sprite/sprite_engine.h>

#include "model/all_data.h"

namespace ds {
namespace ui {
class SpriteEngine;
} // namespace ui
} // namespace ds

namespace downstream {
class DataWrangler;

/**
 * \class downstream::Globals
 * \brief Global data for the app.
 */
class Globals {
public:
	Globals(ds::ui::SpriteEngine&, const AllData& d, DataWrangler& dataWrangler);

	ds::ui::SpriteEngine&			mEngine;

	DataWrangler&					mDataWrangler;

	const AllData&					mAllData;

	const float						getAnimDur();

	void							initialize();

	//Shortcuts
	const ds::cfg::Text&			getText(const std::string& name) const;
	ds::cfg::Settings&				getAppSettings() const;
	ds::cfg::Settings&				getSettings(const std::string& name) const;

private:

	float							mAnimationDuration;
};

} // !namespace downstream

#endif // !_GENERIC_DATA_MODEL_APP_GLOBALS_
