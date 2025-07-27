console.log("Welcome to Sets in Javascript tutorial");

let set = [];
set[0] = new Set();

set[0].add({ class: "Monkey", gender: "female", age: 15 });
set[0].add("LOL");
set[0].add(() => alert("Broke"));
set[0].add(true);
set[0].add(false);

// Doesn't let the values repeat if they're exactly the same
set[0].add("LOL");
set[0].add("lol");

newRow(0);

// document.getElementById("Set").innerHTML += "<tr> <td> <ul id='Set1'></ul> </td> </tr>";
// set[0].forEach(e => document.getElementById("Set1").innerHTML += `<li><pre>${e}</pre></li>`);

//  Second method to set values
set[1] = new Set([set[0], "String", ...set[0], set[0]]);

// document.getElementById("Set").innerHTML += "<tr> <td> <ul id='Set2'></ul> </td> </tr>";
// set[1].forEach(e => document.getElementById("Set2").innerHTML += `<li><pre>${e}</pre></li>`);

// Delete an element from the Set
set[1].delete(set[0]);

newRow(1);


// Get the size of the Set
set.forEach((e, i) => console.log("Size of set " + (i + 1) + " is", e.size));

// Figure out if the Set contains something
set.forEach((e, i) => console.log("Does the set " + (i + 1) + " contains 'String'? ", e.has("String")));


console.log(set);


function newRow(index) {
    const num = index + 1;

    document.getElementById("Set").innerHTML += `<tr> <td> <ul id="Set${num}"></ul> </td> </tr>`;
    set[index].forEach(e => document.getElementById("Set" + num).innerHTML += `<li><pre>${e}</pre></li>`);
}