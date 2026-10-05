/**
 * Web Audio API Sound Synthesizer & Music Engine for Subway Surfers 3D
 * Generates all sound effects & background music procedurally with zero external assets.
 */

class SoundEngine {
  constructor() {
    this.ctx = null;
    this.muted = false;
    this.bgmPlaying = false;
    this.bgmInterval = null;
    this.comboCount = 0;
    this.comboTimer = null;
    this.jetpackOsc = null;
    this.jetpackGain = null;
  }

  init() {
    if (!this.ctx) {
      const AudioCtx = window.AudioContext || window.webkitAudioContext;
      if (AudioCtx) {
        this.ctx = new AudioCtx();
      }
    }
    if (this.ctx && this.ctx.state === 'suspended') {
      this.ctx.resume();
    }
  }

  toggleMute() {
    this.muted = !this.muted;
    if (this.muted) {
      this.stopBGM();
      this.stopJetpackSound();
    } else {
      this.init();
      if (!this.bgmPlaying) {
        this.startBGM();
      }
    }
    return this.muted;
  }

  // --- SOUND EFFECTS ---

  playJump() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sine';
    osc.frequency.setValueAtTime(150, t);
    osc.frequency.exponentialRampToValueAtTime(620, t + 0.16);

    gain.gain.setValueAtTime(0.35, t);
    gain.gain.linearRampToValueAtTime(0.01, t + 0.22);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.22);
  }

  playSuperJump() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'triangle';
    osc.frequency.setValueAtTime(200, t);
    osc.frequency.exponentialRampToValueAtTime(950, t + 0.3);

    gain.gain.setValueAtTime(0.4, t);
    gain.gain.linearRampToValueAtTime(0.01, t + 0.35);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.35);
  }

  playSlide() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    // White noise swoosh
    const bufferSize = this.ctx.sampleRate * 0.25;
    const buffer = this.ctx.createBuffer(1, bufferSize, this.ctx.sampleRate);
    const data = buffer.getChannelData(0);
    for (let i = 0; i < bufferSize; i++) {
      data[i] = Math.random() * 2 - 1;
    }

    const noise = this.ctx.createBufferSource();
    noise.buffer = buffer;

    const filter = this.ctx.createBiquadFilter();
    filter.type = 'bandpass';
    filter.frequency.setValueAtTime(800, t);
    filter.frequency.exponentialRampToValueAtTime(220, t + 0.25);
    filter.Q.value = 3.0;

    const gain = this.ctx.createGain();
    gain.gain.setValueAtTime(0.3, t);
    gain.gain.linearRampToValueAtTime(0.01, t + 0.25);

    noise.connect(filter);
    filter.connect(gain);
    gain.connect(this.ctx.destination);

    noise.start(t);
  }

  playLaneSwitch() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sine';
    osc.frequency.setValueAtTime(320, t);
    osc.frequency.exponentialRampToValueAtTime(480, t + 0.08);

    gain.gain.setValueAtTime(0.18, t);
    gain.gain.linearRampToValueAtTime(0.01, t + 0.1);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.1);
  }

  playCoin() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    this.comboCount = (this.comboCount + 1) % 8;
    clearTimeout(this.comboTimer);
    this.comboTimer = setTimeout(() => {
      this.comboCount = 0;
    }, 1200);

    const baseFreqs = [987.77, 1174.66, 1318.51, 1567.98, 1760.00, 1975.53, 2349.32, 2637.02];
    const freq = baseFreqs[this.comboCount];

    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sine';
    osc.frequency.setValueAtTime(freq, t);
    osc.frequency.setValueAtTime(freq * 1.5, t + 0.05);

    gain.gain.setValueAtTime(0.22, t);
    gain.gain.exponentialRampToValueAtTime(0.001, t + 0.18);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.18);
  }

  playPowerup() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    const notes = [440, 554.37, 659.25, 880];
    notes.forEach((freq, idx) => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();

      osc.type = 'triangle';
      osc.frequency.setValueAtTime(freq, t + idx * 0.07);

      gain.gain.setValueAtTime(0, t + idx * 0.07);
      gain.gain.linearRampToValueAtTime(0.25, t + idx * 0.07 + 0.02);
      gain.gain.exponentialRampToValueAtTime(0.01, t + idx * 0.07 + 0.18);

      osc.connect(gain);
      gain.connect(this.ctx.destination);

      osc.start(t + idx * 0.07);
      osc.stop(t + idx * 0.07 + 0.2);
    });
  }

  playShieldBreak() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'sawtooth';
    osc.frequency.setValueAtTime(700, t);
    osc.frequency.exponentialRampToValueAtTime(100, t + 0.35);

    gain.gain.setValueAtTime(0.35, t);
    gain.gain.linearRampToValueAtTime(0.01, t + 0.35);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.35);
  }

  playCrash() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    // Low thud
    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = 'triangle';
    osc.frequency.setValueAtTime(160, t);
    osc.frequency.exponentialRampToValueAtTime(30, t + 0.4);

    gain.gain.setValueAtTime(0.6, t);
    gain.gain.linearRampToValueAtTime(0.01, t + 0.45);

    osc.connect(gain);
    gain.connect(this.ctx.destination);

    osc.start(t);
    osc.stop(t + 0.45);

    // Crash noise crunch
    const bufferSize = this.ctx.sampleRate * 0.3;
    const buffer = this.ctx.createBuffer(1, bufferSize, this.ctx.sampleRate);
    const data = buffer.getChannelData(0);
    for (let i = 0; i < bufferSize; i++) {
      data[i] = (Math.random() * 2 - 1) * Math.exp(-i / (bufferSize * 0.3));
    }
    const noise = this.ctx.createBufferSource();
    noise.buffer = buffer;
    const noiseGain = this.ctx.createGain();
    noiseGain.gain.setValueAtTime(0.5, t);
    noiseGain.gain.linearRampToValueAtTime(0.01, t + 0.35);
    noise.connect(noiseGain);
    noiseGain.connect(this.ctx.destination);
    noise.start(t);
  }

  playGameOver() {
    if (this.muted) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    const notes = [440, 415.3, 392, 349.23];
    notes.forEach((freq, idx) => {
      const osc = this.ctx.createOscillator();
      const gain = this.ctx.createGain();

      osc.type = 'sawtooth';
      osc.frequency.setValueAtTime(freq, t + idx * 0.18);

      gain.gain.setValueAtTime(0.3, t + idx * 0.18);
      gain.gain.exponentialRampToValueAtTime(0.01, t + idx * 0.18 + 0.28);

      osc.connect(gain);
      gain.connect(this.ctx.destination);

      osc.start(t + idx * 0.18);
      osc.stop(t + idx * 0.18 + 0.3);
    });
  }

  startJetpackSound() {
    if (this.muted || this.jetpackOsc) return;
    this.init();
    if (!this.ctx) return;

    const t = this.ctx.currentTime;
    this.jetpackOsc = this.ctx.createOscillator();
    this.jetpackGain = this.ctx.createGain();

    this.jetpackOsc.type = 'sawtooth';
    this.jetpackOsc.frequency.setValueAtTime(110, t);

    // Filter to give it an engine roar
    const filter = this.ctx.createBiquadFilter();
    filter.type = 'lowpass';
    filter.frequency.value = 400;

    this.jetpackGain.gain.setValueAtTime(0.01, t);
    this.jetpackGain.gain.linearRampToValueAtTime(0.2, t + 0.2);

    this.jetpackOsc.connect(filter);
    filter.connect(this.jetpackGain);
    this.jetpackGain.connect(this.ctx.destination);

    this.jetpackOsc.start();
  }

  stopJetpackSound() {
    if (this.jetpackOsc && this.jetpackGain && this.ctx) {
      const t = this.ctx.currentTime;
      this.jetpackGain.gain.linearRampToValueAtTime(0.001, t + 0.2);
      setTimeout(() => {
        try {
          if (this.jetpackOsc) {
            this.jetpackOsc.stop();
            this.jetpackOsc.disconnect();
            this.jetpackOsc = null;
          }
        } catch (e) {}
      }, 250);
    }
  }

  // --- BACKGROUND MUSIC ENGINE (Catchy Subway Groove) ---

  startBGM() {
    if (this.bgmPlaying || this.muted) return;
    this.init();
    if (!this.ctx) return;

    this.bgmPlaying = true;
    let step = 0;
    const tempo = 136; // BPM
    const stepDuration = 60 / tempo / 4; // 16th notes

    // Funky bassline notes (MIDI pitch or Hz)
    // Scale: D Dorian (D, E, F, G, A, B, C)
    const bassline = [
      146.83, 0, 146.83, 0, 174.61, 0, 196.00, 0,
      146.83, 0, 220.00, 0, 196.00, 174.61, 146.83, 0,
      130.81, 0, 130.81, 0, 164.81, 0, 174.61, 0,
      130.81, 0, 196.00, 0, 174.61, 164.81, 130.81, 0
    ];

    const leadMelody = [
      587.33, 0, 659.25, 0, 698.46, 0, 880.00, 783.99,
      698.46, 0, 587.33, 0, 440.00, 587.33, 0, 0,
      523.25, 0, 659.25, 0, 783.99, 0, 698.46, 659.25,
      587.33, 659.25, 698.46, 783.99, 880.00, 0, 0, 0
    ];

    const playStep = () => {
      if (!this.bgmPlaying || !this.ctx || this.muted) return;

      const t = this.ctx.currentTime;
      const bIndex = step % bassline.length;
      const bFreq = bassline[bIndex];

      // Bass synth
      if (bFreq > 0) {
        const bOsc = this.ctx.createOscillator();
        const bGain = this.ctx.createGain();
        bOsc.type = 'triangle';
        bOsc.frequency.setValueAtTime(bFreq, t);

        bGain.gain.setValueAtTime(0.2, t);
        bGain.gain.exponentialRampToValueAtTime(0.01, t + stepDuration * 1.8);

        bOsc.connect(bGain);
        bGain.connect(this.ctx.destination);
        bOsc.start(t);
        bOsc.stop(t + stepDuration * 2);
      }

      // Lead melody
      const mFreq = leadMelody[bIndex];
      if (mFreq > 0) {
        const mOsc = this.ctx.createOscillator();
        const mGain = this.ctx.createGain();
        mOsc.type = 'sawtooth';
        mOsc.frequency.setValueAtTime(mFreq, t);

        const filter = this.ctx.createBiquadFilter();
        filter.type = 'lowpass';
        filter.frequency.setValueAtTime(1400, t);

        mGain.gain.setValueAtTime(0.08, t);
        mGain.gain.exponentialRampToValueAtTime(0.005, t + stepDuration * 1.5);

        mOsc.connect(filter);
        filter.connect(mGain);
        mGain.connect(this.ctx.destination);
        mOsc.start(t);
        mOsc.stop(t + stepDuration * 1.6);
      }

      // Drum beat (Kick on 0, 4, 8, 12, Hi-hat on every 2, Snare on 4, 12)
      const subStep = step % 16;
      if (subStep % 4 === 0) {
        // Kick drum
        const kOsc = this.ctx.createOscillator();
        const kGain = this.ctx.createGain();
        kOsc.frequency.setValueAtTime(120, t);
        kOsc.frequency.exponentialRampToValueAtTime(35, t + 0.1);
        kGain.gain.setValueAtTime(0.25, t);
        kGain.gain.linearRampToValueAtTime(0.01, t + 0.12);
        kOsc.connect(kGain);
        kGain.connect(this.ctx.destination);
        kOsc.start(t);
        kOsc.stop(t + 0.12);
      }

      if (subStep === 4 || subStep === 12) {
        // Snare drum (noise burst)
        const snBuffer = this.ctx.createBuffer(1, this.ctx.sampleRate * 0.1, this.ctx.sampleRate);
        const snData = snBuffer.getChannelData(0);
        for (let i = 0; i < snData.length; i++) snData[i] = Math.random() * 2 - 1;
        const snSource = this.ctx.createBufferSource();
        snSource.buffer = snBuffer;
        const snGain = this.ctx.createGain();
        snGain.gain.setValueAtTime(0.12, t);
        snGain.gain.linearRampToValueAtTime(0.01, t + 0.1);
        snSource.connect(snGain);
        snGain.connect(this.ctx.destination);
        snSource.start(t);
      }

      if (subStep % 2 === 0) {
        // Hi-hat
        const hhBuffer = this.ctx.createBuffer(1, this.ctx.sampleRate * 0.03, this.ctx.sampleRate);
        const hhData = hhBuffer.getChannelData(0);
        for (let i = 0; i < hhData.length; i++) hhData[i] = Math.random() * 2 - 1;
        const hhSource = this.ctx.createBufferSource();
        hhSource.buffer = hhBuffer;
        const hhGain = this.ctx.createGain();
        hhGain.gain.setValueAtTime(0.05, t);
        hhGain.gain.linearRampToValueAtTime(0.001, t + 0.03);
        hhSource.connect(hhGain);
        hhGain.connect(this.ctx.destination);
        hhSource.start(t);
      }

      step++;
    };

    this.bgmInterval = setInterval(playStep, stepDuration * 1000);
  }

  stopBGM() {
    this.bgmPlaying = false;
    if (this.bgmInterval) {
      clearInterval(this.bgmInterval);
      this.bgmInterval = null;
    }
  }
}

const Sound = new SoundEngine();
