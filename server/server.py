from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import json

import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import computation

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class MarketDataPayload(BaseModel):
    spotPrice: float
    strikePrice: float
    volatility: float
    riskFreeRate: float
    timeToExpiry: float

@app.post("/api/pricing")
def get_pricing(data: MarketDataPayload):
    payload_dict = data.model_dump()
    data_obj = computation.MarketData(**payload_dict)
    bs_metrics = json.loads(computation.compute(data_obj, "BlackScholes"))
    bin_metrics = json.loads(computation.compute(data_obj, "Binomial"))
    
    return {
        "blackScholes": bs_metrics,
        "binomial": bin_metrics
    }