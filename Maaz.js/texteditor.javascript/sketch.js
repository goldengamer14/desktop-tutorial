
document.addEventListener("DOMContentLoaded", function () {
    window.document.getElementById("code").value = window.localStorage.getItem("code value");


    document.getElementById("code").style.color = window.localStorage.getItem("font-color of code") || window.localStorage.getItem("custom font-color of code");

    if (window.localStorage.getItem("font-color of code")) {
        Array.from(document.getElementsByTagName("select")[0].children).forEach(function (e) {

            if (e.value == window.localStorage.getItem("font-color of code")) {
                document.getElementById("select").value = e.innerHTML;
            }

        })
    }
    else if (window.localStorage.getItem("custom font-color of code")) {

        let e = document.getElementById("custom");
        e.innerHTML = window.localStorage.getItem("custom font-color of code");
        e.value = e.innerHTML;

        document.getElementById("select").value = e.innerHTML;
        document.getElementById("customInput").value = e.innerHTML;

    }


    var Run;

    if (window.sessionStorage.getItem("Running")) {
        Run = JSON.parse(window.sessionStorage.getItem("Running"));
    }
    else {
        Run = true;
    }


    if (Run == true) {
        document.getElementById("code").value = window.localStorage.getItem("code value");
    }
    else {
        document.getElementById("code").value = window.sessionStorage.getItem("code value");
    }

    Array.from(document.getElementsByClassName("heads")).forEach(function (e) {
        e.onmouseover = function (f) {
            f.target.style.backgroundColor = "yellow";
        }

        e.onmouseout = function (f) {
            f.target.style.backgroundColor = "rgb(255,0,100)";
        }
    });

    document.getElementById("customInput").oninput = function (e) {

        document.getElementById("custom").value = e.target.value;
        document.getElementById("custom").innerText = e.target.value;

        document.getElementById("code").style.color = e.target.value;
        window.localStorage.removeItem("font-color of code");
        window.localStorage.setItem("custom font-color of code", e.target.value);

    }

    document.getElementsByTagName("select")[0].onclick = function (e) {

        document.getElementById("code").style.color = e.target.value;
        window.localStorage.removeItem("custom font-color of code");
        window.localStorage.setItem("font-color of code", e.target.value);

    }

    document.getElementById("run").onclick = function () {
        Run = true;
        window.sessionStorage.setItem("Running", Run);

        let code = document.getElementById("code");
        window.sessionStorage.setItem("code value", code.value);

        let href = "http://127.0.0.1:5500/texteditor.javascript/runPage/index.html";
        window.open(href, "_blank");
    }

    var save = document.getElementById("save");
    save.onclick = function () {
        let code = document.getElementById("code");
        window.localStorage.setItem("code value", code.value);
    }

    var stop = document.getElementById("stop");
    stop.onclick = function () {
        Run = false;
        window.sessionStorage.setItem("Running", Run);
    }

});