console.log("Welcome to %cSymbols", "color: #f0f", "in Javascript");

// Symbols
const symbols = [];

symbols[0] = Symbol("Hello, World!");

// Can contain all types of data
symbols[1] = [Symbol(() => alert("Hi!")), Symbol(() => alert("Hi!"))];

// Each Symbol is unique and different from each other
console.log("Are both Symbols equal?", symbols[1][0] === symbols[1][1]);

// Use of Symbols:
// To add keys in an Object that won't collide with other keys that'll be added by another code
const keys = [Symbol("k"), Symbol("k"), "k", "k"];
const obj = {};

obj[keys[0]] = "Hey";
obj[keys[1]] = "There";

// Won't work with something else
obj[keys[2]] = "My";
obj[keys[3]] = "Friend"; // Will overwrite the old k

console.log("Are k1 and k2 equal?", obj[keys[0]] === obj[keys[1]],
    "\nAre k3 and k4 equal?", obj[keys[2]] === obj[keys[3]]);
console.log(obj[keys[0]], obj[keys[1]], obj[keys[2]], obj[keys[3]], obj);

// Symbols are ignored in For-in loop
for (let key in obj) {
    console.log("%cfor in:", "color: #00f", key, obj[key]);
}

// Symbols can't be stringified
console.log("%cStrigified obj:", "color: #0a0", JSON.stringify(obj));

console.log(symbols);