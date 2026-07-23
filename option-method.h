#ifndef OPTION_METHOD_H
#define OPTION_METHOD_H

#include <memory>
#include "payoff.h"

class OptionMethod {
    private:
        double strike;
        double expiry;
        std::shared_ptr<Payoff> payoff;
    
    public:
        OptionMethod(double expiry_, double strike_, std::shared_ptr<Payoff> payoff_): 
            strike(strike_), expiry(expiry_), payoff(payoff_) {}
        
        double getExpiry() {return expiry;}
        double getStrike() {return strike;}

        double executePayoff(double spot) const {return (*payoff)(spot);}
};

#endif