console.log("Hey there!");

// Declaring variables
const collapsable = {
    json: document.getElementById("jsonCollapsable"),
    custom: document.getElementById("customCollapsable")
};
const radio = {
    json: document.getElementById("json"),
    custom: document.getElementById("custom"),
};
const addParams = document.getElementById("addParams");
const addedParams = document.getElementById("addedParams");

// Setting to collapse and show the Request Inputs
contents("json", "custom");
contents("custom", "json");

// Function to collapse and show the Request Inputs
function contents(type, other) {
    radio[type].onclick = () => {
        collapsable[type].style.display = "flex";
        collapsable[other].style.display = "none";
    }
}

// Add a key-value parameters pair on clicking on the '+' button
addParams.onclick = function () {
    addedParams.innerHTML += `
    <div class="form-row input-group my-1">
    <br>
        <span class="input-group-text">Key</span>
        <input type="text" aria-label="Last name" class="form-control keys">
        <span class="input-group-text">Value</span>
        <input type="text" aria-label="First name" class="form-control values">

        <button class="btn btn-primary removeParam">-</button>
    </div>`;

    const removeParam = document.getElementsByClassName("removeParam");
    for (let button of removeParam) {
        button.onclick = e => {
            const askForConfirmation = document.getElementById("askForConfirmation").checked;

            if (askForConfirmation) {
                const removeIt = confirm("Are you sure you want to delete this parameter?");
                removeIt == true ? button.parentElement.remove() : null;
            } else {
                button.parentElement.remove()
            }
        }
    }
}

// Setting the function to run after clicking submit button
document.getElementById("form").onsubmit = submitted;

// Creating function to run after clicking submit button
function submitted(e) {
    e.preventDefault();

    // Declaring local variations
    let url = document.getElementById("url").value;
    let requestType = document.getElementsByName("requestType");
    let typeOfRequest = ((requestType[0].checked ? requestType[0] : requestType[1]).getAttribute("id")).toUpperCase();
    let contentType = document.getElementsByName("contentType");
    let typeOfContent = ((contentType[0].checked ? contentType[0] : contentType[1]).getAttribute("id")).toUpperCase();
    let typeOfResponse = document.getElementsByName("responseType");
    let responseType = (typeOfResponse[0].checked ? "json" : "text");
    console.log(responseType);
    let content;
    let response;

    // Collect custom parameters keys and values in an object {Only if Custom type request is selected}
    console.log("The content is:", typeOfContent);
    if (typeOfContent == "CUSTOM") {
        let customs = {
            keys: Array.from(document.getElementsByClassName("keys")),
            values: Array.from(document.getElementsByClassName("values"))
        };
        obj = { keys: [], values: [] };

        customs.keys.forEach(e => {
            obj.keys.push(e.value);
        });
        customs.values.forEach(e => {
            obj.values.push(e.value);
        });

        content = {};
        for (let i = 0; i < obj.keys.length && obj.values.length; i++) {
            let key = obj.keys[i];
            let value = obj.values[i];

            content[key] = value;
        }

        content = JSON.stringify(content);

    } else {
        content = document.getElementById("jsonTextarea").value;
    }

    // Function to carry out if request type is post

    if (typeOfRequest == "GET") {
        new Promise(async (done, undone) => {
            response = await fetch(url).then(r => r[responseType]())
                .then(d => {
                    done(responseType == "json" ? d.data : d);
                })
                .catch(e => undone(e));
        }).then(r => {
            response = r;
            console.log("response:", response);
            document.getElementById("responseTextarea").value = (responseType == "json" ? JSONToValue(JSON.stringify(response))
                : response);
        }).catch(e => console.error(e));
    } else {
        new Promise(async (done, undone) => {
            await fetch(url, {
                method: "POST",
                headers: {
                    "Content-Type": "application/" + responseType
                },
                body: content
            }).then(d => done(d)).catch(e => undone(e));
        }).then(r => {
            response = r;
            console.log("response:", response);
            document.getElementById("responseTextarea").value = `{
                "body": ${JSON.stringify(response.body)}
                "headers": ${JSON.stringify(response.headers)}
                "ok": "${response.ok}"
                "redirected": "${response.redirected}"
                "status": "${response.status}"
                "statusText": "${response.statusText}"
                "type": "${response.type}"
                "url": "${response.url}"
            }`;
        }).catch(e => console.log(e));
    }

    console.log("URL is:", url);
    console.log("Type of Request is:", typeOfRequest);
    console.log("Type of Content is:", typeOfContent);
    console.log("The content is:", content);

    function JSONToValue(insertedString) {
        let str = insertedString;

        str = str.replaceAll("{", `\n{\n    `);
        str = str.replaceAll("}", `\n}`);

        return str;
    }
}