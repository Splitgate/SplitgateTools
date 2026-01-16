#pragma once

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>

namespace Strings
{
	std::wstring StringToWide(const std::string& s);
	std::string WideToString(const std::wstring& s);
	
	template <typename T>
	T Uppercase(const T& str)
	{
		T upper;
		std::transform(str.begin(), str.end(), std::back_inserter(upper), toupper);
		return upper;
	}

	template <typename T>
	T Lowercase(const T& str)
	{
		T upper;
		std::transform(str.begin(), str.end(), std::back_inserter(upper), tolower);
		return upper;
	}
	
	std::string StringToHexString(std::string& input);
		
	std::vector<std::string> Split(std::string InString, const char Delimiter);
	std::vector<std::wstring> Split(std::wstring InString, const wchar_t Delimiter);
	std::vector<std::string> Split(std::string InString, std::string Delimiter, bool bCullEmpty = true);
	std::vector<std::wstring> Split(std::wstring InString, std::wstring Delimiter, bool bCullEmpty = true);
	
	CHAR NibbleToChar(uint8_t Num);
	
	void ByteToHex(uint8_t In, std::string& Result);
	std::string BytesToHex(const uint8_t* In, int32_t Count);
	
	// start StringToType
	template<typename T>
	T StringToType(const std::string& InString);
	
	template<>
	int StringToType<int>(const std::string& InString);
		
	template<>
	std::string StringToType<std::string>(const std::string& InString);
	
	template<>
	bool StringToType<bool>(const std::string& InString);
		
	template<>
	double StringToType<double>(const std::string& InString);
		
	template<>
	float StringToType<float>(const std::string& InString);
	// end StringToType

	std::string Longest(std::vector<std::string> Vector);

	std::string Replace(std::string& String, const std::string& From, const std::string& To);
	bool ReplaceInline(std::string& String, const std::string& From, const std::string& To);
}