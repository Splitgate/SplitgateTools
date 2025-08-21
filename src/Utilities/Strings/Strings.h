#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <time.h>

static std::wstring StringToWide(const std::string& s)
{
	const int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


static std::string WideToString(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string r(buf);
	delete[] buf;
	return r;
}

template <typename T> // ONLY INTENDED FOR STD::STRING AND STD::WSTRING
static T Uppercase(const T& str)
{
	T upper;
	std::transform(str.begin(), str.end(), std::back_inserter(upper), toupper);
	return upper;
}

template <typename T> // ONLY INTENDED FOR STD::STRING AND STD::WSTRING
static T Lowercase(const T& str)
{
	T upper;
	std::transform(str.begin(), str.end(), std::back_inserter(upper), tolower);
	return upper;
}

static std::string StringToHexString(std::string& input) {
	std::ostringstream hexStream;
	for (char c : input) {
		hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
	}
	return hexStream.str();
}