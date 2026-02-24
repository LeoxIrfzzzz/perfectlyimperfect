# IGNITE.AI Fitness & Training App

A premium gym and fitness tracking application featuring an AI-driven coaching system and genetic-optimized diet planning.

## Tech Stack
- **Backend:** C++ (using [Crow](https://github.com/CrowCpp/Crow) for the REST API and [nlohmann/json](https://github.com/nlohmann/json) for data handling).
- **Frontend:** Vanilla HTML5, CSS3 (Modern Glassmorphism Design), and JavaScript.
- **Styling:** Custom CSS with neon accents and futuristic dark mode.

## Project Structure
- `/backend`: C++ source code and CMake configuration.
- `/frontend`: Web interface (index.html, CSS, JS).

## Features
1. **Authentication:** User registration and login system.
2. **Dashboard:** Tracking user stats and membership status.
3. **AI Trainers:** 4 specialized AI personas (Strength, Yoga, HIIT, Nutrition).
4. **Bio-Diet Plans:** Tailored nutritional data for Ectomorph, Mesomorph, and Endomorph body types.
5. **Membership:** Tiered access (Basic, Elite AI, Pro Guru) with an integrated payment simulation.

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
- **Vibrant UI:** Uses a high-contrast neon-on-dark palette.
- **Interactivity:** Smooth transitions, glassmorphism cards, and responsive layouts.
- **UX:** Single-page application (SPA) experience for fast navigation.
