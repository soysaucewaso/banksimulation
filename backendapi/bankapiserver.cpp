//
// Created by Sawyer Blankenship on 6/30/24.
//
#include "../sharedabstractclasses/EndpointTemplate.h"

#include <iostream>
#include "bankrequests.h"
using namespace Pistache;
class BankEndpoint : public EndpointTemplate
{
public:
    explicit BankEndpoint() {
        requestHandler = new BankRequests();
    }
    // default destructor works
    
    
private:
    void initRoutes() override{
        using namespace Rest;
        router = new Router();
        Routes::Get(*router, "/balance/:username", Routes::bind(&BankEndpoint::getBalance,this));
        Routes::Post(*router, "/deposit/:username/:amt", Routes::bind(&BankEndpoint::deposit,this));
        Routes::Post(*router, "/withdraw/:username/:amt", Routes::bind(&BankEndpoint::withdraw, this));
    }
    void getBalance(const Rest::Request& req, Http::ResponseWriter resp){
        auto uname = req.param(":username").as<std::string>();
        auto balance = requestHandler->getBalance(uname);
        if (balance == -1){
            resp.send(Http::Code::Not_Found);
        }
        else{resp.send(Http::Code::Ok, to_string(balance));}
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
        auto uname = req.param(":username").as<std::string>();
        double amt;
        try {
            amt = req.param(":amt").as<double>();
        }catch(exception &e){
            cout << e.what();
            return;}
        string reqResp;
        Http::Code code = requestHandler->withdraw(uname,amt, reqResp);
        resp.send(code,reqResp);
    }
    BankRequests* requestHandler;
};

int main(){
    // server
    Address addr(Ipv4::any(), Port(9080));
    auto opts = Http::Endpoint::options().threads(5);
    std::cout << std::this_thread::get_id()<<std::endl;
    EndpointTemplate* server = new BankEndpoint();
    server->init(addr,opts);
    
    // tls
    //server.useSSL();
    //server.useSSLAuth();
    
    
    std::this_thread::sleep_for(std::chrono::seconds(50));
    
    return 0;
}

