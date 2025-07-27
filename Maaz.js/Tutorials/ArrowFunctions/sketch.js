// Part No.1

document.getElementById("printer").innerHTML += "<ul>Part No.1:";

// Old, Simple and Time Wasting way to write a Function
let hello = function () {
    document.getElementById("printer").innerHTML += "<li class=\"fun\">normal function: Hello World</li>";
    console.log("normal function: %cHello World", "color:red");
}
hello();

// modern, stylish and easier way to write a Function
hello = () => {
    document.getElementById("printer").innerHTML += "<li class=\"arrow\">arrow function: Hello World</li></ul><br><br>";
    console.log("arrow function: %cHello World", "color:blue");
}
hello();

// Part No.2

document.getElementById("printer").innerHTML += "<ul>Part No.2:";

// Have to use Curly Braces for single-lines
let greet = function () {
    return "Hey! How are you";
}
document.getElementById("printer").innerHTML += "<li class=\"fun\">" + greet() + "</li>";
console.log("normal function: " + "%c" + greet(), "color:red");

// No need for Braces or return for single-lines
greet = () => "Hey! how are you";
document.getElementById("printer").innerHTML += "<li class=\"arrow\">" + greet() + "</li></ul><br><br>";
console.log("normal function: " + "%c" + greet(), "color:blue");

//Part No.3
document.getElementById("printer").innerHTML += "<ul>Part No.3:";

// Have to use Curved Brackets everytime
let hi = function (name) {
    return "Hello " + name;
}
document.getElementById("printer").innerHTML += "<li class=\"fun\">" + hi("World") + "</li>";
console.log("normal function: " + "%c" + hi("World"), "color:red");

// No need to use Curved Brackets when only one parameter
hi = name => "Hello " + name;
document.getElementById("printer").innerHTML += "<li class=\"arrow\">" + hi("World") + "</li></ul><br>";
console.log("arrow function: " + "%c" + hi("World"), "color:blue");