console.log("Hello, World");

// Declaring variables
const screen = document.getElementById("screen");
const buttons = [];

Array.from(document.getElementsByTagName("td")).forEach(e => {
    buttons.push(e.firstElementChild);
});

// Putting event-listeners on buttons
buttons.forEach(e => {
    e.onclick = () => reactOnClick(e);
});

console.log(screen, buttons);

function reactOnClick(e) {

    if (Number.isInteger(Number(e.innerHTML)) || e.innerHTML == ".") {
        screen.value += e.innerHTML;
    }

    else if (e.innerHTML == "÷" || e.innerHTML == "×" || e.innerHTML == "-" || e.innerHTML == "+" || e.innerHTML == "(" || e.innerHTML == ")") {
        screen.value += (e.innerHTML == "×" ? "*" : e.innerHTML == "÷" ? "/" : e.innerHTML);
    }

    else if (e.innerHTML == "C") {
        screen.value = "";
    }

    else if (e.innerHTML == "=") {
        screen.value = eval(screen.value);
    }

    else if (e.innerHTML == "del") {
        screen.value = screen.value.substring(0, screen.value.length - 1);
    }

    else if (e.innerHTML == "%") {
        screen.value += "/100";
    }

    else if (e.innerHTML == "+/-") {
        screen.value += "*-1";
    }
};