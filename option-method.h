#ifndef OPTION_METHOD_H
#define OPTION_METHOD_H

#include <memory>
#include "payoff.h"

struct Greeks{
    double delta;
    double gamma;
    double vega;
    double rho;
    double theta;
};

class OptionMethod {
    private:
        double strike;
        double expiry;
        std::shared_ptr<Payoff> payoff;
    
    public:
        OptionMethod(double expiry_, double strike_, std::shared_ptr<Payoff> payoff_)
            : strike(strike_), expiry(expiry_), payoff(payoff_) {}
        
        double getExpiry() const {return expiry;}
        double getStrike() const {return strike;}

        double executePayoff(double spot) const {return (*payoff)(spot);}
};

class ModelMethod {
    public:
        static double calculatePrice(const OptionMethod& option, double spot, double vol, double rate, bool isCall){return 0.0;}
        static Greeks createGreeks(const OptionMethod& option, double spot, double vol, double rate, bool isCall){return {0,0,0,0,0};}
};

#endif