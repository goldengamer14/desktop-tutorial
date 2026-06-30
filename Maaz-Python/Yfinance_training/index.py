import yfinance as yf
import pandas as pd
from os import system

system("clear")

dt: yf.Ticker = yf.Ticker("TTWO")

history_df: pd.DataFrame = dt.history(
    period="1y", interval="1mo", auto_adjust=True, actions=False
)

print("\n\n")

for i in history_df.iterrows():
    print(
        f"{i[0]} | {(i[1]['Open']):.2f} (USD) {(i[1]['Open'] * 94.69):.2f} (INR) | {(i[1]['Close']):.2f} (USD) {(i[1]['Close'] * 94.69):.2f} (INR)"
    )

print("\n\n")

history_meta: dict = dt.history_metadata

for i in history_meta:
    print(f"{i}: {history_meta[i]}")

print("\n\n")

info: list[tuple[str, float]] = dt.get_fast_info().items()
infos_set: set[str] = {
    "currency",
    "symbol",
    "dayHigh",
    "dayLow",
    "open",
    "previousClose",
    "yearHigh",
    "yearLow",
}

for key, value in info:
    if not (key in infos_set):
        continue
    print(f"{key}: {value}")


print("\n\n")

funcs: list[str] = [
    "get_income_stmt",
    "get_balance_sheet",
    "get_cash_flow"
]

for func in funcs:
    data: pd.DataFrame = getattr(dt, func)(pretty=True)
    print(func, "\n")
    filtered_rows: pd.DataFrame = data[
        data.index.to_series().str.contains(
            "income|cash|cost|revenue",
            case=False,
            na=False,
            regex=True,
        )
    ]
    print(filtered_rows)
    print("\n\n")
