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

static std::vector<std::string> Split(std::string InString, const char Delimiter)
{
    std::istringstream StringStream(InString);
    std::vector<std::string> Values;

    std::string Value;
    while (std::getline(StringStream, Value, Delimiter))
        Values.push_back(Value);

    return Values;
}

static std::vector<std::wstring> Split(std::wstring InString, const wchar_t Delimiter)
{
    std::wistringstream StringStream(InString);
    std::vector<std::wstring> Values;

    std::wstring Value;
    while (std::getline(StringStream, Value, Delimiter))
        Values.push_back(Value);

    return Values;
}

static std::vector<std::string> Split(std::string InString, std::string Delimiter)
{
    std::vector<std::string> result;

    size_t start = 0;
    size_t end = InString.find(Delimiter);

    while (end != std::string::npos) {
        result.push_back(InString.substr(start, end - start));
        result.push_back(Delimiter);
        start = end + Delimiter.length();
        end = InString.find(Delimiter, start);
    }

    result.push_back(InString.substr(start));

    return result;
}

static std::vector<std::wstring> Split(std::wstring InString, std::wstring Delimiter)
{
    std::vector<std::wstring> result;

    size_t start = 0;
    size_t end = InString.find(Delimiter);

    while (end != std::string::npos) {
        result.push_back(InString.substr(start, end - start));
        result.push_back(Delimiter);
        start = end + Delimiter.length();
        end = InString.find(Delimiter, start);
    }

    result.push_back(InString.substr(start));

    return result;
}