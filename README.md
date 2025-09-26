# 🕹️ Arduino Sim Racing Control Panel
![Control Panel](https://github.com/ManInTurtleneck/Fanatec-Accessory/blob/main/images/control-panel.png)

A **low-cost, open-source sim racing control panel** designed around the **Arduino Pro Micro (ATmega32u4)**.  
It features **6 mechanical keyboard switches**, **2 rotary encoders**, and an **ignition switch**, mounted in a **3D-printed enclosure** built to fit the **Fanatec CSL DD** base.  

Commercial alternatives cost hundreds of dollars — this project comes in under **$20 CAD** in parts.  

---

## 🚀 Features
- Built with **Arduino Joystick Library** for full joystick input support  
- Uses **internal pull-up resistors** (no extra hardware needed)  
- **Rotary encoders** for music control, in-game functions, or auxiliary buttons  
- **Mechanical keyboard switches** → customizable & durable  
- **3D-printed body** (3 parts, easy assembly with M3 screws)  
- Ergonomic design: mounts on left side of wheelbase (no interference with shifter/handbrake)  
- **Open source & hackable**  

---

## 🛠️ Bill of Materials (BOM)
- 6 × Mechanical plate-mount keyboard switches  
- 2 × EC11 rotary encoders  
- 1 × Electrical ignition switch  
- 1 × Arduino Pro Micro (ATmega32u4)  
- 4 × M3 × 10 mm screws (assembly)  
- 2 × M6 × 40 mm screws (mounting to Fanatec CSL DD)  

---

## 💻 Firmware
The firmware uses the **[Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-2.0?tab=readme-ov-file)** to map inputs to joystick signals.  

- **All switches** wired to ground  
- **Internal pull-ups** enabled for simplicity  
- Encoders & buttons mapped as joystick inputs (plug-and-play with most sim racing software)  

👉 Code included in this repository.  

---

## 🔧 3D Printing & Assembly
- Case printed in **3 parts**: top, middle, and base  
- Assembled with **M3 screws** (no glue/tape required)  
- Designed in CAD to **fit Fanatec CSL DD** wheelbase  

📦 STL files & full build guide: [Thingiverse Project Page](https://www.thingiverse.com/thing:7153622)

---

## 🌍 Open Source
This project is fully **open source hardware**.  
Feel free to fork, remix, and improve the design.  

---

### 🔑 Keywords
Arduino, ATmega32u4, Arduino Pro Micro, Joystick Library, Embedded Systems, Open Source Hardware, 3D Printing, CAD Design, Sim Racing, Control Panel, Prototyping, Firmware Development
