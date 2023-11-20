#pragma once

#include "Common.h"

struct HTTPResponse
{
public:
	explicit HTTPResponse(Version _version, StatusCode _code, std::string _body)
		: m_version(_version)
		, m_status(_code)
		, m_body(_body) { }
	
	HTTPResponse()						= default;	
	~HTTPResponse()						= default; 
	HTTPResponse(const HTTPResponse& _other)		= default;
	HTTPResponse& operator=(const HTTPResponse& _other)	= default;
	HTTPResponse(HTTPResponse&& _other)			= default;
	HTTPResponse& operator=(HTTPResponse&& _other)		= default;
	
	bool		isValid()	const;
	std::string	toString()	const;

public:
	Version					m_version;
	StatusCode				m_status;
	std::map<std::string, std::string>	m_headers;
	std::string				m_body;	
};
