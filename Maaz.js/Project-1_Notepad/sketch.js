//the variables
const note = document.getElementById("note");
const st = document.getElementById("save-title");
const d = document.createElement("div");
d.innerHTML = "<hr>";
d.setAttribute("id", "border");
document.body.appendChild(d);

//the search input
if (document.getElementsByClassName("saved-note")) {
    const s = document.getElementById("search");
    s.oninput = function () {
        if (s.value != "") {

            let v = document.getElementsByClassName("saved-note");
            Array.from(v).forEach(function (e) {
                for (let i = 0; i < 100; i++) {

                    if (String(e.getAttribute("backup")).toLowerCase().includes(String(s.value).toLowerCase())) {

                        e.style.visibility = "visible";
                        Array.from(document.getElementsByClassName("delete-note"))[Array.from(v).indexOf(e)].style.visibility = "visible";
                        e.style.visibility = "visible";
                        Array.from(document.getElementsByClassName("delete-note"))[Array.from(v).indexOf(e)].style.visibility = "visible";

                        // highlight the searched part
                        e.innerHTML = e.getAttribute("backup").toString().toLowerCase().replace(s.value, `<span style="color:red;">${s.value}</span>`);

                    }

                    else if (String(e.innerText).toLowerCase().includes(String(s.value).toLowerCase()) !== true) {

                        e.style.backgroundColor = "black"; e.style.visibility = "hidden";
                        document.getElementsByClassName("delete-note")[Array.from(v).indexOf(e)].style.visibility = "hidden";

                    }

                }
            })
        } if (String(s.value) == "") {

            Array.from(document.getElementsByClassName("delete-note")).forEach(function (e) {
                e.style.visibility = "visible";
            })
            Array.from(document.getElementsByClassName("saved-note")).forEach(function (e) {
                e.style.visibility = "visible";
                e.innerHTML = e.innerHTML.toString().replace(`<span style="color:red;">`, "");
                e.innerHTML = e.innerHTML.toString().replace(`</span>`, "");
            })

        }
    }
}


//random background-color
let r, g, b;
setInterval(function () {
    r = 255 * Math.random(); g = 255 * Math.random(); b = 255 * Math.random();
    document.body.style.backgroundColor = `rgb(${r},${g},${b})`;
}, 1000);

//fetching notes from local storage
for (let index = 0; index <= 99; index++) {
    let a = localStorage.getItem(`dive${index + 1}`);
    if (a) {
        const dv = document.createElement("div");
        dv.setAttribute("value", JSON.parse(a)[0]);
        dv.setAttribute("class", JSON.parse(a)[1]);
        dv.innerHTML = JSON.parse(a)[2]
        dv.setAttribute("backup", dv.innerText);

        dv.onclick = function () {
            note.value = dv.getAttribute("value");
        }
        dv.onmouseover = function () {
            dv.style.cursor = "pointer";
        }
        document.body.appendChild(dv);

        const dn = document.createElement("input");
        dn.type = "button";
        dn.setAttribute("class", "delete-note");
        dn.value = "Delete Note";
        document.body.appendChild(dn);
        dn.onclick = function () {
            localStorage.removeItem(`dive${index + 1}`);
            dv.parentNode.removeChild(dv);
            dn.parentNode.removeChild(dn);
        }

        for (let i = 0; i < 100; i += Math.round(window.innerWidth / 250)) {
            const sn = document.getElementsByClassName("saved-note");
            if (sn.length == i) {
                const brd = document.createElement("div");
                brd.innerHTML = "<br>";
                document.body.appendChild(brd);
            }
        }

    }
}

// if clicked on the "save note" button
document.getElementById("note-saver").onclick = function () {

    //create a Div that can hold your notes
    const dv = document.createElement("div");
    document.body.appendChild(dv);
    dv.setAttribute("class", "saved-note");
    dv.setAttribute("value", note.value);
    dv.innerHTML = st.value;

    //create a Button to delete a specific note
    const dn = document.createElement("input");
    dn.type = "button";
    dn.setAttribute("class", "delete-note");
    dn.value = "Delete Note";
    document.body.appendChild(dn);

    //show what you stored in that note on the textArea
    dv.onclick = function () {
        note.value = dv.getAttribute("value");
    }
    dv.onmouseover = function () {
        dv.style.cursor = "pointer";
    }
    st.value = "My Title";
    note.value = "";

    dv.setAttribute("backup", dv.innerText);

    //save note on local storage
    let lsl = localStorage.length + 1;
    const noteObj = [];
    noteObj.push(dv.getAttribute("value"));
    noteObj.push(dv.getAttribute("class"));
    noteObj.push(dv.innerHTML);
    noteObj.push(dn);
    localStorage.setItem(`dive${lsl}`, JSON.stringify(noteObj));

    //delete note on click
    dn.onclick = function () {
        localStorage.removeItem(`dive${lsl}`);
        dv.parentNode.removeChild(dv);
        dn.parentNode.removeChild(dn);
    }

    //create a <br> for spaces notes
    for (let i = 0; i < 100; i += Math.round(window.innerWidth / 250)) {
        if (localStorage.length == i) {
            const brd = document.createElement("div");
            brd.innerHTML = "<br>";
            document.body.appendChild(brd);
        }
    }

}

//if clicked on the "delete all notes" button
document.getElementById("deleter").onclick = function () {
    localStorage.clear();
    const ab = document.getElementsByClassName("saved-note");
    const ac = document.getElementsByClassName("delete-note");
    Array.from(ab).forEach(function (e) {
        e.parentNode.removeChild(e);
    })
    Array.from(ac).forEach(function (e) {
        e.parentNode.removeChild(e);
    })
}