SendXML("POST", "https://reqres.in/api/users2", "application/json", "name: morpheus, job: leader")
    .onload = function () {
        let data = JSON.stringify(this.response);
        get("#show").innerHTML += filterString(data);
    };

Fetch("https://reqres.in/api/users", "json", e =>
    get("#show").innerHTML += filterString(JSON.stringify(e[0])));

let theData = `{ "name": "morpheus", "job": "leader"}`;
Send("https://reqres.in/api/users2", "json", theData, e =>
    get("#show").innerHTML += filterString(JSON.stringify(e)));

let usedWords = [];

get("#btn").onclick = () => {
    let input = get("#inp").value;
    let execute = true;

    // usedWords.forEach(e => e == input ? execute = false : console.log(e == input));
    // usedWords.push(input);

    // execute ? findTheWords(input, [get("#show")]) : console.log(usedWords, input);
    findTheWords(input, [get("#show")]);
}