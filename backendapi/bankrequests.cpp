//
// Created by Sawyer Blankenship on 7/1/24.
//

#include "bankrequests.h"

BankRequests::BankRequests() {
    conn = new pqxx::connection("dbname=bankbalances");
}

double BankRequests::getBalance(std::string &username) {
    sanitizeString(username);
    Guard g(lock);
    pqxx::work txn(*conn);
    double bal = getBalanceHelper(username,txn);
    txn.commit();
    return bal;
}

Pistache::Http::Code BankRequests::withdraw(std::string &username, int amt, std::string &resp) {
    Guard g(lock);
    pqxx::work txn(*conn);
    double bal = getBalanceHelper(username,txn);
    double newBal = bal - amt;
    try{
        if (bal == -1){
            resp = "Username not Found\n";
        }else if(newBal < 0){
            resp = to_string(bal);
        }else{
            txn.exec_params(UPDATEQUERY,username,newBal);
            resp = to_string(newBal);
            txn.commit();
            return Pistache::Http::Code::Accepted;
        }
    }catch(std::exception& e){
        resp = "WITHDRAW DB error: " + string(e.what()) + '\n';
        cout <<  resp;
        
    }txn.commit();
    return Pistache::Http::Code::Bad_Request;
}

Pistache::Http::Code BankRequests::deposit(string &username, int amt, string &resp) {
    
    Guard g(lock);
    pqxx::work txn(*conn);
    double bal = getBalanceHelper(username,txn);
    try {
        if (bal == -1) {
            const string addquery = "INSERT INTO balance (username, balance) VALUES ($1, $2)";
            txn.exec_params(addquery,username,amt);
            resp = to_string(amt);
        }else {
            bal += amt;
            txn.exec_params(UPDATEQUERY,username,bal);
            resp = to_string(bal);
        }
        
    } catch (const exception& e){
        resp = "DEPOSIT DB error: " + string(e.what()) + '\n';
        cout <<  resp;
        txn.commit();
        return Pistache::Http::Code::Bad_Request;
    }
    txn.commit();
    return Pistache::Http::Code::Accepted;
}

double BankRequests::getBalanceHelper(string &username, pqxx::work& txn) {
    sanitizeString(username);
    try {
        const string query = "SELECT balance FROM balance WHERE username = $1";
        pqxx::result res = txn.exec_params(query,username);
        auto bal = res[0][0];
        return res[0][0].as<double>();
    } catch (const exception& e){
        cout << "GET DB error: " << e.what() << endl;
    }
    return -1;
}
void BankRequests::sanitizeString(std::string &str) {
    str = conn->esc(str);
}

