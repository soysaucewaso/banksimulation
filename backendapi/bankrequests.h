//
// Created by Sawyer Blankenship on 7/1/24.
//
#include <string>
#include <mutex>
#include <pistache/http.h>

#include <pqxx/pqxx>

#ifndef BACKENDAPI_BANKREQUESTS_H
#define BACKENDAPI_BANKREQUESTS_H

using namespace std;

class BankRequests {
public:
    BankRequests();
    
    double getBalance(string &username);

    Pistache::Http::Code transact(string &srcname, string& destname, int amt, string& resp);
    
    Pistache::Http::Code getToken(string& srcname, string& destname, int amt, string& token);
    
private:
    const string UPDATEQUERY = "UPDATE balance SET balance = $2 WHERE username = $1";
    pqxx::connection* conn;
    using Guard = lock_guard<mutex>;
    mutex lock;
    void initTrans();
    void sanitizeString(string& str);
    double getBalanceHelper(string& username, pqxx::work& txn);
};
    

#endif //BACKENDAPI_BANKREQUESTS_H
