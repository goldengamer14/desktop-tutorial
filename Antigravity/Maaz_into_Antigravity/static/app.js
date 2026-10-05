// Qwen Desktop Controller Frontend Logic

const chatMessages = document.getElementById('chatMessages');
const chatForm = document.getElementById('chatForm');
const chatInput = document.getElementById('chatInput');
const clearChatBtn = document.getElementById('clearChatBtn');
const appSearchInput = document.getElementById('appSearchInput');
const appsList = document.getElementById('appsList');
const procList = document.getElementById('procList');
const appsCountBadge = document.getElementById('appsCountBadge');
const totalAppsNum = document.getElementById('totalAppsNum');
const toastEl = document.getElementById('toast');

let allApps = [];

// Initialize on page load
document.addEventListener('DOMContentLoaded', () => {
  fetchStatus();
  fetchApps();
  fetchRunningApps();

  chatForm.addEventListener('submit', handleChatSubmit);
  clearChatBtn.addEventListener('click', handleClearChat);

  let searchTimeout = null;
  appSearchInput.addEventListener('input', (e) => {
    clearTimeout(searchTimeout);
    searchTimeout = setTimeout(() => {
      renderApps(e.target.value);
    }, 200);
  });
});

// Show Toast notification
function showToast(message, isError = false) {
  toastEl.textContent = message;
  toastEl.style.borderColor = isError ? 'var(--accent-rose)' : 'var(--accent-emerald)';
  toastEl.style.display = 'block';
  setTimeout(() => {
    toastEl.style.display = 'none';
  }, 3500);
}

// Fetch general system & model status
async function fetchStatus() {
  try {
    const res = await fetch('/api/status');
    const data = await res.json();
    if (data.status === 'ok') {
      document.getElementById('statusText').textContent = `Ollama (${data.model}) Ready`;
      totalAppsNum.textContent = data.total_apps || '--';
    }
  } catch (e) {
    document.getElementById('statusText').textContent = 'Ollama Disconnected';
    document.getElementById('statusIndicator').className = 'status-indicator offline';
  }
}

// Fetch installed applications
async function fetchApps() {
  try {
    const res = await fetch('/api/apps');
    const data = await res.json();
    allApps = data.apps || [];
    appsCountBadge.textContent = `${allApps.length} apps`;
    totalAppsNum.textContent = allApps.length;
    renderApps();
  } catch (e) {
    appsList.innerHTML = `<div class="loading-spinner">Failed to load apps: ${e.message}</div>`;
  }
}

// Render apps list filtered
function renderApps(filter = '') {
  const q = filter.trim().toLowerCase();
  const filtered = q
    ? allApps.filter(a => a.name.toLowerCase().includes(q))
    : allApps;

  if (filtered.length === 0) {
    appsList.innerHTML = `<div class="loading-spinner">No applications found.</div>`;
    return;
  }

  appsList.innerHTML = filtered.map(app => `
    <div class="app-item">
      <div class="app-item-info">
        <span class="app-item-name" title="${app.name}">${app.name}</span>
        <span class="app-item-type">${app.type}</span>
      </div>
      <button class="btn-launch-sm" onclick="directLaunch('${escapeStr(app.name)}')">Launch</button>
    </div>
  `).join('');
}

// Fetch running applications
async function fetchRunningApps() {
  try {
    procList.innerHTML = `<div class="loading-spinner">Refreshing processes...</div>`;
    const res = await fetch('/api/running');
    const data = await res.json();
    const procs = data.running || [];

    if (procs.length === 0) {
      procList.innerHTML = `<div class="loading-spinner">No active applications.</div>`;
      return;
    }

    procList.innerHTML = procs.map(p => `
      <div class="proc-item">
        <div class="proc-item-info">
          <span class="proc-name">${p.name}</span>
          <span class="proc-mem">${p.memory_mb} MB</span>
        </div>
        <button class="btn-kill-sm" onclick="directClose('${escapeStr(p.name)}')">Close</button>
      </div>
    `).join('');
  } catch (e) {
    procList.innerHTML = `<div class="loading-spinner">Error: ${e.message}</div>`;
  }
}

// Direct app launch
async function directLaunch(appName) {
  showToast(`Launching ${appName}...`);
  try {
    const res = await fetch('/api/launch', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ app_name: appName })
    });
    const data = await res.json();
    showToast(data.message, !data.success);
    setTimeout(fetchRunningApps, 1500);
  } catch (e) {
    showToast(`Error: ${e.message}`, true);
  }
}

// Direct app close
async function directClose(appName) {
  showToast(`Closing ${appName}...`);
  try {
    const res = await fetch('/api/close', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ app_name: appName })
    });
    const data = await res.json();
    showToast(data.message, !data.success);
    setTimeout(fetchRunningApps, 1000);
  } catch (e) {
    showToast(`Error: ${e.message}`, true);
  }
}

// Send suggested prompt chip
function sendSuggested(text) {
  chatInput.value = text;
  chatForm.dispatchEvent(new Event('submit'));
}

// Handle chat submit
async function handleChatSubmit(e) {
  e.preventDefault();
  const text = chatInput.value.trim();
  if (!text) return;

  // Add user bubble
  appendMessage('user', text);
  chatInput.value = '';

  // Show loading indicator
  const loadingBubble = appendMessage('assistant', '<span class="loading-spinner">🤖 Qwen is thinking and checking tools...</span>');

  try {
    const res = await fetch('/api/chat', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ prompt: text })
    });
    const data = await res.json();

    // Remove loading bubble
    loadingBubble.remove();

    if (data.error) {
      appendMessage('assistant', `<p style="color: var(--accent-rose)">⚠️ Error: ${data.error}</p>`);
      return;
    }

    // Format assistant response with tool cards & thinking
    let contentHtml = '';

    if (data.thinking) {
      contentHtml += `
        <details class="thinking-box">
          <summary>🧠 Model Reasoning (${Math.min(data.thinking.length, 120)} chars)</summary>
          <pre style="margin-top: 6px; white-space: pre-wrap;">${escapeHtml(data.thinking)}</pre>
        </details>
      `;
    }

    if (data.tool_calls && data.tool_calls.length > 0) {
      data.tool_calls.forEach(tc => {
        const argsStr = Object.entries(tc.arguments || {}).map(([k, v]) => `${k}="${v}"`).join(', ');
        contentHtml += `
          <div class="tool-card">
            <div class="tool-header">⚡ Tool: ${tc.name}(${escapeHtml(argsStr)})</div>
            <div class="tool-result">↳ ${escapeHtml(tc.result)}</div>
          </div>
        `;
      });
      // Refresh active processes if an app was opened or closed
      setTimeout(fetchRunningApps, 1500);
    }

    if (data.response) {
      contentHtml += `<p style="margin-top: 8px;">${escapeHtml(data.response).replace(/\n/g, '<br>')}</p>`;
    }

    appendMessage('assistant', contentHtml);

  } catch (err) {
    loadingBubble.remove();
    appendMessage('assistant', `<p style="color: var(--accent-rose)">⚠️ Connection failed: ${err.message}</p>`);
  }
}

// Append message element
function appendMessage(role, htmlContent) {
  const msgDiv = document.createElement('div');
  msgDiv.className = `message ${role}`;
  msgDiv.innerHTML = `
    <div class="avatar">${role === 'user' ? '👤' : '🤖'}</div>
    <div class="msg-bubble">${htmlContent}</div>
  `;
  chatMessages.appendChild(msgDiv);
  chatMessages.scrollTop = chatMessages.scrollHeight;
  return msgDiv;
}

// Clear chat context
async function handleClearChat() {
  try {
    await fetch('/api/clear', { method: 'POST' });
    chatMessages.innerHTML = `
      <div class="message assistant">
        <div class="avatar">🤖</div>
        <div class="msg-bubble">
          <p>Context cleared. Ready for your next Windows system command!</p>
        </div>
      </div>
    `;
    showToast('Context reset.');
  } catch (e) {
    showToast(`Error: ${e.message}`, true);
  }
}

function escapeHtml(str) {
  return String(str)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#039;');
}

function escapeStr(str) {
  return String(str).replace(/'/g, "\\'");
}
