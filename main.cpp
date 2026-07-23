#include <iostream>
#include <memory>
#include <iomanip>
#include "payoff.h"
#include "option-method.h"
#include "black-scholes.h"

int main(){
    double spotPrice = 100.0;
    double strikePrice = 100.0;
    double volatility = 0.2;
    double riskFreeRate = 0.05;
    double timeToExpiry = 1.0;

    auto callPayoff = std::make_shared<CallPayoff>(strikePrice);
    auto putPayoff = std::make_shared<PutPayoff>(strikePrice);

    OptionMethod callOption(timeToExpiry, strikePrice, callPayoff);
    OptionMethod putOption(timeToExpiry, strikePrice, putPayoff);

    double callPrice = BlackScholesEngine::calculatePrice(callOption, spotPrice, volatility, riskFreeRate, true);
    double putPrice = BlackScholesEngine::calculatePrice(putOption, spotPrice, volatility, riskFreeRate, false);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "---Pricing Engine Metrics---" << std::endl;
    std::cout << "Underlying Spot Price: " << spotPrice << std::endl;
    std::cout << "Underlying Strike Price: " << strikePrice << std::endl;
    std::cout << "Theoretical Call Value: $" << callPrice << std::endl;
    std::cout << "Theoretical Put Value: $" << putPrice << std::endl;
}