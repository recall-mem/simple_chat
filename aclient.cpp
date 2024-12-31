#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        std::cout << "./chat-client <ip/dns> <port>" << std::endl;
        return 0;
    }

    asio::io_context io_c;
    asio::ip::tcp::endpoint ep_;

    try
    {
        ep_ = asio::ip::tcp::endpoint(
            asio::ip::address_v4::from_string(argv[1]), 5005);
    }
    catch (...)
    {
        using tcp = asio::ip::tcp;

        tcp::resolver resolver(io_c);
        ep_ = tcp::resolver::iterator(
                  resolver.resolve(tcp::resolver::query(argv[1], argv[2])))
                  ->endpoint();
    }

    asio::ip::tcp::socket socket_(io_c);

    system::error_code ec;

    std::thread read_thread([&socket_](){

        system::error_code ecr;
        while(!ecr){

            char buff[256];

            if(socket_.available()){    
                std::size_t bytes = socket_.read_some(asio::buffer(buff), ecr);
                std::cout << std::string(buff, bytes) << std::flush;
                if(!ecr)
                {
                    std::cerr << ecr.message() << '\n';
                }
            }
        } });

    read_thread.join();

    while(!ec){
        std::string message;
        std::getline(std::cin, message);
        asio::write(socket_, asio::buffer(message), asio::transfer_all(), ec);
    }

    return 0;
}