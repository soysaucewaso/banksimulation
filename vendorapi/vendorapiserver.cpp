//
// Created by Sawyer Blankenship on 7/8/24.
//
#include "../sharedabstractclasses/EndpointTemplate.h"

using namespace Pistache;

class VendorEndpoint : public EndpointTemplate
{
public:
    explicit VendorEndpoint(Address addr, Http::Endpoint::Options opts){
        
    }
private:
    void initRoutes() override{
        using namespace Rest;
        router = new Router();
        Routes::Get(*router, "/transact/:username/:amt",Routes::bind(&VendorEndpoint::validateToken, this));
    }
    void validateToken(const Rest::Request& req, Http::ResponseWriter resp){
        
    }
};
