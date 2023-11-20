#include "HTTPResponse.h"

bool HTTPResponse::isValid() const
{
	return m_version.isValid();
}

std::string HTTPResponse::toString() const
{
	std::stringstream stream;
	stream << versionPrefix << m_version.m_major << "." << m_version.m_minor << " ";
	stream << static_cast<int32_t>(m_status) << " ";
	stream << getMessage(m_status) << std::endl;
	
	for(const auto& it: m_headers)
	{
		stream << it.first << ": " << it.second << std::endl;
	}
	
	stream << std::endl;
	stream << m_body;
	
	return stream.str();
}
