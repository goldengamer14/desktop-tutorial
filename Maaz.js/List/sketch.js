let showStuff = (localStorage.getItem("CarriedStuff") ? JSON.parse(localStorage.getItem("CarriedStuff")) : []);

let stuff = "bhindi,12 chawal,60 daal,25 roti,60 chicken,80 bade-ka-gosh,150 chote-ka-gosh,120 vim-bar,10";
stuff = stuff.split(" ");

stuff = (localStorage.getItem("Stuffs") ? JSON.parse(localStorage.getItem("Stuffs")) : stuff);
stuff.sort();


function updateOption() {
    document.getElementById("selectStuff").innerHTML = "";
    stuff.sort();
    stuff.forEach(function (s, i) {
        Array.isArray(s) ? stuff[i] = s : stuff[i] = s.split(",");
        document.getElementById("selectStuff").innerHTML += `<option style="background-color:balck"
        value="${stuff[i][0]}">${stuff[i][0]}</option>`;
    });
}

updateOption();

document.getElementById("addForm").onsubmit = function (e) {
    e.preventDefault();

    const enterName = document.getElementById("enterName");
    const enterValue = document.getElementById("enterValue");
    const alph = "abcdefghijklmnopqrstuvwxyz";
    let validEnter = false;

    Array.from(enterName.value).forEach(e => {
        if (alph.includes(e) && enterValue.value != "") {
            validEnter = true;
        }
    });

    if (validEnter == true) {
        let pushable = [enterName.value.toLowerCase().replaceAll(" ", "-"), enterValue.value];
        const changeIt = (arr, nam) => {
            arr.forEach((e, i, arr) => {

                if (e[0] == nam) {
                    arr[i] = pushable; stuff.sort();
                }

            });
        };

        String(stuff).includes(enterName.value.toLowerCase().replaceAll(" ", "-")) ?
            changeIt(stuff, enterName.value.toLowerCase().replaceAll(" ", "-")) :
            stuff.push(pushable);

        enterName.value = "";
        enterValue.value = "";
        localStorage.setItem("Stuffs", JSON.stringify(stuff));

        updateOption();
    } else {
        alert("Please enter a valid Name or Value for the Item");
    }
}

document.getElementById("resetList").onclick = function () {
    localStorage.removeItem("Stuffs");
    stuff = "bhindi,12 chawal,60 daal,25 roti,60 chicken,80 bade-ka-gosh,150 chote-ka-gosh,120 vim-bar,10";
    stuff = stuff.split(" ");
    stuff.sort();

    localStorage.removeItem("CarriedStuff");
    showStuff.length = 0;

    updateOption();
    showTheStuffs();
}

document.getElementById("adder").onclick = function () {
    const selectStuff = document.getElementById("selectStuff");
    let pushable;

    for (let e of stuff) {
        if (e[0] == selectStuff.value) {
            const numberOfStuffs = document.getElementById("numberOfStuffs");

            pushable = [e[0], e[1], (numberOfStuffs.value == ("" || 0) ? 1 : numberOfStuffs.value)];
            numberOfStuffs.value = "";
            break;
        } else { continue; }
    }

    if (showStuff.length != 0) {
        let isthere = false;
        showStuff.forEach(function (e) {
            e[0] == selectStuff.value ? isthere = true : isthere = isthere;
        });

        const doThis = () => {
            showStuff.forEach((e, i, arr) => {
                const addIt = () => { arr[i] = pushable; showStuff.sort(); };
                e[0] == selectStuff.value ? addIt() : null;
            });
        }

        isthere == true ? doThis() : showStuff.push(pushable);
        showStuff.sort();
    } else { showStuff.push(pushable); }

    localStorage.setItem("CarriedStuff", JSON.stringify(showStuff));

    showTheStuffs();
}

document.getElementById("clearList").onclick = () => {
    localStorage.removeItem("CarriedStuff");
    showStuff.length = 0;
    showTheStuffs();
}

document.getElementById("submitForm").onsubmit = e => { e.preventDefault(); showTheStuffs(); }

document.getElementById("hideList").onclick = () =>
    document.getElementById("stuffCarrier").innerHTML = "";

function showTheStuffs() {
    showStuff.sort();

    const stuffCarrier = document.getElementById("stuffCarrier");
    stuffCarrier.innerHTML = `<tr>
    <th class="stuffs">Quantity</th>
    <th class="stuffs" style="color:red">Stuff</th>
    <th class="stuffs" style="color:chartreuse">Cost</th>
    <th class="stuffs" style="color:yellow">Delete</th>
    </tr>
    `;

    showStuff.forEach(e => {
        stuffCarrier.innerHTML += `<tr class="stuffs">
        <td>${e[2]}</td>
        <td style="color:blue">${e[0]}</td>
        <td style="color:white">${e[1] * e[2]}</td>
        <td style="color:yellow">
            <button class="removeFromList">-</button>
        </td>
        </tr>
        `;
    });

    updateList();
}

function updateList() {
    let removeFromList = document.getElementsByClassName("removeFromList");

    Array.from(removeFromList).forEach((e, i) =>
        e.onclick = function () {

            this.parentNode.parentNode.remove();
            showStuff.splice(i, 1);
            localStorage.setItem("CarriedStuff", JSON.stringify(showStuff));

        }
    );
}

document.getElementById("replit-badge") ? document.getElementById("replit-badge").remove() : null;