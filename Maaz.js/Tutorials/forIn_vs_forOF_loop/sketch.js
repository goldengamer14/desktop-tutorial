console.log("here starts the file");

let companies = ["Honda", "Bajaj", "Asian Paints", "Microsoft"];

console.log("%cCompanies", "color: blue; font-style: italic; font-weight: bold");

// Original or Traditional For loop
for (let i = 0; i < companies.length; i++) {
    console.log(companies[i]);
}

// For-of loop: better for Arrays
for (const company of companies) {
    document.getElementById("forOf").innerHTML += `<li>${company}</li>`;
}

let stats = {
    name: "Microsoft",
    profit: "$30 Billion",
    loan: "$4 Billion",
    reserve: "$10 Billion"
};

console.log("%cStats of a company", "color: red; font-style: italic; font-weight: bold");

// Original or Traditional For loop
for (let i = 0; i < Object.keys(stats).length; i++) {
    console.log(stats[Object.keys(stats)[i]]);
}

// For-in loop: better for Objects
for (const key in stats) {
    document.getElementById("forIn").innerHTML += `<li>${stats[key]}</li>`;
}

// Bonus fact: For-in with String will extract index of it's characters
//             and For-of will extract all the characters
let string = "Hi! How're you, my boy."
let strArr = [];

console.log("%cCan be used with Strings as well", "color: darkgreen; font-weigth: bold; font-style: italic");
for (const letter of string) {
    strArr.push(letter);
}
console.log("%cFor of:", "color: blue", strArr);

strArr = [];
for (const indexOfLetter in string) {
    strArr.push(string[indexOfLetter]);
}
console.log("%cFor in:", "color: red", strArr);

strArr = [];
for (let index = 0; index < string.length; index++) {
    strArr.push(string[index]);
}
console.log("For", strArr);


// Conclusion:
//          For-in: Extracts the indexes and keys of the parent
//          For-of: Extracts the values of the parent