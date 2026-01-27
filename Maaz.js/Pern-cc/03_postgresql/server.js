import express from "express";
import { db, schema } from "./db.js";
import cors from "cors";
import { eq, ne, gt, lt, gte, lte } from "drizzle-orm";
// const express = require("express");

const app = express();
const port = 3000;

const router = express.Router();

// console.log("\nDB Schema cars table:")
// console.log(db);
// console.log(db.schema);
// console.log(db?.schema?.cars);
// console.log(db._.fullSchema);
// console.log("\nDB Schema cars table:")
// console.log(db.select().from(db.schema.cars));

// Fetch cars data from a PostgreSQL database
let cars = await db.select().from(schema.cars);
const carNames = new Set();

cars = cars.reduce((acc, car) => {
    {
        acc[car.id] = {
            name: car.name,
            type: car.type,
            price: car.price,
            mileage: car.mileage
        };
        carNames.add(car.name);
        return acc;
    }
}, {});

console.log(cars);

// // Initialize some cars data
// const cars = {
//     127: { name: "BMW", type: "Casual", price: 5e6, mileage: 15},
//     45: { name: "Audi", type: "Casual", price: 6e6, mileage: 12},
//     289: {name: "Tesla", type: "Electric", price: 3e7, mileage: 40},
//     239: { name: "Ferrari", type: "Sports", price: 2e7, mileage: 8},
//     389: { name: "Lamborghini", type: "Sports", price: 25e6, mileage: 6}
// };

// // Debug: Print all cars
// for (const car in cars) {
//     console.log(cars[car]);
// }

// {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{     APP MIDDLEWARES AND ROUTES     }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}

// Middleware to parse JSON bodies
app.use(express.json());

app.use(cors());

// Make a middleware to log requests
app.use((req, res, next) => {
    console.log(`\n\n[${new Date().toISOString()}] ${req.method} request for ${req.url}`);
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
        return res.status(404).send({ error: "Car not found" });

    res.json(cars[req.params.id]);
});

router.post("/:car", async (req, res) => {
    console.log("req.body:", req.body);

    if (carNames.has(req.body.car))
        return res.status(409).json({ error: "Car already exists" });
    else if (!req.body.car.trim() || !req.body.type.trim() || req.body.price < 0 || req.body.mileage < 0)
        return res.status(400).json({ error: "Invalid car data" });

    const newCarResult = await db.insert(schema.cars)
        .values({ name: req.body.car, type: req.body.type, price: req.body.price, mileage: req.body.mileage })
        .returning();

    const newCar = newCarResult[0];
    cars[newCar.id] = { name: newCar.name, type: newCar.type, price: newCar.price, mileage: newCar.mileage };
    carNames.add(newCar.name);

    // console.log("New car added:", newCar);
    // console.log(carNames);
    // console.log(cars);
    console.log("POST result:", newCarResult);

    res.status(201).json(newCar);
});

router.put("/:id", async (req, res) => {
    if (!(req.params.id in cars))
        return res.status(404).json({ error: "Car not found" });

    cars[req.params.id].price = req.body.price ?? cars[req.params.id].price;
    cars[req.params.id].mileage = req.body.mileage ?? cars[req.params.id].mileage;

    console.log("req.params:", req.params);
    console.log("req.body:", req.body);

    const result = await db.update(schema.cars)
        .set({ price: cars[req.params.id].price, mileage: cars[req.params.id].mileage })
        .where(eq(schema.cars.id, parseInt(req.params.id)));

    console.log("Update result:", result);
    res.json(cars[req.params.id]);
});

router.delete("/:id", async (req, res) => {
    const deletedCar = cars[req.params.id];

    if (!deletedCar)
        return res.status(404).json({ error: "Car not found" });

    delete cars[req.params.id];
    carNames.delete(deletedCar.name);

    console.log("req.params:", req.params);

    const result = await db.delete(schema.cars).where(eq(schema.cars.id, parseInt(req.params.id)));
    console.log("Delete result:", result);

    res.json({ message: "Car deleted successfully", car: deletedCar });
});

app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}`);
});