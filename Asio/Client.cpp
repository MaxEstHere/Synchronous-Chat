#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

std::string get(tcp::socket& socket)
{
	streambuf buf;
	read_until(socket, buf, "\n");
	std::string data = buffer_cast<const char*>(buf.data());
	return data;
}
void post(tcp::socket& socket, const std::string& mess)
{
	write(socket, buffer(mess + "\n"));
}

int main(int argc, char* argv[])
{
	io_service service;
	tcp::socket client_socket(service);
	//connect client
	client_socket.connect(tcp::endpoint(address::from_string("127.0.0.1"), 9999));
	//send name
	std::cout << "Enter your name, pls: " << std::endl;
	std::string name, res, response;
	getline(std::cin, name);

	post(client_socket, name);
	//loop for chat
	while (true)
	{
		response = get(client_socket);
		//delete "\n"
		response.pop_back();
		if (response == "exit")
		{
			std::cout << "Error connect" << std::endl;
			break;
		}
		std::cout << "Server: " << response << std::endl;
		//read new mess
		std::cout << name << ": ";
		getline(std::cin, res);
		post(client_socket, res);
		if (res == "exit") break;
	}
	return 0;
}