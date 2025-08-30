#pragma once

#include "httplib/httplib.h"

// TO:DO: Remove and replace with just using httplib::clients
enum ClientToUse_Temp
{
	Race = 0,
	Forge = 1
};

class HttpJob
{
public:

	HttpJob()
	{}

	HttpJob(ClientToUse_Temp InClient, std::function<void(httplib::Client)> InRequestFunction, std::function<void(std::string)> OnCompleted = nullptr)
		: Client(InClient), RequestFunction(InRequestFunction), CompletedCallback(OnCompleted)
	{ };

private:

	ClientToUse_Temp Client;
	std::function<void(httplib::Client)> RequestFunction;
	std::function<void(std::string)> CompletedCallback;
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
			for (auto i = JobList.size(); i--;)
			{

			}
		}

		return NULL;
	}

	static httplib::Client RaceBaseURL;

private:

	static std::vector<HttpJob> JobList;
};
//static HttpSystem GHttpSystem;

//DWORD WINAPI HttpThread(LPVOID)
//{
//	while (true)
//	{
//		for (auto& OutstandingRequest : HttpRequestList)
//		{
//			// This is blocking
//			OutstandingRequest();
//
//			// Remove the request since its done
//			HttpRequestList.erase(std::remove_if(HttpRequestList.begin(), HttpRequestList.end(),
//				[&](const std::function<void()>& func) {
//					return func.target_type() == OutstandingRequest.target_type() &&
//						func.target<void()>() == OutstandingRequest.target<void>();
//				}));
//
//			UE_LOG(LogThreads, Warning, "Executed event, removing!");
//		}
//	}
//
//	return NULL;
//}