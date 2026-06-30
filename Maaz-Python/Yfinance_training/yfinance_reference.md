# yfinance — Compressed Reference

Unofficial wrapper around Yahoo Finance's internal API. Not affiliated with Yahoo. For personal/research use — re-read their ToS if you build anything beyond a side project. No API key needed.

```bash
pip install yfinance
```

Everything below is the **real, current public API** (yfinance ≥ 0.2.x), trimmed of marketing text. Each section = one object/concern. Assume `import yfinance as yf` everywhere.

---

## 1. Mental model

```
yf.Ticker("MSFT")        -> one symbol, rich object, lots of lazily-fetched properties (1 HTTP call per property group, then cached on the instance)
yf.Tickers("MSFT AAPL")  -> dict-like wrapper around several Ticker objects
yf.download(...)         -> one-shot bulk OHLCV puller -> pandas DataFrame, no Ticker object needed
```

Every `xxx` property has a matching `get_xxx(...)` **method**. The property = `get_xxx()` called with defaults, cached. Call the method directly when you need non-default args (e.g. `freq='quarterly'`).

```python
dat = yf.Ticker("MSFT")
dat.info                      # property, no args
dat.get_info()                 # same thing, explicit method form
dat.income_stmt                # yearly, dict-pretty=False by default
dat.get_income_stmt(freq='quarterly', pretty=True)   # method form, custom args
```

---

## 2. `Ticker` — single symbol

```python
yf.Ticker(ticker: str | tuple[str, str], session=None)
# ticker: "AAPL"  or  (symbol, MIC) e.g. ('OR', 'XPAR') for disambiguating exchange
```

### 2.1 Price history — the core feature

```python
dat.history(
    period='1mo',        # used only if start/end are None. 1d,5d,1mo,3mo,6mo,1y,2y,5y,10y,ytd,max
    interval='1d',        # 1m,2m,5m,15m,30m,60m,90m,1h,1d,5d,1wk,1mo,3mo
    start=None,           # "YYYY-MM-DD" or datetime, inclusive. default: 99y ago
    end=None,             # "YYYY-MM-DD" or datetime, exclusive. default: now
    prepost=False,        # include pre/post market data
    actions=True,         # include dividends + stock split columns
    auto_adjust=True,     # adjust OHLC for splits/dividends automatically
    back_adjust=False,    # back-adjust to mimic raw historical prices
    repair=False,         # fix Yahoo data glitches: 100x errors, missing data, bad div-adjust (see §7)
    keepna=False,         # keep NaN rows Yahoo returns
    rounding=False,       # round to 2dp (default = Yahoo's own precision)
    timeout=10,           # seconds, None = no timeout
    raise_errors=False,   # raise Exceptions instead of just logging
) -> pd.DataFrame   # columns: Open High Low Close Volume [Dividends Stock Splits]
```

Rules:
- Set **at most 2 of** `period`, `start`, `end` — setting all 3 raises `ValueError`.
- Intraday (`interval` < `1d`) only available for the **last 60 days**; `1m` bars only for the **last 7 days**.
- `auto_adjust=True` (default) means Close is already split/dividend-adjusted — don't double-adjust.

```python
dat.history(period="1mo")
dat.history(start="2020-01-01", end="2023-01-01")
dat.history(period="5d", interval="1m")          # max granularity, 7-day window only
```

### 2.2 Corporate actions & metadata

```python
dat.dividends           # pd.Series, date-indexed
dat.splits               # pd.Series
dat.capital_gains        # pd.Series (funds only)
dat.actions               # DataFrame combining dividends + splits
dat.get_shares_full(start=None, end=None)   # historical shares-outstanding, daily granularity
dat.isin                   # ISIN code (str | None)
dat.history_metadata      # dict: exchange, currency, timezone, trading periods, etc.
```

### 2.3 `info` vs `fast_info`

```python
dat.info        # dict, ~150 keys: sector, marketCap, trailingPE, website, longBusinessSummary, etc.
                # SLOW — scrapes a big quoteSummary payload. Don't call in a tight loop.

dat.fast_info   # dict-like, ~20 keys, cheap & fast: lastPrice, open, dayHigh, dayLow, previousClose,
                # fiftyDayAverage, twoHundredDayAverage, yearHigh, yearLow, marketCap, shares,
                # currency, exchange, quoteType, timezone
                # Use this when you only need current price / basic stats.
```

### 2.4 Financial statements

All four follow the identical signature pattern:

```python
dat.get_income_stmt(as_dict=False, pretty=False, freq='yearly')   # 'yearly' | 'quarterly' | 'trailing'
dat.get_balance_sheet(as_dict=False, pretty=False, freq='yearly')  # 'yearly' | 'quarterly'
dat.get_cash_flow(as_dict=False, pretty=False, freq='yearly')      # 'yearly' | 'quarterly'
dat.get_earnings(as_dict=False, freq='yearly')                      # 'yearly' | 'quarterly' | 'trailing'

# pretty=True -> human-readable row labels ("Total Revenue" instead of camelCase)
# as_dict=True -> Python dict instead of DataFrame

# Property shortcuts (yearly unless prefixed):
dat.income_stmt / dat.quarterly_income_stmt / dat.ttm_income_stmt
dat.balance_sheet / dat.quarterly_balance_sheet
dat.cashflow / dat.quarterly_cashflow / dat.ttm_cashflow
```
(`incomestmt`/`balancesheet`/`cashflow` no-underscore aliases also exist for all of the above — same data.)

```python
dat.calendar             # dict: next earnings date, ex-dividend date, etc.
dat.get_earnings_dates(limit=12, offset=0) -> DataFrame   # historical + upcoming earnings dates, EPS est/actual
dat.sec_filings          # dict of recent SEC filings (10-K, 10-Q, 8-K, ...)
```

### 2.5 Analyst data & holders

```python
dat.analyst_price_targets        # dict: current, low, high, mean, median
dat.recommendations               # DataFrame: period, strongBuy, buy, hold, sell, strongSell (counts over time)
dat.recommendations_summary       # same data, different shape
dat.upgrades_downgrades           # DataFrame indexed by grade date: Firm, ToGrade, FromGrade, Action
dat.sustainability                 # ESG scores DataFrame

dat.earnings_estimate             # index: 0q +1q 0y +1y | cols: numberOfAnalysts avg low high yearAgoEps growth
dat.revenue_estimate              # index: 0q +1q 0y +1y | cols: numberOfAnalysts avg low high yearAgoRevenue growth
dat.earnings_history              # DatetimeIndex | cols: epsEstimate epsActual epsDifference surprisePercent
dat.eps_trend                     # index: 0q +1q 0y +1y | cols: current 7daysAgo 30daysAgo 60daysAgo 90daysAgo
dat.eps_revisions                 # index: 0q +1q 0y +1y | cols: upLast7days upLast30days downLast7days downLast30days
dat.growth_estimates              # index: 0q +1q 0y +1y +5y -5y | cols: stock industry sector index

dat.major_holders                # DataFrame: % held by insiders/institutions
dat.institutional_holders        # DataFrame: top institutional holders
dat.mutualfund_holders           # DataFrame: top mutual fund holders
dat.insider_purchases             # DataFrame
dat.insider_transactions          # DataFrame
dat.insider_roster_holders        # DataFrame
```

### 2.6 Options chain

```python
dat.options                       # tuple of available expiry date strings "YYYY-MM-DD"
chain = dat.option_chain(date=None, tz=None)   # date omitted -> nearest expiry
chain.calls                       # DataFrame
chain.puts                        # DataFrame
# columns: contractSymbol, strike, lastPrice, bid, ask, volume, openInterest, impliedVolatility, inTheMoney, ...

dat.option_chain(dat.options[0]).calls   # canonical "first available expiry, calls" pattern
```

### 2.7 News & funds

```python
dat.get_news(count=10, tab='news')   # tab: 'news' | 'all' | 'press releases'
dat.news                              # property, same as default get_news()

dat.funds_data                        # FundsData object — populated only for ETFs/mutual funds, see §6
```

### 2.8 Live streaming for one ticker

```python
dat.live(message_handler=None, verbose=True)   # convenience wrapper around WebSocket for this single symbol
```

---

## 3. `Tickers` — multiple symbols, one object

```python
tickers = yf.Tickers("MSFT AAPL GOOG")     # space-separated string OR list

tickers.tickers["MSFT"]              # dict of symbol -> Ticker object, access individually
tickers.tickers["MSFT"].info

tickers.history(period='1mo', interval='1d', ...)   # same kwargs as download(), returns one combined DataFrame
tickers.download(...)                  # alias, identical to .history()
tickers.news()                         # combined news across all tickers
```
`Tickers.history`/`download` do **not** accept `start`+`end`+`period` together either (same "max 2 of 3" rule as `Ticker.history`).

---

## 4. `yf.download()` — bulk one-shot puller (no Ticker objects)

```python
yf.download(
    tickers,                 # str (space-sep) or list
    start=None, end=None,     # same rules as history()
    period='1mo',
    interval='1d',
    actions=False,            # include dividends/splits — default False here (unlike Ticker.history!)
    threads=True,             # bool or int: parallel download workers
    ignore_tz=None,           # default False for intraday, True for daily+ — strips tz when merging multi-tz tickers
    group_by='column',        # 'column' (default) or 'ticker' — controls MultiIndex column order
    auto_adjust=True,
    back_adjust=False,
    repair=False,
    keepna=False,
    progress=True,             # show progress bar
    prepost=False,
    rounding=False,
    timeout=10,
    session=None,
    multi_level_index=True,    # False -> flat single-level columns when downloading ONE ticker
) -> pd.DataFrame | None
```

```python
yf.download(['MSFT', 'AAPL', 'GOOG'], period='1mo')
df = yf.download("AAPL", start="2020-01-01", end="2023-01-01", multi_level_index=False)
```

### Multi-level columns gotcha (very common pain point)
Multiple tickers → columns are a `MultiIndex`: level 0 = field (`Close`,`Open`,...), level 1 = ticker (when `group_by='column'`, the default).

```python
df['Close']                 # DataFrame: one column per ticker
df['Close']['AAPL']         # Series
df.xs('AAPL', axis=1, level=1)        # all fields for AAPL, single-level columns
```
With `group_by='ticker'`, the levels swap: `df['AAPL']['Close']`.
For a single ticker, pass `multi_level_index=False` to skip the MultiIndex entirely.
When saving to CSV: `pd.read_csv(path, header=[0,1], index_col=0)` to read a MultiIndex CSV back correctly.

---

## 5. Search & Lookup

```python
yf.Search("AAPL", max_results=10).quotes              # list of matching quote dicts
yf.Search("Google", news_count=10).news                # list of news article dicts
yf.Search("apple", include_research=True).research     # list of research report dicts
```

```python
yf.Lookup("AAPL").all                       # property: combined cross-asset-type results
yf.Lookup("AAPL").get_all(count=100)        # method form w/ explicit count
yf.Lookup("AAPL").stock / .get_stock(count=100)
yf.Lookup("AAPL").etf / .get_etf(count=100)
yf.Lookup("AAPL").mutualfund / .get_mutualfund(count=100)
yf.Lookup("AAPL").index / .get_index(count=100)
yf.Lookup("AAPL").future / .get_future(count=100)
yf.Lookup("AAPL").currency / .get_currency(count=100)
yf.Lookup("AAPL").cryptocurrency / .get_cryptocurrency(count=100)
```
`Search`/`Lookup` use fuzzy text matching against Yahoo's autocomplete — good for "what's the ticker for X" lookups.

---

## 6. Funds: ETFs & mutual funds — `FundsData`

```python
spy = yf.Ticker('SPY').funds_data        # only populated for fund-type tickers

spy.description          # str: fund description
spy.fund_overview         # dict: category, family, legal type, ...
spy.fund_operations       # DataFrame: expense ratio, turnover, etc.
spy.asset_classes         # dict: {"stocks": 0.98, "bonds": 0.0, ...}
spy.sector_weightings     # dict: {"Technology": 0.30, ...}
spy.top_holdings          # DataFrame: top constituent holdings
spy.equity_holdings       # DataFrame: equity-specific holding stats (P/E, P/B, ...)
spy.bond_holdings         # DataFrame: bond-specific holding stats
spy.bond_ratings          # dict: {"AAA": 0.4, "AA": 0.2, ...}
spy.quote_type()          # 'ETF' | 'MUTUALFUND'
```

---

## 7. Sector / Industry

```python
tech = yf.Sector('technology')                      # slug-style key
software = yf.Industry('software-infrastructure')

# common to both:
tech.key; tech.name; tech.symbol; tech.ticker        # .ticker -> returns a Ticker object for the sector's index/ETF proxy
tech.overview; tech.top_companies; tech.research_reports

# Sector-only:
tech.top_etfs; tech.top_mutual_funds; tech.industries     # map of industry-key -> industry

# Industry-only:
software.sector_key; software.sector_name
software.top_performing_companies; software.top_growth_companies

# Chaining with Ticker:
msft = yf.Ticker('MSFT')
tech = yf.Sector(msft.info.get('sectorKey'))
software = yf.Industry(msft.info.get('industryKey'))

# Region scoping (ISO 3166-1 alpha-2, case-insensitive) — affects top_companies/top_etfs/top_mutual_funds:
yf.Sector("technology", region="GB").top_companies     # UK
yf.Sector("technology", region="DE")                    # Germany
```

---

## 8. Market summary & status

```python
EUROPE = yf.Market("EUROPE")     # one of: US, GB, ASIA, EUROPE, RATES, COMMODITIES, CURRENCIES, CRYPTOCURRENCIES
EUROPE.status      # market open/closed info — NOTE: Yahoo's backing endpoint currently ignores the
                     # market param and only ever returns US data; non-US .status is None + logs a warning
EUROPE.summary      # regional index summary — this one DOES return real per-region data
```

---

## 9. Calendars — earnings/IPO/splits/economic events

```python
from datetime import datetime, timedelta

calendar = yf.Calendars()                              # default window: today -> +7 days
calendar = yf.Calendars(start, end)                      # custom window

calendar.earnings_calendar          # property (lazy fetch)
calendar.ipo_info_calendar
calendar.splits_calendar
calendar.economic_events_calendar

calendar.get_earnings_calendar(limit=100, market_cap=100_000_000, filter_most_active=True)
# market_cap: filter out small caps below this value
# filter_most_active: True -> only actively-traded names (internally uses screen("MOST_ACTIVES"))
calendar.get_ipo_info_calendar()
calendar.get_splits_calendar()
calendar.get_economic_events_calendar()
```

```python
# Real use-case: find earnings due in next 2 (or 4 over a weekend) days that haven't reported yet
today = datetime.now()
is_friday = today.weekday() == 4
horizon = today + timedelta(days=4 if is_friday else 2)
df = yf.Calendars(today, horizon).get_earnings_calendar(limit=100)
unreported = df[df["Reported EPS"].isnull()]
```

---

## 10. Screener — `EquityQuery` / `FundQuery` / `ETFQuery` + `screen()`

Build boolean filter trees, then run them against Yahoo's screener backend.

```python
from yfinance import EquityQuery

# Operators: eq, is-in, btwn, gt, lt, gte, lte  (value ops)  +  and, or  (combinators)
q = EquityQuery('and', [
    EquityQuery('gt', ['percentchange', 3]),
    EquityQuery('eq', ['region', 'us']),
])

response = yf.screen(q, sortField='percentchange', sortAsc=True)
```

```python
yf.screen(
    query,                # str (predefined name) | EquityQuery | FundQuery | ETFQuery
    offset=None,           # default 0
    size=None,             # custom query result count, default 100, max 250
    count=None,            # predefined query result count, default 25, max 250
    sortField=None,        # default "ticker"
    sortAsc=None,           # default False
    userId=None, userIdType=None, session=None,
)
```

**Predefined queries** (instant, no filter-building needed):
```python
yf.screen("aggressive_small_caps")          # equity screens:
# aggressive_small_caps, day_gainers, day_losers, growth_technology_stocks, most_actives,
# most_shorted_stocks, small_cap_gainers, undervalued_growth_stocks, undervalued_large_caps
# fund screens: conservative_foreign_funds, high_yield_bond, portfolio_anchors,
#   solid_large_growth_funds, solid_midcap_growth_funds, top_mutual_funds
# ETF screens: top_etfs_us, top_performing_etfs, technology_etfs, bond_etfs

yf.PREDEFINED_SCREENER_QUERIES.keys()        # list all available predefined query names
yf.PREDEFINED_SCREENER_QUERIES['aggressive_small_caps']   # inspect the underlying EquityQuery tree
```

**Valid filter fields** (`EquityQuery.valid_fields`) — grouped by category, abbreviated; call `EquityQuery.valid_fields` / `.valid_values` yourself for the full live list:
- `eq_fields`: `exchange`, `industry`, `peer_group`, `region`, `sector`
- `price`: `intradayprice`, `intradaymarketcap`, `percentchange`, `fiftytwowkpercentchange`, ...
- `trading`: `avgdailyvol3m`, `beta`, `dayvolume`, `pctheldinsider`, `pctheldinst`, ...
- `valuation`: `peratio.lasttwelvemonths`, `pegratio_5y`, `pricebookratio.quarterly`, ...
- `income_statement` / `balance_sheet` / `cash_flow` / `profitability` / `leverage` / `liquidity` / `esg`: dozens of `*.lasttwelvemonths` fundamental metrics.

`FundQuery`/`ETFQuery` work identically but with their own field sets (`categoryname`, `performanceratingoverall`, `annualreportnetexpenseratio`, ...).

---

## 11. Live streaming — `WebSocket` / `AsyncWebSocket`

```python
# Synchronous
def message_handler(message):
    print("Received message:", message)

with yf.WebSocket() as ws:
    ws.subscribe(["AAPL", "BTC-USD"])
    ws.listen(message_handler)

# or without context manager:
ws = yf.WebSocket()
ws.subscribe(["AAPL", "BTC-USD"])
ws.listen(message_handler)
```

```python
# Asynchronous
import asyncio

async def main():
    async with yf.AsyncWebSocket() as ws:
        await ws.subscribe(["AAPL", "BTC-USD"])
        await ws.listen()

asyncio.run(main())
```
In Jupyter, nested event loops break `asyncio.run`; fix with:
```python
import nest_asyncio; nest_asyncio.apply()
```

---

## 12. Authentication (optional, for logged-in-only data)

Yahoo's login form is reCAPTCHA-gated, so yfinance **cannot automate the login itself**. You manually lift cookies from a browser session.

```python
auth = yf.Auth()
auth.set_login_cookies(os.getenv("COOKIE_T"), os.getenv("COOKIE_Y"))
auth.check_login()     # bool
auth.user               # dict: account info, once logged in
# all subsequent requests in this process now run as the logged-in user
```
To get `T`/`Y` cookies: log into finance.yahoo.com in a browser → DevTools (F12) → Application/Storage tab → Cookies → `https://finance.yahoo.com` → copy values of cookies named `T` and `Y`.

---

## 13. Global config (`yf.config`)

```python
yf.config
# {
#   "network": {"proxy": null, "retries": 0},
#   "debug":   {"hide_exceptions": true, "logging": false}
# }

yf.config.network.proxy = "http://user:pass@host:port"   # route ALL requests through a proxy
yf.config.network.retries = 2          # auto-retry transient network errors, exponential backoff (1s,2s,4s,...)

yf.config.debug.hide_exceptions = False   # stop yfinance from silently swallowing exceptions
yf.config.debug.logging = True            # verbose debug logging (also via yf.enable_debug_mode())
```

```python
yf.enable_debug_mode()                       # shortcut for yf.config.debug.logging = True
```

### Sessions & timeouts
Every fetch-capable call/constructor accepts `session=` (a `requests.Session`, or a `curl_cffi` session for harder-to-block requests) and most accept `timeout=` in seconds. Pass your own `session` once and reuse across many `Ticker(...)` calls to share connection pooling / custom headers / proxies per-request instead of globally.

---

## 14. Caching

yfinance persists two things locally to cut down on requests: **timezone lookups** and **cookies**.

| OS | Default cache path |
|---|---|
| Windows | `C:/Users/<USER>/AppData/Local/py-yfinance` |
| Linux | `/home/<USER>/.cache/py-yfinance` |
| macOS | `/Users/<USER>/Library/Caches/py-yfinance` |

```python
yf.set_tz_cache_location("custom/cache/location")   # redirect cache elsewhere (e.g. read-only filesystems)
```

---

## 15. Price repair (`repair=True`)

Yahoo's raw data occasionally has: 100x unit errors (price suddenly ×100 or ÷100 for one bar), missing bars, and bad dividend-adjustment math. Passing `repair=True` to `history()`/`download()` detects and fixes these using internal heuristics (cross-checking against volume, adjacent days, dividend events). Off by default because it costs extra requests/compute. Turn on if you see suspicious one-bar spikes in cleaned datasets.

---

## 16. Error handling cheat-sheet

```python
from yfinance.exceptions import (
    YFException, YFDataException, YFNotImplementedError,
    YFInvalidPeriodError, YFPricesMissingError, YFTzMissingError, YFRateLimitError,
)
```
- By default yfinance **hides** most exceptions and logs instead (`yf.config.debug.hide_exceptions = True` is the default). Flip to `False` to get real tracebacks while developing.
- `YFRateLimitError` — you're being throttled by Yahoo; back off, add delay, or set `yf.config.network.retries`.
- `download()`/bulk `history()` failures across many tickers are summarized, not raised, by default (`"N Failed downloads"` log line) — check returned DataFrame for `NaN`-only ticker columns rather than expecting an exception per ticker.
- Setting `period` + `start` + `end` together always raises plain `ValueError("Setting period, start and end is nonsense. Set maximum 2 of them.")`, for both `Ticker.history()` and `download()`.

---

## 17. Quick patterns you'll actually reuse

```python
# Current price without the slow full info dict
yf.Ticker("AAPL").fast_info["lastPrice"]

# Last 6 months daily OHLCV, ready for pandas/numpy work
df = yf.Ticker("AAPL").history(period="6mo")

# Bulk-grab close prices for a basket, single tidy DataFrame
closes = yf.download(["AAPL","MSFT","GOOG"], period="1y")["Close"]

# Nearest-expiry option chain
t = yf.Ticker("AAPL")
calls = t.option_chain(t.options[0]).calls

# Today's biggest gainers, no manual filter-building
gainers = yf.screen("day_gainers")

# Custom screen: US tech stocks with >25% trailing EPS growth, sorted by volume
from yfinance import EquityQuery
q = EquityQuery('and', [
    EquityQuery('eq', ['region', 'us']),
    EquityQuery('eq', ['sector', 'Technology']),
    EquityQuery('gt', ['epsgrowth.lasttwelvemonths', 25]),
])
result = yf.screen(q, sortField='dayvolume', sortAsc=False)

# Whole income statement, quarterly, pretty row labels, as a dict
t.get_income_stmt(freq='quarterly', pretty=True, as_dict=True)
```

---

## Source
Compressed from the official docs: https://ranaroussi.github.io/yfinance/ (API Reference + Advanced sections) and the project source on GitHub (`ranaroussi/yfinance`), current as of mid-2026. Library is actively developed — if a property/method here 404s on you, check the live API Reference page for renames; Yahoo's undocumented backend also changes occasionally without yfinance's control.
