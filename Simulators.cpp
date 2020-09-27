#include "pch.h"
#include "Simulators.h"


//default balance is $1000
double _balance = 1000;

//only valid account number is 123456789
bool BearRoboticsBank::BRBProvideAccountNum(double accountNum) {
	return (accountNum == 123456789 ? true : false);
}

//only valid pin number is 1234
bool BearRoboticsBank::BRBVerifyPin(int pin) {
	return (pin == 1234 ? true : false);
}

//withdrawal if amount is in account
bool BearRoboticsBank::BRBWithdraw(double amount) {
	if (amount <= _balance)
	{
		_balance -= amount;
		return true;
	}

	return false;
}

//deposit amounts of $1 or more
bool BearRoboticsBank::BRBDeposit(double amount)
{
	if (amount >= 1)
	{
		_balance += amount;
		return true;
	}

	return false;
}

//return the balance
double BearRoboticsBank::BRBBalance() {
	return _balance;
}

//card is always inserted
bool BearRoboticsATM::BRACardInserted()
{
	return true;
}



//card number is always 123456789
double BearRoboticsATM::BRACardNumber() 
{
	return 123456789;
}

//money dispersal is always successful
bool BearRoboticsATM::BRADispenseMoney(double amt)
{
	return true;
}

//card dispersal is always successful
bool BearRoboticsATM::BRAReturnCard()
{
	return true;
}

//accepting and verifying deposited cash is always successful
bool BearRoboticsATM::BRAAcceptAndVerifyCash(double amt)
{
	return true;
}