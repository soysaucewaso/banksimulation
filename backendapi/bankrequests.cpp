//
// Created by Sawyer Blankenship on 7/1/24.
//

#include "bankrequests.h"

double BankRequests::getBalance(std::string &username) {
    if (username.compare("john") == 0)
        return 10.0;
    return 3.0;
}

Pistache::Http::Code BankRequests::deposit(string &username, int amt, string &resp) {
    resp = to_string(amt);
    return Pistache::Http::Code::Accepted;
}
