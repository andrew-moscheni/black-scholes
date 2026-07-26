#include <memory>
#include <string>
#include "../include/payoff.h"
#include "../include/option-method.h"
#include "../include/black-scholes.h"
#include "../include/binomial.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <nlohmann/json.hpp>

struct MarketData{
    double spotPrice;
    double strikePrice;
    double volatility;
    double riskFreeRate;
    double timeToExpiry;
};

namespace py = pybind11;
using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MarketData, spotPrice, strikePrice, volatility, timeToExpiry, riskFreeRate)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Greeks, delta, gamma, vega, rho, theta)

std::string computePricesAndMetrics(const MarketData& marketData, const std::string& method){
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

    return metrics.dump();
}

PYBIND11_MODULE(computation, m){
    py::class_<MarketData>(m, "MarketData")
        .def(py::init([](double spotPrice, double strikePrice, double volatility, double timeToExpiry, double riskFreeRate) {
            MarketData md;
            md.spotPrice = spotPrice;
            md.strikePrice = strikePrice;
            md.volatility = volatility;
            md.timeToExpiry = timeToExpiry;
            md.riskFreeRate = riskFreeRate;
            return md;
        }), py::arg("spotPrice"), py::arg("strikePrice"), py::arg("volatility"), py::arg("timeToExpiry"), py::arg("riskFreeRate")) 
        .def_readwrite("spotPrice", &MarketData::spotPrice)
        .def_readwrite("strikePrice", &MarketData::strikePrice)
        .def_readwrite("volatility", &MarketData::volatility)
        .def_readwrite("timeToExpiry", &MarketData::timeToExpiry)
        .def_readwrite("riskFreeRate", &MarketData::riskFreeRate);
    m.doc() = "C++ plugin for FastAPI";
    m.def("compute", &computePricesAndMetrics, "Computes the pricing of the option with the Greeks of the option.");
}