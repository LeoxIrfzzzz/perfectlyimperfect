#include "crow.h"
#include <cmath>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

struct User {
  std::string username;
  std::string password;
  std::string email;
  std::string membership = "Free";
};

std::map<std::string, User> users;

struct CORSMiddleware {
  struct context {};
  void before_handle(crow::request &req, crow::response &res, context &ctx) {}
  void after_handle(crow::request &req, crow::response &res, context &ctx) {
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type");
    if (req.method == crow::HTTPMethod::OPTIONS) {
      res.code = 200;
      res.end();
    }
  }
};

void sendRegistrationEmail(std::string toEmail, std::string username) {
  std::cout << "\n[GREETING SYSTEM] User registered: " << username << std::endl;

#ifdef _WIN32
  // Directly trigger a Windows Native Notification (Toast)
  // This bypasses the need for SMTP passwords but gives the "Direct Mail" feel.
  std::string toastCmd =
      std::string("powershell -WindowStyle Hidden -Command \"& {") +
      "$title = 'PerfectlyImperfect Welcome';" + "$msg = 'Hello " + username +
      "! Your account is ready. Welcome to a balanced life. Check your "
      "dashboard for the first protocol.';" +
      "[Windows.UI.Notifications.ToastNotificationManager, "
      "Windows.UI.Notifications, ContentType = WindowsRuntime] | Out-Null;" +
      "$template = "
      "[Windows.UI.Notifications.ToastNotificationManager]::GetTemplateContent("
      "[Windows.UI.Notifications.ToastTemplateType]::ToastText02);" +
      "$textNodes = $template.GetElementsByTagName('text');" +
      "$textNodes.Item(0).AppendChild($template.CreateTextNode($title)) | "
      "Out-Null;" +
      "$textNodes.Item(1).AppendChild($template.CreateTextNode($msg)) | "
      "Out-Null;" +
      "$toast = [Windows.UI.Notifications.ToastNotification]::new($template);" +
      "[Windows.UI.Notifications.ToastNotificationManager]::"
      "CreateToastNotifier('PerfectlyImperfect').Show($toast);" +
      "}\"";

  system(toastCmd.c_str());
  std::cout
      << "[GREETING SYSTEM] Native Greeting Sent to Windows Action Center."
      << std::endl;
#else
  std::cout << "[GREETING SYSTEM] Welcome Notification logged for: " << username
            << " (Native Toast only available on Windows)" << std::endl;
#endif
}

int main() {
  crow::App<CORSMiddleware> app;

  CROW_ROUTE(app,
             "/")([]() { return "PerfectlyImperfect Backend is Running!"; });

  CROW_ROUTE(app, "/api/register")
      .methods(crow::HTTPMethod::POST,
               crow::HTTPMethod::OPTIONS)([](const crow::request &req) {
        if (req.method == crow::HTTPMethod::OPTIONS)
          return crow::response(200);
        try {
          auto x = json::parse(req.body);
          std::string username = x["username"];
          std::string email = x["email"];
          if (users.find(username) != users.end())
            return crow::response(400, "User already exists");
          users[username] = {username, x["password"], email, "Free"};
          sendRegistrationEmail(email, username);
          return crow::response(200, "OK");
        } catch (...) {
          return crow::response(400, "Invalid JSON");
        }
      });

  CROW_ROUTE(app, "/api/login")
      .methods(crow::HTTPMethod::POST,
               crow::HTTPMethod::OPTIONS)([](const crow::request &req) {
        if (req.method == crow::HTTPMethod::OPTIONS)
          return crow::response(200);
        try {
          auto x = json::parse(req.body);
          std::string username = x["username"];
          std::string password = x["password"];
          if (users.count(username) && users[username].password == password) {
            json resp;
            resp["status"] = "success";
            resp["username"] = username;
            resp["membership"] = users[username].membership;
            return crow::response(200, resp.dump());
          }
          return crow::response(401, "Invalid credentials");
        } catch (...) {
          return crow::response(400, "Bad Request");
        }
      });

  CROW_ROUTE(app, "/api/trainers")([]() {
    json coaches = json::array(
        {{{"id", 1},
          {"name", "Alex Rivera"},
          {"specialty", "Bodybuilding"},
          {"img", "assets/coach1.png"},
          {"bio", "Alex is a veteran of the stage specializing in form-centric "
                  "hypertrophy and physique symmetry."}},
         {{"id", 2},
          {"name", "Jordan Smith"},
          {"specialty", "Functional HIIT"},
          {"img", "assets/coach2.png"},
          {"bio", "Jordan is a high-energy specialist in functional movements "
                  "and high-intensity metabolic conditioning."}},
         {{"id", 3},
          {"name", "Samantha Reed"},
          {"specialty", "Yoga & Flow"},
          {"img",
           "https://images.unsplash.com/"
           "photo-1594381898411-846e7d193883?auto=format&fit=crop&q=80&w=400"},
          {"bio", "Samantha focuses on mindful mobility and postural "
                  "correction to optimize functional range of motion."}},
         {{"id", 4},
          {"name", "Dr. Kevin White"},
          {"specialty", "Nutrition Scientist"},
          {"img",
           "https://images.unsplash.com/"
           "photo-1537368910025-700350fe46c7?auto=format&fit=crop&q=80&w=400"},
          {"bio", "Dr. Kevin utilizes biochemical analysis to tailor nutrition "
                  "protocols for peak performance and recovery."}}});
    return crow::response(200, coaches.dump());
  });

  CROW_ROUTE(app, "/api/diet")([]() {
    json diets = {
        {"ectomorph",
         {{"title", "The Ectomorph Fuel System"},
          {"paragraphs",
           {"For the Ectomorph, the primary challenge is caloric density. Your "
            "metabolism runs at a higher speed than average, meaning you burn "
            "fuel almost as fast as you consume it. A successful day starts "
            "with a complex carbohydrate load of at least 80g. Think steel-cut "
            "oats with a handful of walnuts and a tablespoon of almond butter "
            "to ensure sustained energy release.",
            "Your mid-day routine must include a high-protein, calorie-dense "
            "meal. A common mistake is eating too 'clean'—which often means "
            "low calorie. Instead, opt for 200g of grilled chicken thigh "
            "paired with a large serving of sweet potato and avocado. The "
            "healthy fats are crucial for hormonal health and keeping your "
            "energy levels stable through the afternoon.",
            "In the evening, focus on recovery through a mix of slow-digesting "
            "proteins and high-glycemic carbs post-workout. A meal consisting "
            "of wild salmon, a cup of quinoa, and roasted root vegetables "
            "provides the essential fatty acids and amino acids needed for "
            "muscle repair. Don't skip the extra virgin olive oil drizzle for "
            "easy healthy calories.",
            "Consistency is the secret for this body type. You should aim for "
            "4 to 5 smaller meals rather than 3 large ones to avoid digestive "
            "stress. Supplementing with a casein protein shake before bed can "
            "also help prevent catabolism during sleep, ensuring your body "
            "stays in a growth state for the full cycle."}},
          {"img",
           "https://images.unsplash.com/"
           "photo-1547592166-23ac45744acd?auto=format&fit=crop&q=80&w=800"}}},
        {"mesomorph",
         {{"title", "The Mesomorph Balanced Protocol"},
          {"paragraphs",
           {"Mesomorphs have a unique biological advantage: an efficient "
            "metabolism paired with a naturally high muscle-to-fat ratio. A "
            "structured protein timing protocol is essential. Start your "
            "morning with an egg-white omelet containing two whole eggs and "
            "plenty of spinach to jumpstart your metabolic activity for the "
            "day.",
            "Lunch should be a 40/40/20 split of protein, carbs, and fats. A "
            "lean beef bowl with brown rice and steamed broccoli is a staple "
            "for this body type. The goal is to provide enough glucose to fuel "
            "muscle glycogen stores without creating a significant insulin "
            "spike that leads to fat storage.",
            "Post-workout nutrition is where you win. Within 30 minutes of "
            "training, consume a fast-absorbing whey protein shake paired with "
            "a simple carb like a banana. This stops muscle breakdown "
            "immediately. For dinner, choose a light protein like white fish "
            "or lean turkey with focused fibrous vegetables.",
            "Avoid the 'dirty bulk' trap. Because Mesomorphs gain muscle "
            "easily, there is a temptation to eat excess calories. Instead, "
            "focus on nutrient-dense whole foods. Keep your hydration levels "
            "high—3 to 4 liters a day—to support the increased metabolic "
            "demand of lean muscle mass."}},
          {"img",
           "https://images.unsplash.com/"
           "photo-1546069901-ba9599a7e63c?auto=format&fit=crop&q=80&w=800"}}},
        {"endomorph",
         {{"title", "The Endomorph Metabolic Reset"},
          {"paragraphs",
           {"The Endomorph body type is biologically programmed to store "
            "energy efficiently. To combat this, we minimize insulin spikes. "
            "Your day should start with high-quality protein and fats rather "
            "than carbs. A scramble of eggs and avocado is perfect; it signals "
            "your body to use fat for fuel.",
            "Lunch should prioritize satiety through volume. Focus on massive "
            "salads with 200g of lean protein. Increase your vegetable intake "
            "significantly—think two cups of greens minimum. Use apple cider "
            "vinegar-based dressings; the acidity helps improve insulin "
            "sensitivity before minor carb intake.",
            "Carbohydrate timing is non-negotiable for Endomorphs. Save your "
            "carb intake exclusively for the window immediately following your "
            "most intense workout. This ensures glucose goes straight to the "
            "muscles. Outside of this window, focus on 'slow' carbs from "
            "legumes in very moderate amounts.",
            "Dinner should be early and light. A focus on lean protein and "
            "cruciferous vegetables like cauliflower will help keep you full "
            "without weighing you down before sleep. Intermittent fasting "
            "often works wonders, giving the insulin levels a significant "
            "break for deeper fat oxidation."}},
          {"img", "https://images.unsplash.com/"
                  "photo-1512621776951-a57141f2eefd?auto=format&fit=crop&q=80&"
                  "w=800"}}}};
    return crow::response(200, diets.dump());
  });

  // --- NEW SLEEP FEATURES (C++ LOGIC) ---

  // Sleep Readiness Checker Mechanism
  CROW_ROUTE(app, "/api/sleep-check")
      .methods(crow::HTTPMethod::POST,
               crow::HTTPMethod::OPTIONS)([](const crow::request &req) {
        if (req.method == crow::HTTPMethod::OPTIONS)
          return crow::response(200);
        try {
          auto x = json::parse(req.body);
          int screenTime = x["screenTime"]; // minutes
          bool caffeine = x["caffeine"];
          int stress = x["stress"]; // 1-10

          json response;
          int score = 100;

          // C++ Backend Logic for Readiness
          if (screenTime > 30)
            score -= 20;
          if (caffeine)
            score -= 30;
          score -= (stress * 5);
          if (score < 0)
            score = 0;

          response["score"] = score;

          // Dynamic routines from C++
          if (score > 80) {
            response["advice"] =
                "Perfect readiness. 5-min light mindfulness is enough.";
            response["routine"] = {"Deep belly breathing",
                                   "Gratitude visualization"};
          } else if (score > 50) {
            response["advice"] =
                "Moderate readiness. Add light stretches to lower heart rate.";
            response["routine"] = {"Neck rolls", "Child's pose for 2 mins",
                                   "4-7-8 Breathing"};
          } else {
            response["advice"] = "Low readiness detected. Focus on total "
                                 "nervous system shutdown.";
            response["routine"] = {"Legs-up-the-wall pose", "Forward fold",
                                   "Box breathing", "No cold water"};
          }

          return crow::response(200, response.dump());
        } catch (...) {
          return crow::response(400, "Invalid Data");
        }
      });

  // Sleep Cycle Meter Mechanism
  CROW_ROUTE(app, "/api/sleep-calculate")
      .methods(crow::HTTPMethod::POST,
               crow::HTTPMethod::OPTIONS)([](const crow::request &req) {
        if (req.method == crow::HTTPMethod::OPTIONS)
          return crow::response(200);
        try {
          auto x = json::parse(req.body);
          std::string wakeTime = x["wakeTime"]; // Format HH:MM

          int wakeH = std::stoi(wakeTime.substr(0, 2));
          int wakeM = std::stoi(wakeTime.substr(3, 2));

          json suggestion;
          std::vector<std::string> bedtimes;

          // C++ Logic: 90 minute cycles (1.5 hours)
          // 6 cycles = 9 hours, 5 cycles = 7.5 hours, 4 cycles = 6 hours
          int cycles[] = {6, 5, 4};

          for (int c : cycles) {
            double totalMinutes = c * 90 + 15; // 15 mins to fall asleep
            int bh = wakeH, bm = wakeM;

            int totalMinsToSubtract = (int)totalMinutes;
            bm -= totalMinsToSubtract;

            while (bm < 0) {
              bm += 60;
              bh -= 1;
            }
            while (bh < 0) {
              bh += 24;
            }

            char buffer[16];
            sprintf(buffer, "%02d:%02d", bh, bm);
            bedtimes.push_back(std::string(buffer));
          }

          suggestion["bedtimes"] = bedtimes;
          suggestion["message"] = "Calculating for 90-minute neural cycles.";
          return crow::response(200, suggestion.dump());
        } catch (...) {
          return crow::response(400, "Invalid Time");
        }
      });

  app.bindaddr("127.0.0.1").port(18080).multithreaded().run();
}
