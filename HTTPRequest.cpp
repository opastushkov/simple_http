#include <sstream>

#include "HTTPRequest.h"

HTTPRequest::HTTPRequest(std::string _data)
{
	std::stringstream	stream(_data);
	std::string		method,
				path,
				version,
				line;
				
	getline(stream, line);
	std::stringstream	reqLine(line);
	reqLine >> method >> path >> version;
	
	m_method	= stringToMethod(method);
	m_path		= path;
	m_version	= Version(version);
	
	while(std::getline(stream, line) && line != "\r")
	{
		uint32_t colonIndex = line.find(':');
		if(colonIndex != std::string::npos)
		{

			std::string	key	= line.substr(0, colonIndex);
			std::string	value	= line.substr(colonIndex + 2);
			value.pop_back();
			if(supportedHeaders.find(key) != supportedHeaders.end()) { m_headers[key] = value; }
		}
	}

	std::getline(stream, m_body, '\0');
}

bool HTTPRequest::isValid() const
{
	return m_method != Method::UNKNOWN && m_version.isValid();
}
