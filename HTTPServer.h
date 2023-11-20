#pragma once

#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <thread>
#include <mutex>
#include <poll.h>

#include "HTTPRequest.h"
#include "HTTPResponse.h"

using Task = std::pair<int, HTTPRequest>;
using Response = std::pair<int, HTTPResponse>;

class HTTPServer
{
public:
	HTTPServer()	= default;
	~HTTPServer()	= default;

	void	run();
	void	job();

private:
	int		setup();
	std::string	getFullPath(const HTTPRequest& _req);
	HTTPResponse	exec(const HTTPRequest& _req);
	HTTPResponse	get(const HTTPRequest& _req);
	HTTPResponse	post(const HTTPRequest& _req);
	HTTPResponse	del(const HTTPRequest& _req);

private:
	int			m_socket;
	std::string		m_rootDirectory = ".";
	std::vector<int>	m_clients;
	uint16_t		m_port		= 1111;
	std::mutex		m_tasksMutex;
	std::queue<Task>	m_tasks;
	std::mutex		m_finishedMutex;
	std::queue<Response>	m_finished;
	std::thread		m_worker;
	bool			m_isRunning	= true;
	std::vector<pollfd>	m_fds;
};
