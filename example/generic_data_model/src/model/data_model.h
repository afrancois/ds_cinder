#pragma once
#ifndef DS_MODEL_DATA_MODEL
#define DS_MODEL_DATA_MODEL

#include <map>
#include <memory>
#include <vector>
#include <ds/data/resource.h>
#include <cinder/Color.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>


namespace ds {

namespace ui {
class SpriteEngine;
}

namespace model {

class DataProperty {
public:
	DataProperty();
	DataProperty(const std::string& name, const std::string& value);

	/// Get the name of this property
	const std::string&				getName() const;
	void							setName(const std::string& name);

	const std::string&				getValue() const;
	void							setValue(const std::string& value);
	void							setValue(const std::wstring& value);
	void							setValue(const int& value);
	void							setValue(const double& value);
	void							setValue(const float& value);
	void							setValue(const ci::Color& value);
	void							setValue(const ci::ColorA& value);
	void							setValue(const ci::vec2& value);
	void							setValue(const ci::vec3& value);
	void							setValue(const ci::Rectf& value);

	ds::Resource					getResource() const;
	void							setResource(const ds::Resource& resource);


	// ------- This value, type converted when called --------- //

	bool							getBool() const;
	int								getInt() const;
	float							getFloat() const;
	double							getDouble() const;

	/// The Engine is supplied to look up named colors
	const ci::Color					getColor(ds::ui::SpriteEngine&) const;
	const ci::ColorA				getColorA(ds::ui::SpriteEngine&) const;

	const std::string&				getString() const; // same as getValue(), but supplied here for convenience
	const std::wstring				getWString() const;

	const ci::vec2					getVec2() const;
	const ci::vec3					getVec3() const;
	const ci::Rectf					getRect() const;

protected:
	std::string						mName;
	std::string						mValue;
	ds::Resource					mResource;

};

/**
* \class ds::model::DataModelRef
*/
class DataModelRef {
public:

	// TODO: operators (equality and such)
	// TODO: duplicate
	// TODO: auto validation (e.g. exists, is a date, media meets certain qualifications, etc)

	DataModelRef();
	DataModelRef(const std::string& name, const int id = 0, const std::string& label = "");

	/// Get the id for this item
	const int&						getId() const;
	void							setId(const int& id);

	/// Get the name of this item
	/// Name is generally inherited by the table or thing this belongs to
	/// This is used in the getChildByName()
	const std::string&				getName() const;
	void							setName(const std::string& name);

	/// Get the label for this item
	/// This is a helpful name or display name for this thing
	const std::string&				getLabel() const;
	void							setLabel(const std::string& label);

	/// If this item has no data, value, name, id, properties or children
	const bool						empty() const;


	// -------  end of this value ---------------------------- //

	/// Use this for looking stuff up only. Use the other functions to manage the list
	const std::map<std::string, DataProperty>&				getProperties();
	void													setProperties(const std::map<std::string, DataProperty>& newProperties);

	/// This can return an empty property, which is why it's const.
	/// If you want to modify a property, use the setProperty() function
	const DataProperty										getProperty(const std::string& propertyName);
	const std::string										getPropertyValue(const std::string& propertyName);
	bool													getPropertyBool(const std::string& propertyName);
	int														getPropertyInt(const std::string& propertyName);
	float													getPropertyFloat(const std::string& propertyName);
	double													getPropertyDouble(const std::string& propertyName);
	/// The Engine is supplied to look up named colors
	const ci::Color											getPropertyColor(ds::ui::SpriteEngine&, const std::string& propertyName);
	const ci::ColorA										getPropertyColorA(ds::ui::SpriteEngine&, const std::string& propertyName);
	const std::string										getPropertyString(const std::string& propertyName); // same as getPropertyValue(), but supplied here for convenience
	const std::wstring										getPropertyWString(const std::string& propertyName);
	const ci::vec2											getPropertyVec2(const std::string& propertyName);
	const ci::vec3											getPropertyVec3(const std::string& propertyName);
	const ci::Rectf											getPropertyRect(const std::string& propertyName);

	/// Set the property with a given name
	void													setProperty(const std::string& propertyName, DataProperty datamodel);
	void													setProperty(const std::string& propertyName, const std::string& value);
	void													setProperty(const std::string& propertyName, const std::wstring& value);
	void													setProperty(const std::string& propertyName, const int& value);
	void													setProperty(const std::string& propertyName, const double& value);
	void													setProperty(const std::string& propertyName, const float& value);
	void													setProperty(const std::string& propertyName, const ci::Color& value);
	void													setProperty(const std::string& propertyName, const ci::ColorA& value);
	void													setProperty(const std::string& propertyName, const ci::vec2& value);
	void													setProperty(const std::string& propertyName, const ci::vec3& value);
	void													setProperty(const std::string& propertyName, const ci::Rectf& value);

	/// Gets all of the children
	/// Don't modify the children here, use the other functions
	const std::vector<DataModelRef>&						getChildren() const;

	/// If no children exist, returns an empty data model
	/// If index is greater than the size of the children, returns the last child
	DataModelRef											getChild(const size_t index);

	/// Get the first child that matches this id
	/// If no children exist or match that id, returns an empty data model
	DataModelRef											getChildById(const int id);

	/// Get the first child that matches this name
	/// Can get nested children using dot notation. for example: getChildByName("the_stories.chapter_one.first_paragraph");
	/// If no children exist or match that id, returns an empty data model
	DataModelRef											getChildByName(const std::string& childName);

	/// Adds this child to the end of this children list, or at the index supplied
	void													addChild(DataModelRef datamodel, const size_t index = std::numeric_limits<std::size_t>::max());

	/// Is there a child with this name?
	bool													hasChild(const std::string& name);

	/// Is there at least one child?
	bool													hasChildren();

	/// Replaces all children
	void													setChildren(std::vector<ds::model::DataModelRef> children);
	
	/// Logs this, it's properties, and all it's children recursively
	void					printTree(const bool verbose, const std::string& indent);

private:
	void					createData();
	class Data;
	std::shared_ptr<Data>	mData;
};

} // namespace model
} // namespace ds

#endif

#pragma once
