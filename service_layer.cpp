#include <memory>
#include <string>
#include "payoff.h"
#include "option-method.h"
#include "black-scholes.h"
#include "binomial.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <nlohmann/json.hpp>

namespace py = pybind11;
using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MarketData, spotPrice, strikePrice, volatility, timeToExpiry, riskFreeRate)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Greeks, delta, gamma, vega, rho, theta)

json computePricesAndMetrics(const py::dict& pyMarketData, const std::string& method){
    py::object jsonModule = py::module_::import("json");
    std::string serialized = py::str(jsonModule.attr("dumps")(pyMarketData));
    json j = json::parse(serialized);
    
    MarketData marketData = j.get<MarketData>();

    double spotPrice = marketData.spotPrice;
    double strikePrice = marketData.strikePrice;
    double volatility = marketData.volatility;
    double timeToExpiry = marketData.timeToExpiry;
    double riskFreeRate = marketData.riskFreeRate;
    json metrics;

    auto callPayoff = std::make_shared<CallPayoff>(strikePrice);
    auto putPayoff = std::make_shared<PutPayoff>(strikePrice);

    OptionMethod callOption(timeToExpiry, strikePrice, callPayoff);
    OptionMethod putOption(timeToExpiry, strikePrice, putPayoff);

    if (method == "Binomial"){
        metrics["callPrice"] = BinomialEngine::calculatePrice(callOption, spotPrice, volatility, riskFreeRate, true);
        metrics["putPrice"] = BinomialEngine::calculatePrice(putOption, spotPrice, volatility, riskFreeRate, false);
        metrics["greeksCall"] = BinomialEngine::createGreeks(callOption, spotPrice, volatility, riskFreeRate, true);
        metrics["greeksPut"] = BinomialEngine::createGreeks(putOption, spotPrice, volatility, riskFreeRate, false);
    }else{
        metrics["callPrice"] = BlackScholesEngine::calculatePrice(callOption, spotPrice, volatility, riskFreeRate, true);
        metrics["putPrice"] = BlackScholesEngine::calculatePrice(putOption, spotPrice, volatility, riskFreeRate, false);
        metrics["greeksCall"] = BlackScholesEngine::createGreeks(callOption, spotPrice, volatility, riskFreeRate, true);
        metrics["greeksPut"] = BlackScholesEngine::createGreeks(putOption, spotPrice, volatility, riskFreeRate, false);
    }

    return metrics;
}

PYBIND11_MODULE(computation, m){
    m.doc() = "C++ plugin for FastAPI";
    m.def("compute", &computePricesAndMetrics, "Computes the pricing of the option with the Greeks of the option.");
}