#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "HTTPServer.h"
#include "HTTPRequest.h"

namespace fs = std::filesystem;

int HTTPServer::setup()
{
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_socket == -1)
	{
		std::cerr << "socket(...) failed!" << std::endl;
		return -1;
	}

	struct sockaddr_in servAddr;
	servAddr.sin_family		= AF_INET;
	servAddr.sin_addr.s_addr	= INADDR_ANY;
	servAddr.sin_port		= htons(m_port);

	if(bind(m_socket, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr)) == -1)
	{
		std::cerr << "bind(...) failed!" << std::endl;
		close(m_socket);
		return -1;
	}

	if(listen(m_socket, 10))
	{
		std::cerr << "listen(...) failed!" << std::endl;
		close(m_socket);
		return -1;
	}
	
	return 0;
}

std::string HTTPServer::getFullPath(const HTTPRequest& _req)
{
	auto		mime		= _req.m_headers.find("Content-Type");
	std::string	extension	= getExtensionByMIME(mime != _req.m_headers.end() ? mime->second : "");
	return m_rootDirectory + _req.m_path + extension;
}

HTTPResponse HTTPServer::exec(const HTTPRequest& _req)
{
	if(_req.m_method == Method::GET)		{ return get(_req); }
	else if(_req.m_method == Method::POST)		{ return post(_req); }
	else if(_req.m_method == Method::DELETE)	{ return del(_req); }
	return HTTPResponse();
}

HTTPResponse HTTPServer::get(const HTTPRequest& _req)
{
	std::string fullPath = getFullPath(_req);
	if(fs::exists(fullPath))
	{
		std::ifstream file(fullPath);
		if(file.is_open())
		{
			std::string	content;
			std::string	line;
			while(std::getline(file, line))
			{
				content += line + "\n";
			}
			file.close();
			return HTTPResponse{ _req.m_version, StatusCode::SUCCESS, content };
		}
		else
		{
			return HTTPResponse{ _req.m_version, StatusCode::INTERNAL_SERVER_ERROR, {} };
		}
	}

	return HTTPResponse{ _req.m_version, StatusCode::NOT_FOUND, {} };
}

HTTPResponse HTTPServer::post(const HTTPRequest& _req)
{
	std::string	fullPath	= getFullPath(_req);
	fs::path	dir		= fs::path(fullPath).parent_path();
	if(!fs::exists(dir))
	{
		if(!fs::create_directories(dir))
		{
			return HTTPResponse{ _req.m_version, StatusCode::INTERNAL_SERVER_ERROR, {} };
		} 
	}
	
	std::ofstream file(fullPath);
	if(file.is_open())
	{
		file << _req.m_body;
		file.close();
		return HTTPResponse{ _req.m_version, StatusCode::SUCCESS, {} };
	}

	return HTTPResponse{ _req.m_version, StatusCode::INTERNAL_SERVER_ERROR, {} };
}

HTTPResponse HTTPServer::del(const HTTPRequest& _req)
{
	std::string fullPath = getFullPath(_req);
	if(fs::exists(fullPath))
	{
		if(fs::remove(fullPath))
		{
			return HTTPResponse{ _req.m_version, StatusCode::SUCCESS, {} };
		}
		else
		{
			return HTTPResponse{ _req.m_version, StatusCode::INTERNAL_SERVER_ERROR, {} };
		}
	}

	return HTTPResponse{ _req.m_version, StatusCode::NOT_FOUND, {} };
}

void HTTPServer::run()
{
	std::cout << "Server is starting..." << std::endl;

	if(setup() == -1) { return; }

	std::vector<pollfd> fds;
	fds.push_back({m_socket, POLLIN});

	while(true)
	{
		int res = poll(fds.data(), fds.size(), -1);
		if(res == -1)
		{
			std::cerr << "poll(...) failed!" << std::endl;
			break;
		}

		if(fds[0].revents & POLLIN)
		{
			int clSocket = accept(m_socket, nullptr, nullptr);
			if(clSocket == -1)
			{
				std::cerr << "accept(...) failed!" << std::endl;
			}
			else
			{
				fds.push_back({clSocket, POLLIN});
				std::cout << "New client connection was accepted!" << std::endl;
			}
		}

		for(uint32_t i = 1; i < fds.size(); ++i)
		{
			if(fds[i].revents & POLLIN)
			{
				char buff[1024];
				memset(buff, 0, sizeof(buff));
				int bytesRecv = recv(fds[i].fd, buff, sizeof(buff), 0);
				if(bytesRecv > 0)
				{
					HTTPRequest 	req(buff);
					HTTPResponse	resp;
					if(req.isValid())
					{
						resp = exec(req);
					}
					
					std::string msg = resp.toString();
					if(resp.isValid())
					{
						send(fds[i].fd, msg.c_str(), msg.size(), 0);
					}
				}

				close(fds[i].fd);
				fds.erase(fds.begin() + i);
				--i;
			}
		}
	}
	
	for(uint32_t i = 0; i < fds.size(); ++i) { close(fds[i].fd); }
}

