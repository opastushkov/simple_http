#pragma once

#include <sstream>
#include <map>
#include <unordered_set>

enum class Method
{
	GET,
	POST,
	DELETE,
	UNKNOWN
};

const std::map<std::string, Method> strToMethod =
{
	{ "GET", Method::GET },
	{ "POST", Method::POST },
	{ "DELETE", Method::DELETE },
};

const std::map<Method, std::string> methodToStr =
{
	{ Method::GET, "GET" },
	{ Method::POST, "POST" },
	{ Method::DELETE, "DELETE" },
};

const std::map<std::string, std::string> mimeToExtension =
{
	{ "text/plain", ".txt" },
	{ "text/html", ".html" },
	{ "application/json", ".json" },
};

std::string getExtensionByMIME(std::string _mime);

enum class StatusCode : uint32_t
{
	SUCCESS			= 200,
	NOT_FOUND		= 404,
	INTERNAL_SERVER_ERROR	= 500,
};

const std::map<StatusCode, std::string> statusCodeToMessage =
{
	{ StatusCode::SUCCESS, "Success." },
	{ StatusCode::NOT_FOUND, "Not found." },
	{ StatusCode::INTERNAL_SERVER_ERROR, "Internal server error." },
};

std::string getMessage(StatusCode _code);

const std::unordered_set<std::string> supportedHeaders =
{
	"Content-Type",
	"Content-Length",
};

const std::string versionPrefix = "HTTP/";

Method stringToMethod(std::string _method);
std::string methodToString(Method _method);

struct Version
{
public:
	explicit Version(std::string _data);
	Version()					= default;	
	~Version()					= default; 
	Version(const Version& _other)			= default;
	Version& operator=(const Version& _other)	= default;
	Version(Version&& _other)			= default;
	Version& operator=(Version&& _other)		= default;

	bool isValid() const;
	
public:
	int32_t	m_major = -1;
	int32_t m_minor	= -1;
};
