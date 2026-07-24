#include <memory>
#include "payoff.h"
#include "option-method.h"
#include "black-scholes.h"
#include <nlohmann/json.hpp>
#include <pybind11/pybind11.h>

using json = nlohmann::json;

json computePricesAndMetrics(const MarketData marketData, ModelMethod method){
    double spotPrice = marketData.spotPrice;
    double strikePrice = marketData.strikePrice;
    double volatility = marketData.volatility;
    double timeToExpiry = marketData.timeToExpiry;
    double  riskFreeRate = marketData.riskFreeRate;
    json metrics;

    auto callPayoff = std::make_shared<CallPayoff>(strikePrice);
    auto putPayoff = std::make_shared<PutPayoff>(strikePrice);

    OptionMethod callOption(timeToExpiry, strikePrice, callPayoff);
    OptionMethod putOption(timeToExpiry, strikePrice, putPayoff);

    metrics["callPrice"] = method.calculatePrice(callOption, spotPrice, volatility, riskFreeRate, true);
    metrics["putPrice"] = method.calculatePrice(putOption, spotPrice, volatility, riskFreeRate, false);
    metrics["greeksCall"] = method.createGreeks(callOption, spotPrice, volatility, riskFreeRate, true);
    metrics["greeksPut"] = method.createGreeks(putOption, spotPrice, volatility, riskFreeRate, false);

    return metrics;
}

PYBIND11_MODULE(computation, m){
    m.doc() = "C++ plugin for FastAPI";
    m.def("compute", &computePricesAndMetrics, "Computes the pricing of the option with the Greeks of the option.");
}