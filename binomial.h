#ifndef BINOMIAL_H
#define BINOMIAL_H
#include <cmath>
#include <vector>
#include <algorithm>
#include "option-method.h"

class BinomialEngine : public ModelMethod {
    private:
        static constexpr int NUM_STEPS = 200;
    public:
        static double calculatePrice(const OptionMethod& option, double spot, double vol, double rate, bool isCall){
            double T = option.getExpiry();
            if (T <= 0.0) return option.executePayoff(spot);

            double dt = T / NUM_STEPS;
            double u = std::exp(vol * std::sqrt(dt));
            double d = 1.0 / u;
            double discount = std::exp(-rate * dt);
            
            double p = (std::exp(rate * dt) - d) / (u - d);

            std::vector<double> values(NUM_STEPS + 1);
            for (int i = 0; i <= NUM_STEPS; ++i) {
                double spotAtN = spot * std::pow(u, NUM_STEPS - i) * std::pow(d, i);
                values[i] = option.executePayoff(spotAtN);
            }
            
            for (int j = NUM_STEPS - 1; j >= 0; --j) {
                for (int i = 0; i <= j; ++i) {
                    values[i] = discount * (p * values[i] + (1.0 - p) * values[i + 1]);
                }
            }

            return values[0];
        }

        static Greeks createGreeks(const OptionMethod& option, double spot, double vol, double rate, bool isCall){
            double dS = spot * 0.01;
            double dVol = 0.01;
            double dRate = 0.0001;
            double dT = 1.0 / 365.0;

            double p0 = calculatePrice(option, spot, vol, rate, isCall);

            double p_up   = calculatePrice(option, spot + dS, vol, rate, isCall);
            double p_down = calculatePrice(option, spot - dS, vol, rate, isCall);

            double delta = (p_up - p_down) / (2.0 * dS);
            double gamma = (p_up - 2.0 * p0 + p_down) / (dS * dS);

            double p_vol_up = calculatePrice(option, spot, vol + dVol, rate, isCall);
            double vega = (p_vol_up - p0) / dVol;

            double p_rate_up = calculatePrice(option, spot, vol, rate + dRate, isCall);
            double rho = (p_rate_up - p0) / dRate;

            double theta = 0.0;
            if (option.getExpiry() > dT) {
                OptionMethod option_shortened(option.getExpiry() - dT, option.getStrike(), nullptr); // Payoff isn't used directly here
                double p_time = calculatePriceWithExpiry(option, spot, vol, rate, isCall, option.getExpiry() - dT);
                theta = (p_time - p0) / dT; // Daily theta
            }

            return {delta, gamma, vega, rho, theta};
        }
    private:
        static double calculatePriceWithExpiry(const OptionMethod& option, double spot, double vol, double rate, bool isCall, double T) {
            if (T <= 0.0) return option.executePayoff(spot);

            double dt = T / NUM_STEPS;
            double u = std::exp(vol * std::sqrt(dt));
            double d = 1.0 / u;
            double discount = std::exp(-rate * dt);
            double p = (std::exp(rate * dt) - d) / (u - d);

            std::vector<double> values(NUM_STEPS + 1);
            for (int i = 0; i <= NUM_STEPS; ++i) {
                double spotAtN = spot * std::pow(u, NUM_STEPS - i) * std::pow(d, i);
                values[i] = option.executePayoff(spotAtN);
            }

            for (int j = NUM_STEPS - 1; j >= 0; --j) {
                for (int i = 0; i <= j; ++i) {
                    values[i] = discount * (p * values[i] + (1.0 - p) * values[i + 1]);
                }
            }

            return values[0];
        }
};

#endif