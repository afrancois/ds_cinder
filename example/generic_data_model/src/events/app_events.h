#ifndef _GENERIC_DATA_MODEL_APP_APPEVENTS_H_
#define _GENERIC_DATA_MODEL_APP_APPEVENTS_H_

#include <ds/app/event.h>

namespace downstream {

class IdleStartedEvent : public ds::RegisteredEvent < IdleStartedEvent > {
public:
	IdleStartedEvent(){};
};

class IdleEndedEvent : public ds::RegisteredEvent < IdleEndedEvent > {
public:
	IdleEndedEvent(){};

};

class StoryDataUpdatedEvent : public ds::RegisteredEvent<StoryDataUpdatedEvent>{
public:
	StoryDataUpdatedEvent(){};
}; 

class RequestAppExitEvent : public ds::RegisteredEvent<RequestAppExitEvent>{
public:
	RequestAppExitEvent(){};
};

} // !namespace downstream

#endif // !_GENERIC_DATA_MODEL_APP_APPEVENTS_H_
