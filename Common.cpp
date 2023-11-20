#include "Common.h"

#include <algorithm>

Method stringToMethod(std::string _method)
{
	std::transform(_method.begin(), _method.end(), _method.begin(), [](unsigned char c){
		return std::toupper(c);
	});
	auto it = strToMethod.find(_method);
	return it != strToMethod.end() ? it->second : Method::UNKNOWN;
}

std::string methodToString(Method _method)
{
	auto it = methodToStr.find(_method);
	return it != methodToStr.end() ? it->second : "UNKNOWN";	
}

std::string getExtensionByMIME(std::string _mime)
{
	auto it = mimeToExtension.find(_mime);
	return it != mimeToExtension.end() ? it->second : ".txt";	
}

std::string getMessage(StatusCode _code)
{
	auto it = statusCodeToMessage.find(_code);
	return it != statusCodeToMessage.end() ? it->second : "Unknown.";
}

Version::Version(std::string _data)
{
	if(_data.substr(0, versionPrefix.size()) == versionPrefix)
	{
		std::stringstream	stream(_data.substr(versionPrefix.size()));
		char			delim;
		stream >> m_major >> delim >> m_minor;
	}
}

bool Version::isValid() const
{
	return m_major > 0 && m_minor > 0;
}
