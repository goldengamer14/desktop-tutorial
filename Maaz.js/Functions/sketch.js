document.body.innerHTML +=
    `<link rel="stylesheet" href="http://127.0.0.1:5500/Functions/style.css" type="text/css"`;

function filterString(str) {

    str = str.replaceAll("\\n", "<br><span class=\"common\"></span>");
    str = str.replaceAll("\r", "");
    str = str.replaceAll("\":", "\":<span class=\"colon\"></span>");

    str.startsWith("\"") && str.endsWith("\"") ? str = str.substring(1, str.length - 1) : null;

    str = str.replaceAll("\\\"", "\"");

    str = str.replaceAll("{", "<br>" +
        "{ <span class=\"brackets\"> <br>" +
        "<span class=\"common\"></span>");

    str = str.replaceAll("}", "</span> <br></span>" +
        "}<br>" +
        "<span class=\"braces\"></span>");


    str = str.replaceAll("[", "<br><span class=\"braces\"></span>" +
        "[ <span class=\"brackets\"> <br>" +
        "<span class=\"braces\"></span>");

    str = str.replaceAll("]", "</span> <br><span class=\"braces\"></span>" +
        "] <br>" +
        "<span class=\"braces\"></span>");


    str = str.replaceAll(",", ",<br>" +
        "<span class=\"common\"></span>");


    setTimeout(() => {
        const brackets = document.getElementsByClassName("brackets");
        Array.from(brackets).forEach(e => {
            e.innerHTML = e.innerHTML.replaceAll("<br>", "<br><span class=\"common\"></span>");
        });
    }, 1000);

    return str;

}

function SendXML(method, href, type, param = "") {
    this.xhr = new XMLHttpRequest();
    this.xhr.open(method, href, true);

    const params = (method == "POST" ? param : "");

    this.xhr.getResponseHeader("Content-Type", type);
    this.xhr.send(params);

    return this.xhr;
}

function get(ele) {
    let sym = "@";
    const method = (ele.startsWith("#") ? "querySelector" :
        ele.startsWith(sym) ? "getElementByName" :
            "querySelectorAll");

    const element = ele.startsWith(sym) ? ele.substring(1, ele.length) : ele;

    return document[method](element);
}

async function Fetch(url, type, postLoaded) {
    const data = type;

    await fetch(url).then(r => r[data]()).then(d => postLoaded(d.data));
}

async function Send(url, type, sendingData, postLoaded) {
    const json = ["application/json", "json"];
    const text = ["application/www-urlencoded", "text"];

    let check = async () => {
        let validType;
        let returnNing;
        do {
            await new Promise((resolve, reject) => {
                let data = (type == "json" ? json : type == "text" ? text : null);
                data == null ? reject("Please type the right form of data") : resolve(data);
            }).catch(err => { prompt(err); validType = false }).then(data => {
                returnNing = data;
            });
        } while (validType);

        return returnNing;
    }
    const data = await check();

    await fetch(url, {
        method: "POST",
        headers: { "Content-Type": data[0] },
        body: sendingData
    }).then(r => r[data[1]]()).then(d => postLoaded(d));
}

function ascendDecend(string) {
    let arr = string.split(",");
    arr.forEach((e, i, ar) => ar[i] = Number.parseFloat(e));

    let decendingArr = [];

    const minArr = [...arr];
    for (let i = 0; i < arr.length; i++) {
        const minValue = Math.min(...minArr);
        const minIndex = minArr.indexOf(minValue);

        decendingArr.push(minValue);
        minArr.splice(minIndex, 1);
    }


    let ascendingArr = [...arr].map((e, i, arr) => {
        const pushable = Math.max(...arr);
        arr[arr.indexOf(pushable)] = "";
        return pushable;
    });

    return [ascendingArr, decendingArr];
}


function findTheWords(theWord, containers) {


    var word = theWord;

    var docBody = containers;

    for (let k = 0; k < docBody.length; k++) {

        var container = (Array.isArray(docBody[k]) ? docBody[k] : docBody);
        for (var e of container) {

            var replacementBody = [word,
                "color: hsl(0, 75%, 50%); background-color: #afa; border: 2px solid red; font-weight: bolder; margin: 0px"];

            if (String(e.innerHTML).toLowerCase().includes(replacementBody/*[j]*/[0])) {

                var index = [];

                var f = String(e.innerHTML).split(" ");
                let textHolder = "";

                for (let i = 0; i < f.length; i++) {

                    if (String(f[i]).toLowerCase().includes(replacementBody/*[j]*/[0])) {

                        index[0] = container.indexOf(e);
                        index[1] = String(f[i]).toLowerCase().indexOf(replacementBody/*[j]*/[0]);
                        index[2] = String(f[i]).substring(index[1], index[1] + replacementBody/*[j]*/[0].length);

                        textHolder += String(f[i]).replace(index[2], `<span style="${replacementBody/*[j]*/[1]}">${index[2]}</span> `);

                    }
                    else { textHolder += String(f[i]) + " " }

                }

                e.innerHTML = textHolder;

            }

        }

    }


}