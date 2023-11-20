#pragma once

#include "Common.h"

struct HTTPRequest
{
public:
	explicit HTTPRequest(std::string _data);

	HTTPRequest()						= default;	
	~HTTPRequest()						= default; 
	HTTPRequest(const HTTPRequest& _other)			= default;
	HTTPRequest& operator=(const HTTPRequest& _other)	= default;
	HTTPRequest(HTTPRequest&& _other)			= default;
	HTTPRequest& operator=(HTTPRequest&& _other)		= default;
	
	bool isValid() const;
	
public:
	Method					m_method;
	std::string				m_path;
	Version					m_version;
	std::map<std::string, std::string>	m_headers;
	std::string				m_body;
};
