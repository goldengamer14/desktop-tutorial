console.log("Generators in Javascript");

// Syntax for generator function '*'
function* generator() {
    let i = 1;

    while (true) {
        yield i++;
        i > 60 ? i = 1 : null;
    }

    // yield 1;
    // yield 2;
    // yield 3;
}

let generate = generator();

// Returns an object with the 'value' and 'done' to show if the values are ended or not
console.log(generate.next());
console.log(generate.next());
console.log(generate.next());
// Returns undefined if all the values are extracted
console.log(generate.next());
// Use '.value' if you want just the value and not the object
console.log(generate.next().value);
console.log(generate.next().value);