// Variables Declaration
const userId = document.getElementById("user-id");
const initialDeposit = document.getElementById("initial-deposit");
const changeId = document.getElementById("change-id");
const depositWithdrawAmount = document.getElementById("deposit-withdraw");
const company = document.getElementById("company");
const shareQuantity = document.getElementById("share-quantity");
const outputSection = document.getElementById("output-section");
const transactionHeader = document.getElementById("transaction-header");
const transactionOutput = document.getElementById("transaction-output");
const portfolioHeader = document.getElementById("portfolio-header");
const portfolioOutput = document.getElementById("portfolio-output");


const portfolios = JSON.parse(localStorage.getItem("Account Portfolios")) ?? {};
const userHeader = document.getElementById("user-header");
if (localStorage.getItem("Current User")) userHeader.innerHTML = `Current User: ${localStorage.getItem("Current User")}`;

const companies = { "None": null, "AAPL": 150 * 87.21, "TSLA": 700 * 87.21, "GOOGL": 2800 * 87.21 };

// Button Events handlers
document.getElementById("create-account").addEventListener("click", createAccount);
document.getElementById("deposit").addEventListener("click", deposit);
document.getElementById("withdraw").addEventListener("click", withdraw);
document.getElementById("buy-shares").addEventListener("click", buyShares);
document.getElementById("sell-shares").addEventListener("click", sellShares);
document.getElementById("show-transactions").addEventListener("click", showTransactions);
document.getElementById("show-portfolio").addEventListener("click", showPortfolio);
document.getElementById("remove-account").addEventListener("click", removeAccount);
document.getElementById("switch-id").addEventListener("click", switchAccount);


// Account Handler Functions
function createAccount() {
    this.error = document.getElementById("creation-error");
    const initialAmount = Number.parseFloat(initialDeposit.value) || 0;
    const currentId = userId.value;

    if (!currentId) {
        throwError(this.error, "Enter User ID to create an account");
        return;
    } else if (initialAmount < 0) {
        throwError(this.error, "The amount must be more than 0");
        return;
    } else if (portfolios[currentId]) {
        throwError(this.error, "The named account already exists, do you wanna reset it?");
        if (!confirm("The named account already exists, do you wanna reset it?"))
            return;
    } else
        resetError(this.error);

    const newAccount = {
        userId: userId.value, initialDeposit: initialAmount, currentAmount: initialAmount,
        heldShares: {}, transactions: []
    };
    portfolios[currentId] = newAccount;

    localStorage.setItem("Account Portfolios", JSON.stringify(portfolios));
    localStorage.setItem("Current User", currentId);

    userHeader.innerHTML = `Current User: ${currentId}`;
}

function deposit() {
    this.error = document.getElementById("deposit-withdrawal-error");
    const currentId = localStorage.getItem("Current User");
    const currentAmount = Number.parseFloat(depositWithdrawAmount.value);

    if (!portfolios[currentId]) {
        throwError(this.error, "Create an account first to deposit into it");
        return;
    }
    // else if (!currentAmount) {
    //     throwError(this.error, "Enter the amount to be deposited");
    //     return;
    // } 
    else if (currentAmount <= 0) {
        throwError(this.error, "The amount to be deposited must be more than 0");
        return;
    } else
        resetError(this.error);

    portfolios[currentId].currentAmount += currentAmount;
    portfolios[currentId].transactions.push({ action: "deposit", amount: currentAmount });

    localStorage.setItem("Account Portfolios", JSON.stringify(portfolios));
}

function withdraw() {
    this.error = document.getElementById("deposit-withdrawal-error");
    const currentId = localStorage.getItem("Current User");
    const currentAmount = Number.parseFloat(depositWithdrawAmount.value);

    if (!portfolios[currentId]) {
        throwError(this.error, "Create an account first to withdraw from it");
        return;
    }
    // else if (!currentAmount) {
    //     throwError(this.error, "Enter the amount to be withdrawn");
    //     return;
    // }
    else if (currentAmount <= 0) {
        throwError(this.error, "The amount to be withdrawn must be more than 0");
        return;
    } else if (currentAmount > portfolios[currentId].currentAmount) {
        throwError(this.error, "Your account does not contain enough wealth");
        return;
    } else
        resetError(this.error);

    portfolios[currentId].currentAmount -= currentAmount;
    portfolios[currentId].transactions.push({ action: "withdraw", amount: currentAmount });

    localStorage.setItem("Account Portfolios", JSON.stringify(portfolios));
}

function buyShares() {
    this.error = document.getElementById("buy-sell-error");
    const currentId = localStorage.getItem("Current User");
    const currentCompany = company.value;
    const quantity = Number.parseInt(shareQuantity.value);

    if (!portfolios[currentId]) {
        throwError(this.error, "Create an account first to buy shares");
        return;
    } else if (quantity <= 0) {
        throwError(this.error, "The quantity of shares to be bought must be more than 0");
        return;
    } else if (companies[currentCompany] == "None") {
        throwError(this.error, "Select a company to proceed");
        return;
    } else if (quantity * companies[currentCompany] > portfolios[currentId].currentAmount) {
        throwError(this.error, "Your account does not contain enough wealth");
        return;
    } else
        resetError(this.error);

    portfolios[currentId].currentAmount -= quantity * companies[currentCompany];
    portfolios[currentId].transactions.push({
        action: "Buy Shares", company: currentCompany,
        quantity: quantity, amount: quantity * companies[currentCompany]
    });
    portfolios[currentId].heldShares[currentCompany] ? portfolios[currentId].heldShares[currentCompany] += quantity
        : portfolios[currentId].heldShares[currentCompany] = quantity;

    localStorage.setItem("Account Portfolios", JSON.stringify(portfolios));
}

function sellShares() {
    this.error = document.getElementById("buy-sell-error");
    const currentId = localStorage.getItem("Current User");
    const currentCompany = company.value;
    const quantity = Number.parseInt(shareQuantity.value);

    if (!portfolios[currentId]) {
        throwError(this.error, "Create an account first to sell shares");
        return;
    } else if (quantity <= 0) {
        throwError(this.error, "The quantity of shares to be sold must be more than 0");
        return;
    } else if (companies[currentCompany] == "None") {
        throwError(this.error, "Select a company to proceed");
        return;
    } else if (!portfolios[currentId].heldShares[currentCompany] || quantity > portfolios[currentId].heldShares[currentCompany]) {
        throwError(this.error, "You don't possess enough shares to sell");
        return;
    } else
        resetError(this.error);

    portfolios[currentId].currentAmount += quantity * companies[currentCompany];
    portfolios[currentId].transactions.push({
        action: "Sell Shares", company: currentCompany,
        quantity: quantity, amount: quantity * companies[currentCompany]
    });
    portfolios[currentId].heldShares[currentCompany] -= quantity;

    localStorage.setItem("Account Portfolios", JSON.stringify(portfolios));
}

function showTransactions() {
    const currentAccount = portfolios[localStorage.getItem("Current User")];

    transactionOutput.innerHTML = "<br>";

    currentAccount.transactions.forEach(e => {
        switch (e.action) {
            case "deposit":
                transactionOutput.innerHTML += `Deposited amount ${e.amount}`;
                break;

            case "withdraw":
                transactionOutput.innerHTML += `Withdrawn amount ${e.amount}`;
                break;

            case "Buy Shares":
                transactionOutput.innerHTML += `Bought ${e.quantity} shares of ${e.company} for ${e.amount}`;
                break;

            case "Sell Shares":
                transactionOutput.innerHTML += `Sold ${e.quantity} shares of ${e.company} for ${e.amount}`;
                break;

            default:
                break;
        }

        transactionOutput.innerHTML += "<br><br>";
    });

    console.log(currentAccount.transactions);
    outputSection.style.display = transactionHeader.style.display = "block";

}

function showPortfolio() {
    const currentAccount = portfolios[localStorage.getItem("Current User")];

    portfolioOutput.innerHTML = `<br>
    User ID: ${currentAccount.userId}<br><br>
    Initial Deposit: ${currentAccount.initialDeposit}<br><br>
    Current Amount: ${currentAccount.currentAmount}<br><br>
    Held Shares: {<br><br>
    `;

    let totalShareValue = 0;
    for (const comp in companies)
        if (currentAccount.heldShares[comp]) {
            portfolioOutput.innerHTML +=
                `<span style="padding-left:1rem;">${comp}: ${currentAccount.heldShares[comp]}</span><br><br>`;
            totalShareValue += currentAccount.heldShares[comp] * companies[comp];
        }

    portfolioOutput.innerHTML += "}<br><br>";

    portfolioOutput.innerHTML += `Total Portfolio Value: ${currentAccount.currentAmount + totalShareValue}`;
    portfolioOutput.innerHTML += "<br><br>";

    const logAccount = {
        userId: currentAccount.userId, initialDeposit: currentAccount.initialDeposit, currentAmount: currentAccount.currentAmount,
        heldShares: currentAccount.heldShares
    };
    console.log(logAccount);

    outputSection.style.display = portfolioHeader.style.display = "block";
}

function removeAccount() {
    this.error = document.getElementById("removal-error");
    const currentId = localStorage.getItem("Current User");

    if (!portfolios[currentId]) {
        throwError(this.error, "You don't have an Account to delete");
        return;
    } else
        resetError(this.error);

    delete portfolios[currentId];

    if (Object.keys(portfolios).length) {
        localStorage.setItem("Account Portfolios", JSON.stringify(portfolios));

        const newUser = Object.keys(portfolios)[Object.keys(portfolios).length - 1] || "";
        localStorage.setItem("Current User", newUser);

        userHeader.innerHTML = `Current User: ${newUser}`;
    }
    else {
        localStorage.removeItem("Account Portfolios");
        localStorage.removeItem("Current User");
        userHeader.innerHTML = "Current User:";
    }
}

function switchAccount() {
    this.error = document.getElementById("switch-error");
    const switchId = changeId.value;

    if (!portfolios[switchId]) {
        throwError(this.error, "The Entered Account doesn't exist");
        return;
    } else
        resetError(this.error);

    localStorage.setItem("Current User", switchId);
    userHeader.innerHTML = `Current User: ${switchId}`;
}

// Error Handlers
function throwError(errorElement, message) {
    console.error(message);
    errorElement.style.display = "block";
    errorElement.innerHTML = message;
}

function resetError(errorElement) {
    errorElement.style.display = "none";
    errorElement.innerHTML = "";
}