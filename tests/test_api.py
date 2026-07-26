import pytest
from fastapi.testclient import TestClient
from server.server import app

client = TestClient(app)

def test_calculate_price_endpoint():
    payload = {
        "spot": 100, "strike": 100, "vol": 0.2, 
        "rate": 0.05, "expiry": 1.0, "is_call": True, "model": "black_scholes"
    }
    response = client.post("/api/v1/price", json=payload)
    assert response.status_code == 200
    data = response.json()
    
    assert "price" in data
    assert "greeks" in data
    assert round(data["price"], 2) == 10.45