#pragma once
#ifndef DS_MODEL_AUTOGENERATED_FilterableExxonTag
#define DS_MODEL_AUTOGENERATED_FilterableExxonTag

#include <ds/data/resource.h>
#include <memory>
#include <vector>
#include <cinder/Vector.h>



namespace ds {
namespace model{




/**
* \class ds::model::FilterableExxonTagRef
*			Auto-generated data model for FilterableExxonTag
*/
class FilterableExxonTagRef {
public:

	FilterableExxonTagRef();

	const unsigned int& getId() const;
	const unsigned int& getFilterableId() const;
	const unsigned int& getExxonTagId() const;


	FilterableExxonTagRef& setId(const unsigned int& Id);
	FilterableExxonTagRef& setFilterableId(const unsigned int& FilterableId);
	FilterableExxonTagRef& setExxonTagId(const unsigned int& ExxonTagId);



private:
	class Data;
	std::shared_ptr<Data>	mData;
};

} // namespace model
} // namespace ds

#endif
