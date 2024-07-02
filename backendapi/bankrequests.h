//
// Created by Sawyer Blankenship on 7/1/24.
//
#include <string>
#include <pistache/http.h>

#include <pqxx/pqxx>

#ifndef BACKENDAPI_BANKREQUESTS_H
#define BACKENDAPI_BANKREQUESTS_H

using namespace std;

class BankRequests {
public:
    double getBalance(string &username);

    bool withdraw(string &username, int amt, string& resp);

    Pistache::Http::Code deposit(string &username, int amt, string& resp);
};
    

#endif //BACKENDAPI_BANKREQUESTS_H
