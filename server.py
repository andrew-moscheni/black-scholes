from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
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
    bs_metrics = computation.compute(payload_dict, "BlackScholes") 
    bin_metrics = computation.compute(payload_dict, "Binomial")
    
    return {
        "blackScholes": bs_metrics,
        "binomial": bin_metrics
    }