//
// Created by Sawyer Blankenship on 7/8/24.
//

#include "EndpointTemplate.h"

namespace Pistache {
    
} // Pistache

void EndpointTemplate::init(Address addr, Http::Endpoint::Options opts) {
    initRoutes();
    //must be last (blocks)
    initEndpoint(addr, opts);
}

void EndpointTemplate::initEndpoint(Address addr, Http::Endpoint::Options opts) {
    Pistache::Http::Endpoint* server = new Http::Endpoint(addr);
    server->init(opts);
    server->setHandler(router->handler());
    server->serveThreaded();
}
