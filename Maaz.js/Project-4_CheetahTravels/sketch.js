get("#home").onclick = () => window.scrollTo(0, 0);
get("#about").onclick = () => window.scrollTo(1431, 41);

get("#form").onsubmit = function (e) {
    e.preventDefault();

    let isValid = [];

    for (let e in validCheck) {
        if (validCheck[e] == true) isValid.push(true);
    }

    if (isValid.length == Object.keys(validCheck).length) {
        get("#result").innerHTML =
            `<div class="alert alert-success alert-dismissible fade show" id="alert" role="alert">
        <strong>Success!</strong> Your form has been successfully submitted.
        <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button>
        </div>`;
    } else {
        get("#result").innerHTML =
            `<div class="alert alert-danger alert-dismissible fade show" id="alert" role="alert">
        <strong>Failure!</strong> Your form could'nt be successfully submitted. <br>
        Please recheck and submit the form again
        <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button>
        </div>`;
    }
}

let validCheck = {
    preName: false,
    name: false,
    country: false,
    phone: false,
    userName: false,
    server: false
};

let preName = get("#preName");
let name = get("#name");
let address = get("#address");
let country = get("#country");
let phone = get("#phone");
let userName = get("#userName");
let server = get("#server");

preName.onchange = function () {
    let regex = /^[A-Z]([a-zA-Z_]){1,10}$/;

    if (regex.test(this.value)) { valid(this, regex); }
    else { invalid(this, regex); }
}

name.onchange = function () {
    let regex = /^[A-Z]([a-z A-Z_]){3,20}$/;

    if (regex.test(this.value)) { valid(this, regex); }
    else { invalid(this, regex); }
}

address.onchange = function () {
    let regex = /^[A-Z][\w \d,]{15,60}$/;

    if (regex.test(this.value)) { valid(this, regex); }
    else { invalid(this, regex); }
}

country.onchange = function () {
    let regex = /^\d{2}$/;

    if (regex.test(this.value)) { valid(this, regex); }
    else { invalid(this, regex); }
}

phone.onchange = function () {
    let regex = /^\d{10}$/;

    if (regex.test(this.value)) { valid(this, regex); }
    else { invalid(this, regex); }
}
userName.onchange = function () {
    let regex = /^\w{6,30}$/;

    if (regex.test(this.value)) { valid(this, regex); }
    else { invalid(this, regex); }
}
server.onchange = function () {
    let regex = /^\w{4,10}\.com$/;

    if (regex.test(this.value)) { valid(this, regex); }
    else { invalid(this, regex); }
}

function valid(parent, regex) {
    // console.log(parent.value.match(regex));
    // console.log(parent, regex);

    validCheck[parent.id] = true;
    parent.classList.remove("is-invalid");
}

function invalid(parent, regex) {
    // console.log(parent);
    // console.log(parent.value, regex);

    validCheck[parent.id] = false;
    !parent.classList.contains("is-invalid") ? parent.classList.add("is-invalid") : null;
}