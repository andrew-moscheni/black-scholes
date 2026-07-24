#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H
#include <cmath>
#include "option-method.h"

class BlackScholesEngine : public ModelMethod {
    private:
        static double normalCDF(double x){return 0.5 * (1.0 + erf(x / sqrt(2.0)));}
        static double normalPDF(double x){return 1/(2*2.0*std::acos(0.0))*exp(-x*x/2);}
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

        static Greeks createGreeks(const OptionMethod& option, double spot, double vol, double rate, bool isCall){
            double K = option.getStrike();
            double T = option.getExpiry();

            double callFlag = isCall ? 1.0 : -1.0;
            
            double d1 = (log(spot / K) + T*(rate + 0.5*vol*vol)) / (vol * sqrt(T));
            double delta = callFlag*normalCDF(callFlag*d1);
            double gamma = 1 / (spot * sqrt(T)) * normalPDF(d1);
            double vega = spot*sqrt(T)*normalPDF(d1);
            double rho = callFlag*K*exp(-rate*T)*normalCDF(callFlag*(d1-vol*sqrt(T)));
            double theta = callFlag*spot*normalCDF(callFlag*d1) - callFlag*K*exp(-rate*T)*rate*normalCDF(callFlag*(d1-vol*sqrt(T))) - spot * vol/(2*sqrt(T))*normalPDF(d1);
            return {delta, gamma, vega, rho, theta};
        }
};

#endif
