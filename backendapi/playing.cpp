//
// Created by Sawyer Blankenship on 6/30/24.
//
#include <iostream>
#include <pistache/endpoint.h>
#include <pthread.h>
using namespace Pistache;

class HelloHandler : public Http::Handler {
public:
    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) override{
        std::cout << std::this_thread::get_id()<<std::endl;
        response.send(Http::Code::Ok, "Hello World\n");
    }
};


int main(){
    Address addr(Ipv4::any(), Port(9080));
    auto opts = Http::Endpoint::options().threads(20);

    auto server = std::make_shared<Http::Endpoint>(addr);
    auto* serverPtr = &server;
    server->init(opts);
    server->setHandler(Http::make_handler<HelloHandler>());
    server->serve();
    return 0;
}

