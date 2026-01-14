#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib/httplib.h"

#define RACEBASE_URL "https://sgracers.vercel.app"

class HttpSystem
{
public:
	typedef void (CallbackFunc)(httplib::Response, httplib::Error);
	class Job
	{
	public:
		Job(const std::string& InHost, const httplib::Request& InRequest, const std::function<CallbackFunc>& InCallback)
			: Host(InHost), Request(InRequest), Callback(InCallback) {}

		std::string Host;
		httplib::Request Request;
		std::function<CallbackFunc> Callback;
		bool bFinished = false;
	};

	static void SendRequest(const std::string& InHost, const httplib::Request& InRequest, const std::function<CallbackFunc>& InCallback);

	// Separate thread created in DLLMain, used for executing http requests without causing frame hitching
	static DWORD WINAPI Thread(LPVOID);
};