from fastapi import FastAPI
from databases import Database
import datetime
 
app = FastAPI()
database = Database("sqlite:///alerts.db")

@app.on_event("startup")
async def database_connect():
    await database.connect()
    query = """CREATE TABLE if not exists alerts (id INTEGER PRIMARY KEY, temperature INTEGER, timestamp VARCHAR(100))"""
    await database.execute(query=query)


@app.on_event("shutdown")
async def database_disconnect():
    await database.disconnect()

@app.post("/temperature/{temperature}")
async def post_temperature(temperature):
    try:
        timestamp = datetime.datetime.now().strftime("%m/%d/%Y, %H:%M:%S")
        query = "INSERT INTO alerts(temperature, timestamp) VALUES (:temperature, :timestamp)"
        values = [
            {"temperature": temperature, "timestamp": timestamp}
        ]
        await database.execute_many(query=query, values=values)

        return {"response": "ok"}
    except:
        return {"response": "error"}


@app.get("/temperatures/")
async def get_temperatures():
    try:
        query = "SELECT * FROM alerts"
        rows = await database.fetch_all(query=query)
        
        return {"response": rows}
    except:
        return {"response": "error"}