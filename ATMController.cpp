
//Notes:
//
//Many other use cases should be considered for the production of an ATM controller,
//but were not implemented for the purposes of this exercise.
//Some examples include:
// - Insufficient cash at the ATM location
// - Card confescation due to entering incorrect PIN or using stolen card
// - Card is not retrieved by the user after transaction
// - Receipt printing
// - etc
//
//All uses of BearRoboticsBank and BearRoboticsATM APIs are for testing purposes
//These should be replaced with API calls to the desired Bank or ATM software
//A more scalable solution may use function pointers associated with the specific Bank

#include "pch.h"
#include <iostream>
#include "ATMController.h"
#include "Simulators.h"

using namespace std;

//true when the account number from the card has been verified by the bank
bool ATMController::_accountNumValid = false;
//true when the account number and pin have been verified by the bank
bool ATMController::_accountAccessed = false;
//For the purposes of this exercise, it is assumed that once the bank validates
//the account number and pin provided by this atm (_accountAccessed is true),
//it will associate all subsequent transactions from this ATM with this account
//until the card is removed.

//Retreive the account balance from the Bank
double ATMController::getBalance()
{
    //only request if the account was successfully accessed
    if (true == _accountAccessed)
    {
        return BearRoboticsBank::BRBBalance();
    }

    //if balance was request before account was accessed, return invalid value
    return DBL_MIN;
}

//Withdraw specifcied amount from balance at the Bank
//and initiate cash dispersal from the ATM
//Only dispense the money if the Bank approves the withdrawal
bool ATMController::withdraw(double amount)
{
    //only request if the account was successfully accessed
    //and the amount is valid
    if (true == _accountAccessed && amount > 0)
    {
        if (true == BearRoboticsBank::BRBWithdraw(amount))
        {
            //if money was successfully withdrawn from account at Bank,
            //initiate cash dispersal
            if (true == dispenseCash(amount))
            {
                return true;
            }
            //if unable to dispense cash, make a good faith
            //effort to return amount to account
            else
            {
                deposit(amount);
            }
        }
    }

    return false;
}

//Deposit requested amount 
bool ATMController::deposit(double amount)
{
    //only request deposit if the account was successfully accessed
    //and the amount is valid
    //and the matching amount was received by the ATM
    if (true == _accountAccessed && 
        amount > 0 &&
        BearRoboticsATM::BRAAcceptAndVerifyCash(amount))
    {
        return BearRoboticsBank::BRBDeposit(amount);
    }

    return false;    
}

//Check with the Bank if the account number is valid
bool ATMController::isAccountNumValid(double accountNum)
{
    return _accountNumValid = BearRoboticsBank::BRBProvideAccountNum(accountNum);
}

//Check if valid pin was entered
bool ATMController::isPinValid(int pin)
{
    //only check pin if the account number was already determined valid
    if (true == _accountNumValid)
    {
        return _accountAccessed = BearRoboticsBank::BRBVerifyPin(pin);
    }

    return false;
}

//Read the card number form the ATM
double ATMController::readCardNumber()
{
    //only read the number if the card is inserted
    if (true == isCardInserted())
    {
        return BearRoboticsATM::BRACardNumber();
    }

    return false;
}

//Check if card is inserted
bool ATMController::isCardInserted()
{
    return BearRoboticsATM::BRACardInserted();
}

//Return the card back to the customer
bool ATMController::dispenseCard()
{
    if (true == BearRoboticsATM::BRAReturnCard())
    {
        //All transactions with this account are complete
        //Reset valid flags for next customer
        _accountNumValid = false;
        _accountAccessed = false;
        return true;
    }

    return true;
}

//Dispense the specified amount of cash
bool ATMController::dispenseCash(double amount)
{
    //only dispense if valid amount
    if (amount > 0)
    {
        return BearRoboticsATM::BRADispenseMoney(amount);
    }

    return false;
}


