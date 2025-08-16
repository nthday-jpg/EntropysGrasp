#pragma once
#include <entt/entt.hpp>
#include "../components/Account.h"
#include <vector>
#include <nlohmann/json.hpp>

class AccountManager {
    std::string pathToSaveFile = "assets/save/account.json";
    Account* currentAccount;
    bool loggedIn = false;

public:
    AccountManager();
    bool registerAccount(const std::string& user, const std::string& pass);
    bool login(const std::string& user, const std::string& pass);
    void logout();
    void saveProgress();
	~AccountManager();

    Account& getCurrentAccount() { return *currentAccount; }
    bool isLoggedIn() const { return loggedIn; }
};