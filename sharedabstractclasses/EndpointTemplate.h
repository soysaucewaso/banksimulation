//
// Created by Sawyer Blankenship on 7/8/24.
//

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <thread>

#ifndef BACKENDAPI_ENDPOINTTEMPLATE_H
#define BACKENDAPI_ENDPOINTTEMPLATE_H

using namespace Pistache;
// semi abstract class
class EndpointTemplate{
public:
    // call after construction
    virtual void init(Address addr, Http::Endpoint::Options opts);
protected:
    // even functions with implementations should be virtual
    
    // no constructor
    
    // by default don't override
    // by default start server
    virtual void initEndpoint(Address addr, Http::Endpoint::Options opts);
    
    virtual void initRoutes() = 0;
    
    
    Http::Endpoint* server;
    Rest::Router* router;
};

#endif //BACKENDAPI_ENDPOINTTEMPLATE_H
