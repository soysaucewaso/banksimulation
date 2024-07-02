//
// Created by Sawyer Blankenship on 6/30/24.
//
#include <iostream>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <thread>
#include "bankrequests.h"
using namespace Pistache;

class BankEndpoint
{
public:
    explicit BankEndpoint(Address addr, Http::Endpoint::Options opts) {
        server = new Http::Endpoint(addr);
        server->init(opts);
        
        initRoutes();
        server->setHandler(router.handler());
        server->serveThreaded();
    }
    // default destructor works
    
    
private:
    void initRoutes(){
        using namespace Rest;
        Routes::Get(router, "/balance/:username", Routes::bind(&BankEndpoint::getBalance,this));
    }
    void getBalance(const Rest::Request& req, Http::ResponseWriter resp){
        auto uname = req.param(":username").as<std::string>();
        
    }
    Http::Endpoint* server;
    Rest::Router router;
};
class HelloHandler : public Http::Handler {
public:
    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& req, Http::ResponseWriter resp) override{
        std::cout << std::this_thread::get_id()<<std::endl;
        if (req.resource() == "/ping"){
            resp.send(Http::Code::Ok, "PONG");
        }
        else if (req.resource().substr(0,12) == "/getbalance/" && req.method() == Http::Method::Get){
            std::cout << req.resource();
            
            resp.send(Http::Code::Accepted);
        }else{
            resp.send(Http::Code::Not_Found);
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
};


int main(){
    // server
    Address addr(Ipv4::any(), Port(9080));
    auto opts = Http::Endpoint::options().threads(5);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(Http::make_handler<HelloHandler>());
    
    // tls
    //server.useSSL();
    //server.useSSLAuth();
    
    std::cout << std::this_thread::get_id()<<std::endl;
    server.serveThreaded();
    /*std::thread serverThread([server](){
        std::cout << std::this_thread::get_id()<<std::endl;
        server->serve();
    });*/
    std::this_thread::sleep_for(std::chrono::seconds(50));
    
    server.shutdown();
    return 0;
}

