import "dotenv/config";
import { defineConfig } from "drizzle-kit";

if (!process.env.DATABASE_URL) {
  throw new Error("DATABASE_URL is not defined in environment variables");
}

console.log("Successfully loaded DATABASE_URL from environment variables.");

export default defineConfig({
    schema: "../Maaz.js/Pern-cc/03_postgresql/schema.js",
    out: "../Maaz.js/Pern-cc/03_postgresql/drizzle",
    dialect: "postgresql",
    dbCredentials: {
        url: process.env.DATABASE_URL,
    }
});