//
// Created by Sawyer Blankenship on 7/1/24.
//
#include <string>
#ifndef BACKENDAPI_BANKREQUESTS_H
#define BACKENDAPI_BANKREQUESTS_H

namespace std {

    class bankrequests {
    public:
        double getBalance(string& username);
        
        bool withdraw(string& username, int amt);
        
        bool deposit(string& username, int amt);
    };

} // std

#endif //BACKENDAPI_BANKREQUESTS_H
