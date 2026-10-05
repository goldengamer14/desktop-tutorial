// game.js

const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

const overlay = document.getElementById('overlay');
const finalScoreEl = document.getElementById('finalScore');
const restartBtn = document.getElementById('restartBtn');

const WIDTH = canvas.width;
const HEIGHT = canvas.height;

// Spaceship properties
const ship = {
    x: 80,
    y: HEIGHT / 2,
    width: 40,
    height: 30,
    vy: 0,
    gravity: 0.5,
    lift: -10,
    color: '#ff0'
};

let asteroids = [];
let asteroidTimer = 0;
let asteroidInterval = 90; // frames
let asteroidSpeed = 2;
let score = 0;
let gameOver = false;
let frameCount = 0;

function resetGame() {
    ship.y = HEIGHT / 2;
    ship.vy = 0;
    asteroids = [];
    asteroidTimer = 0;
    asteroidInterval = 90;
    asteroidSpeed = 2;
    score = 0;
    gameOver = false;
    frameCount = 0;
    overlay.classList.add('hidden');
    loop();
}

function spawnAsteroid() {
    const size = Math.random() * 30 + 20; // 20-50px
    const x = Math.random() * (WIDTH - size);
    const y = -size;
    asteroids.push({ x, y, size, speed: asteroidSpeed });
}

function update() {
    // Ship physics
    ship.vy += ship.gravity;
    ship.y += ship.vy;
    if (ship.y + ship.height > HEIGHT) {
        ship.y = HEIGHT - ship.height;
        ship.vy = 0;
    }
    if (ship.y < 0) {
        ship.y = 0;
        ship.vy = 0;
    }

    // Asteroids
    asteroidTimer++;
    if (asteroidTimer > asteroidInterval) {
        spawnAsteroid();
        asteroidTimer = 0;
    }
    asteroids.forEach(a => a.y += a.speed);
    // Remove off-screen
    asteroids = asteroids.filter(a => a.y < HEIGHT + a.size);

    // Collision detection
    for (let a of asteroids) {
        if (rectIntersect(ship.x, ship.y, ship.width, ship.height, a.x, a.y, a.size, a.size)) {
            endGame();
            return;
        }
    }

    // Scoring: increase every 60 frames (~1 sec at 60fps)
    if (frameCount % 60 === 0) {
        score++;
        // Increase difficulty
        if (score % 10 === 0) {
            asteroidSpeed += 0.5;
            if (asteroidInterval > 30) asteroidInterval -= 5;
        }
    }
    frameCount++;
}

function draw() {
    // Clear
    ctx.clearRect(0, 0, WIDTH, HEIGHT);
    // Draw ship
    ctx.fillStyle = ship.color;
    ctx.fillRect(ship.x, ship.y, ship.width, ship.height);
    // Draw asteroids
    ctx.fillStyle = '#888';
    asteroids.forEach(a => {
        ctx.beginPath();
        ctx.arc(a.x + a.size / 2, a.y + a.size / 2, a.size / 2, 0, Math.PI * 2);
        ctx.fill();
    });
    // Draw score
    ctx.fillStyle = '#fff';
    ctx.font = '20px Arial';
    ctx.fillText('Score: ' + score, 10, 30);
}

function loop() {
    if (gameOver) return;
    update();
    draw();
    requestAnimationFrame(loop);
}

function rectIntersect(x1, y1, w1, h1, x2, y2, w2, h2) {
    return !(x2 > x1 + w1 ||
             x2 + w2 < x1 ||
             y2 > y1 + h1 ||
             y2 + h2 < y1);
}

function endGame() {
    gameOver = true;
    finalScoreEl.textContent = score;
    overlay.classList.remove('hidden');
}

// Input handling
function handleInput(e) {
    if (gameOver) return;
    ship.vy = ship.lift;
}

canvas.addEventListener('mousedown', handleInput);
canvas.addEventListener('touchstart', handleInput);
window.addEventListener('keydown', function(e) {
    if (e.code === 'Space') {
        e.preventDefault();
        handleInput();
    }
});

restartBtn.addEventListener('click', resetGame);

// Start the game
loop();
