let xhr = new XMLHttpRequest();
let file = "http://127.0.0.1:5500/Ajax-Tutorial/js/function.js";

xhr.open("GET", file, false);
xhr.getResponseHeader("Content-Type", "application/json");

xhr.onload = function () {
    let json = this.response;
    let scr = document.createElement("script");
    scr.type = "text/javascript";
    scr.innerHTML = json;

    document.body.appendChild(scr);
    console.log(scr);

    scr.onload = function () {
        SendXML("GET", file, "application/json").xhr.onload =
            function () { console.log(this.response); };
    }();
}

xhr.send();