#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <istream>

using namespace boost;

void handle_client(asio::ip::tcp::socket& socket){
    socket.async_read_some();
}

int main(){

    asio::io_context context_;
    asio::io_context::work idleWork(context_);
    asio::ip::tcp::endpoint ep_(asio::ip::address_v4::any(), 5005);
    asio::ip::tcp::acceptor acceptor_(context_, ep_);
    asio::ip::tcp::socket   socket_(context_);

    const std::size_t max_thread_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for(std::size_t i = 0; i < max_thread_count; i++){
        threads.emplace_back([&socket_, &acceptor_, &context_](){
            acceptor_.accept(socket_, ec);
            handle_client(socket_)
        });
    } 

    return 0;
} 