import "dotenv/config";
import { drizzle } from "drizzle-orm/node-postgres";
import { Pool } from "pg";
import * as schema from "./schema.js";
// import * as schema from "./schema.js";


if (!process.env.DATABASE_URL) {
  throw new Error("DATABASE_URL is not defined in environment variables");
}

// console.log("cars in db.js\n\n", schema.cars);

const pool = new Pool({
  connectionString: process.env.DATABASE_URL,
});

export const db = drizzle(pool, schema);

export { schema };

export { pool };

pool.connect()
  .then(client => {
    console.log("Successfully connected to the database!");
    client.release(); // Release the client back to the pool
  })
  .catch(err => {
    console.error("Failed to connect to the database:", err.message);
  });