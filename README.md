# PerfectlyImperfect Fitness & Training App

A premium gym and fitness tracking application featuring a human-centric coaching system and science-optimized diet planning.

## Tech Stack
- **Backend:** C++ (using [Crow](https://github.com/CrowCpp/Crow) for the REST API and [nlohmann/json](https://github.com/nlohmann/json) for data handling).
- **Frontend:** Vanilla HTML5, CSS3 (Modern Glassmorphism Design), and JavaScript.
- **Styling:** Custom CSS with minimalist accents and futuristic dark mode.

## Project Structure
- `/backend`: C++ source code and CMake configuration.
- `/frontend`: Web interface (index.html, CSS, JS).

## Features
1. **Authentication:** User registration and login system with instant Windows notifications.
2. **Dashboard:** Tracking user stats, streaks, and heart rate metrics.
3. **Elite Coaches:** 4 specialized human coaching profiles with direct WhatsApp integration.
4. **Bio-Diet Plans:** 4-paragraph detailed nutritional protocols for Ectomorph, Mesomorph, and Endomorph body types.
5. **Sleep Science:** C++ driven Sleep Readiness Checker and Cycle Meter.
6. **Membership:** Tiered plans (Basic to Elite) with a direct WhatsApp payment bridge.

## Setup Instructions

### Backend (C++)
1. Ensure you have a C++17 compatible compiler and CMake installed.
2. The `CMakeLists.txt` is configured to automatically download dependencies (Crow, nlohmann_json, asio) using `FetchContent`.
3. Build the backend:
   ```bash
   cd backend
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
4. Run the executable:
   ```bash
   ./fitness_server
   ```
   The server will start at `http://localhost:18080`.

### Frontend
1. Open `frontend/index.html` in any modern web browser.
2. Ensure the backend server is running so that login, registration, and data fetching work correctly.

## Premium Design Notes
- **Vibrant UI:** Uses a curated minimalist palette (Off-white, Blue-grey, Black).
- **Interactivity:** Smooth transitions, glassmorphism cards, and responsive layouts.
- **UX:** Single-page application (SPA) experience for fast navigation.
