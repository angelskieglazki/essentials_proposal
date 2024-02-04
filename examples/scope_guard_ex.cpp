#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include "ScopeGuard.h"

// /////////////////////////////////////////////
// // тестовый код
// /////////////////////////////////////////////

class UserDatabase
{
public:
    void AddFriend(const std::string &name, const std::string &friendName)
    {
        if (friendName == "qwerty")
        {
            // это просто для теста исключение, моделируем что в базе данных произошло исключение
            throw std::runtime_error("I don't like qwerty!!!");
        }

        friendsDB_[name].push_back(friendName);
    }

private:
    using FriendList = std::unordered_map<std::string, std::vector<std::string>>;
    FriendList friendsDB_;
};

class User
{
public:
    explicit User(const std::string &name, UserDatabase *pDB) : name_(name), pDB_(pDB) {}
    std::string GetName() const { return name_; }
    void AddFriend(User &newFriend)
    {
        friends_.push_back(&newFriend);
        // ScopeGuard removeFriend = ObjScopeGuardImpl0<UserCont, void (UserCont::*)()>::MakeObjGuard(friends_, &UserCont::pop_back);
        // auto removeFriend = MakeGuard([&friends = friends_]()
        //                               { friends.pop_back(); }); // это должен быть ON_BLOCK_FAIL
        // SCOPE_FAIL {friends_.pop_back();};
        pDB_->AddFriend(name_, newFriend.GetName());
    }
    void printFriends() const
    {
        for (const auto &friendName : friends_)
        {
            std::cout << friendName->GetName() << '\n';
        }
    }

private:
    using UserCont = std::vector<User *>;
    UserCont friends_;
    std::string name_;
    UserDatabase *pDB_;
};

void AddFriend(User &max, User &newFriend)
{
    try
    {
        max.AddFriend(newFriend);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

/////////////////////////////////////////////
// тестовый код
/////////////////////////////////////////////

void Decrement(int &x)
{
    --x;
    std::cout << "Decrement! " << x << '\n';
}
void DecrementBy(int &x, int by)
{
    x = x - by;
    std::cout << "Decrementby! " << x << '\n';
}
void UseRescource(int &counter)
{
    ++counter;
    // SCOPE_EXIT { Decrement(std::ref(counter)); };
    // SCOPE_EXIT(Decrement, std::ref(counter));
    // SCOPE_EXIT(DecrementBy, std::ref(counter), 10);
}

enum class ErrorBehavior
{
    SUCCESS,
    HANDLED_ERROR,
    UNHANDLED_ERROR
};

void testScopeFailAndSucces(ErrorBehavior error, bool expect_fail)
{
    bool scopeFailExecuted = false;
    bool scopeSuccessExecuted = false;

    try
    {
        SCOPE_FAIL { scopeFailExecuted = true; };
        SCOPE_SUCCESS { scopeSuccessExecuted = true; };

        try
        {
            if (error == ErrorBehavior::HANDLED_ERROR)
            {
                throw std::runtime_error("throwing an expected error");
            }
            else if (error == ErrorBehavior::UNHANDLED_ERROR)
            {
                throw "never throw raw strings";
            }
        }
        catch (const std::runtime_error &)
        {
        }
    }
    catch (...)
    {
        // Outer catch to swallow the error for the UNHANDLED_ERROR behavior
    }

    if (expect_fail != scopeFailExecuted)
    {
        throw std::logic_error("scopeFailExecuted != expect_fail");
    }
    if (!expect_fail != scopeSuccessExecuted)
    {
        throw std::logic_error("scopeSuccessExecuted != !expect_fail");
    }
}

void testScopeFailExceptionPtr()
{
    bool catchExecuted = false;
    bool failExecuted = false;

    try
    {
        SCOPE_FAIL { failExecuted = true; };

        std::exception_ptr ep;
        try
        {
            throw std::runtime_error("test");
        }
        catch (...)
        {
            ep = std::current_exception();
        }
        std::rethrow_exception(ep);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
        catchExecuted = true;
    }

    if (!catchExecuted) {
        throw std::logic_error("catchExecuted was not TRUE!");
    }

    if (!failExecuted) {
        throw std::logic_error("failExecuted was not TRUE!");
    }
}

int main()
{
    {
        testScopeFailAndSucces(ErrorBehavior::SUCCESS, false);
        testScopeFailAndSucces(ErrorBehavior::HANDLED_ERROR, false);
        testScopeFailAndSucces(ErrorBehavior::UNHANDLED_ERROR, true);
    }
    {
        testScopeFailExceptionPtr();
    }
    // {
    //     int counter0 = 0;
    //     {
    //         SCOPE_EXIT([](int &n)
    //                    { n += 2; },
    //                    std::ref(counter0));
    //         SCOPE_EXIT([&counter0]()
    //                    { counter0 += 2; });
    //         SCOPE_EXIT(Decrement, std::ref(counter0));
    //         auto j = MakeJanitor([&counter0]() { counter0 += 2; });
    //         j.dismiss();
    //     }

    //     std::cout << "0counter is " << counter0 << '\n';
    // }
    // {
    //     int counter = 0;
    //     UseRescource(counter);
    //     std::cout << counter << '\n';
    // }

    // {

    //     UserDatabase db;
    //     User max("Max", &db);
    //     User slava("Slava", &db);
    //     User dima("Dima", &db);
    //     User qwerty("qwerty", &db);

    //     AddFriend(max, slava);
    //     AddFriend(max, dima);
    //     AddFriend(max, qwerty);

    //     max.printFriends();
    // }

    return 0;
}
