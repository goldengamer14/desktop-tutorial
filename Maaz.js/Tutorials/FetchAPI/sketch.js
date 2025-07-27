// The Fetch function starts here
async function getFromFunction() {
    let url = "http://127.0.0.1:5500/Ajax-Tutorial/js/function.js";
    await fetch(url).then(response => response.text()).then(data => {
        document.getElementById("printer").innerHTML +=
            `<ul>Fetching Text from a file:<li>${filterString(JSON.stringify(data))}</li>`;
    });
}
getFromFunction();

// Fetching the JSON file
async function getFromInternet() {
    const url = "http://127.0.0.1:5500/Ajax-Tutorial/js/json/internet.json";
    await fetch(url).then(response => response.json()).then(data => {
        document.getElementById("printer").innerHTML +=
            `Fetching JSON from a file:<li>${filterString(JSON.stringify(data))}</li></ul>`;
    });
}
getFromInternet();

// Posting a JSON Object
let url = "https://reqres.in/api/users";

// Modified it after the Async Await Tutorial
new Promise(async (resolve, reject) => {

    let data = { "name": "morpheus", "job": "leader" };

    let parameters = {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    };

    let fetchData = await fetch(url, parameters).catch(function () {
        reject("Status Code: " + this.status);
    });
    console.log("fetchData is:", fetchData);
    data = await fetchData.json();

    console.log("Status Code: " + fetchData.status);

    fetchData.status >= 200 && fetchData.status < 300 ?
        resolve(data) : reject("Status Code: " + fetchData.status + ` ${fetchData.statusText}`);

}).then(response => console.log(response)).catch(err => console.error(err));


// url = "http://127.0.0.1:5500/Ajax-Tutorial/js/json/internet.json";
url = "https://reqres.in/api/users2";

new Promise(async (resolve, reject) => {

    let toSend = `{
        "name": "Ayush Varma",
        "class": "10th",
        "RollNo.": 18
    }`;

    let data = await fetch(url,
        {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: toSend

        }).then(response => response.json()).catch(() => reject("Status Code: " + data.status))
        .then(r => resolve(r))
        .catch(() => reject("Status Code: " + data.status));

}).then(r => console.log("POST of my friend: ", r)).catch(err => console.error(err));