#include "HttpSystem.h"

httplib::Client HttpSystem::RaceBaseURL = httplib::Client("http://racertest.vercel.app");
std::vector<HttpJob> HttpSystem::JobList = {};
//HttpSystem GHttpSystem = HttpSystem();