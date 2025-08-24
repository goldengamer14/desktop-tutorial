// Simulated stock prices
function get_share_price(symbol) {
    const prices = { AAPL: 150, TSLA: 700, GOOGL: 2800 };
    return prices[symbol.toUpperCase()] || null;
}

class TradingAccount {
    constructor(initialDeposit) {
        this.initialDeposit = initialDeposit;
        this.balance = initialDeposit;
        this.holdings = {}; // { AAPL: 10, TSLA: 2 }
        this.transactions = []; // { type, symbol, quantity, price, date }
    }

    deposit(amount) {
        if (amount <= 0) throw new Error("Invalid deposit amount.");
        this.balance += amount;
        this.transactions.push({ type: 'deposit', amount, date: new Date() });
    }

    withdraw(amount) {
        if (amount <= 0) throw new Error("Invalid withdrawal amount.");
        if (this.balance < amount) throw new Error("Insufficient funds.");
        this.balance -= amount;
        this.transactions.push({ type: 'withdraw', amount, date: new Date() });
    }

    buy(symbol, quantity) {
        symbol = symbol.toUpperCase();
        const price = get_share_price(symbol);
        if (!price) throw new Error("Invalid symbol.");
        const cost = price * quantity;
        if (this.balance < cost) throw new Error("Insufficient funds to buy.");
        this.balance -= cost;
        this.holdings[symbol] = (this.holdings[symbol] || 0) + quantity;
        this.transactions.push({ type: 'buy', symbol, quantity, price, date: new Date() });
    }

    sell(symbol, quantity) {
        symbol = symbol.toUpperCase();
        if (!this.holdings[symbol] || this.holdings[symbol] < quantity) {
            throw new Error("Insufficient shares to sell.");
        }
        const price = get_share_price(symbol);
        if (!price) throw new Error("Invalid symbol.");
        const revenue = price * quantity;
        this.balance += revenue;
        this.holdings[symbol] -= quantity;
        this.transactions.push({ type: 'sell', symbol, quantity, price, date: new Date() });
    }

    getPortfolioValue() {
        let total = this.balance;
        for (let symbol in this.holdings) {
            const price = get_share_price(symbol);
            total += (this.holdings[symbol] || 0) * price;
        }
        return total;
    }

    getProfitOrLoss() {
        return this.getPortfolioValue() - this.initialDeposit;
    }

    getHoldings() {
        return { ...this.holdings };
    }

    getTransactions() {
        return [...this.transactions];
    }
}

// UI Logic
let account = null;

function createAccount() {
    const amount = parseFloat(document.getElementById('initialDeposit').value);
    if (isNaN(amount) || amount <= 0) return alert("Enter a valid deposit.");
    account = new TradingAccount(amount);
    showOutput(`Account created with $${amount.toFixed(2)}.`);
}

function depositFunds() {
    try {
        const amount = parseFloat(document.getElementById('fundsAmount').value);
        account.deposit(amount);
        showOutput(`Deposited $${amount.toFixed(2)}.`);
    } catch (e) {
        alert(e.message);
    }
}

function withdrawFunds() {
    try {
        const amount = parseFloat(document.getElementById('fundsAmount').value);
        account.withdraw(amount);
        showOutput(`Withdrew $${amount.toFixed(2)}.`);
    } catch (e) {
        alert(e.message);
    }
}

function buyShares() {
    try {
        const symbol = document.getElementById('symbol').value;
        const quantity = parseInt(document.getElementById('quantity').value);
        if (!symbol || isNaN(quantity) || quantity <= 0) throw new Error("Invalid input.");
        account.buy(symbol, quantity);
        showOutput(`Bought ${quantity} shares of ${symbol.toUpperCase()}.`);
    } catch (e) {
        alert(e.message);
    }
}

function sellShares() {
    try {
        const symbol = document.getElementById('symbol').value;
        const quantity = parseInt(document.getElementById('quantity').value);
        if (!symbol || isNaN(quantity) || quantity <= 0) throw new Error("Invalid input.");
        account.sell(symbol, quantity);
        showOutput(`Sold ${quantity} shares of ${symbol.toUpperCase()}.`);
    } catch (e) {
        alert(e.message);
    }
}

function showPortfolio() {
    const holdings = account.getHoldings();
    const value = account.getPortfolioValue();
    const profit = account.getProfitOrLoss();
    let output = `Balance: $${account.balance.toFixed(2)}\nPortfolio Value: $${value.toFixed(2)}\nProfit/Loss: $${profit.toFixed(2)}\n\nHoldings:\n`;
    for (let symbol in holdings) {
        output += `${symbol}: ${holdings[symbol]} shares @ $${get_share_price(symbol)}\n`;
    }
    showOutput(output);
}

function showTransactions() {
    const txs = account.getTransactions();
    let output = `Transactions:\n`;
    for (let tx of txs) {
        if (tx.type === 'buy' || tx.type === 'sell') {
            output += `[${tx.date.toLocaleString()}] ${tx.type.toUpperCase()} ${tx.quantity} ${tx.symbol} @ $${tx.price}\n`;
        } else {
            output += `[${tx.date.toLocaleString()}] ${tx.type.toUpperCase()} $${tx.amount}\n`;
        }
    }
    showOutput(output);
}

function showOutput(text) {
    document.getElementById('output').textContent = text;
}