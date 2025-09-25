#include "HttpSystem.h"

httplib::Client HttpSystem::ProxyClient = httplib::Client("localhost:8888");
httplib::Client HttpSystem::RaceBase = httplib::Client("https://sgracers.vercel.app");
httplib::Client HttpSystem::RaceTestBase = httplib::Client("https://racertest.vercel.app");
std::vector<HttpJob> HttpSystem::JobList = {};

HttpJob::HttpJob(httplib::Client* InClient, httplib::Request InRequest, std::function<void(httplib::Response, httplib::Error)> InCompletedCallback)
	: CallingClient(InClient), Request(InRequest), CompletedCallback(InCompletedCallback)
{
	HttpSystem::JobList.push_back(*this);
}
