#ifndef PAYOFF_H
#define PAYOFF_H

#include <algorithm>

class Payoff{
    public:
        virtual ~Payoff() = default;
        virtual double operator()(double spot) const = 0;
};

class CallPayoff : public Payoff {
    private:
        double strike;
    public:
        explicit CallPayoff(double strike_): strike(strike_){}
        double operator()(double spot) const override {
            return std::max(spot-strike, 0.0);
        }
};

class PutPayoff : public Payoff {
    private:
        double strike;
    public:
        explicit PutPayoff(double strike_): strike(strike_){}
        double operator()(double spot) const override {
            return std::max(strike-spot, 0.0);
        }
};

#endif