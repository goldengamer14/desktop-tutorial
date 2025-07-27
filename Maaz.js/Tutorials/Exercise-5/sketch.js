
let api;
const printer = document.getElementById("printer");
const inp = document.getElementsByTagName("input")[0];
const btn = document.getElementsByTagName("button")[0];

SendXML("GET", "http://127.0.0.1:5500/Tutorials/Exercise-5/api.json",
    "application/json").xhr.onload =
    function () {
        api = JSON.parse(this.response);
    };

btn.onclick = function () {
    const val = inp.value;
    printer.innerHTML = "";
    if (api.word == val) {
        printer.innerHTML = "";
        printer.innerHTML += `<h1 class="validWords">definitions of ${val}:</h1>`;
        for (const e of api.results) {
            document.getElementById("printer").innerHTML += `<li class="validWords">${e.definition}</li>`;
        }
    }
    else {
        printer.innerHTML = `<h1 class="invalidWord">Invalid word: ${val}</h1>`;
    }
}