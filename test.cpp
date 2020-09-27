#include "pch.h"
#include "ATMController.h"
#include "Simulators.h"


struct account_state
{
	int accountNumber;
	int pinNumber;
	int initialBalance;
	int withdrawAmount;
	int depositAmount;
	int finalBalance;
	bool success1;
	bool success2;
	bool expectError;
};

//Check that ATM can detect card is inserted and read account number from card

struct ATMControllerCardAccessTester : testing::Test, testing::WithParamInterface<account_state>{};

TEST_P(ATMControllerCardAccessTester, CardAccess)
{
	account_state state = GetParam();	

	EXPECT_EQ(ATMController::isCardInserted(), true);

	//Expect and error if account invalid account number is given
	if(!state.expectError)
		EXPECT_EQ(ATMController::readCardNumber(), state.accountNumber);
	else
		EXPECT_NE(ATMController::readCardNumber(), state.accountNumber);
}


INSTANTIATE_TEST_CASE_P(Default, ATMControllerCardAccessTester,
	testing::Values(account_state{ 123456789, 1234, 0, 0, 0, 0, true, true, false }, //Correct account number
					account_state{ 12345678, 1234, 0, 0, 0, 0, false, false, true } //Invalid account number (simulator only returns 123456789)
					));




//Check that Bank can verify account number and pin

struct ATMControllerAccountAccessTester : testing::Test, testing::WithParamInterface<account_state> {};

TEST_P(ATMControllerAccountAccessTester, AccountAccess)
{
	account_state state = GetParam();

	EXPECT_EQ(ATMController::isAccountNumValid(state.accountNumber), state.success1);
	EXPECT_EQ(ATMController::isPinValid(state.pinNumber), state.success2);
}


INSTANTIATE_TEST_CASE_P(Default, ATMControllerAccountAccessTester,
	testing::Values(account_state{ 123456789, 1234, 0, 0, 0, 0, true, true, false }, //Correct account number and pin
					account_state{ 12345678, 1234, 0, 0, 0, 0, false, false, false }, //Invalid account number
					account_state{ 123456789, 123, 0, 0, 0, 0, true, false, false } //Invalid pin
));




//Check that initial balanace is being set correctly - this is only used for testing purposes
//and that once set, the balance is correctly retrieved

struct ATMControllerBalanceTester : testing::Test, testing::WithParamInterface<account_state> {};

TEST_P(ATMControllerBalanceTester, Balance)
{
	account_state state = GetParam();

	EXPECT_EQ(ATMController::isAccountNumValid(state.accountNumber), true);
	EXPECT_EQ(ATMController::isPinValid(state.pinNumber), true);
	_balance = state.initialBalance;
	EXPECT_EQ(ATMController::getBalance(), state.initialBalance);
}


INSTANTIATE_TEST_CASE_P(Default, ATMControllerBalanceTester,
	testing::Values(account_state{ 123456789, 1234, 1000, 0, 0, 0, false, false, false }, //Positive balance
					account_state{ 123456789, 1234, 0, 0, 0, 0, false, false, false }, //0
					account_state{ 123456789, 1234, -1000, 0, 0, 0, false, false, false }//Negative balance
));




//Check that correct amount of many can be withdrawn

struct ATMControlleWithdrawTester : testing::Test, testing::WithParamInterface<account_state> {};

TEST_P(ATMControlleWithdrawTester, Withdraw)
{
	account_state state = GetParam();

	EXPECT_EQ(ATMController::isAccountNumValid(state.accountNumber), true);
	EXPECT_EQ(ATMController::isPinValid(state.pinNumber), true);
	_balance = state.initialBalance;
	EXPECT_EQ(ATMController::getBalance(), state.initialBalance);
	EXPECT_EQ(ATMController::withdraw(state.withdrawAmount), state.success1);
	EXPECT_EQ(ATMController::getBalance(), state.finalBalance);
}


INSTANTIATE_TEST_CASE_P(Default, ATMControlleWithdrawTester,
	testing::Values(account_state{ 123456789, 1234, 1000, 100, 0, 900, true, false, false }, //Withdraw valid amount
					account_state{ 123456789, 1234, 1000, 10000, 0, 1000, false, false, false }, //Withdraw amount creater than balance
					account_state{ 123456789, 1234, 1000, 1000, 0, 0, true, false, false }, //Withdraw full balance
					account_state{ 123456789, 1234, 1000, 1001, 0, 1000, false, false, false }, //Withdraw just over balance
					account_state{ 123456789, 1234, 1000, 0, 0, 1000, false, false, false }, //Withdraw 0
					account_state{ 123456789, 1234, 1000, -100, 0, 1000, false, false, false } //Withdraw negative number
));




//Check that money can be correctly deposited

struct ATMControlleDepositTester : testing::Test, testing::WithParamInterface<account_state> {};

TEST_P(ATMControlleDepositTester, Deposit)
{
	account_state state = GetParam();

	EXPECT_EQ(ATMController::isAccountNumValid(state.accountNumber), true);
	EXPECT_EQ(ATMController::isPinValid(state.pinNumber), true);
	_balance = state.initialBalance;
	EXPECT_EQ(ATMController::getBalance(), state.initialBalance);
	EXPECT_EQ(ATMController::deposit(state.depositAmount), state.success1);
	EXPECT_EQ(ATMController::getBalance(), state.finalBalance);
}


INSTANTIATE_TEST_CASE_P(Default, ATMControlleDepositTester,
	testing::Values(account_state{ 123456789, 1234, 1000, 0, 500, 1500, true, false, false }, //Deposit valid amount
					account_state{ 123456789, 1234, 1000, 0, 0, 1000, false, false, false }, //Deposit 0
					account_state{ 123456789, 1234, 1000, 0, -500, 1000, false, false, false } //Deposit negative amount
));


//Check that card will be dispensed

struct ATMControllerDispenseCardTester : testing::Test, testing::WithParamInterface<account_state> {};

TEST_P(ATMControllerDispenseCardTester, CardAccess)
{
	account_state state = GetParam();

	EXPECT_EQ(ATMController::dispenseCard(), true);
}


INSTANTIATE_TEST_CASE_P(Default, ATMControllerDispenseCardTester,
	testing::Values(account_state{ 123456789, 1234, 0, 0, 0, 0, false, false, false } //Card should always dispense per simulator
));

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}