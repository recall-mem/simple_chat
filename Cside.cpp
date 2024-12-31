#define BOOST_ASIO_STANDALONE

#include <thread>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

void async_read_(asio::ip::tcp::socket* socket)
{   
    auto buffer = std::make_shared<std::vector<char>>(256);
    
    socket->async_read_some(asio::buffer(buffer->data(), buffer->size()), [buffer, socket](system::error_code ec, size_t){
        if(!ec)
        {
            for(auto t : *buffer){
                std::cout << t;
            }   std::cout << std::endl;

            async_read_(socket);
        }

        else
        {
            std::cerr << ec << std::endl;
        }
    });
}

int main()
{   
    asio::io_context context;

    boost::system::error_code ec;

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address_v4("127.0.0.1"), 5005);

    asio::ip::tcp::socket socket(context);

    
    socket.open(asio::ip::tcp::v4());

    socket.connect(endpoint, ec);
    

    if(ec)
    {
        std::cerr << ec.message() << '\n';
        return 1;
    }
    
    async_read_(&socket);
    
    std::thread contextTh([&context](){
        context.run();
    });

    contextTh.join();


    std::string message;
    while(!ec){
        std::getline(std::cin, message);

        asio::write(socket, asio::buffer(message), ec);
    }

    std::cout << ec.message() << std::endl;

    return 0;
}