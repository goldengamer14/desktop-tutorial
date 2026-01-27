-- QUERY #1
INSERT INTO cars(name, type, price, mileage)
VALUES
	('BMW', 'Casual', 5e6, 15),
    ('Tesla', 'Electric', 3e7, 40),
    ('Audi', 'Casual', 6e6, 12),
    ('Ferrari', 'Sports', 2e7, 8),
    ('Lamborghini', 'Sports', 25e6, 6);

-- QUERY #2
SELECT * FROM cars;

-- QUERY #3
SELECT name, price FROM cars WHERE price < 3e7 AND mileage >= 12 ORDER BY price ASC;

-- QUERY #4
SELECT name FROM cars ORDER BY price ASC LIMIT 3;

-- QUERY #5
UPDATE cars SET price = price * 0.9 WHERE price >= 1e7;

-- QUERY #6
SELECT * FROM cars ORDER BY price desc;

-- QUERY #7
DELETE FROM cars where name = 'Audi';

-- QUERY #8
SELECT * FROM cars ORDER BY id asc