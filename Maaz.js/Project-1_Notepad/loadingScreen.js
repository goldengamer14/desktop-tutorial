let dots = [];
let loscr = document.createElement("h1");
// loscr.style.color = "white";
// loscr.style.position = "absolute";
// loscr.style.left = "40vw";
// loscr.style.top = "48vh";
loscr.setAttribute("style", "position:absolute;left:15vw;top:48vh;color:yellow;background-color:blueviolet;border:2px solid red");
loscr.innerHTML = "You may need to wait 5 seconds: Loading";
document.body.appendChild(loscr);
let loadingScreen = true;

setInterval(function () {

    if (loadingScreen === true) {
        if (dots.length < 5) {
            dots.push('.');
            loscr.innerHTML += dots[0];
        }
        if (dots.length > 4) {
            dots.splice(1, dots.length);
            loscr.innerHTML = "Loading";
            loscr.style.left = "40vw";

        }
    }

}, 200);


let loadingTime = setTimeout(function () {

    let script = document.createElement("script");
    script.setAttribute("src", "sketch.js");
    script.setAttribute("id", "scr");
    document.body.appendChild(script);

}, 5000);


setTimeout(function () {

    if (document.getElementById("scr")) {
        loadingScreen = false;
        loscr.parentNode.removeChild(loscr);
    }

}, 5000);