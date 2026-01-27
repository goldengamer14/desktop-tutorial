const requestSelect = document.getElementById("request-select");
const sendRequest = document.getElementById("send-request");
const responseDisplay = document.getElementById("response-display");

const req = {
    id: document.getElementById("car-id"),
    car: document.getElementById("car-name"),
    type: document.getElementById("car-type"),
    price: document.getElementById("car-price"),
    mileage: document.getElementById("car-mileage")
};

sendRequest.addEventListener("click", () => {
    const method = requestSelect.value;
    const url = `http://localhost:3000/api/v1/cars/${method == "POST" ? req.car.value : req.id.value}`;

    const requestBody = (method === "POST" || method === "PUT") ? {
        car: req.car.value,
        type: req.type.value,
        price: parseFloat(req.price.value),
        mileage: parseInt(req.mileage.value)
    } : null;

    const options = {
        method: method,
        headers: { "Content-Type": "application/json" },
        body: requestBody ? JSON.stringify(requestBody) : null
    };

    console.table(options);

    fetch(url, options)
        .then(res => res.json())
        .then(data => {
            console.log(data);
            responseDisplay.innerHTML = JSON.stringify(data, null, 2);
        }).catch(err => console.log(err));
});