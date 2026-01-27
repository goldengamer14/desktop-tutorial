import express from "express";
// const express = require("express");

console.log("This is hello.js file");

const app = express();
const port = 3000;

const router = express.Router();

// Initialize some cars data
const cars = {
    127: { name: "BMW", type: "Casual", price: 5e6, mileage: 15},
    45: { name: "Audi", type: "Casual", price: 6e6, mileage: 12},
    289: {name: "Tesla", type: "Electric", price: 3e7, mileage: 40},
    239: { name: "Ferrari", type: "Sports", price: 2e7, mileage: 8},
    389: { name: "Lamborghini", type: "Sports", price: 25e6, mileage: 6}
};

// Debug: Print all cars
for (const car in cars) {
    console.log(cars[car]);
}

// {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{     APP MIDDLEWARES AND ROUTES     }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}

// Middleware to parse JSON bodies
app.use(express.json());

// Make a middleware to log requests
app.use((req, res, next) => {
    console.log(`[${new Date().toISOString()}] ${req.method} request for ${req.url}` || next())
    next();
});

// Mount the router at /api/v1/cars
app.use("/api/v1/cars", router);

router.get("/", (req, res) => {
    if (Object.keys(cars).length === 0)
        return res.json([]);

    res.json(cars);
});

router.get("/:id", (req, res) => {
    if (!(req.params.id in cars))
        return res.status(404).send("<h1>Car not found.</h1>");

    // res.send(`Your vegetable: ${vegetables[req.params.id]}`);
    res.json(cars[req.params.id]);
});

router.post("/:car", (req, res) => {
    res.send("<h1>This is a POST request to add a new car.</h1>");
    if (req.body.car in cars)
        return res.status(409).json({ error: "Car already exists" });
    else if (!req.body.car.trim() || !req.body.type.trim() || !req.body.price || !req.body.mileage)
        return res.status(400).json({ error: "Invalid car data" });
    else
        cars[req.body.car] = {id: Number.parseInt(Math.random() * 18000), name: req.body.car, type: req.body.type, price: req.body.price, mileage: req.body.mileage};
});

router.put("/:id", (req, res) => {
    res.send(`<h1>This is a PUT request to update the car ${cars[req.params.id]} with id ${req.params.id}.</h1>\nUpdated car details:`);
    cars[req.params.id].price = req.body.price ?? cars[req.params.id].price;
    cars[req.params.id].mileage = req.body.mileage ?? cars[req.params.id].mileage;

    res.json(cars[req.params.id]);
});

router.delete("/:id", (req, res) => {
    res.send(`<h1>This is a DELETE request to delete the car ${cars[req.params.id]} with id ${req.params.id}.</h1>`);
    delete cars[req.params.id];
});

app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`);
});