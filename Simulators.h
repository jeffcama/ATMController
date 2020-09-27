#pragma once

#ifndef SIMULATORS_H
#define SIMULATORS_H

extern double _balance;

class BearRoboticsBank
{
public:
	static bool BRBProvideAccountNum(double acctNum);
	static bool BRBVerifyPin(int pin);
	static bool BRBWithdraw(double amt);
	static bool BRBDeposit(double amt);
	static double BRBBalance();
};


class BearRoboticsATM
{
public:
	static bool BRACardInserted();
	static double BRACardNumber();
	static bool BRADispenseMoney(double amt);
	static bool BRAReturnCard();
	static bool BRAAcceptAndVerifyCash(double amt);
};

#endif // !SIMULATORS_H
