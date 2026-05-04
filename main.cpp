#include <iomanip>
#include <iostream>
#include <cmath>

using namespace std;

double normalCDF(double x){
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

struct BlackScholesOptionPrices{
    double call_price;
    double put_price;
};

BlackScholesOptionPrices computeOptions(double underlying_price, double strike_price, double expiration_time, double risk_free_rate, double volatility) {
    //calculate d1 & d2
    double d1 = (log(underlying_price / strike_price) + expiration_time*(risk_free_rate + 0.5*volatility*volatility)) / (volatility * sqrt(expiration_time));
    double d2 = d1 - volatility * sqrt(expiration_time);

    // calculate the call option and put option prices
    double call_option_price = underlying_price * normalCDF(d1) - strike_price * exp(-1.0*risk_free_rate*expiration_time) * normalCDF(d2);
    double put_option_price = strike_price * exp(-1.0*risk_free_rate*expiration_time) * normalCDF(-1.0*d2) - underlying_price * normalCDF(-1.0 * d1);

    return {call_option_price,put_option_price};
}

int main(int, char**){
    cout << "Hello, from black-scholes!" << endl;
    double S; //If I was to go into the market and buy 1 share of a stock
    double K; //at ToE, I have option to buy stock at this price regardless of current price
    double T; //time of expiration (in years)
    double r; //risk-free rate (can look at current yields for this)
    double vol; //volatility

    //Inut the variables
    cout << "Enter the underlying price: ";
    cin >> S;

    cout << "Enter the strike price: ";
    cin >> K;

    cout << "Enter the time to expiration: ";
    cin >> T;

    cout << "Enter the risk-free rate: ";
    cin >> r;

    cout << "Enter the volatility: ";
    cin >> vol;

    //Compute the call and put prices from the function defined above
    auto [call, put] = computeOptions(S,K,T,r,vol);
    cout << "Call price: $" << fixed << setprecision(2) << call << ", Put price: $" << put << endl;

    return 0;
}
