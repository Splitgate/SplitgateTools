#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib/httplib.h"

class HttpJob
{
	friend class HttpSystem;

public:

	HttpJob(httplib::Client* InClient, httplib::Request InRequest, std::function<void(httplib::Response)> InCompletedCallback);

private:

	httplib::Client* CallingClient;
	httplib::Request Request;
	std::function<void(httplib::Response)> CompletedCallback;
};

class HttpSystem
{
	friend HttpJob;

public:

	// Separate thread created in DLLMain, used for executing http requests without causing frame hitching
	static DWORD WINAPI Thread(LPVOID)
	{
		for (;;)
		{
			for (size_t i = JobList.size(); i--;)
			{
				auto& Job = JobList[i];

				httplib::Response Resp;
				httplib::Error Error;
				Job.CallingClient->send(Job.Request, Resp, Error);

				// Some might not have a callback
				if (Job.CompletedCallback)
					Job.CompletedCallback(Resp);

				JobList.erase(JobList.begin(), JobList.end()--);
			}
		}

		return NULL;
	}

	static httplib::Client ProxyClient;
	static httplib::Client RaceBase;

private:

	static std::vector<HttpJob> JobList;
};