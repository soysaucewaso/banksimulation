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

Pistache::Http::Code BankRequests::transact(std::string &srcname, std::string &destname, double amt, std::string &resp) {
    Guard g(lock);
    pqxx::work txn(*conn);
    double srcbal = getBalanceHelper(srcname,txn);
    double destbal = getBalanceHelper(destname,txn);
    double newBal = srcbal - amt;
    bool withdrawSuccess = false;
    try{
        // validate withdrawal
        if (srcbal == -1){
            resp = "Username not Found\n";
        }else if(newBal < 0){
            resp = "Transaction cancelled. Not enough funds: "+to_string(srcbal)+'\n';
        }else{
            withdrawSuccess = true;
        }
        // move money
        if (withdrawSuccess) {
            txn.exec_params(UPDATEQUERY, srcname, newBal);
            if(destbal == -1){
                const string addquery = "INSERT INTO balance (username, balance) VALUES ($1, $2)";
                txn.exec_params(addquery,destname,amt);
            }else{
                double newDBal = destbal + amt;
                txn.exec_params(UPDATEQUERY,destname,newDBal);
            }
            resp = "Transaction successful. "+srcname +" now has " + to_string(newBal)+'\n';
            txn.commit();
            return Pistache::Http::Code::Accepted;
        }else{
            txn.abort();
            return Pistache::Http::Code::Bad_Request;
        }
    }catch(std::exception& e){
        resp = "DB Transact error: " + string(e.what()) + '\n';
        cout <<  resp;
        txn.abort();
        return Pistache::Http::Code::Bad_Request;
    }
    
}
Pistache::Http::Code BankRequests::getToken(string &srcname, string &destname, double amt, string &token) {
    Guard g(lock);
    pqxx::work txn(*conn);
    // its ok to generate a token for a bad transact since it will be rejected
    sanitizeString(srcname);
    sanitizeString(destname);
    const string tokenquery = "INSERT INTO tokens (srcname, destname, amt) VALUES ($1, $2, $3) RETURNING id";
    try {
        pqxx::result res = txn.exec_params(tokenquery,srcname,destname,amt);
        token = res[0][0].as<string>();
        return Pistache::Http::Code::Accepted;
    } catch (const exception &e) {
        cout << "Token Error : " << e.what() << endl;
        return Pistache::Http::Code::Bad_Request;
    }
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



