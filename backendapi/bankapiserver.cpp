//
// Created by Sawyer Blankenship on 6/30/24.
//
#include <iostream>
#include <pistache/endpoint.h>

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
        server->setHandler(router->handler());
        server->serveThreaded();
    }
    // default destructor works
    
    
private:
    void initRoutes(){
        using namespace Rest;
        router = new Router();
        Routes::Get(*router, "/balance/:username", Routes::bind(&BankEndpoint::getBalance,this));
        Routes::Post(*router, "/deposit/:username/:amt", Routes::bind(&BankEndpoint::deposit,this));
        Routes::Post(*router, "/withdraw/:username/:amt", Routes::bind(&BankEndpoint::withdraw, this));
    }
    void getBalance(const Rest::Request& req, Http::ResponseWriter resp){
        auto uname = req.param(":username").as<std::string>();
        auto balance = requestHandler->getBalance(uname);
        cout << balance;
        resp.send(Http::Code::Ok, to_string(balance));
    }
    void deposit(const Rest::Request& req, Http::ResponseWriter resp){
        auto uname = req.param(":username").as<std::string>();
        double amt;
        try {
           amt = req.param(":amt").as<double>();
        }catch(exception &e){ 
            cout << e.what();
            
            return;}
        string reqResp;
        Http::Code code = requestHandler->deposit(uname,amt, reqResp);
        resp.send(code,reqResp);
    }
    void withdraw(const Rest::Request& req, Http::ResponseWriter resp){
        
    }
    Http::Endpoint* server;
    Rest::Router* router;
    BankRequests* requestHandler;
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
    std::cout << std::this_thread::get_id()<<std::endl;
    BankEndpoint server(addr, opts);
    
    // tls
    //server.useSSL();
    //server.useSSLAuth();
    
    
    std::this_thread::sleep_for(std::chrono::seconds(50));
    
    return 0;
}

