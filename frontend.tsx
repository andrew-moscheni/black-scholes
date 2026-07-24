import React, { useState } from 'react';
import ReactDOM from 'react-dom/client';

interface MarketData {
  spotPrice: number;
  strikePrice: number;
  volatility: number;
  timeToExpiry: number;
  riskFreeRate: number;
}

interface Greeks {
  delta: number;
  gamma: number;
  vega: number;
  rho: number;
  theta: number;
}

interface ModelMetrics {
  callPrice: number;
  putPrice: number;
  greeksCall: Greeks;
  greeksPut: Greeks;
}

interface PricingResponse {
  blackScholes: ModelMetrics;
  binomial: ModelMetrics;
}

export default function OptionPricingEngine() {
  const [formData, setFormData] = useState<MarketData>({
    spotPrice: 100.0,
    strikePrice: 100.0,
    volatility: 0.2,
    timeToExpiry: 1.0,
    riskFreeRate: 0.05,
  });

  const [results, setResults] = useState<PricingResponse | null>(null);
  const [loading, setLoading] = useState<boolean>(false);
  const [error, setError] = useState<string | null>(null);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const { name, value } = e.target;
    setFormData((prev) => ({
      ...prev,
      [name]: parseFloat(value),
    }));
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    setError(null);

    try {
      const response = await fetch('http://127.0.0.1:8000/api/pricing', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(formData),
      });

      if (!response.ok) {
        throw new Error('Failed to fetch pricing data from the server.');
      }

      const data: PricingResponse = await response.json();
      setResults(data);
    } catch (err: any) {
      setError(err.message || 'An unknown error occurred.');
    } finally {
      setLoading(false);
    }
  };

  const formatNum = (num: number) => num.toFixed(4);

  return (
    <div className="min-h-screen bg-gray-50 flex flex-col font-sans">
      <header className="bg-slate-900 text-white p-6 shadow-md">
        <h1 className="text-3xl font-bold tracking-tight">Option Pricing Engine</h1>
      </header>

      <div className="flex flex-1 flex-col md:flex-row p-6 gap-6 max-w-7xl mx-auto w-full">
        <aside className="w-full md:w-1/3 lg:w-1/4 bg-white p-6 rounded-lg shadow-sm border border-gray-200 h-fit">
          <h2 className="text-xl font-semibold mb-4 text-gray-800 border-b pb-2">Parameters</h2>
          <form onSubmit={handleSubmit} className="space-y-4">
            
            <div className="flex flex-col">
              <label className="text-sm font-medium text-gray-700 mb-1">Spot Price (S)</label>
              <input type="number" step="0.01" name="spotPrice" value={formData.spotPrice} onChange={handleChange} className="border border-gray-300 rounded p-2 focus:ring-2 focus:ring-blue-500 focus:outline-none" required />
            </div>

            <div className="flex flex-col">
              <label className="text-sm font-medium text-gray-700 mb-1">Strike Price (K)</label>
              <input type="number" step="0.01" name="strikePrice" value={formData.strikePrice} onChange={handleChange} className="border border-gray-300 rounded p-2 focus:ring-2 focus:ring-blue-500 focus:outline-none" required />
            </div>

            <div className="flex flex-col">
              <label className="text-sm font-medium text-gray-700 mb-1">Volatility (σ)</label>
              <input type="number" step="0.01" name="volatility" value={formData.volatility} onChange={handleChange} className="border border-gray-300 rounded p-2 focus:ring-2 focus:ring-blue-500 focus:outline-none" required />
            </div>

            <div className="flex flex-col">
              <label className="text-sm font-medium text-gray-700 mb-1">Time to Expiry (T in years)</label>
              <input type="number" step="0.01" name="timeToExpiry" value={formData.timeToExpiry} onChange={handleChange} className="border border-gray-300 rounded p-2 focus:ring-2 focus:ring-blue-500 focus:outline-none" required />
            </div>

            <div className="flex flex-col">
              <label className="text-sm font-medium text-gray-700 mb-1">Risk-Free Rate (r)</label>
              <input type="number" step="0.001" name="riskFreeRate" value={formData.riskFreeRate} onChange={handleChange} className="border border-gray-300 rounded p-2 focus:ring-2 focus:ring-blue-500 focus:outline-none" required />
            </div>

            <button type="submit" disabled={loading} className="w-full bg-blue-600 text-white font-semibold py-2 px-4 rounded hover:bg-blue-700 transition-colors disabled:bg-blue-400 mt-4">
              {loading ? 'Computing...' : 'Calculate Prices'}
            </button>
            
            {error && <p className="text-red-500 text-sm mt-2">{error}</p>}
          </form>
        </aside>

        <main className="w-full md:w-2/3 lg:w-3/4 bg-white p-6 rounded-lg shadow-sm border border-gray-200">
          <h2 className="text-xl font-semibold mb-6 text-gray-800 border-b pb-2">Pricing & Greeks Comparison</h2>
          
          {!results && !loading && (
            <div className="text-gray-500 italic text-center py-12">
              Enter market parameters and click calculate to view results.
            </div>
          )}

          {results && (
            <div className="overflow-x-auto">
              <table className="w-full text-left border-collapse">
                <thead>
                  <tr className="bg-gray-100 text-gray-700 text-sm uppercase">
                    <th className="p-3 border-b-2 border-gray-300 w-1/5">Metric</th>
                    <th className="p-3 border-b-2 border-gray-300 text-right">BS (Call)</th>
                    <th className="p-3 border-b-2 border-gray-300 text-right">BS (Put)</th>
                    <th className="p-3 border-b-2 border-gray-300 text-right">Binomial (Call)</th>
                    <th className="p-3 border-b-2 border-gray-300 text-right">Binomial (Put)</th>
                  </tr>
                </thead>
                <tbody className="text-sm text-gray-800">
                  <tr className="hover:bg-gray-50 border-b border-gray-200 bg-blue-50/30">
                    <td className="p-3 font-semibold">Price</td>
                    <td className="p-3 text-right font-medium text-blue-700">{formatNum(results.blackScholes.callPrice)}</td>
                    <td className="p-3 text-right font-medium text-blue-700">{formatNum(results.blackScholes.putPrice)}</td>
                    <td className="p-3 text-right font-medium text-blue-700">{formatNum(results.binomial.callPrice)}</td>
                    <td className="p-3 text-right font-medium text-blue-700">{formatNum(results.binomial.putPrice)}</td>
                  </tr>
                  <tr className="hover:bg-gray-50 border-b border-gray-200">
                    <td className="p-3 font-semibold">Delta (Δ)</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksCall.delta)}</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksPut.delta)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksCall.delta)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksPut.delta)}</td>
                  </tr>
                  <tr className="hover:bg-gray-50 border-b border-gray-200 bg-gray-50/50">
                    <td className="p-3 font-semibold">Gamma (Γ)</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksCall.gamma)}</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksPut.gamma)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksCall.gamma)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksPut.gamma)}</td>
                  </tr>
                  <tr className="hover:bg-gray-50 border-b border-gray-200">
                    <td className="p-3 font-semibold">Vega (ν)</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksCall.vega)}</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksPut.vega)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksCall.vega)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksPut.vega)}</td>
                  </tr>
                  <tr className="hover:bg-gray-50 border-b border-gray-200 bg-gray-50/50">
                    <td className="p-3 font-semibold">Rho (ρ)</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksCall.rho)}</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksPut.rho)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksCall.rho)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksPut.rho)}</td>
                  </tr>
                  <tr className="hover:bg-gray-50">
                    <td className="p-3 font-semibold">Theta (Θ)</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksCall.theta)}</td>
                    <td className="p-3 text-right">{formatNum(results.blackScholes.greeksPut.theta)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksCall.theta)}</td>
                    <td className="p-3 text-right">{formatNum(results.binomial.greeksPut.theta)}</td>
                  </tr>
                </tbody>
              </table>
            </div>
          )}
        </main>
      </div>
    </div>
  );
}

// THIS LINE IS REQUIRED TO MOUNT THE APP TO THE DOM:
ReactDOM.createRoot(document.getElementById('root')!).render(
  <React.StrictMode>
    <OptionPricingEngine />
  </React.StrictMode>
);