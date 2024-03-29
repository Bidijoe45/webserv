#include <stdio.h>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include <poll.h>

#include <iostream>
#include <cstring>

#include "server.hpp"
#include "connection.hpp"
#include "../http/http_parser.hpp"
#include "../http/http_request.hpp"
#include "../settings/parser/settings_parser.hpp"
#include "server_socket.hpp"
#include "../utils/content_type_map.hpp"
#include "../http/request_resolver/request_resolver.hpp"
#include "http_header_map.hpp"
#include "http_headers.hpp"

namespace ws
{
	bool Server::running = false;

	Server::Server()
	{
		this->content_types_.parse_content_types_file("srcs/utils/content_types_list.txt");
		this->http_message_map_ = HttpMessageMap();
	}

	Server::~Server()
	{
		for (size_t i=0; i < poll_.size(); i++)
			close(poll_[i].fd);
	}

	Connection Server::accept_new_connection(int socket, int port) {
		Connection new_conn;

		new_conn.addr_len = sizeof(struct sockaddr_storage);
		new_conn.socket = accept(socket, (struct sockaddr *)&new_conn.addr, &new_conn.addr_len);
		new_conn.port = port;
		this->connections_.insert(std::make_pair(new_conn.socket, new_conn));

		return new_conn;
	}

	int Server::listen_on(int port) {
		int yes=1;        
		int rv;
		char port_str[5];
		ServerSocket server_socket;
		struct addrinfo hints, *ai, *p;

		server_socket.port = port;
		sprintf(port_str, "%d", port);
		memset(&hints, 0, sizeof hints);

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		if ((rv = getaddrinfo(NULL, port_str, &hints, &ai)) != 0) {
			std::cout << "Error: get address info" << std::endl;
			exit(1);
		}
		
		for(p = ai; p != NULL; p = p->ai_next) {
			server_socket.socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (server_socket.socket < 0) { 
				continue;
			}

			setsockopt(server_socket.socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
			fcntl(server_socket.socket, F_SETFL,O_NONBLOCK);

			if (bind(server_socket.socket, p->ai_addr, p->ai_addrlen) < 0) {
				close(server_socket.socket);
				continue;
			}

			break;
		}

		if (p == NULL) {
			return -1;
		}

		freeaddrinfo(ai);

		if (listen(server_socket.socket, LISTEN_BACKLOG) == -1) {
			return -1;
		}

		this->server_sockets_.push_back(server_socket);

		return 0;
	}

	void Server::listen_all()
	{
		std::vector<int>::iterator it = this->ports_.begin();
		std::vector<int>::iterator ite = this->ports_.end();

		for (; it != ite; it++)
		{
			if (this->listen_on(*it) != 0)
			{
				std::cout << "Could not listen on port: " << *it << std::endl;
				exit(1);
			}
		}
		

		it = this->ports_.begin();

		std::cout << "Server listening on:";
		for (; it != ite; it++)
			std::cout << " " << *it;
		std::cout << std::endl;
	}

	std::vector<ServerSocket>::iterator Server::get_server_socket(int socket)
	{
		std::vector<ServerSocket>::iterator it = this->server_sockets_.begin();
		std::vector<ServerSocket>::iterator ite = this->server_sockets_.end();

		for (; it!=ite; it++)
		{
			if ((*it).socket == socket)
				return it;
		}

		return this->server_sockets_.end();
	}

	void Server::set_server_sockets_to_poll()
	{
		std::vector<ServerSocket>::iterator server_sockets_it = this->server_sockets_.begin();
		std::vector<ServerSocket>::iterator server_sockets_ite = this->server_sockets_.end();
		for (; server_sockets_it != server_sockets_ite; server_sockets_it++)
		{
			struct pollfd server_poll;	
			server_poll.fd = (*server_sockets_it).socket;
			server_poll.events = POLLIN;

			this->poll_.push_back(server_poll);
		}
	}

	void Server::poll_connections()
	{
		this->set_server_sockets_to_poll();

		while (this->running)
		{
			int poll_count = poll(&this->poll_[0], this->poll_.size(), POLL_TIMEOUT);

			if (poll_count == -1)
			{
				this->running = false;
				break;
			}

			for (size_t i = 0; i < this->poll_.size() && this->running; i++)
			{
				std::vector<ServerSocket>::iterator ss_it = this->get_server_socket(this->poll_[i].fd);
				if (this->poll_[i].revents & POLLIN && ss_it != this->server_sockets_.end())
				{
					Connection new_conn = this->accept_new_connection(this->poll_[i].fd, (*ss_it).port);
					this->add_to_poll(new_conn);
					if (new_conn.socket == -1)
					{
						std::cout << "Connection: cannot accept new client connection" << std::endl;
						continue;
					}
					std::cout << "New connection from " << new_conn.get_ip_address() << std::endl;
				}
				else
				{
					Connection &conn = this->connections_[this->poll_[i].fd];

					if (this->poll_[i].revents & POLLIN && conn.send_buff.size() == 0)
					{
						ssize_t bytes_read = conn.recv_data();
						if (bytes_read == -1)
						{
							this->close_connection(conn, i, "Connection: read error");
							continue;
						}
						else if (bytes_read == 0)
						{
							this->close_connection(conn, i, "Connection: socket closed by client");
							continue;
						}

						this->on_new_request(conn);
					}
					if (this->poll_[i].revents & POLLOUT && conn.send_buff.size() > 0)
					{
						ssize_t bytes_sent = conn.send_data();
						if (bytes_sent == -1)
						{
							this->close_connection(conn, i, "Connection: send error");
							continue;
						}
					}
					if (conn.must_close == true)
					{
						this->close_connection(conn, i, "Connection: socket closed by server");
						continue;
					}
				}
			}
		}
	}

	bool Server::check_connection_header(const HttpRequest &http_request)
	{
		HttpHeaderMap::const_iterator connection_it = http_request.headers.find(HTTP_HEADER_CONNECTION);
		if (connection_it == http_request.headers.end())
			return false;
		HttpHeaderConnection *connection_header = static_cast<HttpHeaderConnection*>(connection_it->second);
		std::vector<std::string> connection_options = connection_header->options;
		if (connection_options.size() == 0)
			return false;
		if (find(connection_options.begin(), connection_options.end(), "close") == connection_options.end())
			return false;

		return true;
	}

	void Server::parse_request(Connection &connection)
	{
		connection.http_parser.append_to_buff(connection.recv_buff);
		connection.http_parser.must_close = false;
		if (connection.http_parser.get_stage() == HttpParser::REQUEST_LINE)
			connection.http_parser.parse_first_line();
		if (connection.http_parser.get_stage() == HttpParser::HEADERS_BLOCK)
			connection.http_parser.parse_headers();

		if (connection.http_parser.get_stage() > HttpParser::HEADERS_BLOCK && connection.settings_set == false)
		{
			connection.settings = this->settings_.resolve_settings_hostname(connection.http_parser.get_request(), connection.port);
			connection.http_parser.set_max_body_size(connection.settings.client_max_body_size);
			connection.settings_set = true;
		}

		if (connection.http_parser.get_stage() == HttpParser::SIMPLE_BODY)
			connection.http_parser.parse_body();
		if (connection.http_parser.get_stage() == HttpParser::CHUNKED_BODY)
			connection.http_parser.parse_chunked_body();
	}

	void Server::on_new_request(Connection &connection)
	{
		this->parse_request(connection);
		connection.must_close = connection.http_parser.must_close;
		connection.recv_buff.clear();

		if (connection.http_parser.get_stage() == HttpParser::COMPLETED)
		{

			HttpRequest http_request = connection.http_parser.get_request();
	
			RequestResolver request_resolver(connection.settings, http_request, this->env_, connection, this->content_types_, this->http_message_map_);
			HttpResponse response = request_resolver.get_response();

			std::cout << connection.get_ip_address()
				<< " -- Req: " << http_request.request_line.method_to_string() << " " << http_request.request_line.uri.absolute_path()
				<< " -- Res: " << response.status_code << " " << response.status_msg << std::endl;
 
			if (connection.must_close == false)
				connection.must_close = this->check_connection_header(http_request);

			if (connection.must_close == true)
			{
				HttpHeaderConnection *connection_header = new HttpHeaderConnection();
				connection_header->set_value("close");
				response.headers.insert(connection_header);
			}
	
			connection.send_buff.append(response.to_string());

			connection.settings_set = false;
			connection.http_parser.reset();
		}
	}

	void Server::add_to_poll(Connection new_connection)
	{
		struct pollfd new_poll;
		new_poll.fd = new_connection.socket;
		new_poll.events = POLLIN | POLLOUT;

		this->poll_.push_back(new_poll);
	}
	
	void Server::set_env(char **env)
	{
		this->env_.set_from(env);
	}

	void Server::set_config_file(const std::string &config_file)
	{
		this->config_file_ = config_file;
	}
	
	void Server::set_settings(const Settings &settings)
	{
		this->settings_ = settings;
	}

	void Server::delete_connection(const Connection &connection)
	{
		close(connection.socket);
		this->connections_.erase(connection.socket);
	}

	void Server::delete_from_poll(size_t index)
	{
		this->poll_[index] = this->poll_[this->poll_.size() - 1];
		this->poll_.erase(this->poll_.end() - 1);
	}

	void Server::close_connection(const Connection &conn, size_t index, std::string msg)
	{
		std::cout << msg << ". Client IP: " << conn.get_ip_address() << std::endl;
		this->delete_connection(conn);
		this->delete_from_poll(index);
	}

	void Server::set_ports_from_settings()
	{
		std::vector<ServerSettings>::iterator it = this->settings_.servers.begin();
		std::vector<ServerSettings>::iterator ite = this->settings_.servers.end();
		
		for(; it != ite; it++)
		{
			this->ports_.push_back((*it).port);
		}
	}

	void Server::run() {
		running = true;

		if (this->content_types_.is_valid() == false)	
		{
			std::cout << "content types list file invalid" << std::endl;
			return;
		}

		SettingsParser settings_parser(this->config_file_);

		this->settings_ = settings_parser.parse();

		if (!settings_parser.is_valid()) {
			std::cout << "server config file invalid: " << settings_parser.get_error_msg() << std::endl;
			return;
		}
		this->set_ports_from_settings();
		this->listen_all();

		this->poll_connections();
	}


} // namespace ws



