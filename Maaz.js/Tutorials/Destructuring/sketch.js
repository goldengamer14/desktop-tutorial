console.log("Welcome to %cDestructuring", "color: #0aa", "in Javascript");

//                  !!! Array Destructuring !!!
console.warn("%cArray", "color: #d00", "Destructuring!");

// Another way to assign variables
let [a, b] = [34, 564];
console.log("a is:", a, "b is:", b);

let c, d, arr = [];

for (let i = 1; i < 14; i++) {
    arr.push(i);
}

// If more values are assign than the number of variables,
// the variables are assign as their limit
[a, b, c, d] = arr;

console.log("a is:", a, "b is:", b, "c is:", c, "d is:", d); // a = 1, b = 2, c = 3, d = 4
console.log("the array was:", arr);

console.warn("Variables reassigned!");

// Will assign all the rest variables as an array in d
[a, b, c, ...d] = arr;

console.log("a is:", a, "b is:", b, "c is:", c, "d is:", d); // a = 1, b = 2, c = 3, d = [4, 5, 6, 7, 8, 9, 10]
console.log("the array was:", arr);

// Way to assign variables through objects
({ a, b, c } = { a: 45, b: 346, c: 123, d: 32, e: 34 });
console.log("a is:", a, "b is:", b, "c is:", c); // a = 45, b = 346, c = 123

console.warn("Variables reassigned!");

// Will assign all the rest variables as an object in c
({ a, b, ...c } = { a: 45, b: 346, c: 123, d: 32, e: 34 });
console.log("a is:", a, "b is:", b, "c is:", c); // a = 45, b = 346, c = {c: 123, d: 32, e: 34}


//                  !!! Object Destructuring !!!
console.warn("%cObject", "color: #0a0", "Destructuring!");

const cooler = {
    company: "Philips",
    cooling: "very cold",
    warantee: 1,
    price: 16000
};

let { company, cooling, warantee, price, something } = cooler;

// Because something doesn't exist in the Object, it's undefined
console.log("company:", company, "cooling type:", cooling, "warantee:", warantee, "price", price,
    "something", something); // company = "philips", cooling = "very cold", warantee = 1, price = 16000
console.log("The object cooler:", cooler);