#ifndef CALL_OPTION_H
#define CALL_OPTION_H

#include <memory>
#include "../include/option-method.h"
#include "../include/payoff.h"

class CallOption : public OptionMethod {
public:
    CallOption(double strikePrice, double timeToExpiry)
        : OptionMethod(timeToExpiry, strikePrice, std::make_shared<CallPayoff>(strikePrice)) {}
};

#endif