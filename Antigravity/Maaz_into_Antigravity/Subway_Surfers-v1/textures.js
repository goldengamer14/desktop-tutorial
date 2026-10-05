/**
 * Procedural Texture Generator for Subway Surfers 3D
 * Generates rich, crisp canvas textures for trains, graffiti walls, tracks, and signs.
 */

// Canvas roundRect polyfill for universal browser compatibility
if (typeof CanvasRenderingContext2D !== 'undefined' && !CanvasRenderingContext2D.prototype.roundRect) {
  CanvasRenderingContext2D.prototype.roundRect = function (x, y, w, h, r = 0) {
    if (typeof r === 'number') r = [r, r, r, r];
    const radius = Math.min(r[0] || 0, w / 2, h / 2);
    this.moveTo(x + radius, y);
    this.arcTo(x + w, y, x + w, y + h, radius);
    this.arcTo(x + w, y + h, x, y + h, radius);
    this.arcTo(x, y + h, x, y, radius);
    this.arcTo(x, y, x + w, y, radius);
    this.closePath();
    return this;
  };
}

const TextureGenerator = {
  // Generate asphalt / gravel ballast texture
  createTrackGroundTexture() {
    const canvas = document.createElement('canvas');
    canvas.width = 512;
    canvas.height = 512;
    const ctx = canvas.getContext('2d');

    // Base gravel dark tone
    ctx.fillStyle = '#222228';
    ctx.fillRect(0, 0, 512, 512);

    // Speckles of ballast stones
    for (let i = 0; i < 6000; i++) {
      const x = Math.random() * 512;
      const y = Math.random() * 512;
      const shade = Math.floor(35 + Math.random() * 45);
      const alpha = 0.3 + Math.random() * 0.4;
      ctx.fillStyle = `rgba(${shade}, ${shade + 5}, ${shade + 10}, ${alpha})`;
      ctx.fillRect(x, y, 2 + Math.random() * 3, 2 + Math.random() * 3);
    }

    // Lane separators / subtle track dirt trails
    ctx.fillStyle = 'rgba(10, 10, 15, 0.4)';
    ctx.fillRect(80, 0, 350, 512);

    const texture = new THREE.CanvasTexture(canvas);
    texture.wrapS = THREE.RepeatWrapping;
    texture.wrapT = THREE.RepeatWrapping;
    return texture;
  },

  // Generate railway ties (wooden sleepers) and metallic rails
  createRailsTexture() {
    const canvas = document.createElement('canvas');
    canvas.width = 256;
    canvas.height = 256;
    const ctx = canvas.getContext('2d');

    ctx.fillStyle = '#1c1b22';
    ctx.fillRect(0, 0, 256, 256);

    // Sleepers (wooden planks across track)
    for (let y = 16; y < 256; y += 48) {
      // Wood plank
      ctx.fillStyle = '#4a3525';
      ctx.fillRect(10, y, 236, 24);

      // Wood grain / highlight
      ctx.fillStyle = '#5c4330';
      ctx.fillRect(12, y + 2, 232, 6);
      ctx.fillStyle = '#322316';
      ctx.fillRect(12, y + 18, 232, 6);

      // Bolts / fasteners
      ctx.fillStyle = '#888899';
      ctx.fillRect(40, y + 8, 8, 8);
      ctx.fillRect(208, y + 8, 8, 8);
    }

    // Steel rails
    const railGradientL = ctx.createLinearGradient(35, 0, 55, 0);
    railGradientL.addColorStop(0, '#555566');
    railGradientL.addColorStop(0.5, '#e0e5ff');
    railGradientL.addColorStop(1, '#333344');
    ctx.fillStyle = railGradientL;
    ctx.fillRect(38, 0, 12, 256);

    const railGradientR = ctx.createLinearGradient(200, 0, 220, 0);
    railGradientR.addColorStop(0, '#555566');
    railGradientR.addColorStop(0.5, '#e0e5ff');
    railGradientR.addColorStop(1, '#333344');
    ctx.fillStyle = railGradientR;
    ctx.fillRect(206, 0, 12, 256);

    const texture = new THREE.CanvasTexture(canvas);
    texture.wrapS = THREE.RepeatWrapping;
    texture.wrapT = THREE.RepeatWrapping;
    return texture;
  },

  // Generate authentic Subway Surfers graffiti wall texture
  createGraffitiWallTexture(themeIndex = 0) {
    const canvas = document.createElement('canvas');
    canvas.width = 1024;
    canvas.height = 512;
    const ctx = canvas.getContext('2d');

    // Concrete wall base with brick texture
    const wallColors = ['#9e3d2b', '#2b4e7a', '#78357a', '#2d6d4b'];
    ctx.fillStyle = '#b09b8c';
    ctx.fillRect(0, 0, 1024, 512);

    // Brick rows
    ctx.strokeStyle = 'rgba(70, 50, 40, 0.4)';
    ctx.lineWidth = 3;
    const brickH = 32;
    const brickW = 64;
    for (let y = 0; y < 512; y += brickH) {
      ctx.beginPath();
      ctx.moveTo(0, y);
      ctx.lineTo(1024, y);
      ctx.stroke();

      const offset = (y / brickH) % 2 === 0 ? 0 : brickW / 2;
      for (let x = offset; x < 1024; x += brickW) {
        ctx.beginPath();
        ctx.moveTo(x, y);
        ctx.lineTo(x, y + brickH);
        ctx.stroke();
      }
    }

    // Dirt & grunge gradients
    const grunge = ctx.createLinearGradient(0, 0, 0, 512);
    grunge.addColorStop(0, 'rgba(30, 25, 20, 0.6)');
    grunge.addColorStop(0.3, 'rgba(0, 0, 0, 0)');
    grunge.addColorStop(0.85, 'rgba(0, 0, 0, 0)');
    grunge.addColorStop(1, 'rgba(20, 15, 10, 0.8)');
    ctx.fillStyle = grunge;
    ctx.fillRect(0, 0, 1024, 512);

    // Wild colorful graffiti tags & art!
    const tags = [
      { text: 'SURF', x: 80, y: 320, size: 120, col: '#ff0055', stroke: '#ffee00', angle: -0.05 },
      { text: 'SUBWAY', x: 440, y: 220, size: 100, col: '#00e5ff', stroke: '#ff00aa', angle: 0.04 },
      { text: 'DASH', x: 260, y: 440, size: 90, col: '#ffea00', stroke: '#ff3300', angle: -0.02 },
      { text: 'RUN!', x: 740, y: 380, size: 130, col: '#76ff03', stroke: '#0066ff', angle: 0.08 },
      { text: 'BOOM', x: 120, y: 150, size: 80, col: '#ff9100', stroke: '#ffffff', angle: -0.08 },
    ];

    tags.forEach(tag => {
      ctx.save();
      ctx.translate(tag.x, tag.y);
      ctx.rotate(tag.angle);

      // Drop shadow for graffiti 3D pop
      ctx.font = `900 ${tag.size}px "Impact", "Arial Black", sans-serif`;
      ctx.fillStyle = 'rgba(0, 0, 0, 0.7)';
      ctx.fillText(tag.text, 12, 12);

      // Outer outline
      ctx.lineWidth = 18;
      ctx.strokeStyle = '#111111';
      ctx.strokeText(tag.text, 0, 0);

      // Inner outline
      ctx.lineWidth = 10;
      ctx.strokeStyle = tag.stroke;
      ctx.strokeText(tag.text, 0, 0);

      // Main fill
      ctx.fillStyle = tag.col;
      ctx.fillText(tag.text, 0, 0);

      // Highlights
      ctx.lineWidth = 3;
      ctx.strokeStyle = '#ffffff';
      ctx.strokeText(tag.text, -2, -2);

      ctx.restore();
    });

    // Splatters & Drips
    const splatterColors = ['#ff0055', '#00e5ff', '#ffea00', '#76ff03', '#ffffff'];
    for (let s = 0; s < 45; s++) {
      const sx = Math.random() * 1024;
      const sy = 100 + Math.random() * 380;
      const rad = 4 + Math.random() * 12;
      const color = splatterColors[s % splatterColors.length];

      ctx.fillStyle = color;
      ctx.beginPath();
      ctx.arc(sx, sy, rad, 0, Math.PI * 2);
      ctx.fill();

      // Drip trail
      if (Math.random() > 0.4) {
        const dripLen = 20 + Math.random() * 60;
        ctx.fillRect(sx - rad * 0.4, sy, rad * 0.8, dripLen);
        ctx.beginPath();
        ctx.arc(sx, sy + dripLen, rad * 0.6, 0, Math.PI * 2);
        ctx.fill();
      }
    }

    const texture = new THREE.CanvasTexture(canvas);
    return texture;
  },

  // Subway train carriage side livery texture
  createTrainSideTexture(trainColor = '#d32f2f') {
    const canvas = document.createElement('canvas');
    canvas.width = 1024;
    canvas.height = 256;
    const ctx = canvas.getContext('2d');

    // Metallic body color
    ctx.fillStyle = '#e8ecf2';
    ctx.fillRect(0, 0, 1024, 256);

    // Distinct Subway Surfers bottom accent stripe
    ctx.fillStyle = trainColor;
    ctx.fillRect(0, 160, 1024, 76);

    // Accent line
    ctx.fillStyle = '#ffd600';
    ctx.fillRect(0, 150, 1024, 10);

    // Dark train roof trim
    ctx.fillStyle = '#2f3542';
    ctx.fillRect(0, 0, 1024, 24);

    // Passenger Windows
    const windowCount = 5;
    const winWidth = 110;
    const winHeight = 80;
    const winY = 48;
    const spacing = 190;

    for (let i = 0; i < windowCount; i++) {
      const x = 50 + i * spacing;

      // Window rubber seal frame
      ctx.fillStyle = '#1e272e';
      ctx.beginPath();
      ctx.roundRect(x - 4, winY - 4, winWidth + 8, winHeight + 8, 8);
      ctx.fill();

      // Tinted window glass with reflection
      const winGrad = ctx.createLinearGradient(x, winY, x + winWidth, winY + winHeight);
      winGrad.addColorStop(0, '#1a3b5c');
      winGrad.addColorStop(0.4, '#487eb0');
      winGrad.addColorStop(0.7, '#81ecec');
      winGrad.addColorStop(1, '#0c2461');
      ctx.fillStyle = winGrad;
      ctx.beginPath();
      ctx.roundRect(x, winY, winWidth, winHeight, 6);
      ctx.fill();

      // Glass shine highlight slash
      ctx.strokeStyle = 'rgba(255, 255, 255, 0.45)';
      ctx.lineWidth = 6;
      ctx.beginPath();
      ctx.moveTo(x + 20, winY + winHeight - 10);
      ctx.lineTo(x + winWidth - 20, winY + 10);
      ctx.stroke();

      // Train door separator between windows
      if (i === 1 || i === 3) {
        ctx.fillStyle = '#57606f';
        ctx.fillRect(x + winWidth + 30, 30, 4, 206);
        ctx.fillRect(x + winWidth + 55, 30, 4, 206);
      }
    }

    // Train number / subway logo
    ctx.font = 'bold 36px "Arial Black", sans-serif';
    ctx.fillStyle = '#ffffff';
    ctx.fillText('METRO 7', 80, 210);

    const texture = new THREE.CanvasTexture(canvas);
    return texture;
  },

  // Subway train front grill and windshield texture
  createTrainFrontTexture(trainColor = '#d32f2f') {
    const canvas = document.createElement('canvas');
    canvas.width = 512;
    canvas.height = 512;
    const ctx = canvas.getContext('2d');

    // Train front base
    ctx.fillStyle = trainColor;
    ctx.fillRect(0, 0, 512, 512);

    // Windshield
    ctx.fillStyle = '#1e272e';
    ctx.beginPath();
    ctx.roundRect(40, 60, 432, 190, 20);
    ctx.fill();

    const glassGrad = ctx.createLinearGradient(40, 60, 472, 250);
    glassGrad.addColorStop(0, '#22527b');
    glassGrad.addColorStop(0.5, '#74b9ff');
    glassGrad.addColorStop(1, '#0984e3');
    ctx.fillStyle = glassGrad;
    ctx.beginPath();
    ctx.roundRect(50, 70, 412, 170, 16);
    ctx.fill();

    // Reflection slash
    ctx.strokeStyle = 'rgba(255, 255, 255, 0.5)';
    ctx.lineWidth = 14;
    ctx.beginPath();
    ctx.moveTo(100, 220);
    ctx.lineTo(380, 80);
    ctx.stroke();

    // Twin High-beam headlights
    // Left headlight
    ctx.fillStyle = '#ffffff';
    ctx.beginPath();
    ctx.arc(100, 370, 40, 0, Math.PI * 2);
    ctx.fill();
    ctx.strokeStyle = '#ffd32a';
    ctx.lineWidth = 8;
    ctx.stroke();

    // Right headlight
    ctx.beginPath();
    ctx.arc(412, 370, 40, 0, Math.PI * 2);
    ctx.fill();
    ctx.stroke();

    // Grill / Cowcatcher
    ctx.fillStyle = '#2f3542';
    ctx.fillRect(160, 340, 192, 120);
    for (let gy = 355; gy < 450; gy += 18) {
      ctx.fillStyle = '#11141a';
      ctx.fillRect(170, gy, 172, 8);
    }

    // Hazard warning stripes along bumper
    for (let bx = 0; bx < 512; bx += 40) {
      ctx.fillStyle = '#ffd32a';
      ctx.beginPath();
      ctx.moveTo(bx, 480);
      ctx.lineTo(bx + 20, 480);
      ctx.lineTo(bx - 10, 512);
      ctx.lineTo(bx - 30, 512);
      ctx.fill();

      ctx.fillStyle = '#1e272e';
      ctx.beginPath();
      ctx.moveTo(bx + 20, 480);
      ctx.lineTo(bx + 40, 480);
      ctx.lineTo(bx + 10, 512);
      ctx.lineTo(bx - 10, 512);
      ctx.fill();
    }

    const texture = new THREE.CanvasTexture(canvas);
    return texture;
  },

  // Hazard striped barricade texture (Yellow & Black diagonal warning)
  createHazardStripeTexture() {
    const canvas = document.createElement('canvas');
    canvas.width = 256;
    canvas.height = 64;
    const ctx = canvas.getContext('2d');

    ctx.fillStyle = '#ffd32a';
    ctx.fillRect(0, 0, 256, 64);

    ctx.fillStyle = '#1e272e';
    for (let x = -64; x < 256; x += 48) {
      ctx.beginPath();
      ctx.moveTo(x, 0);
      ctx.lineTo(x + 32, 0);
      ctx.lineTo(x + 64, 64);
      ctx.lineTo(x + 32, 64);
      ctx.fill();
    }

    const texture = new THREE.CanvasTexture(canvas);
    texture.wrapS = THREE.RepeatWrapping;
    texture.wrapT = THREE.RepeatWrapping;
    return texture;
  },

  // Red & White overhead clearance barrier texture
  createClearanceStripeTexture() {
    const canvas = document.createElement('canvas');
    canvas.width = 256;
    canvas.height = 64;
    const ctx = canvas.getContext('2d');

    ctx.fillStyle = '#ff3838';
    ctx.fillRect(0, 0, 256, 64);

    ctx.fillStyle = '#ffffff';
    for (let x = -64; x < 256; x += 48) {
      ctx.beginPath();
      ctx.moveTo(x, 0);
      ctx.lineTo(x + 28, 0);
      ctx.lineTo(x + 56, 64);
      ctx.lineTo(x + 28, 64);
      ctx.fill();
    }

    const texture = new THREE.CanvasTexture(canvas);
    texture.wrapS = THREE.RepeatWrapping;
    texture.wrapT = THREE.RepeatWrapping;
    return texture;
  },

  // Embossed gold coin face texture
  createCoinTexture() {
    const canvas = document.createElement('canvas');
    canvas.width = 128;
    canvas.height = 128;
    const ctx = canvas.getContext('2d');

    // Outer rim
    const grad = ctx.createRadialGradient(64, 64, 20, 64, 64, 64);
    grad.addColorStop(0, '#fff176');
    grad.addColorStop(0.6, '#ffd600');
    grad.addColorStop(0.9, '#ff9100');
    grad.addColorStop(1, '#ff6d00');

    ctx.fillStyle = grad;
    ctx.beginPath();
    ctx.arc(64, 64, 62, 0, Math.PI * 2);
    ctx.fill();

    // Bevel circle
    ctx.strokeStyle = '#fff9c4';
    ctx.lineWidth = 6;
    ctx.beginPath();
    ctx.arc(64, 64, 52, 0, Math.PI * 2);
    ctx.stroke();

    // Star / Subway Token emblem in center
    ctx.fillStyle = '#ff6d00';
    ctx.font = '900 68px "Arial Black", sans-serif';
    ctx.textAlign = 'center';
    ctx.textBaseline = 'middle';
    ctx.fillText('★', 64, 66);

    ctx.fillStyle = '#ffffff';
    ctx.fillText('★', 63, 63);

    const texture = new THREE.CanvasTexture(canvas);
    return texture;
  }
};
