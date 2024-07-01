//
// Created by Sawyer Blankenship on 6/30/24.
//
#include <iostream>
#include <pistache/endpoint.h>
#include <thread>
using namespace Pistache;

class HelloHandler : public Http::Handler {
public:
    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override{
        
        std::cout << std::this_thread::get_id()<<std::endl;
        response.send(Http::Code::Ok, "Hello World\n");
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
};


int main(){
    Address addr(Ipv4::any(), Port(9080));
    auto opts = Http::Endpoint::options().threads(5);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(Http::make_handler<HelloHandler>());
    std::cout << std::this_thread::get_id()<<std::endl;
    server.serveThreaded();
    /*std::thread serverThread([server](){
        std::cout << std::this_thread::get_id()<<std::endl;
        server->serve();
    });*/
    std::this_thread::sleep_for(std::chrono::seconds(15));
    
    server.shutdown();
    return 0;
}

