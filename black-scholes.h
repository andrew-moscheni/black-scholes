#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H
#include <cmath>
#include "option-method.h"




class BlackScholesEngine {
    private:
        static double normalCDF(double x){return 0.5 * (1.0 + erf(x / sqrt(2.0)));}
    public:
        static double calculatePrice(const OptionMethod& option, double spot, double vol, double rate, bool isCall){
            double K = option.getStrike();
            double T = option.getExpiry();

            double d1 = (log(spot / K) + T*(rate + 0.5*vol*vol)) / (vol * sqrt(T));
            double d2 = d1 - vol * sqrt(T);

            if(isCall){
                return spot * normalCDF(d1) - K * exp(-1.0*rate*T) * normalCDF(d2);
            }else{
                return K * exp(-1.0*rate*T) * normalCDF(-1.0*d2) - spot * normalCDF(-1.0 * d1);
            }
        }
};

#endif
