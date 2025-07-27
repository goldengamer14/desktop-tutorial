/*
    const get = async function (url) {
        const getting = await fetch(url);
        const response = await getting.json();

        return response;
    }

    let myVariable = get("https://api.github.com/users");

    myVariable.then(response => {
        console.log(response);
        document.getElementById("showMe").innerHTML = JSON.stringify(response);
    });

    console.log(myVariable);
    document.getElementById("showMe").innerHTML = JSON.stringify(myVariable);
*/

new Promise(async function (resolve, reject) {

    let json;
    await fetch("https://api.github.com/users").then(response => response.json()).then(
        data => json = data);
    if (json != undefined) {
        resolve(json);
    } else { reject("Could'nt complete it"); }

}).then(data => {

    data.length = 1;

    console.log(JSON.stringify(data));
    document["getElementById"]("showMe").innerHTML = filterString(JSON.stringify(data));

}).catch(err => console.error(err));