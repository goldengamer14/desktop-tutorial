/**
 * Subway Surfers 3D - Main Game Engine
 * Powered by Three.js with full arrow-key controls, procedural world generation,
 * character animation, trains, obstacles, collectibles, and power-ups.
 */

// Global Game State
const GAME = {
  active: false,
  paused: false,
  score: 0,
  highScore: parseInt(localStorage.getItem('subway_highscore') || '0', 10),
  coins: 0,
  multiplier: 1,
  speed: 38,
  baseSpeed: 38,
  maxSpeed: 85,
  distance: 0,

  // Powerups active states
  powerups: {
    magnet: { active: false, timer: 0, maxDuration: 12 },
    sneakers: { active: false, timer: 0, maxDuration: 14 },
    jetpack: { active: false, timer: 0, maxDuration: 8 },
    multiplier: { active: false, timer: 0, maxDuration: 15 },
    hoverboard: { active: false, timer: 0, maxDuration: 25 }
  },

  // Lane configuration: 3 lanes
  lanes: [-2.8, 0, 2.8],
  currentLane: 1, // 0 = Left, 1 = Center, 2 = Right
  targetX: 0,

  // Player physics & kinematics
  playerY: 0,
  playerVY: 0,
  isGrounded: true,
  isJumping: false,
  isSliding: false,
  slideTimer: 0,
  currentGroundY: 0, // 0 for ground, 3.2 for train roofs

  // Invulnerability after hoverboard smash or respawn
  invulnerableTimer: 0
};

// Three.js Core Handles
let scene, camera, renderer;
let playerGroup, characterMeshes, hoverboardMesh, jetpackMesh, sneakersMeshes;
let chaserGroup;
let worldChunks = [];
let obstacles = [];
let coins = [];
let powerupItems = [];
let particles = [];
let speedLines = [];

const CHUNK_SIZE = 80;
const ACTIVE_CHUNKS = 6;
let nextSpawnZ = -CHUNK_SIZE;

// Timing
let clock = new THREE.Clock();

// Textures cache
let textures = {};

// Initialize Game Engine
function initEngine() {
  const container = document.getElementById('game-container');
  const width = window.innerWidth;
  const height = window.innerHeight;

  // Scene
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0x87ceeb); // Bright warm sky
  scene.fog = new THREE.FogExp2(0xd6ecfa, 0.009);

  // Camera
  camera = new THREE.PerspectiveCamera(65, width / height, 0.1, 450);
  camera.position.set(0, 4.8, 7.5);
  camera.lookAt(0, 2.0, -10);

  // Renderer
  renderer = new THREE.WebGLRenderer({ antialias: true, powerPreference: 'high-performance' });
  renderer.setSize(width, height);
  renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
  renderer.shadowMap.enabled = true;
  renderer.shadowMap.type = THREE.PCFSoftShadowMap;
  container.appendChild(renderer.domElement);

  // Lights
  setupLighting();

  // Create procedural textures
  textures.ground = TextureGenerator.createTrackGroundTexture();
  textures.rails = TextureGenerator.createRailsTexture();
  textures.graffiti = TextureGenerator.createGraffitiWallTexture();
  textures.trainSides = [
    TextureGenerator.createTrainSideTexture('#e53935'), // Red express
    TextureGenerator.createTrainSideTexture('#1e88e5'), // Blue metro
    TextureGenerator.createTrainSideTexture('#43a047')  // Green commuter
  ];
  textures.trainFronts = [
    TextureGenerator.createTrainFrontTexture('#e53935'),
    TextureGenerator.createTrainFrontTexture('#1e88e5'),
    TextureGenerator.createTrainFrontTexture('#43a047')
  ];
  textures.hazard = TextureGenerator.createHazardStripeTexture();
  textures.clearance = TextureGenerator.createClearanceStripeTexture();
  textures.coin = TextureGenerator.createCoinTexture();

  // Build Player & Character Rig
  buildPlayer();

  // Build Chaser (Inspector & Dog)
  buildChaser();

  // Initial World Chunks
  for (let i = 0; i < ACTIVE_CHUNKS; i++) {
    spawnTrackChunk(i * -CHUNK_SIZE);
  }

  // Setup speed lines in air
  createSpeedLines();

  // Event Listeners
  window.addEventListener('resize', onWindowResize, false);
  setupControls();

  // Initial UI sync
  updateHUD();

  // Render loop
  clock.start();
  requestAnimationFrame(gameLoop);
}

// Lighting Setup
function setupLighting() {
  const ambientLight = new THREE.AmbientLight(0xfff3e0, 0.7);
  scene.add(ambientLight);

  const sunLight = new THREE.DirectionalLight(0xffffff, 1.1);
  sunLight.position.set(25, 45, 20);
  sunLight.castShadow = true;
  sunLight.shadow.mapSize.width = 1024;
  sunLight.shadow.mapSize.height = 1024;
  sunLight.shadow.camera.near = 5;
  sunLight.shadow.camera.far = 120;
  sunLight.shadow.camera.left = -18;
  sunLight.shadow.camera.right = 18;
  sunLight.shadow.camera.top = 25;
  sunLight.shadow.camera.bottom = -25;
  sunLight.shadow.bias = -0.001;
  scene.add(sunLight);

  // Subtle sky hemisphere fill
  const hemiLight = new THREE.HemisphereLight(0x70c5ff, 0x444422, 0.45);
  scene.add(hemiLight);
}

// -------------------------------------------------------------
// BUILD CHARACTER: Subway Runner Kid (Jake Style)
// -------------------------------------------------------------
function buildPlayer() {
  playerGroup = new THREE.Group();
  playerGroup.position.set(0, 0, 0);

  characterMeshes = {
    root: new THREE.Group(),
    bodyGroup: new THREE.Group(),
    headGroup: new THREE.Group(),
    leftArm: null,
    rightArm: null,
    leftLeg: null,
    rightLeg: null
  };

  const matCap = new THREE.MeshLambertMaterial({ color: 0xeb3b5a }); // Red cap
  const matCapBrim = new THREE.MeshLambertMaterial({ color: 0x2d3436 }); // Black brim
  const matSkin = new THREE.MeshLambertMaterial({ color: 0xfad390 }); // Skin
  const matHoodie = new THREE.MeshLambertMaterial({ color: 0x4b7bec }); // Cool Blue Hoodie
  const matPants = new THREE.MeshLambertMaterial({ color: 0x3867d6 }); // Denim pants
  const matShoes = new THREE.MeshLambertMaterial({ color: 0xffffff }); // White sneakers
  const matShoeSole = new THREE.MeshLambertMaterial({ color: 0xf7b731 }); // Yellow accent

  // Head & Cap
  const headGeo = new THREE.SphereGeometry(0.32, 16, 16);
  const head = new THREE.Mesh(headGeo, matSkin);
  head.position.y = 1.45;
  head.castShadow = true;

  // Cap worn backward (signature Subway Surfers style)
  const capGeo = new THREE.SphereGeometry(0.33, 16, 16, 0, Math.PI * 2, 0, Math.PI * 0.55);
  const cap = new THREE.Mesh(capGeo, matCap);
  cap.position.y = 0.05;
  head.add(cap);

  // Backward Cap Brim
  const brimGeo = new THREE.BoxGeometry(0.24, 0.04, 0.22);
  const brim = new THREE.Mesh(brimGeo, matCapBrim);
  brim.position.set(0, 0.08, 0.32);
  brim.rotation.x = -0.15;
  head.add(brim);

  // Eyes
  const eyeMat = new THREE.MeshBasicMaterial({ color: 0x1e272e });
  const eyeL = new THREE.Mesh(new THREE.BoxGeometry(0.06, 0.06, 0.04), eyeMat);
  eyeL.position.set(-0.11, 0.02, -0.3);
  const eyeR = eyeL.clone();
  eyeR.position.x = 0.11;
  head.add(eyeL);
  head.add(eyeR);

  characterMeshes.headGroup.add(head);

  // Torso / Hoodie
  const torsoGeo = new THREE.BoxGeometry(0.55, 0.65, 0.35);
  const torso = new THREE.Mesh(torsoGeo, matHoodie);
  torso.position.y = 0.95;
  torso.castShadow = true;

  // Backpack on back
  const bagGeo = new THREE.BoxGeometry(0.42, 0.48, 0.22);
  const bagMat = new THREE.MeshLambertMaterial({ color: 0x20bf6b });
  const backpack = new THREE.Mesh(bagGeo, bagMat);
  backpack.position.set(0, 0.02, 0.24);
  torso.add(backpack);

  // Spray can attached to backpack!
  const sprayGeo = new THREE.CylinderGeometry(0.05, 0.05, 0.2, 8);
  const sprayMat = new THREE.MeshLambertMaterial({ color: 0xff3838 });
  const sprayCan = new THREE.Mesh(sprayGeo, sprayMat);
  sprayCan.position.set(0.22, -0.05, 0.22);
  sprayCan.rotation.z = 0.2;
  torso.add(sprayCan);

  characterMeshes.bodyGroup.add(torso);
  characterMeshes.bodyGroup.add(characterMeshes.headGroup);

  // Left Arm
  const armGeo = new THREE.BoxGeometry(0.14, 0.52, 0.14);
  armGeo.translate(0, -0.22, 0); // Pivot at shoulder
  const leftArm = new THREE.Mesh(armGeo, matHoodie);
  leftArm.position.set(-0.35, 1.25, 0);
  leftArm.castShadow = true;
  characterMeshes.leftArm = leftArm;
  characterMeshes.bodyGroup.add(leftArm);

  // Right Arm
  const rightArm = new THREE.Mesh(armGeo, matHoodie);
  rightArm.position.set(0.35, 1.25, 0);
  rightArm.castShadow = true;
  characterMeshes.rightArm = rightArm;
  characterMeshes.bodyGroup.add(rightArm);

  // Legs & Sneakers
  const legGeo = new THREE.BoxGeometry(0.18, 0.58, 0.18);
  legGeo.translate(0, -0.25, 0); // Pivot at hip

  // Left Leg
  const leftLeg = new THREE.Mesh(legGeo, matPants);
  leftLeg.position.set(-0.16, 0.65, 0);
  leftLeg.castShadow = true;

  // Sneaker Left
  const shoeGeo = new THREE.BoxGeometry(0.2, 0.14, 0.32);
  const shoeL = new THREE.Mesh(shoeGeo, matShoes);
  shoeL.position.set(0, -0.5, -0.06);
  const soleL = new THREE.Mesh(new THREE.BoxGeometry(0.21, 0.04, 0.33), matShoeSole);
  soleL.position.y = -0.06;
  shoeL.add(soleL);
  leftLeg.add(shoeL);
  characterMeshes.leftLeg = leftLeg;
  playerGroup.add(leftLeg);

  // Right Leg
  const rightLeg = new THREE.Mesh(legGeo, matPants);
  rightLeg.position.set(0.16, 0.65, 0);
  rightLeg.castShadow = true;

  // Sneaker Right
  const shoeR = new THREE.Mesh(shoeGeo, matShoes);
  shoeR.position.set(0, -0.5, -0.06);
  const soleR = new THREE.Mesh(new THREE.BoxGeometry(0.21, 0.04, 0.33), matShoeSole);
  soleR.position.y = -0.06;
  shoeR.add(soleR);
  rightLeg.add(shoeR);
  characterMeshes.rightLeg = rightLeg;
  playerGroup.add(rightLeg);

  playerGroup.add(characterMeshes.bodyGroup);

  // --- ACCESSORIES: HOVERBOARD ---
  const boardGeo = new THREE.BoxGeometry(0.7, 0.08, 1.6);
  const boardMat = new THREE.MeshStandardMaterial({
    color: 0x00d2d3,
    emissive: 0x01a3a4,
    emissiveIntensity: 0.6,
    roughness: 0.2
  });
  hoverboardMesh = new THREE.Mesh(boardGeo, boardMat);
  hoverboardMesh.position.set(0, 0.08, 0);
  hoverboardMesh.visible = false;

  // Glowing hover thrusters under board
  const thrusterGeo = new THREE.CylinderGeometry(0.12, 0.12, 0.06, 12);
  const thrusterMat = new THREE.MeshBasicMaterial({ color: 0x54a0ff });
  const thrust1 = new THREE.Mesh(thrusterGeo, thrusterMat);
  thrust1.position.set(0, -0.05, -0.5);
  const thrust2 = thrust1.clone();
  thrust2.position.z = 0.5;
  hoverboardMesh.add(thrust1);
  hoverboardMesh.add(thrust2);
  playerGroup.add(hoverboardMesh);

  // --- ACCESSORIES: JETPACK ---
  jetpackMesh = new THREE.Group();
  const jetCylGeo = new THREE.CylinderGeometry(0.1, 0.1, 0.5, 12);
  const jetMat = new THREE.MeshStandardMaterial({ color: 0xff9f43, metalness: 0.8, roughness: 0.2 });
  const tube1 = new THREE.Mesh(jetCylGeo, jetMat);
  tube1.position.set(-0.16, 0.95, 0.32);
  const tube2 = tube1.clone();
  tube2.position.x = 0.16;
  jetpackMesh.add(tube1);
  jetpackMesh.add(tube2);

  // Jet exhaust nozzles
  const nozzleGeo = new THREE.ConeGeometry(0.08, 0.12, 12);
  const nozzleMat = new THREE.MeshLambertMaterial({ color: 0x333333 });
  const noz1 = new THREE.Mesh(nozzleGeo, nozzleMat);
  noz1.position.set(-0.16, 0.65, 0.32);
  const noz2 = noz1.clone();
  noz2.position.x = 0.16;
  jetpackMesh.add(noz1);
  jetpackMesh.add(noz2);

  jetpackMesh.visible = false;
  playerGroup.add(jetpackMesh);

  // --- ACCESSORIES: SUPER SNEAKERS (Springs) ---
  sneakersMeshes = [];
  [shoeL, shoeR].forEach(shoe => {
    const springGeo = new THREE.CylinderGeometry(0.08, 0.08, 0.15, 8);
    const springMat = new THREE.MeshStandardMaterial({ color: 0x10ac84, emissive: 0x1dd1a1, emissiveIntensity: 0.5 });
    const spring = new THREE.Mesh(springGeo, springMat);
    spring.position.y = -0.12;
    spring.visible = false;
    shoe.add(spring);
    sneakersMeshes.push(spring);
  });

  scene.add(playerGroup);
}

// -------------------------------------------------------------
// BUILD CHASER: Inspector & Bulldog behind player
// -------------------------------------------------------------
function buildChaser() {
  chaserGroup = new THREE.Group();
  chaserGroup.position.set(0, 0, 7.5); // Starts just behind player

  // Inspector body (Blue uniform)
  const inspectorMat = new THREE.MeshLambertMaterial({ color: 0x0c2461 });
  const inspBody = new THREE.Mesh(new THREE.BoxGeometry(0.7, 0.85, 0.4), inspectorMat);
  inspBody.position.y = 1.1;

  // Inspector head & Police Hat
  const inspHead = new THREE.Mesh(new THREE.SphereGeometry(0.28, 12, 12), new THREE.MeshLambertMaterial({ color: 0xfad390 }));
  inspHead.position.y = 1.7;
  const hat = new THREE.Mesh(new THREE.CylinderGeometry(0.32, 0.32, 0.1, 12), inspectorMat);
  hat.position.y = 0.18;
  const badge = new THREE.Mesh(new THREE.BoxGeometry(0.08, 0.08, 0.04), new THREE.MeshBasicMaterial({ color: 0xf1c40f }));
  badge.position.set(0, 0.18, -0.32);
  inspHead.add(hat);
  inspHead.add(badge);

  // Inspector Legs
  const legMat = new THREE.MeshLambertMaterial({ color: 0x1e272e });
  const legL = new THREE.Mesh(new THREE.BoxGeometry(0.2, 0.7, 0.2), legMat);
  legL.position.set(-0.2, 0.4, 0);
  const legR = legL.clone();
  legR.position.x = 0.2;

  chaserGroup.add(inspBody);
  chaserGroup.add(inspHead);
  chaserGroup.add(legL);
  chaserGroup.add(legR);

  // Bulldog pet beside inspector
  const dogMat = new THREE.MeshLambertMaterial({ color: 0xd35400 });
  const dogBody = new THREE.Mesh(new THREE.BoxGeometry(0.35, 0.3, 0.5), dogMat);
  dogBody.position.set(0.65, 0.3, 0.2);
  const dogHead = new THREE.Mesh(new THREE.BoxGeometry(0.25, 0.25, 0.25), dogMat);
  dogHead.position.set(0.65, 0.45, -0.15);
  chaserGroup.add(dogBody);
  chaserGroup.add(dogHead);

  scene.add(chaserGroup);
}

// -------------------------------------------------------------
// WORLD GENERATION: Subway Track Chunks
// -------------------------------------------------------------
function spawnTrackChunk(zPos) {
  const chunk = new THREE.Group();
  chunk.position.z = zPos;

  // 1. Ground Ballast / Asphalt
  const groundGeo = new THREE.PlaneGeometry(16, CHUNK_SIZE);
  const groundMat = new THREE.MeshLambertMaterial({
    map: textures.ground,
    roughness: 0.9
  });
  const ground = new THREE.Mesh(groundGeo, groundMat);
  ground.rotation.x = -Math.PI / 2;
  ground.position.set(0, 0, -CHUNK_SIZE / 2);
  ground.receiveShadow = true;
  chunk.add(ground);

  // 2. Three sets of railway tracks (Sleepers & Steel Rails)
  const railGeo = new THREE.PlaneGeometry(2.4, CHUNK_SIZE);
  const railMat = new THREE.MeshLambertMaterial({
    map: textures.rails,
    transparent: true,
    roughness: 0.4
  });

  GAME.lanes.forEach(laneX => {
    const railsMesh = new THREE.Mesh(railGeo, railMat);
    railsMesh.rotation.x = -Math.PI / 2;
    railsMesh.position.set(laneX, 0.02, -CHUNK_SIZE / 2);
    railsMesh.receiveShadow = true;
    chunk.add(railsMesh);
  });

  // 3. Side Walls with Subway Graffiti Art
  const wallGeo = new THREE.BoxGeometry(1.2, 7.5, CHUNK_SIZE);
  const wallMat = new THREE.MeshLambertMaterial({
    map: textures.graffiti,
    roughness: 0.8
  });

  const wallL = new THREE.Mesh(wallGeo, wallMat);
  wallL.position.set(-6.6, 3.75, -CHUNK_SIZE / 2);
  wallL.castShadow = true;
  wallL.receiveShadow = true;
  chunk.add(wallL);

  const wallR = new THREE.Mesh(wallGeo, wallMat);
  wallR.position.set(6.6, 3.75, -CHUNK_SIZE / 2);
  wallR.castShadow = true;
  wallR.receiveShadow = true;
  chunk.add(wallR);

  // 4. Overhead Arch Support Structures & Power Cables
  const archInterval = 25;
  for (let az = -archInterval; az > -CHUNK_SIZE; az -= archInterval) {
    const arch = createRailwayArch();
    arch.position.set(0, 0, az);
    chunk.add(arch);
  }

  // 5. Overhead power cables running along the tracks
  const cableGeo = new THREE.CylinderGeometry(0.03, 0.03, CHUNK_SIZE, 6);
  const cableMat = new THREE.MeshBasicMaterial({ color: 0x111111 });
  [-2.8, 0, 2.8].forEach(cx => {
    const cable = new THREE.Mesh(cableGeo, cableMat);
    cable.rotation.x = Math.PI / 2;
    cable.position.set(cx, 5.2, -CHUNK_SIZE / 2);
    chunk.add(cable);
  });

  scene.add(chunk);
  worldChunks.push(chunk);

  // Populate chunk with obstacles, coins, and powerups (skip initial spawn buffer)
  if (zPos < -50) {
    populateChunk(zPos);
  }
}

// Overhead Railway Arch with Signal Lights
function createRailwayArch() {
  const group = new THREE.Group();
  const archMat = new THREE.MeshLambertMaterial({ color: 0x2f3542 });

  // Columns Left and Right
  const colGeo = new THREE.BoxGeometry(0.4, 6.2, 0.4);
  const colL = new THREE.Mesh(colGeo, archMat);
  colL.position.set(-5.8, 3.1, 0);
  const colR = colL.clone();
  colR.position.x = 5.8;
  group.add(colL);
  group.add(colR);

  // Horizontal Crossbeam
  const beamGeo = new THREE.BoxGeometry(12.0, 0.5, 0.5);
  const beam = new THREE.Mesh(beamGeo, archMat);
  beam.position.set(0, 5.8, 0);
  group.add(beam);

  // Railway Signal Light (Green / Red)
  const signalGeo = new THREE.BoxGeometry(0.3, 0.7, 0.2);
  const signalMat = new THREE.MeshLambertMaterial({ color: 0x111111 });
  const signal = new THREE.Mesh(signalGeo, signalMat);
  signal.position.set(0, 5.2, 0);

  const lightG = new THREE.Mesh(new THREE.SphereGeometry(0.08, 8, 8), new THREE.MeshBasicMaterial({ color: 0x2ed573 }));
  lightG.position.set(0, 0.15, -0.11);
  signal.add(lightG);

  const lightR = new THREE.Mesh(new THREE.SphereGeometry(0.08, 8, 8), new THREE.MeshBasicMaterial({ color: 0xff4757 }));
  lightR.position.set(0, -0.15, -0.11);
  signal.add(lightR);

  group.add(signal);
  return group;
}

// -------------------------------------------------------------
// CHUNK POPULATION: Obstacles, Coins & Powerups
// -------------------------------------------------------------
function populateChunk(chunkZ) {
  // We place 2 to 3 obstacle patterns per chunk
  const stepZ = 28;
  const numPatterns = Math.floor(CHUNK_SIZE / stepZ);

  for (let p = 0; p < numPatterns; p++) {
    const patternZ = chunkZ - (p * stepZ + 15);
    const patternType = Math.floor(Math.random() * 6);

    switch (patternType) {
      case 0:
        // Train in one lane, hurdle in another, coins in the third
        spawnTrainPattern(patternZ);
        break;
      case 1:
        // High clearance barrier (requires slide) with coins underneath
        spawnBarrierPattern(patternZ, 'HIGH');
        break;
      case 2:
        // Low hurdle (requires jump) with arch of coins over it
        spawnBarrierPattern(patternZ, 'LOW');
        break;
      case 3:
        // Moving train approaching!
        spawnMovingTrainPattern(patternZ);
        break;
      case 4:
        // Double hurdle setup with single open lane
        spawnDoubleBarrierPattern(patternZ);
        break;
      case 5:
        // Power-up corridor
        spawnPowerupPattern(patternZ);
        break;
    }
  }
}

// 1. Train Pattern
function spawnTrainPattern(z) {
  const trainLane = Math.floor(Math.random() * 3);
  const otherLanes = [0, 1, 2].filter(l => l !== trainLane);
  const barrierLane = otherLanes[Math.floor(Math.random() * otherLanes.length)];

  // Spawn train
  spawnTrain(trainLane, z, false, true); // Has ramp on back!

  // Spawn low hurdle on another lane
  spawnHurdle(barrierLane, z - 4, false);

  // Coins on top of train!
  spawnCoinRow(trainLane, z - 8, 5, 3.4);

  // Coins in clear lane
  const clearLane = otherLanes.find(l => l !== barrierLane);
  if (clearLane !== undefined) {
    spawnCoinRow(clearLane, z - 12, 6, 1.0);
  }
}

// 2. Moving Train Pattern
function spawnMovingTrainPattern(z) {
  const trainLane = Math.floor(Math.random() * 3);
  spawnTrain(trainLane, z - 10, true, false);

  // Other lanes get coins and low hurdle
  const otherLanes = [0, 1, 2].filter(l => l !== trainLane);
  spawnCoinRow(otherLanes[0], z - 10, 5, 1.0);
  if (Math.random() > 0.4) {
    spawnHurdle(otherLanes[1], z - 5, false);
  }
}

// 3. Single Barrier (Low Jump or High Slide)
function spawnBarrierPattern(z, type) {
  const targetLane = Math.floor(Math.random() * 3);
  if (type === 'HIGH') {
    // High barrier (must slide)
    spawnHighBarrier(targetLane, z);
    // Slide coins right underneath!
    spawnCoinRow(targetLane, z - 3, 4, 0.45);
  } else {
    // Low hurdle (must jump)
    spawnHurdle(targetLane, z, false);
    // Arc of coins over hurdle!
    spawnCoinArc(targetLane, z);
  }

  // Maybe put power-up or coins in neighbor lanes
  const otherLanes = [0, 1, 2].filter(l => l !== targetLane);
  spawnCoinRow(otherLanes[0], z - 6, 4, 1.0);
  if (Math.random() > 0.7) {
    spawnRandomPowerup(otherLanes[1], z);
  }
}

// 4. Double Barrier
function spawnDoubleBarrierPattern(z) {
  const openLane = Math.floor(Math.random() * 3);
  [0, 1, 2].forEach(l => {
    if (l !== openLane) {
      if (Math.random() > 0.5) {
        spawnHurdle(l, z, false);
      } else {
        spawnHighBarrier(l, z);
      }
    } else {
      spawnCoinRow(l, z - 6, 5, 1.0);
    }
  });
}

// 5. Power-up corridor
function spawnPowerupPattern(z) {
  const pLane = Math.floor(Math.random() * 3);
  spawnRandomPowerup(pLane, z);
  spawnCoinRow((pLane + 1) % 3, z - 8, 6, 1.0);
  spawnHurdle((pLane + 2) % 3, z, false);
}

// -------------------------------------------------------------
// OBSTACLE BUILDERS: Trains, Hurdle Barriers, Signs
// -------------------------------------------------------------

// Subway Train Carriage
function spawnTrain(laneIndex, z, isMoving = false, hasRamp = false) {
  const trainGroup = new THREE.Group();
  const laneX = GAME.lanes[laneIndex];

  const trainLength = 22;
  const trainHeight = 3.2;
  const trainWidth = 2.3;

  const colorIdx = Math.floor(Math.random() * textures.trainSides.length);

  // Train Body
  const bodyGeo = new THREE.BoxGeometry(trainWidth, trainHeight, trainLength);
  const materials = [
    new THREE.MeshLambertMaterial({ map: textures.trainSides[colorIdx] }), // Right side
    new THREE.MeshLambertMaterial({ map: textures.trainSides[colorIdx] }), // Left side
    new THREE.MeshLambertMaterial({ color: 0x475569 }), // Roof
    new THREE.MeshLambertMaterial({ color: 0x1e293b }), // Underside
    new THREE.MeshLambertMaterial({ color: 0x334155 }), // Back
    new THREE.MeshLambertMaterial({ map: textures.trainFronts[colorIdx] }) // Front facing player
  ];
  const body = new THREE.Mesh(bodyGeo, materials);
  body.position.y = trainHeight / 2;
  body.castShadow = true;
  body.receiveShadow = true;
  trainGroup.add(body);

  // Wheels / Bogies
  const wheelMat = new THREE.MeshLambertMaterial({ color: 0x0f172a });
  const wheelGeo = new THREE.CylinderGeometry(0.35, 0.35, 0.15, 12);
  [-7, 7].forEach(wz => {
    [-1.05, 1.05].forEach(wx => {
      const wheel = new THREE.Mesh(wheelGeo, wheelMat);
      wheel.rotation.z = Math.PI / 2;
      wheel.position.set(wx, 0.35, wz);
      trainGroup.add(wheel);
    });
  });

  // Front Headlight Glow
  const lightGeo = new THREE.SphereGeometry(0.2, 8, 8);
  const lightMat = new THREE.MeshBasicMaterial({ color: 0xfffa65 });
  const light1 = new THREE.Mesh(lightGeo, lightMat);
  light1.position.set(-0.6, 2.3, trainLength / 2 + 0.05);
  const light2 = light1.clone();
  light2.position.x = 0.6;
  trainGroup.add(light1);
  trainGroup.add(light2);

  // Climb Ramp (allows running up onto train roof!)
  if (hasRamp) {
    const rampLength = 6.0;
    const rampGeo = new THREE.BoxGeometry(trainWidth * 0.95, 0.3, rampLength);
    const rampMat = new THREE.MeshLambertMaterial({ map: textures.hazard });
    const ramp = new THREE.Mesh(rampGeo, rampMat);
    ramp.rotation.x = Math.atan2(trainHeight - 0.2, rampLength);
    ramp.position.set(0, trainHeight / 2, trainLength / 2 + rampLength / 2 - 0.5);
    ramp.receiveShadow = true;
    trainGroup.add(ramp);
  }

  trainGroup.position.set(laneX, 0, z);

  const obstacleData = {
    type: 'TRAIN',
    mesh: trainGroup,
    lane: laneIndex,
    isMoving: isMoving,
    moveSpeed: isMoving ? 14 : 0,
    hasRamp: hasRamp,
    bounds: {
      minX: laneX - trainWidth / 2,
      maxX: laneX + trainWidth / 2,
      minY: 0,
      maxY: trainHeight,
      length: trainLength,
      hasRoof: true,
      roofY: trainHeight
    }
  };

  scene.add(trainGroup);
  obstacles.push(obstacleData);
}

// Low Hurdle (Yellow/Black Hazard Jump Barrier)
function spawnHurdle(laneIndex, z, tall = false) {
  const hurdleGroup = new THREE.Group();
  const laneX = GAME.lanes[laneIndex];
  const hurdleH = tall ? 1.5 : 1.15;
  const hurdleW = 2.4;

  // Horizontal bar
  const barGeo = new THREE.BoxGeometry(hurdleW, 0.38, 0.16);
  const barMat = new THREE.MeshLambertMaterial({ map: textures.hazard });
  const bar = new THREE.Mesh(barGeo, barMat);
  bar.position.y = hurdleH - 0.19;
  bar.castShadow = true;
  hurdleGroup.add(bar);

  // Stand posts Left & Right
  const postGeo = new THREE.CylinderGeometry(0.06, 0.08, hurdleH, 8);
  const postMat = new THREE.MeshLambertMaterial({ color: 0x333333 });

  const postL = new THREE.Mesh(postGeo, postMat);
  postL.position.set(-hurdleW / 2 + 0.12, hurdleH / 2, 0);
  postL.castShadow = true;
  hurdleGroup.add(postL);

  const postR = postL.clone();
  postR.position.x = hurdleW / 2 - 0.12;
  hurdleGroup.add(postR);

  hurdleGroup.position.set(laneX, 0, z);

  const obstacleData = {
    type: 'LOW_HURDLE',
    mesh: hurdleGroup,
    lane: laneIndex,
    bounds: {
      minX: laneX - hurdleW / 2 + 0.1,
      maxX: laneX + hurdleW / 2 - 0.1,
      minY: 0,
      maxY: hurdleH,
      minZ: z - 0.5,
      maxZ: z + 0.5
    }
  };

  scene.add(hurdleGroup);
  obstacles.push(obstacleData);
}

// High Overhead Clearance Barrier (Red/White - Slide Under)
function spawnHighBarrier(laneIndex, z) {
  const barrierGroup = new THREE.Group();
  const laneX = GAME.lanes[laneIndex];
  const barrierW = 2.5;

  // Tall posts
  const postGeo = new THREE.BoxGeometry(0.16, 3.2, 0.16);
  const postMat = new THREE.MeshLambertMaterial({ color: 0x222222 });

  const postL = new THREE.Mesh(postGeo, postMat);
  postL.position.set(-barrierW / 2 + 0.1, 1.6, 0);
  barrierGroup.add(postL);

  const postR = postL.clone();
  postR.position.x = barrierW / 2 - 0.1;
  barrierGroup.add(postR);

  // Overhead warning bar (suspended between 1.15 and 2.5)
  const barGeo = new THREE.BoxGeometry(barrierW, 1.3, 0.2);
  const barMat = new THREE.MeshLambertMaterial({ map: textures.clearance });
  const bar = new THREE.Mesh(barGeo, barMat);
  bar.position.set(0, 2.05, 0);
  bar.castShadow = true;
  barrierGroup.add(bar);

  // "DANGER / SLIDE" sign
  const signGeo = new THREE.BoxGeometry(1.4, 0.45, 0.22);
  const signMat = new THREE.MeshLambertMaterial({ color: 0xff3838 });
  const sign = new THREE.Mesh(signGeo, signMat);
  sign.position.set(0, 2.05, 0.04);
  barrierGroup.add(sign);

  barrierGroup.position.set(laneX, 0, z);

  const obstacleData = {
    type: 'HIGH_BARRIER',
    mesh: barrierGroup,
    lane: laneIndex,
    bounds: {
      minX: laneX - barrierW / 2 + 0.1,
      maxX: laneX + barrierW / 2 - 0.1,
      minY: 1.15, // Head collides if standing, passes under if sliding!
      maxY: 2.8,
      minZ: z - 0.5,
      maxZ: z + 0.5
    }
  };

  scene.add(barrierGroup);
  obstacles.push(obstacleData);
}

// -------------------------------------------------------------
// COLLECTIBLES & POWERUPS
// -------------------------------------------------------------

// Single 3D Gold Coin
function spawnCoin(x, y, z) {
  const coinGeo = new THREE.CylinderGeometry(0.38, 0.38, 0.08, 16);
  const coinMat = new THREE.MeshStandardMaterial({
    map: textures.coin,
    color: 0xffd700,
    metalness: 0.85,
    roughness: 0.25,
    emissive: 0xff9900,
    emissiveIntensity: 0.35
  });

  const coin = new THREE.Mesh(coinGeo, coinMat);
  coin.rotation.x = Math.PI / 2;
  coin.position.set(x, y, z);
  coin.castShadow = true;

  scene.add(coin);
  coins.push({
    mesh: coin,
    baseY: y,
    active: true
  });
}

// Row of coins along a lane
function spawnCoinRow(laneIndex, startZ, count = 5, y = 1.0) {
  const x = GAME.lanes[laneIndex];
  for (let i = 0; i < count; i++) {
    spawnCoin(x, y, startZ - i * 3.2);
  }
}

// Parabolic Arc of coins (e.g. over a hurdle)
function spawnCoinArc(laneIndex, z) {
  const x = GAME.lanes[laneIndex];
  const arcCoins = 5;
  for (let i = 0; i < arcCoins; i++) {
    const t = (i - (arcCoins - 1) / 2) / ((arcCoins - 1) / 2); // -1 to 1
    const coinY = 1.1 + (1.0 - t * t) * 1.8; // Peak at 2.9
    const coinZ = z + t * 4.0;
    spawnCoin(x, coinY, coinZ);
  }
}

// Power-up Item (Magnet, Sneakers, Jetpack, 2X Multiplier)
function spawnRandomPowerup(laneIndex, z) {
  const x = GAME.lanes[laneIndex];
  const types = ['MAGNET', 'SNEAKERS', 'JETPACK', 'MULTIPLIER'];
  const type = types[Math.floor(Math.random() * types.length)];

  const group = new THREE.Group();

  if (type === 'MAGNET') {
    // Horseshoe magnet
    const torusGeo = new THREE.TorusGeometry(0.35, 0.1, 10, 16, Math.PI);
    const magnetMat = new THREE.MeshStandardMaterial({ color: 0xe74c3c, metalness: 0.8 });
    const magnet = new THREE.Mesh(torusGeo, magnetMat);
    magnet.rotation.z = Math.PI;

    // Silver tips
    const tipGeo = new THREE.BoxGeometry(0.2, 0.22, 0.2);
    const tipMat = new THREE.MeshStandardMaterial({ color: 0xbdc3c7, metalness: 0.9 });
    const tipL = new THREE.Mesh(tipGeo, tipMat);
    tipL.position.set(-0.35, 0.05, 0);
    const tipR = tipL.clone();
    tipR.position.x = 0.35;
    group.add(magnet);
    group.add(tipL);
    group.add(tipR);

  } else if (type === 'SNEAKERS') {
    // Neon winged shoe
    const shoeGeo = new THREE.BoxGeometry(0.4, 0.25, 0.6);
    const shoeMat = new THREE.MeshStandardMaterial({ color: 0x1dd1a1, emissive: 0x10ac84, emissiveIntensity: 0.6 });
    const shoe = new THREE.Mesh(shoeGeo, shoeMat);

    // Spring base
    const springGeo = new THREE.CylinderGeometry(0.15, 0.15, 0.25, 12);
    const springMat = new THREE.MeshStandardMaterial({ color: 0xfff200 });
    const spring = new THREE.Mesh(springGeo, springMat);
    spring.position.y = -0.22;
    group.add(shoe);
    group.add(spring);

  } else if (type === 'JETPACK') {
    // Rocket Jetpack with exhaust fire cone
    const cylGeo = new THREE.CylinderGeometry(0.18, 0.18, 0.7, 12);
    const rocketMat = new THREE.MeshStandardMaterial({ color: 0xff9f43, metalness: 0.8 });
    const r1 = new THREE.Mesh(cylGeo, rocketMat);
    r1.position.x = -0.2;
    const r2 = r1.clone();
    r2.position.x = 0.2;

    const fireGeo = new THREE.ConeGeometry(0.16, 0.35, 12);
    const fireMat = new THREE.MeshBasicMaterial({ color: 0xff3838 });
    const f1 = new THREE.Mesh(fireGeo, fireMat);
    f1.position.set(-0.2, -0.45, 0);
    f1.rotation.x = Math.PI;
    const f2 = f1.clone();
    f2.position.x = 0.2;

    group.add(r1);
    group.add(r2);
    group.add(f1);
    group.add(f2);

  } else if (type === 'MULTIPLIER') {
    // Glowing '2X' emblem
    const circleGeo = new THREE.CylinderGeometry(0.4, 0.4, 0.1, 16);
    const circleMat = new THREE.MeshStandardMaterial({ color: 0x9b59b6, emissive: 0x8e44ad, emissiveIntensity: 0.8 });
    const disc = new THREE.Mesh(circleGeo, circleMat);
    disc.rotation.x = Math.PI / 2;

    const textMat = new THREE.MeshBasicMaterial({ color: 0xffffff });
    const star = new THREE.Mesh(new THREE.BoxGeometry(0.2, 0.5, 0.14), textMat);
    group.add(disc);
    group.add(star);
  }

  // Floating hover ring aura around power-up
  const ringGeo = new THREE.RingGeometry(0.5, 0.65, 24);
  const ringMat = new THREE.MeshBasicMaterial({ color: 0xfffa65, side: THREE.DoubleSide, transparent: true, opacity: 0.75 });
  const ring = new THREE.Mesh(ringGeo, ringMat);
  ring.rotation.x = Math.PI / 2;
  group.add(ring);

  group.position.set(x, 1.4, z);
  scene.add(group);

  powerupItems.push({
    mesh: group,
    type: type,
    active: true,
    baseY: 1.4
  });
}

// -------------------------------------------------------------
// CONTROLS & INPUT SYSTEM (Arrow Keys, Swipe, Hoverboard)
// -------------------------------------------------------------
function setupControls() {
  window.addEventListener('keydown', (e) => {
    // Unlock Audio Context on first keypress
    Sound.init();

    if (e.code === 'KeyP' || e.code === 'Escape') {
      togglePause();
      return;
    }

    // PRESS SPACE OR ENTER TO START / RESTART GAME!
    if (e.code === 'Space' || e.code === 'Enter') {
      const isGameOverActive = document.getElementById('gameover-modal')?.classList.contains('active');
      if (!GAME.active || isGameOverActive) {
        e.preventDefault();
        startNewGame();
        return;
      }
    }

    if (!GAME.active || GAME.paused) return;

    switch (e.code) {
      case 'ArrowLeft':
      case 'KeyA':
        handleMoveLeft();
        e.preventDefault();
        break;

      case 'ArrowRight':
      case 'KeyD':
        handleMoveRight();
        e.preventDefault();
        break;

      case 'ArrowUp':
      case 'KeyW':
        handleJump();
        e.preventDefault();
        break;

      case 'ArrowDown':
      case 'KeyS':
        handleSlide();
        e.preventDefault();
        break;

      case 'Space':
        // Activate Hoverboard when running!
        activateHoverboard();
        e.preventDefault();
        break;
    }
  });

  // Touch & Mouse Swipe Handlers for mobile & laptops
  let touchStartX = 0;
  let touchStartY = 0;
  let touchStartTime = 0;

  window.addEventListener('touchstart', (e) => {
    Sound.init();
    touchStartX = e.touches[0].clientX;
    touchStartY = e.touches[0].clientY;
    touchStartTime = Date.now();
  }, { passive: true });

  window.addEventListener('touchend', (e) => {
    if (!GAME.active || GAME.paused) return;
    const dx = e.changedTouches[0].clientX - touchStartX;
    const dy = e.changedTouches[0].clientY - touchStartY;
    const dt = Date.now() - touchStartTime;

    // Detect double tap for hoverboard
    if (Math.abs(dx) < 20 && Math.abs(dy) < 20 && dt < 250) {
      activateHoverboard();
      return;
    }

    handleSwipe(dx, dy);
  }, { passive: true });

  // Double click / double tap anywhere to activate hoverboard
  let lastClickTime = 0;
  window.addEventListener('click', () => {
    Sound.init();
    const now = Date.now();
    if (now - lastClickTime < 300 && GAME.active && !GAME.paused) {
      activateHoverboard();
    }
    lastClickTime = now;
  });
}

function handleSwipe(dx, dy) {
  const threshold = 35;
  if (Math.abs(dx) > Math.abs(dy)) {
    if (dx > threshold) handleMoveRight();
    else if (dx < -threshold) handleMoveLeft();
  } else {
    if (dy < -threshold) handleJump();
    else if (dy > threshold) handleSlide();
  }
}

function handleMoveLeft() {
  if (GAME.currentLane > 0) {
    GAME.currentLane--;
    GAME.targetX = GAME.lanes[GAME.currentLane];
    Sound.playLaneSwitch();
  }
}

function handleMoveRight() {
  if (GAME.currentLane < 2) {
    GAME.currentLane++;
    GAME.targetX = GAME.lanes[GAME.currentLane];
    Sound.playLaneSwitch();
  }
}

function handleJump() {
  // If sliding, cancel slide immediately to jump
  if (GAME.isSliding) {
    cancelSlide();
  }

  // Can only jump if on ground/roof and not jetpacking
  if ((GAME.isGrounded || GAME.playerY <= GAME.currentGroundY + 0.2) && !GAME.powerups.jetpack.active) {
    const jumpBoost = GAME.powerups.sneakers.active ? 1.55 : 1.0;
    GAME.playerVY = 16.5 * jumpBoost;
    GAME.isGrounded = false;
    GAME.isJumping = true;

    if (GAME.powerups.sneakers.active) {
      Sound.playSuperJump();
      spawnParticleBurst(playerGroup.position.x, playerGroup.position.y, playerGroup.position.z, 0x1dd1a1, 14);
    } else {
      Sound.playJump();
    }
  }
}

function handleSlide() {
  // Classic Subway Surfers fast-dive mechanic:
  // If in mid-air, slamming down immediately dives to ground and enters slide!
  if (GAME.isJumping || !GAME.isGrounded) {
    GAME.playerVY = -24; // Fast slam down
  }

  GAME.isSliding = true;
  GAME.slideTimer = 0.85; // Slide duration
  Sound.playSlide();

  // Slide sparks
  spawnParticleBurst(playerGroup.position.x, playerGroup.position.y + 0.1, playerGroup.position.z, 0xffd32a, 8);
}

function cancelSlide() {
  GAME.isSliding = false;
  GAME.slideTimer = 0;
  characterMeshes.bodyGroup.rotation.x = 0;
  characterMeshes.bodyGroup.position.y = 0;
}

function activateHoverboard() {
  if (GAME.powerups.hoverboard.active) return;
  GAME.powerups.hoverboard.active = true;
  GAME.powerups.hoverboard.timer = GAME.powerups.hoverboard.maxDuration;
  hoverboardMesh.visible = true;
  Sound.playPowerup();
  showPowerupToast('HOVERBOARD ACTIVE! SHIELD ON');
  spawnParticleBurst(playerGroup.position.x, playerGroup.position.y, playerGroup.position.z, 0x00d2d3, 20);
}

// -------------------------------------------------------------
// GAME LOOP & PHYSICS
// -------------------------------------------------------------
function gameLoop() {
  requestAnimationFrame(gameLoop);

  const dt = Math.min(clock.getDelta(), 0.1);

  if (GAME.active && !GAME.paused) {
    updateGame(dt);
  } else if (!GAME.active) {
    updateStartupMenuScene(dt);
  }

  renderer.render(scene, camera);
}

// Idle preview scene animation while on the startup menu
function updateStartupMenuScene(dt) {
  const time = clock.getElapsedTime();

  // Subtle character idle breathing and arm swaying
  if (characterMeshes && characterMeshes.bodyGroup) {
    characterMeshes.bodyGroup.position.y = Math.sin(time * 2.4) * 0.05;
    characterMeshes.bodyGroup.rotation.x = 0.05;
    characterMeshes.bodyGroup.rotation.y = Math.sin(time * 0.8) * 0.12;

    if (characterMeshes.leftArm) characterMeshes.leftArm.rotation.x = Math.sin(time * 1.8) * 0.2;
    if (characterMeshes.rightArm) characterMeshes.rightArm.rotation.x = -Math.sin(time * 1.8) * 0.2 + 0.25;
    if (characterMeshes.headGroup) characterMeshes.headGroup.rotation.y = Math.sin(time * 1.0) * 0.2;
    if (characterMeshes.leftLeg) characterMeshes.leftLeg.rotation.x = 0;
    if (characterMeshes.rightLeg) characterMeshes.rightLeg.rotation.x = 0;
  }

  // Smooth cinematic camera floating
  const camBobX = Math.sin(time * 0.5) * 0.6;
  const camBobY = 4.6 + Math.cos(time * 0.7) * 0.25;
  camera.position.x += (camBobX - camera.position.x) * 4 * dt;
  camera.position.y += (camBobY - camera.position.y) * 4 * dt;
  camera.position.z = 7.2;
  camera.lookAt(0, 1.8, -10);

  // Inspector and dog idling behind
  if (chaserGroup) {
    chaserGroup.position.set(0, 0, 8.2);
    chaserGroup.position.y = Math.sin(time * 2.0) * 0.04;
  }

  // Slowly rotate coins in the preview scene
  coins.forEach(c => {
    if (c.mesh) c.mesh.rotation.z += 2.0 * dt;
  });

  // Slowly float speed lines
  updateSpeedLines(dt * 0.3);
}

function updateGame(dt) {
  // Speed progression: gradually accelerates
  GAME.speed = Math.min(GAME.maxSpeed, GAME.baseSpeed + (GAME.distance / 180));

  // Forward distance accumulation
  const moveDist = GAME.speed * dt;
  GAME.distance += moveDist;

  // Score progression
  const scoreMultiplier = (GAME.powerups.multiplier.active ? 2 : 1) * GAME.multiplier;
  GAME.score += Math.floor(moveDist * scoreMultiplier * 0.9);

  if (GAME.score > GAME.highScore) {
    GAME.highScore = GAME.score;
    localStorage.setItem('subway_highscore', GAME.highScore.toString());
  }

  // Update Powerup Timers
  updatePowerups(dt);

  // Update Player Horizontal Motion (Lane switch lerp)
  const currentX = playerGroup.position.x;
  playerGroup.position.x += (GAME.targetX - currentX) * 16 * dt;

  // Character tilt when changing lanes
  const laneDeltaX = GAME.targetX - playerGroup.position.x;
  playerGroup.rotation.z = -laneDeltaX * 0.08;
  playerGroup.rotation.y = -laneDeltaX * 0.04;

  // Determine current ground level (e.g. are we above a train roof?)
  detectGroundRoofLevel();

  // Vertical Physics (Jump / Gravity / Jetpack)
  updatePlayerVerticalPhysics(dt);

  // Character procedural limb animations (Run cycle, jump pose, slide roll)
  animateCharacter(dt);

  // Move World Elements (Endless runner illusion)
  updateWorld(moveDist, dt);

  // Check Collisions (Obstacles, Coins, Powerups)
  checkCollisions();

  // Update Chaser Position (Inspector & Dog)
  updateChaser(dt);

  // Update Particles & Speed lines
  updateParticles(dt);
  updateSpeedLines(dt);

  // Dynamic Camera follow & slight shake
  updateCamera(dt);

  // Update HUD
  updateHUD();
}

// Check if player is positioned directly over a train roof
function detectGroundRoofLevel() {
  if (GAME.powerups.jetpack.active) {
    GAME.currentGroundY = 8.5; // High in sky!
    return;
  }

  let groundY = 0;
  const pX = playerGroup.position.x;
  const pZ = playerGroup.position.z;

  for (let obs of obstacles) {
    if (obs.type === 'TRAIN' && obs.bounds.hasRoof) {
      const b = obs.bounds;
      const trainZ = obs.mesh.position.z;
      const minZ = trainZ - b.length / 2;
      const maxZ = trainZ + b.length / 2 + (obs.hasRamp ? 6 : 0);

      if (Math.abs(pX - obs.mesh.position.x) < 1.3 && pZ >= minZ && pZ <= maxZ) {
        // If on the ramp: slope upwards!
        if (obs.hasRamp && pZ > trainZ + b.length / 2 - 1) {
          const rampT = 1.0 - (pZ - (trainZ + b.length / 2 - 1)) / 6.0;
          groundY = Math.max(0, Math.min(b.roofY, b.roofY * rampT));
        } else {
          groundY = b.roofY;
        }
        break;
      }
    }
  }

  GAME.currentGroundY = groundY;
}

// Vertical physics update
function updatePlayerVerticalPhysics(dt) {
  if (GAME.powerups.jetpack.active) {
    // Soar smoothly to sky level
    playerGroup.position.y += (8.5 - playerGroup.position.y) * 6 * dt;
    GAME.playerVY = 0;
    GAME.isGrounded = false;
    return;
  }

  const gravity = 48; // Snappy arcade gravity
  GAME.playerVY -= gravity * dt;
  playerGroup.position.y += GAME.playerVY * dt;

  // Ground collision check
  if (playerGroup.position.y <= GAME.currentGroundY) {
    playerGroup.position.y = GAME.currentGroundY;
    GAME.playerVY = 0;

    if (!GAME.isGrounded) {
      GAME.isGrounded = true;
      GAME.isJumping = false;
      // Landing dust puff
      spawnParticleBurst(playerGroup.position.x, playerGroup.position.y + 0.1, playerGroup.position.z, 0xffffff, 6);
    }
  } else {
    GAME.isGrounded = false;
  }

  // Update Slide Duration
  if (GAME.isSliding) {
    GAME.slideTimer -= dt;
    if (GAME.slideTimer <= 0) {
      cancelSlide();
    }
  }
}

// Character procedural animation rig
function animateCharacter(dt) {
  const time = clock.getElapsedTime() * 14 * (GAME.speed / 38);

  if (GAME.powerups.jetpack.active) {
    // Superman / Jetpack soaring pose
    characterMeshes.bodyGroup.rotation.x = 0.85;
    characterMeshes.leftArm.rotation.x = -1.6;
    characterMeshes.rightArm.rotation.x = -1.6;
    characterMeshes.leftLeg.rotation.x = 0.3;
    characterMeshes.rightLeg.rotation.x = 0.3;
    hoverboardMesh.visible = false;
    jetpackMesh.visible = true;
    return;
  }

  jetpackMesh.visible = false;

  if (GAME.powerups.hoverboard.active) {
    // Surfer Stance on Hoverboard!
    hoverboardMesh.visible = true;
    hoverboardMesh.rotation.y = Math.sin(time * 0.5) * 0.08;
    characterMeshes.bodyGroup.rotation.y = 0.75; // Sideways stance
    characterMeshes.bodyGroup.rotation.x = 0.1;
    characterMeshes.leftArm.rotation.x = -0.5;
    characterMeshes.rightArm.rotation.x = 0.5;
    characterMeshes.leftLeg.rotation.x = 0.2;
    characterMeshes.rightLeg.rotation.x = -0.2;
    return;
  }

  hoverboardMesh.visible = false;
  characterMeshes.bodyGroup.rotation.y = 0;

  if (GAME.isSliding) {
    // Slide / Roll Pose: Tucked low to ground
    characterMeshes.bodyGroup.rotation.x = -1.1;
    characterMeshes.bodyGroup.position.y = -0.45;
    characterMeshes.leftArm.rotation.x = 1.2;
    characterMeshes.rightArm.rotation.x = 1.2;
    characterMeshes.leftLeg.rotation.x = -1.3;
    characterMeshes.rightLeg.rotation.x = -1.1;
  } else if (GAME.isJumping) {
    // Airborne Leap Pose
    characterMeshes.bodyGroup.position.y = 0;
    characterMeshes.bodyGroup.rotation.x = 0.2;
    characterMeshes.leftArm.rotation.x = -2.2;
    characterMeshes.rightArm.rotation.x = -2.2;
    characterMeshes.leftLeg.rotation.x = 0.6;
    characterMeshes.rightLeg.rotation.x = -0.5;
  } else {
    // Running cycle!
    characterMeshes.bodyGroup.position.y = Math.abs(Math.sin(time)) * 0.08;
    characterMeshes.bodyGroup.rotation.x = 0.18; // Lean forward while running

    // Alternating arms swing
    characterMeshes.leftArm.rotation.x = Math.sin(time) * 0.9;
    characterMeshes.rightArm.rotation.x = -Math.sin(time) * 0.9;

    // Alternating leg strides
    characterMeshes.leftLeg.rotation.x = -Math.sin(time) * 0.95;
    characterMeshes.rightLeg.rotation.x = Math.sin(time) * 0.95;
  }

  // Invulnerability flicker after shield break
  if (GAME.invulnerableTimer > 0) {
    GAME.invulnerableTimer -= dt;
    playerGroup.visible = Math.floor(Date.now() / 80) % 2 === 0;
  } else {
    playerGroup.visible = true;
  }
}

// -------------------------------------------------------------
// WORLD UPDATES: Endless Runner Track Recycling
// -------------------------------------------------------------
function updateWorld(moveDist, dt) {
  // Move all track chunks backwards
  for (let i = worldChunks.length - 1; i >= 0; i--) {
    const chunk = worldChunks[i];
    chunk.position.z += moveDist;

    // When chunk passes behind camera, recycle it to the front!
    if (chunk.position.z > CHUNK_SIZE * 1.5) {
      scene.remove(chunk);
      worldChunks.splice(i, 1);

      // Find the furthest chunk
      let minZ = 0;
      worldChunks.forEach(c => {
        if (c.position.z < minZ) minZ = c.position.z;
      });

      spawnTrackChunk(minZ - CHUNK_SIZE);
    }
  }

  // Move obstacles
  for (let i = obstacles.length - 1; i >= 0; i--) {
    const obs = obstacles[i];
    obs.mesh.position.z += moveDist + (obs.isMoving ? obs.moveSpeed * dt : 0);

    // If passed far behind camera, clean up
    if (obs.mesh.position.z > 25) {
      scene.remove(obs.mesh);
      obstacles.splice(i, 1);
    }
  }

  // Move & spin coins
  for (let i = coins.length - 1; i >= 0; i--) {
    const coin = coins[i];
    coin.mesh.position.z += moveDist;
    coin.mesh.rotation.z += 4.5 * dt;

    // Coin Magnet attraction: Sucks coins towards player!
    if (GAME.powerups.magnet.active && coin.active) {
      const dx = playerGroup.position.x - coin.mesh.position.x;
      const dy = playerGroup.position.y + 0.8 - coin.mesh.position.y;
      const dz = playerGroup.position.z - coin.mesh.position.z;
      const dist = Math.sqrt(dx * dx + dy * dy + dz * dz);

      if (dist < 20) {
        coin.mesh.position.x += dx * 14 * dt;
        coin.mesh.position.y += dy * 14 * dt;
        coin.mesh.position.z += dz * 14 * dt;
      }
    }

    if (coin.mesh.position.z > 20) {
      scene.remove(coin.mesh);
      coins.splice(i, 1);
    }
  }

  // Move & spin powerups
  for (let i = powerupItems.length - 1; i >= 0; i--) {
    const pItem = powerupItems[i];
    pItem.mesh.position.z += moveDist;
    pItem.mesh.rotation.y += 3.0 * dt;
    pItem.mesh.position.y = pItem.baseY + Math.sin(clock.getElapsedTime() * 4) * 0.15;

    if (pItem.mesh.position.z > 20) {
      scene.remove(pItem.mesh);
      powerupItems.splice(i, 1);
    }
  }
}

// -------------------------------------------------------------
// COLLISION DETECTION & POWER-UP LOGIC
// -------------------------------------------------------------
function checkCollisions() {
  const pX = playerGroup.position.x;
  const pY = playerGroup.position.y;
  const pZ = playerGroup.position.z;

  // Hitbox dimensions (lower height when sliding!)
  const playerHeight = GAME.isSliding ? 0.75 : 1.75;
  const playerTop = pY + playerHeight;
  const playerBottom = pY + 0.1;
  const playerWidth = 0.65;

  // 1. Coin Collisions
  for (let i = 0; i < coins.length; i++) {
    const coin = coins[i];
    if (!coin.active) continue;

    const cX = coin.mesh.position.x;
    const cY = coin.mesh.position.y;
    const cZ = coin.mesh.position.z;

    if (Math.abs(pX - cX) < 1.1 && Math.abs(pZ - cZ) < 1.4 && pY <= cY + 1.2 && playerTop >= cY - 0.5) {
      // Coin collected!
      coin.active = false;
      scene.remove(coin.mesh);
      coins.splice(i, 1);
      i--;

      GAME.coins += 1;
      GAME.score += 50;
      Sound.playCoin();
      spawnParticleBurst(cX, cY, cZ, 0xffd700, 8);
    }
  }

  // 2. Power-up Collisions
  for (let i = 0; i < powerupItems.length; i++) {
    const pItem = powerupItems[i];
    if (!pItem.active) continue;

    const pm = pItem.mesh.position;
    if (Math.abs(pX - pm.x) < 1.3 && Math.abs(pZ - pm.z) < 1.6 && Math.abs(pY + 0.8 - pm.y) < 1.6) {
      pItem.active = false;
      scene.remove(pItem.mesh);
      powerupItems.splice(i, 1);
      i--;

      activatePowerup(pItem.type);
    }
  }

  // 3. Obstacle Collisions (Only if not jetpacking and not currently invulnerable)
  if (GAME.powerups.jetpack.active || GAME.invulnerableTimer > 0) return;

  for (let obs of obstacles) {
    const om = obs.mesh.position;

    if (obs.type === 'TRAIN') {
      const b = obs.bounds;
      const trainZ = om.z;
      const halfLen = b.length / 2;

      // Check if player is within train X and Z bounds
      if (Math.abs(pX - om.x) < 1.2 && pZ >= trainZ - halfLen && pZ <= trainZ + halfLen) {
        // If player is on top of train roof (or landing on it) -> Safe!
        if (pY >= b.roofY - 0.25) {
          // Running on train roof!
          continue;
        }

        // Running on ramp safely?
        if (obs.hasRamp && pZ > trainZ + halfLen - 2) {
          continue;
        }

        // Direct crash into train front/side!
        triggerCrash('CRASHED INTO TRAIN!');
        return;
      }
    } else if (obs.type === 'LOW_HURDLE') {
      const b = obs.bounds;
      if (Math.abs(pX - om.x) < 1.15 && Math.abs(pZ - om.z) < 0.6) {
        // If jumping above hurdle -> Safe!
        if (playerBottom >= b.maxY - 0.1) {
          continue;
        }
        triggerCrash('HIT HURDLE BARRICADE!');
        return;
      }
    } else if (obs.type === 'HIGH_BARRIER') {
      const b = obs.bounds;
      if (Math.abs(pX - om.x) < 1.15 && Math.abs(pZ - om.z) < 0.6) {
        // If sliding under -> playerTop is <= 0.75 + pY, passes under!
        if (playerTop <= b.minY + 0.1) {
          // Succeeded sliding under!
          continue;
        }
        triggerCrash('HIT OVERHEAD BARRIER!');
        return;
      }
    }
  }
}

// Powerup activation handlers
function activatePowerup(type) {
  Sound.playPowerup();

  if (type === 'MAGNET') {
    GAME.powerups.magnet.active = true;
    GAME.powerups.magnet.timer = GAME.powerups.magnet.maxDuration;
    showPowerupToast('COIN MAGNET ACTIVATED!');
  } else if (type === 'SNEAKERS') {
    GAME.powerups.sneakers.active = true;
    GAME.powerups.sneakers.timer = GAME.powerups.sneakers.maxDuration;
    sneakersMeshes.forEach(s => s.visible = true);
    showPowerupToast('SUPER SNEAKERS! JUMP HIGH');
  } else if (type === 'JETPACK') {
    GAME.powerups.jetpack.active = true;
    GAME.powerups.jetpack.timer = GAME.powerups.jetpack.maxDuration;
    Sound.startJetpackSound();
    showPowerupToast('JETPACK BLAST OFF!');

    // Spawn sky coin runs high in the air!
    const curZ = playerGroup.position.z;
    for (let s = 1; s <= 20; s++) {
      spawnCoin(GAME.lanes[Math.floor(Math.random() * 3)], 8.8, curZ - s * 6);
    }
  } else if (type === 'MULTIPLIER') {
    GAME.powerups.multiplier.active = true;
    GAME.powerups.multiplier.timer = GAME.powerups.multiplier.maxDuration;
    showPowerupToast('2X SCORE MULTIPLIER!');
  }

  spawnParticleBurst(playerGroup.position.x, playerGroup.position.y + 1, playerGroup.position.z, 0xfffa65, 18);
}

// Update Active Powerup Timers & Exhaust Effects
function updatePowerups(dt) {
  for (let key in GAME.powerups) {
    const p = GAME.powerups[key];
    if (p.active) {
      p.timer -= dt;

      if (key === 'jetpack') {
        // Emit smoke & flame particles from jetpack
        spawnJetpackFlame();
      }

      if (p.timer <= 0) {
        p.active = false;

        if (key === 'jetpack') {
          Sound.stopJetpackSound();
        } else if (key === 'sneakers') {
          sneakersMeshes.forEach(s => s.visible = false);
        } else if (key === 'hoverboard') {
          hoverboardMesh.visible = false;
        }
      }
    }
  }
}

// Crash / Game Over Trigger
function triggerCrash(reason) {
  // If hoverboard is active, it protects player from 1 death!
  if (GAME.powerups.hoverboard.active) {
    GAME.powerups.hoverboard.active = false;
    GAME.powerups.hoverboard.timer = 0;
    hoverboardMesh.visible = false;
    GAME.invulnerableTimer = 2.0; // 2 seconds safety invulnerability
    Sound.playShieldBreak();
    spawnParticleBurst(playerGroup.position.x, playerGroup.position.y + 0.5, playerGroup.position.z, 0x00d2d3, 30);
    showPowerupToast('HOVERBOARD SAVED YOU!');
    return;
  }

  // Otherwise: Game Over!
  GAME.active = false;
  Sound.stopBGM();
  Sound.stopJetpackSound();
  Sound.playCrash();
  setTimeout(() => Sound.playGameOver(), 400);

  // Inspector rushes in to catch runner!
  chaserGroup.position.z = playerGroup.position.z + 1.2;

  // Stumble player
  characterMeshes.bodyGroup.rotation.x = 1.4;
  characterMeshes.bodyGroup.position.y = -0.5;

  spawnParticleBurst(playerGroup.position.x, playerGroup.position.y + 0.5, playerGroup.position.z, 0xff4757, 25);

  showGameOverModal(reason);
}

// -------------------------------------------------------------
// CHASER (Inspector & Dog)
// -------------------------------------------------------------
function updateChaser(dt) {
  // Inspector runs smoothly behind the player
  const targetChaserX = playerGroup.position.x;
  chaserGroup.position.x += (targetChaserX - chaserGroup.position.x) * 8 * dt;

  // If player is running smoothly, inspector is ~7 units behind.
  // If player crashed, chaser was placed right behind.
  if (GAME.active) {
    chaserGroup.position.z = playerGroup.position.z + 7.2;
    chaserGroup.position.y = playerGroup.position.y;
  }
}

// -------------------------------------------------------------
// PARTICLE SYSTEMS & VISUAL POLISH
// -------------------------------------------------------------
function spawnParticleBurst(x, y, z, colorHex, count = 12) {
  const pGeo = new THREE.BoxGeometry(0.12, 0.12, 0.12);
  const pMat = new THREE.MeshBasicMaterial({ color: colorHex });

  for (let i = 0; i < count; i++) {
    const mesh = new THREE.Mesh(pGeo, pMat);
    mesh.position.set(x, y, z);
    scene.add(mesh);

    const theta = Math.random() * Math.PI * 2;
    const speed = 2 + Math.random() * 6;
    particles.push({
      mesh: mesh,
      vx: Math.cos(theta) * speed,
      vy: (Math.random() * 5 + 2),
      vz: Math.sin(theta) * speed,
      life: 0.5 + Math.random() * 0.3
    });
  }
}

function spawnJetpackFlame() {
  const pX = playerGroup.position.x;
  const pY = playerGroup.position.y;
  const pZ = playerGroup.position.z;

  const fGeo = new THREE.SphereGeometry(0.12, 6, 6);
  const fMat = new THREE.MeshBasicMaterial({
    color: Math.random() > 0.5 ? 0xff4757 : 0xffa502
  });

  [-0.16, 0.16].forEach(ox => {
    const flame = new THREE.Mesh(fGeo, fMat);
    flame.position.set(pX + ox, pY + 0.5, pZ + 0.35);
    scene.add(flame);

    particles.push({
      mesh: flame,
      vx: (Math.random() - 0.5) * 1.5,
      vy: -6 - Math.random() * 4,
      vz: 4 + Math.random() * 4,
      life: 0.25
    });
  });
}

function updateParticles(dt) {
  for (let i = particles.length - 1; i >= 0; i--) {
    const p = particles[i];
    p.life -= dt;

    p.mesh.position.x += p.vx * dt;
    p.mesh.position.y += p.vy * dt;
    p.mesh.position.z += p.vz * dt;
    p.vy -= 16 * dt; // Gravity on particles

    p.mesh.scale.multiplyScalar(0.92);

    if (p.life <= 0) {
      scene.remove(p.mesh);
      particles.splice(i, 1);
    }
  }
}

// Wind speed streaks in air
function createSpeedLines() {
  const lineGeo = new THREE.CylinderGeometry(0.02, 0.02, 3.5, 4);
  const lineMat = new THREE.MeshBasicMaterial({ color: 0xffffff, transparent: true, opacity: 0.35 });

  for (let i = 0; i < 30; i++) {
    const line = new THREE.Mesh(lineGeo, lineMat);
    line.rotation.x = Math.PI / 2;
    resetSpeedLine(line);
    scene.add(line);
    speedLines.push(line);
  }
}

function resetSpeedLine(line) {
  line.position.x = (Math.random() - 0.5) * 12;
  line.position.y = 1.0 + Math.random() * 8;
  line.position.z = -50 - Math.random() * 50;
}

function updateSpeedLines(dt) {
  speedLines.forEach(line => {
    line.position.z += (GAME.speed * 1.6) * dt;
    if (line.position.z > 15) {
      resetSpeedLine(line);
    }
  });
}

// Dynamic Camera Follow & Shake
function updateCamera(dt) {
  // Target position follows player with smooth spring
  const targetCamX = playerGroup.position.x * 0.45;
  const targetCamY = (GAME.powerups.jetpack.active ? 11.5 : 4.6) + playerGroup.position.y * 0.3;
  const targetCamZ = playerGroup.position.z + 7.8;

  camera.position.x += (targetCamX - camera.position.x) * 10 * dt;
  camera.position.y += (targetCamY - camera.position.y) * 8 * dt;
  camera.position.z = targetCamZ;

  // Camera look target
  const lookTargetY = (GAME.powerups.jetpack.active ? 8.5 : 2.0) + playerGroup.position.y * 0.2;
  camera.lookAt(targetCamX * 0.8, lookTargetY, playerGroup.position.z - 12);
}

// -------------------------------------------------------------
// UI SYNCHRONIZATION & TOASTS
// -------------------------------------------------------------
function updateHUD() {
  const scoreEl = document.getElementById('hud-score');
  const coinsEl = document.getElementById('hud-coins');
  const highEl = document.getElementById('hud-high');
  const multBadge = document.getElementById('multiplier-badge');

  if (scoreEl) scoreEl.textContent = GAME.score.toLocaleString();
  if (coinsEl) coinsEl.textContent = GAME.coins.toLocaleString();
  if (highEl) highEl.textContent = `TOP: ${GAME.highScore.toLocaleString()}`;

  const menuHighEl = document.getElementById('menu-high-score');
  if (menuHighEl) menuHighEl.textContent = GAME.highScore.toLocaleString();

  if (multBadge) {
    const totalMult = (GAME.powerups.multiplier.active ? 2 : 1) * GAME.multiplier;
    multBadge.textContent = `${totalMult}X`;
  }

  // Power-up HUD meters
  updatePowerupUI('magnet', GAME.powerups.magnet);
  updatePowerupUI('sneakers', GAME.powerups.sneakers);
  updatePowerupUI('jetpack', GAME.powerups.jetpack);
  updatePowerupUI('hoverboard', GAME.powerups.hoverboard);
}

function updatePowerupUI(name, powerup) {
  const bar = document.getElementById(`powerup-${name}`);
  if (!bar) return;

  if (powerup.active) {
    bar.style.display = 'flex';
    const percent = Math.max(0, (powerup.timer / powerup.maxDuration) * 100);
    const progress = bar.querySelector('.powerup-progress');
    if (progress) progress.style.width = `${percent}%`;
  } else {
    bar.style.display = 'none';
  }
}

function showPowerupToast(text) {
  const toast = document.getElementById('powerup-toast');
  if (!toast) return;
  toast.textContent = text;
  toast.classList.add('show');
  clearTimeout(toast.timer);
  toast.timer = setTimeout(() => {
    toast.classList.remove('show');
  }, 2200);
}

function showGameOverModal(reason) {
  const modal = document.getElementById('gameover-modal');
  const finalScore = document.getElementById('final-score');
  const finalCoins = document.getElementById('final-coins');
  const finalHigh = document.getElementById('final-high');
  const reasonText = document.getElementById('gameover-reason');

  if (modal) {
    if (finalScore) finalScore.textContent = GAME.score.toLocaleString();
    if (finalCoins) finalCoins.textContent = GAME.coins.toLocaleString();
    if (finalHigh) finalHigh.textContent = GAME.highScore.toLocaleString();
    if (reasonText) reasonText.textContent = reason;
    modal.classList.add('active');
  }
}

function togglePause() {
  if (!GAME.active) return;
  GAME.paused = !GAME.paused;

  const pauseModal = document.getElementById('pause-modal');
  if (pauseModal) {
    if (GAME.paused) {
      pauseModal.classList.add('active');
    } else {
      pauseModal.classList.remove('active');
      clock.start();
    }
  }
}

// Start / Restart Game
function startNewGame() {
  // Clear existing items in scene
  obstacles.forEach(o => scene.remove(o.mesh));
  coins.forEach(c => scene.remove(c.mesh));
  powerupItems.forEach(p => scene.remove(p.mesh));
  worldChunks.forEach(c => scene.remove(c));
  particles.forEach(p => scene.remove(p.mesh));

  obstacles = [];
  coins = [];
  powerupItems = [];
  worldChunks = [];
  particles = [];

  // Reset Game state
  GAME.active = true;
  GAME.paused = false;
  GAME.score = 0;
  GAME.coins = 0;
  GAME.speed = GAME.baseSpeed;
  GAME.distance = 0;
  GAME.currentLane = 1;
  GAME.targetX = GAME.lanes[1];
  GAME.playerY = 0;
  GAME.playerVY = 0;
  GAME.isGrounded = true;
  GAME.isJumping = false;
  GAME.isSliding = false;
  GAME.currentGroundY = 0;
  GAME.invulnerableTimer = 0;

  for (let key in GAME.powerups) {
    GAME.powerups[key].active = false;
    GAME.powerups[key].timer = 0;
  }

  hoverboardMesh.visible = false;
  jetpackMesh.visible = false;
  sneakersMeshes.forEach(s => s.visible = false);

  playerGroup.position.set(0, 0, 0);
  playerGroup.rotation.set(0, 0, 0);
  characterMeshes.bodyGroup.position.set(0, 0, 0);
  characterMeshes.bodyGroup.rotation.set(0, 0, 0);

  // Spawn fresh track chunks
  for (let i = 0; i < ACTIVE_CHUNKS; i++) {
    spawnTrackChunk(i * -CHUNK_SIZE);
  }

  // Hide modals with smooth exit transition
  const startOverlay = document.getElementById('start-overlay');
  const gameoverModal = document.getElementById('gameover-modal');
  const pauseModal = document.getElementById('pause-modal');

  if (startOverlay) {
    startOverlay.classList.add('exiting');
    setTimeout(() => {
      startOverlay.style.display = 'none';
      startOverlay.classList.remove('exiting');
    }, 320);
  }
  if (gameoverModal) gameoverModal.classList.remove('active');
  if (pauseModal) pauseModal.classList.remove('active');

  // Start sound & music
  Sound.init();
  Sound.playGameStart();
  setTimeout(() => {
    Sound.startBGM();
  }, 250);
}

function onWindowResize() {
  if (!camera || !renderer) return;
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
}

// Window load bootstrap
window.addEventListener('DOMContentLoaded', () => {
  initEngine();

  // Attach button click events
  document.getElementById('start-btn')?.addEventListener('click', () => {
    startNewGame();
  });

  document.getElementById('restart-btn')?.addEventListener('click', () => {
    startNewGame();
  });

  document.getElementById('resume-btn')?.addEventListener('click', () => {
    togglePause();
  });

  document.getElementById('audio-toggle-btn')?.addEventListener('click', () => {
    const muted = Sound.toggleMute();
    const btn = document.getElementById('audio-toggle-btn');
    if (btn) btn.textContent = muted ? '🔇' : '🔊';
  });

  document.getElementById('board-btn')?.addEventListener('click', () => {
    if (GAME.active && !GAME.paused) {
      activateHoverboard();
    }
  });

  // Touch on-screen control d-pad buttons
  document.getElementById('touch-left')?.addEventListener('click', () => handleMoveLeft());
  document.getElementById('touch-right')?.addEventListener('click', () => handleMoveRight());
  document.getElementById('touch-up')?.addEventListener('click', () => handleJump());
  document.getElementById('touch-down')?.addEventListener('click', () => handleSlide());
});
