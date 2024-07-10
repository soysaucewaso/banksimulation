//
// Created by Sawyer Blankenship on 6/30/24.
//
#include "../sharedabstractclasses/EndpointTemplate.h"

#include <iostream>
#include <thread>
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
        Routes::Post(*router, "/transact/:srcname/:destname/:amt", Routes::bind(&BankEndpoint::transact,this));
        Routes::Get(*router, "/token/:srcname/:destname/:amt", Routes::bind(&BankEndpoint::getToken, this));
    }
    void getBalance(const Rest::Request& req, Http::ResponseWriter resp){
        auto uname = req.param(":username").as<std::string>();
        auto balance = requestHandler->getBalance(uname);
        if (balance == -1){
            resp.send(Http::Code::Not_Found);
        }
        else{resp.send(Http::Code::Ok, to_string(balance));}
        sleep(2);
    }
    void transact(const Rest::Request& req, Http::ResponseWriter resp){
        auto srcname = req.param(":srcname").as<string>();
        auto destname = req.param(":destname").as<string>();
        double amt;
        try {
           amt = req.param(":amt").as<double>();
        }catch(exception &e){ 
            cout << e.what();
            return;
        }
        string reqResp;
        Http::Code code = requestHandler->transact(srcname,destname,amt, reqResp);
        resp.send(code,reqResp);
    }
    void getToken(const Rest::Request& req, Http::ResponseWriter resp){
        auto srcname = req.param(":srcname").as<string>();
        auto destname = req.param(":destname").as<string>();
        double amt;
        try {
            amt = req.param(":amt").as<double>();
        }catch(exception &e){
            cout << e.what();
            return;
        }
        string token = "";
        Http::Code code = requestHandler->getToken(srcname,destname,amt,token);
        resp.send(code,token);
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

