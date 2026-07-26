#include <gtest/gtest.h>
#include <cmath>
#include "../include/black-scholes.h"
#include "../include/binomial.h"
#include "../include/option-method.h"
#include "../include/call-option.h" 

const double SPOT = 100.0;
const double STRIKE = 100.0;
const double RATE = 0.05;
const double VOL = 0.2;
const double EXPIRY = 1.0;
const double TOLERANCE = 0.01;

TEST(BlackScholesTest, AtTheMoneyCallPrice) {
    CallOption callOpt(STRIKE, EXPIRY);
    double price = BlackScholesEngine::calculatePrice(callOpt, SPOT, VOL, RATE, true);
    
    // Known BS ATM Call value for these inputs is approx 10.45
    EXPECT_NEAR(price, 10.4506, TOLERANCE);
}

TEST(BlackScholesTest, GreeksCalculation) {
    CallOption callOpt(STRIKE, EXPIRY);
    Greeks greeks = BlackScholesEngine::createGreeks(callOpt, SPOT, VOL, RATE, true);
    
    EXPECT_GT(greeks.delta, 0.0); // call delta should be positive
    EXPECT_LT(greeks.delta, 1.0);
    EXPECT_GT(greeks.gamma, 0.0); // gamma is always positive for long options
}

TEST(BinomialEngineTest, ConvergenceToBlackScholes) {
    CallOption callOpt(STRIKE, EXPIRY);
    
    double bsPrice = BlackScholesEngine::calculatePrice(callOpt, SPOT, VOL, RATE, true);
    double binPrice = BinomialEngine::calculatePrice(callOpt, SPOT, VOL, RATE, true);
    
    // binomial with 200 steps should be reasonably close to analytical BS
    EXPECT_NEAR(bsPrice, binPrice, 0.15); 
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}