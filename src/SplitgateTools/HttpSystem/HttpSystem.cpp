#include "HttpSystem.h"

std::vector<HttpSystem::Job> JobList;

void HttpSystem::SendRequest(const std::string& InHost, const httplib::Request& InRequest,
	const std::function<CallbackFunc>& InCallback)
{
	// flush previous requests
	std::erase_if(JobList, [](Job& J) {
		return J.bFinished;
	});
	JobList.emplace_back(InHost, InRequest, InCallback);
}

DWORD HttpSystem::Thread(LPVOID)
{
	while (true)
	{
		Sleep(100);
		
		for (auto& Job : JobList)
		{
			httplib::Client Client{Job.Host};
			httplib::Response Resp;
			httplib::Error Error;
			
			if (Client.is_valid())
				Client.send(Job.Request, Resp, Error);
			else
			{
				Resp.body = "http client was invalid";
				Error = httplib::Error::Canceled;
			}

			if (Job.Callback)
				Job.Callback(Resp, Error);

			Job.bFinished = true;
		}
	}
}
