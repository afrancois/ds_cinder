#pragma once
#ifndef DS_NETWORK_HTTPCLIENT_H_
#define DS_NETWORK_HTTPCLIENT_H_

#include <functional>
#include <string>
#include "ds/thread/work_client.h"
#include "ds/thread/work_request_list.h"

namespace ds {

/* Generic HTTP reply info
 ******************************************************************/
class HttpReply {
public:
	static const int		REPLY_OK = 0;
	static const int		REPLY_UNKNOWN_ERROR = -1;
	static const int		REPLY_CONNECTION_ERROR = -2;

public:
	std::wstring			mMsg;
	int						    mStatus;

	HttpReply();

	void					    clear();
};

/* HTTP-CLIENT
 * Perform an HTTP GET operation asynchronously.  Not that
 * HTTPS is not supported -- getting SSL in place is a can
 * I don't want to open.  Also, currently don't support any
 * actual reply, this is a fire and forget for now.
 ******************************************************************/
class HttpClient : public ds::WorkClient
{
public:
	// If the URL contains a query part it needs to be URL encoded, that won't happen automatically.
	static bool						httpGetAndReply(const std::wstring& url, ds::HttpReply*);
	static bool						httpPostAndReply(const std::wstring& url, const std::string& body, ds::HttpReply*);

public:
	HttpClient(ui::SpriteEngine&, const std::function<void(const HttpReply&)>& = nullptr);

  void                  setResultHandler(const std::function<void(const HttpReply&)>&);

	bool							    httpGet(const std::wstring& url);
	bool							    httpPost(const std::wstring& url, const std::string& body);

protected:
  virtual void          handleResult(std::unique_ptr<WorkRequest>&);

private:
    typedef ds::WorkClient  inherited;

    class Request : public ds::WorkRequest {
      public:
        Request(const void* clientId);

        // input
//        int                 mRunId;
		    int							    mOpt;
		    std::wstring				mUrl;
		    std::string					mBody;

        // output
		    ds::HttpReply				mReply;

        virtual void        run();
    };

    ds::WorkRequestList<Request>
                            mCache;
    std::function<void(const HttpReply&)>
			    						      mResultHandler;

private:
	bool							        sendHttp(const int opt, const std::wstring& url, const std::string& body);
	static bool						    httpAndReply(const int opt, const std::wstring& url, const std::string& body, ds::HttpReply*);
};

} // namespace ds

#endif // DS_NETWORK_HTTPCLIENT_H_