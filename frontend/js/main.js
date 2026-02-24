// Smart API Discovery: Automatically detects if running locally or in GitHub Codespaces
const API_BASE = (window.location.hostname === '127.0.0.1' || window.location.hostname === 'localhost') 
    ? 'http://127.0.0.1:18080/api' 
    : `https://${window.location.hostname.replace('content', '18080')}/api`;

console.log("Connecting to PerfectlyImperfect Backend at:", API_BASE);
let currentUser = null;

// Page Navigation
function showPage(pageId) {
    document.querySelectorAll('main > section').forEach(section => {
        section.classList.add('hidden');
    });

    // Reset registration view if navigating to it
    if (pageId === 'auth-register') {
        document.getElementById('reg-card').classList.remove('hidden');
        document.getElementById('reg-success').classList.add('hidden');
    }

    const target = document.getElementById(pageId);
    if (target) {
        target.classList.remove('hidden');
    }

    if (pageId === 'coaches') loadCoaches();
    if (pageId === 'dashboard' && currentUser) updateDashboardUI();
    if (pageId !== 'breathing') stopBreathing();
}

// Authentication
document.getElementById('register-form').addEventListener('submit', async (e) => {
    e.preventDefault();
    const username = document.getElementById('reg-user').value;
    const password = document.getElementById('reg-pass').value;
    const email = document.getElementById('reg-email').value;

    try {
        const res = await fetch(`${API_BASE}/register`, {
            method: 'POST',
            mode: 'cors',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ username, password, email })
        });
        if (res.ok) {
            // Show Success Box instead of immediate alert
            document.getElementById('reg-card').classList.add('hidden');
            document.getElementById('reg-success').classList.remove('hidden');
        } else {
            const err = await res.text();
            alert('Error: ' + err);
        }
    } catch (err) {
        alert('Server connection failed. Ensure backend is running.');
    }
});

document.getElementById('login-form').addEventListener('submit', async (e) => {
    e.preventDefault();
    const username = document.getElementById('login-user').value;
    const password = document.getElementById('login-pass').value;

    try {
        const res = await fetch(`${API_BASE}/login`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ username, password })
        });
        if (res.ok) {
            const data = await res.json();
            currentUser = data;
            document.getElementById('guest-nav').classList.add('hidden');
            document.getElementById('user-nav').classList.remove('hidden');
            showPage('dashboard');
        } else {
            alert('Invalid credentials');
        }
    } catch (err) {
        alert('Authentication server unreachable.');
    }
});

function logout() {
    currentUser = null;
    document.getElementById('guest-nav').classList.remove('hidden');
    document.getElementById('user-nav').classList.add('hidden');
    showPage('hero');
}

// Coaches
async function loadCoaches() {
    const list = document.getElementById('coaches-list');
    list.innerHTML = '<p>Retrieving Expert Roster...</p>';
    try {
        const res = await fetch(`${API_BASE}/trainers`);
        const coaches = await res.json();
        list.innerHTML = '';
        coaches.forEach(c => {
            const div = document.createElement('div');
            div.className = 'glass-card';
            div.style.textAlign = 'center';
            div.innerHTML = `
                <img src="${c.img}" class="coach-avatar" onerror="this.src='https://ui-avatars.com/api/?name=${encodeURIComponent(c.name)}&background=2c3e50&color=fff'">
                <h3>${c.name}</h3>
                <p style="color:var(--accent-light); font-weight:600; margin-bottom:1rem;">${c.specialty}</p>
                <p style="font-size:0.9rem;">${c.bio}</p>
                <button class="btn btn-outline" style="margin-top:1.5rem; width:100%;" onclick="openChat('${c.name}')">Chat with Coach</button>
            `;
            list.appendChild(div);
        });
    } catch (err) {
        list.innerHTML = '<p>Error loading coaches.</p>';
    }
}

function openChat(name) {
    const phoneNumber = "6383027257";
    const msg = encodeURIComponent(`Hello, I'm a member of PerfectlyImperfect and I'd like to consult with ${name}.`);
    window.open(`https://wa.me/${phoneNumber}?text=${msg}`, '_blank');
}

// Nutrition
async function loadDiet(type) {
    const display = document.getElementById('diet-display');
    display.classList.remove('hidden');
    try {
        const res = await fetch(`${API_BASE}/diet`);
        const all = await res.json();
        const data = all[type];

        document.getElementById('diet-img').src = data.img;
        document.getElementById('diet-title').innerText = data.title;

        const content = document.getElementById('diet-content');
        content.innerHTML = '';
        data.paragraphs.forEach(p => {
            const para = document.createElement('p');
            para.className = 'diet-p';
            para.innerText = p;
            content.appendChild(para);
        });
        window.scrollTo({ top: display.offsetTop - 100, behavior: 'smooth' });
    } catch (err) {
        alert('Error loading nutrition data.');
    }
}

// Breathing Exercise
let breathingInterval = null;
let breathState = 0; // 0: Inhale, 1: Exhale

function toggleBreathing() {
    const btn = document.getElementById('breath-btn');
    if (breathingInterval) {
        stopBreathing();
        btn.innerText = "Start Session";
        btn.classList.add('btn-primary');
        btn.classList.remove('btn-outline');
    } else {
        startBreathing();
        btn.innerText = "End Session";
        btn.classList.add('btn-outline');
        btn.classList.remove('btn-primary');
    }
}

function startBreathing() {
    const label = document.getElementById('breath-label');
    const dot = document.getElementById('breath-dot');

    const animate = () => {
        if (breathState === 0) {
            label.innerText = "Inhale";
            dot.style.transform = "scale(8)";
            breathState = 1;
        } else {
            label.innerText = "Exhale";
            dot.style.transform = "scale(1)";
            breathState = 0;
        }
    };

    animate();
    breathingInterval = setInterval(animate, 4000);
}

function stopBreathing() {
    if (breathingInterval) {
        clearInterval(breathingInterval);
        breathingInterval = null;
        document.getElementById('breath-dot').style.transform = "scale(1)";
        document.getElementById('breath-label').innerText = "Inhale";
    }
}

// Membership & Payment
let selectedTier = { name: '', price: '' };

function confirmPayment(name, price) {
    selectedTier = { name, price };
    document.getElementById('p-name').innerText = name;
    document.getElementById('p-price').innerText = price;
    showPage('payment');
}

function processWhatsAppPayment() {
    if (!currentUser) return;
    const phoneNumber = "6383027257";
    const text = `PAYMENT REQUEST\n---\nUser: ${currentUser.username}\nEmail: ${currentUser.email}\nSelected Package: ${selectedTier.name}\nPrice: ₹${selectedTier.price}\n\nPlease provide the payment link to activate my "Excess Care" features.`;
    window.open(`https://wa.me/${phoneNumber}?text=${encodeURIComponent(text)}`, '_blank');

    // Alert the user
    alert("Payment request sent! Our admin will contact you via WhatsApp to finalize the transaction.");
    showPage('dashboard');
}

// Sleep Science Features
async function checkSleepReadiness() {
    const screen = document.getElementById('quiz-screen').value;
    const caffeine = document.getElementById('quiz-caffeine').value === 'true';
    const stress = document.getElementById('quiz-stress').value;

    try {
        const res = await fetch(`${API_BASE}/sleep-check`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                screenTime: parseInt(screen),
                caffeine: caffeine,
                stress: parseInt(stress)
            })
        });
        const data = await res.json();

        document.getElementById('readiness-result').classList.remove('hidden');
        document.getElementById('score-val').innerText = data.score + "%";
        document.getElementById('advice-text').innerText = data.advice;

        const routineDiv = document.getElementById('routine-list');
        routineDiv.innerHTML = '<b>Recommended Routine:</b>';
        data.routine.forEach(item => {
            const div = document.createElement('div');
            div.style.padding = '0.3rem 0';
            div.innerText = "• " + item;
            routineDiv.appendChild(div);
        });
    } catch (err) {
        alert("Sleep analysis server error.");
    }
}

async function calculateSleepCycles() {
    const wakeTime = document.getElementById('wake-time').value;

    try {
        const res = await fetch(`${API_BASE}/sleep-calculate`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ wakeTime })
        });
        const data = await res.json();

        const resultDiv = document.getElementById('cycles-result');
        const listDiv = document.getElementById('bedtimes-list');
        resultDiv.classList.remove('hidden');
        listDiv.innerHTML = '';

        data.bedtimes.forEach(time => {
            const span = document.createElement('span');
            span.className = 'logo-btn';
            span.style.fontSize = '0.8rem';
            span.style.padding = '0.4rem 0.8rem';
            span.innerText = time;
            listDiv.appendChild(span);
        });
    } catch (err) {
        alert("Sleep cycle calculation error.");
    }
}

function updateDashboardUI() {
    document.getElementById('dash-membership').innerText = currentUser.membership.toUpperCase();
}

// Init
showPage('hero');
