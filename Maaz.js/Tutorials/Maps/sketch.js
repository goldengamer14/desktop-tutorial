console.log("Maps in Javascript");

const map = new Map();

let str = "Hi! This is a string.";
let obj = { class: "Empty", id: "Nothing" };
let fun = () => { console.log("This"); };

// How to set keys and values in a Map Object
map.set(str, typeof str);
map.set(obj, typeof obj);
map.set(fun, typeof fun);

// How to know the number of key-value pairs of a Map Object
let size = map.size;

// Can be extracted using the for-of and for-each loops
const newObj = {};

for (const keyVal of map) {
    newObj[keyVal[1]] = keyVal[0];
}

console.log("Extraction through forEach %cstart", "color: #00f");
map.forEach((val, key) => {
    console.log(key, "=>", val);
});
console.log("Extraction through forEach %cend", "color: #f00");

// How to extract only the keys or values
const keys = map.keys();
const values = map.values();

// You get all the entries in an Array of Objects with {key => value} 
const entries = map.entries();

// Conversion to Array
let myArray = Array.from(map);
let myKeys = Array.from(map.keys());
let myValues = Array.from(map.values());

console.log("Conversion to Array %cstart", "color: #00f");
console.log([myArray, myKeys, myValues]);
console.log("Conversion to Array %cend", "color: #f00");

console.log(map, size, newObj, keys, values, entries);