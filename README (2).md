# EE 329 – Lab A4: Reaction Timer Game (STM32L4)

This project implements a **reaction-time game** on the STM32L4A6ZG Nucleo-144 board.  
The user starts the game with a button press, waits through a random delay, then reacts as quickly as possible when the LED turns on. The system measures and displays the reaction time on an LCD.

---

## 🎮 Game Flow
1. **Startup** – LCD displays prompt (“push sw to begin”).  
2. **Wait for first press** – User presses button to start.  
3. **Random delay** – System waits 1–4 seconds before lighting the LED.  
4. **Reaction** – User presses button again to turn off the LED.  
   - Reaction time is measured in **ms** using TIM2 (1 ms interrupts).  
5. **Result** – LCD shows the reaction time in seconds with millisecond precision.  
6. **Reset** – If no button press within 10 s, system resets automatically.  

---

## ⚡ Hardware Setup
- **Board:** STM32L4A6ZG Nucleo-144  
- **Peripherals:**  
  - Pushbutton → PC13  
  - LED → PB7  
  - LCD (2×16) → connected via user wiring  
- **Timer:** TIM2 configured for 1 ms periodic interrupts  

---

## 🧩 Source Files
- `main.c` – State machine for game logic, LCD output, GPIO config  
- `main.h` – Pin mappings, game state enum  
- `timer.c` – 1 ms counter via TIM2 interrupt; handles 10 s timeout  
- `timer.h` – Timer function prototypes  

---

## 🚀 Build & Run
1. Import the source files into **STM32CubeIDE**.  
2. Build and flash to the **NUCLEO-L4A6ZG** board.  
3. Connect pushbutton, LED, and LCD as described.  
4. Play the reaction game!  

---

## 📂 Repository Structure
```
.
├── main.c / main.h          # Game state machine, pin definitions
├── timer.c / timer.h        # 1 ms timer (TIM2) with interrupt
├── .gitignore               # Ignore build artifacts
└── README.md                # Project documentation
```

---

## ✅ Learning Outcomes
- Implementing **finite state machines** in embedded C  
- Using **hardware timers** and **interrupts** for precise time measurement  
- Interfacing with **GPIO, pushbuttons, LEDs, and LCD displays**  
- Structuring a professional, GitHub-ready embedded project  

---

## 📜 License
This project is licensed under the MIT License – see [LICENSE](LICENSE).  

---

👤 **Author:** Nathan Heil  
📅 **Course:** EE-329 (Embedded Systems)  
