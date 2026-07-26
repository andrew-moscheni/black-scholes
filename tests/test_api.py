import pytest
from fastapi.testclient import TestClient
from server.server import app

client = TestClient(app)

def test_calculate_price_endpoint():
    payload = {
        "spotPrice": 100.0,
        "strikePrice": 100.0,
        "volatility": 0.2,
        "riskFreeRate": 0.05,
        "timeToExpiry": 1.0
    }
    response = client.post("/api/pricing", json=payload)
    assert response.status_code == 200

    data = response.json()
    assert "blackScholes" in data
    assert "binomial" in data
    assert "callPrice" in data["blackScholes"]
    
    assert round(data["blackScholes"]["callPrice"], 2) == 10.45