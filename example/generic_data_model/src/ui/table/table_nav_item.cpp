#include "stdafx.h"

#include "table_nav_item.h"

#include <ds/app/environment.h>
#include <ds/ui/sprite/sprite_engine.h>
#include <ds/debug/logger.h>
#include <ds/util/string_util.h>

#include "query/data_wrangler.h"
#include "model/data_model.h"
#include "events/app_events.h"

#include "app/globals.h"

namespace downstream {

TableNavItem::TableNavItem(Globals& g)
	: ds::ui::SmartLayout(g.mEngine, "table_nav_item.xml")
	, mGlobals(g)
	, mExpanded(false)
{
}

bool TableNavItem::getExpanded() {
	return mExpanded;
}

void TableNavItem::setExpanded(const bool isExpanded) {
	mExpanded = isExpanded;
	if(mData.hasChildren()) {
		if(mExpanded) {
			setSpriteText("id", "-");
		} else {
			setSpriteText("id", "+");
		}
	}
	runLayout();
}

void TableNavItem::setData(ds::model::DataModelRef theData) {
	mData = theData;

	if(mData.hasChildren()) {
		setSpriteText("id", "+");
	} else {
		setSpriteText("id", " ");
	}
	setSpriteText("title", "<span weight='bold'>" + theData.getName() + "</span> | <span weight='light'>" + theData.getLabel() + "</span>");

	runLayout();
}


ds::model::DataModelRef TableNavItem::getData() {
	return mData;
}


} // namespace downstream

