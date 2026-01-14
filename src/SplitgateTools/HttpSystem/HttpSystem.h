#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib/httplib.h"

#define RACEBASE_URL "https://sgracers.vercel.app"

class HttpJob
{
	friend class HttpSystem;

public:

	HttpJob(const std::string& InHost, httplib::Request InRequest, std::function<void(httplib::Response, httplib::Error)> InCompletedCallback);

private:

	std::string Host;
	httplib::Request Request;

	typedef void (CompletedCallbackFunc)(httplib::Response, httplib::Error);
	std::function<CompletedCallbackFunc> CompletedCallback;
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
#pragma warning( push )
	#pragma warning( disable : 4267 )
			for (int i = JobList.size() - 1; i >= 0; --i) // Disable warning as a hack, size_t causes crash whereas int does not
#pragma warning( pop )
			{
				auto& Job = JobList[i];

				std::unique_ptr<httplib::Client> Client = std::make_unique<httplib::Client>(Job.Host);

				httplib::Response Resp;
				httplib::Error Error;
				if (Client && Client->is_valid())
					Client->send(Job.Request, Resp, Error);
				else
				{
					Resp.body = "Job.CallingClient was invalid";
					Error = httplib::Error::Canceled;
				}


				// Some might not have a callback
				if (Job.CompletedCallback)
					Job.CompletedCallback(Resp, Error);

				Client.release();
				JobList.erase(JobList.begin() + i);
			}
		}

		return NULL;
	}

	//static httplib::Client ProxyClient;
	//
	//// Race endpoint base urls
	//static httplib::Client RaceBase;
	//static httplib::Client RaceTestBase;

private:

	// Http Jobs
	static std::vector<HttpJob> JobList;
};