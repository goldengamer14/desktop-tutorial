// import SendHttpRequest from ("./js/function.js");

window.console.log("this is an Ajax Tutorial");

// Declaring the variables
var fetchBtn = window.document.getElementById("fetchBtn");
var backupBtn = window.document.getElementById("backupBtn");

// Using Event Listeners on them
fetchBtn.onclick = function () {

    var x = [new XMLHttpRequest(), new XMLHttpRequest()];

    var file = [];
    file[1] = "https://reqres.in/api/unknown/2";
    // file[0] = "http://127.0.0.1:5500/BootstrapClone/index.html";

    x[1].open("GET", file[1], true);
    x[1].onload = function () {
        document.getElementById("response").innerText = this.response;
    }
    x[1].send();

    // Getting JSON-Objects from my internet.json file
    new SendXML("GET", "http://127.0.0.1:5500/Ajax-Tutorial/js/json/internet.json",
        "application/json",
        {
            "name": "Ayush Varma",
            "class": "10th",
            "RollNo.": 37
        }).xhr.onload = function () {
            window.console.log(this.response);
            document.getElementById("responseHTML").innerHTML += this.response;
        };

}

backupBtn.onclick = function () {
    // POSTing my Json-Object tp the website that provides dummy JSON
    var x = new XMLHttpRequest();

    var file = "https://reqres.in/api/users";
    // var file = "http://127.0.0.1:5500/Ajax-Tutorial/js/json/sketch.json";

    x.open("POST", file, false);
    x.getResponseHeader("Content-type", "application/json");

    x.onload = function () {
        document.getElementById("Post").innerText = this.response;
    }

    var param =
    {
        "name": "Ayush Varma",
        "class": "10th",
        "RollNo.": 37
    }
        ;
    x.send(param);
}