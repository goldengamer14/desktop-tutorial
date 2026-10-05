// Simple Flappy Bird clone
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
const width = canvas.width;
const height = canvas.height;

let gameStarted = false;
let gameOver = false;
let score = 0;

// Bird
class Bird {
  constructor() {
    this.x = width * 0.2;
    this.y = height / 2;
    this.radius = 12;
    this.velocity = 0;
    this.gravity = 0.5;
    this.lift = -8;
  }
  update() {
    this.velocity += this.gravity;
    this.y += this.velocity;
    if (this.y + this.radius > height - groundHeight) {
      this.y = height - groundHeight - this.radius;
      this.velocity = 0;
    }
    if (this.y - this.radius < 0) {
      this.y = this.radius;
      this.velocity = 0;
    }
  }
  flap() {
    this.velocity = this.lift;
  }
  draw() {
    ctx.fillStyle = '#ff0';
    ctx.beginPath();
    ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
    ctx.fill();
  }
}

// Pipe
class Pipe {
  constructor(x) {
    this.x = x;
    this.width = 60;
    this.gap = 150;
    this.speed = 2;
    this.topHeight = Math.random() * (height - groundHeight - this.gap - 100) + 50;
    this.passed = false;
  }
  update() {
    this.x -= this.speed;
  }
  draw() {
    ctx.fillStyle = '#0f0';
    // top
    ctx.fillRect(this.x, 0, this.width, this.topHeight);
    // bottom
    ctx.fillRect(this.x, this.topHeight + this.gap, this.width, height - groundHeight - (this.topHeight + this.gap));
  }
  isColliding(bird) {
    // top pipe
    if (bird.x + bird.radius > this.x && bird.x - bird.radius < this.x + this.width) {
      if (bird.y - bird.radius < this.topHeight || bird.y + bird.radius > this.topHeight + this.gap) {
        return true;
      }
    }
    return false;
  }
}

// Cloud
class Cloud {
  constructor(x, y, scale) {
    this.x = x;
    this.y = y;
    this.scale = scale;
    this.speed = 0.5;
  }
  update() {
    this.x -= this.speed;
    if (this.x < -200) this.x = width + 200;
  }
  draw() {
    ctx.fillStyle = 'rgba(255,255,255,0.8)';
    ctx.beginPath();
    ctx.ellipse(this.x, this.y, 40 * this.scale, 25 * this.scale, 0, 0, Math.PI * 2);
    ctx.fill();
  }
}

// Ground
const groundHeight = 80;

// Trees and herbs
class Tree {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    this.width = 20;
    this.height = 60;
    this.speed = 2;
  }
  update() {
    this.x -= this.speed;
    if (this.x < -this.width) this.x = width + Math.random() * 200;
  }
  draw() {
    ctx.fillStyle = '#8b4513';
    ctx.fillRect(this.x, this.y - this.height, this.width, this.height);
    ctx.fillStyle = '#228b22';
    ctx.beginPath();
    ctx.arc(this.x + this.width / 2, this.y - this.height, 30, 0, Math.PI * 2);
    ctx.fill();
  }
}

class Herb {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    this.radius = 5;
    this.speed = 2;
  }
  update() {
    this.x -= this.speed;
    if (this.x < -this.radius) this.x = width + Math.random() * 200;
  }
  draw() {
    ctx.fillStyle = '#32cd32';
    ctx.beginPath();
    ctx.arc(this.x, this.y, this.radius, 0, Math.PI * 2);
    ctx.fill();
  }
}

const bird = new Bird();
let pipes = [];
let clouds = [];
let trees = [];
let herbs = [];

function init() {
  // create initial pipes
  for (let i = 0; i < 3; i++) {
    pipes.push(new Pipe(width + i * 300));
  }
  // clouds
  for (let i = 0; i < 5; i++) {
    clouds.push(new Cloud(width + i * 200, Math.random() * height / 2 + 20, Math.random() * 0.5 + 0.5));
  }
  // trees
  for (let i = 0; i < 3; i++) {
    trees.push(new Tree(width + i * 400, height - groundHeight));
  }
  // herbs
  for (let i = 0; i < 5; i++) {
    herbs.push(new Herb(width + i * 250, Math.random() * (height - groundHeight - 50) + 50));
  }
}

function reset() {
  bird.x = width * 0.2;
  bird.y = height / 2;
  bird.velocity = 0;
  pipes = [];
  clouds = [];
  trees = [];
  herbs = [];
  score = 0;
  init();
}

function drawGround() {
  ctx.fillStyle = '#654321';
  ctx.fillRect(0, height - groundHeight, width, groundHeight);
}

function drawSky() {
  ctx.fillStyle = '#70c5ce';
  ctx.fillRect(0, 0, width, height - groundHeight);
}

function update() {
  if (!gameStarted || gameOver) return;
  bird.update();
  pipes.forEach(p => p.update());
  clouds.forEach(c => c.update());
  trees.forEach(t => t.update());
  herbs.forEach(h => h.update());

  // remove off-screen pipes
  pipes = pipes.filter(p => p.x + p.width > 0);
  // add new pipes
  if (pipes.length < 3) {
    const lastPipe = pipes[pipes.length - 1];
    pipes.push(new Pipe(lastPipe.x + 300));
  }

  // collision
  for (let p of pipes) {
    if (p.isColliding(bird)) {
      gameOver = true;
      showGameOver();
      break;
    }
  }

  // score
  pipes.forEach(p => {
    if (!p.passed && p.x + p.width < bird.x) {
      p.passed = true;
      score++;
    }
  });
}

function draw() {
  drawSky();
  clouds.forEach(c => c.draw());
  trees.forEach(t => t.draw());
  herbs.forEach(h => h.draw());
  drawGround(); // draw ground before pipes and bird
  pipes.forEach(p => p.draw());
  bird.draw();
  // score
  ctx.fillStyle = 'white';
  ctx.font = '30px Arial';
  ctx.fillText(score, width / 2 - 10, 50);
}

function loop() {
  update();
  draw();
  requestAnimationFrame(loop);
}

function startGame() {
  if (gameStarted) return;
  gameStarted = true;
  document.getElementById('startMenu').classList.add('hidden');
  loop();
}

function showGameOver() {
  document.getElementById('gameOver').classList.remove('hidden');
  document.getElementById('scoreText').innerText = 'Score: ' + score;
}

function restartGame() {
  gameOver = false;
  document.getElementById('gameOver').classList.add('hidden');
  reset();
}

canvas.addEventListener('mousedown', () => {
  if (!gameStarted) startGame();
  else if (!gameOver) bird.flap();
  else restartGame();
});

canvas.addEventListener('touchstart', (e) => {
  e.preventDefault();
  if (!gameStarted) startGame();
  else if (!gameOver) bird.flap();
  else restartGame();
});

// Also allow clicking on overlays to start/restart
const startMenu = document.getElementById('startMenu');
const gameOverOverlay = document.getElementById('gameOver');
startMenu.addEventListener('mousedown', startGame);
startMenu.addEventListener('touchstart', (e)=>{e.preventDefault(); startGame();});

gameOverOverlay.addEventListener('mousedown', restartGame);
gameOverOverlay.addEventListener('touchstart', (e)=>{e.preventDefault(); restartGame();});

reset();
