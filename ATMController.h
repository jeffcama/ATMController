#pragma once
#ifndef ATMCONTROLLER_H
#define ATMCONTROLLER_H

//ATM Controller class
//Everything is static as only one user is at the ATM
//and therefore there is no need for objects
//Kept as a class for ogranization
class ATMController
{
private:
    static bool _accountNumValid;
    static bool _accountAccessed;

    //private as it is only called as part of a withdraw action
    static bool dispenseCash(double amount);
public:

    //Functionality that interacts with Bank
    static double getBalance();
    static bool withdraw(double amount);
    static bool deposit(double amount);
    static bool isPinValid(int pin);
    static bool isAccountNumValid(double accountNum);

    //Functionality that interacts with ATM
    static double readCardNumber();
    static bool isCardInserted();
    static bool dispenseCard();
};


#endif // !ATMCONTROLLER_H