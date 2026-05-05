#include <iomanip>
#include <iostream>
#include <cmath>

using namespace std;


//TODO: connect this functionality to a ticker to perform live-action computation
double normalCDF(double x){
    /*
    * Computes the cumulative distribution function (CDF) of a normal distribution
    *
    * Parameters:
    *    double x: the critical value to calculate the CDF for
    *
    * Return Value:
    *    double: the CDF of the normal distribution at critical value x
    */
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

struct BlackScholesOptionPrices{
    /*
    * A class describing the return output of the Black-Scholes Model
    *
    * Attributes:
    *    double call_price: The call option price of the stock
    *    double put_price: The put option price of the stock
    */
    double call_price;
    double put_price;
};

BlackScholesOptionPrices computeOptions(double underlying_price, double strike_price, double expiration_time, double risk_free_rate, double volatility) {
    /*
    * Computes the call and put option prices based upon the Black-Scholes model.
    *
    * Parameters:
    *    double underlying_price: The underlying price of the stock (if you were to enter the market and buy 1 share of a stock)
    *    double strike_price: the strike price (At time of expiration, I have the option of buying the stock at this price regardless of current price)
    *    double expiration_time: The time of expiration of the option (in years)
    *    double risk_free_rate: The risk_free rate of the option
    *    double volatility: The volatility of the stock
    *
    * Return Value:
    *    BlackScholesOptionPrices: A tuple containing the call option price and the put option price, in that order
    */

    double d1 = (log(underlying_price / strike_price) + expiration_time*(risk_free_rate + 0.5*volatility*volatility)) / (volatility * sqrt(expiration_time));
    double d2 = d1 - volatility * sqrt(expiration_time);

    double call_option_price = underlying_price * normalCDF(d1) - strike_price * exp(-1.0*risk_free_rate*expiration_time) * normalCDF(d2);
    double put_option_price = strike_price * exp(-1.0*risk_free_rate*expiration_time) * normalCDF(-1.0*d2) - underlying_price * normalCDF(-1.0 * d1);

    return {call_option_price,put_option_price};
}

int main(int, char**){
    cout << "Hello, from black-scholes!" << endl;
    double S;
    double K;
    double T;
    double r;
    double vol;

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
