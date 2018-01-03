#pragma  once
#ifndef _GENERIC_DATA_MODEL_APP_QUERY_DATA_QUERY_H_
#define _GENERIC_DATA_MODEL_APP_QUERY_DATA_QUERY_H_

#include <functional>
#include <Poco/Runnable.h>
#include <ds/query/query_result.h>

#include "model/data_model.h"

namespace downstream {

/**
* \class downstream::DataQuery
*/
class DataQuery : public Poco::Runnable {
public:
	DataQuery();

	virtual void			run();

	void					getDataFromTable(ds::model::DataModelRef parentModel, const std::string& theTable);
	ds::model::DataModelRef	mData;

};

} // !namespace downstream

#endif //!_GENERIC_DATA_MODEL_APP_QUERY_INDUSTRY_QUERY_H_
