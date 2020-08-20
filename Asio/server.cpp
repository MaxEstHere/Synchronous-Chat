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
	//io_service
	io_service service;
	tcp::acceptor serve_acc(service, tcp::endpoint(tcp::v4(), 9999));
	//connection
	tcp::socket serve_socket(service);
	serve_acc.accept(serve_socket);
	//read end delete "\n"
	std::string name = get(serve_socket);
	name.pop_back();
	//server response
	std::string res, response;
	res = "Hi, " + name + ".";
	std::cout << "Server: " << res << std::endl;
	post(serve_socket, res);

	while (true)
	{
		response = get(serve_socket);
		response.pop_back();
		if (response == "exit")
		{
			std::cout << name << " left." << std::endl;
			break;
		}
		std::cout << name << ": " << response << std::endl;
		std::cout << "Server: ";
		getline(std::cin, res);
		post(serve_socket, res);
		if (res == "exit") break;
	}
	return 0;
}