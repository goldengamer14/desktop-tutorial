// Import necessary types from drizzle-orm for PostgreSQL
import { pgTable, serial, varchar, numeric, integer, timestamp } from 'drizzle-orm/pg-core';

// Define the 'cars' table using Drizzle's pgTable function
export const cars = pgTable('cars', {
  // id SERIAL PRIMARY KEY
  id: serial('id').primaryKey(),

  // name VARCHAR(40) NOT NULL
  name: varchar('name', { length: 40 }).notNull(),

  // type VARCHAR(10) NOT NULL
  type: varchar('type', { length: 10 }).notNull(),

  // price NUMERIC(10, 2) NOT NULL
  // For NUMERIC, Drizzle's numeric function takes precision and scale as options
  price: numeric('price', { precision: 10, scale: 2 }).notNull(),

  // mileage INTEGER NOT NULL
  mileage: integer('mileage').notNull(),

  // created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  // Drizzle's timestamp function allows setting a default value
  createdAt: timestamp('created_at').defaultNow(),
});

// You can define other tables in this same file if needed.
// For example:
// export const users = pgTable('users', {
//   id: serial('id').primaryKey(),
//   username: varchar('username', { length: 255 }).notNull().unique(),
//   // ... other user columns
// });
