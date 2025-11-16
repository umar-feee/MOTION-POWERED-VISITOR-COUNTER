# Motion Powered Visitor Monitor (Bare-Metal AVR)

A complete **bare-metal embedded system** designed using **AVR microcontroller**, dual PIR sensors, a 16x2 LCD, and alert indicators to count the number of visitors entering and exiting a room.  
The system uses **direction-based PIR sensing logic** (PIR1 → PIR2 = entry, PIR2 → PIR1 = exit) and updates the visitor count in real time.

---

## 🚀 Features
- ✔️ **Bare-metal C** (No Arduino libraries)  
- ✔️ Dual **PIR-based entry/exit detection**  
- ✔️ **Directional visitor counting logic**  
- ✔️ Real-time display on **16x2 LCD (4-bit mode)**  
- ✔️ **Buzzer + LED indicators** for each detection  
- ✔️ Clean and stable debounce + flag logic  
- ✔️ Fully interrupt-free, pure polling-based control  

---

## 📌 Hardware Requirements
- **ATmega328P / ATmega16 / ATmega32** (Any AVR MCU)  
- **PIR Sensor 1 (Entrance)**  
- **PIR Sensor 2 (Exit)**  
- **16×2 LCD Display** (4-bit mode)  
- **Buzzer**  
- **LED indicators (optional)**  
- Jumper wires, breadboard or PCB  

---

## 🧩 Pin Configuration

| Component | Pin |
|----------|-----|
| PIR1 | PD2 |
| PIR2 | PD3 |
| LED1 | PD4 |
| LED2 | PD5 |
| Buzzer | PD6 |
| LCD RS | PD7 |
| LCD E | PB0 |
| LCD D4 | PB1 |
| LCD D5 | PB2 |
| LCD D6 | PB3 |
| LCD D7 | PB4 |

---

## 🛠️ Software Tools Used
- **AVR-GCC**
- **AVRDUDE**
- **Microchip Studio / VS Code (optional)**
- **Proteus / Wokwi** (for simulation)

---

## 🔍 How It Works

### **Entry Detection (PIR1 → PIR2)**  
1. Person triggers **PIR1**  
2. Immediately triggers **PIR2**  
3. Visitor count **increments**  
4. Buzzer + LED alert  
5. LCD updates instantly  

### **Exit Detection (PIR2 → PIR1)**  
1. PIR2 triggers first  
2. PIR1 triggers second  
3. Visitor count **decrements**  
4. Audible + visual notification  

### **Reset Logic**  
System waits until both sensors return LOW before detecting the next person — prevents double counting.

---

## 🧾 Code Overview
- Pure **bare-metal AVR logic**  
- Custom **LCD driver** (4-bit mode)
- Direction detection using:
  ```c
  pir1_flag, pir2_flag
