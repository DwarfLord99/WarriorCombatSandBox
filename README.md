# Warrior Combat Sandbox — Unreal Engine 5

A modular third‑person combat prototype built in Unreal Engine 5.  
This project focuses on clean gameplay architecture, data‑driven abilities, and scalable combat systems suitable for action RPGs and character‑action games.

---

## 🎮 Project Overview

Warrior Combat Sandbox is a technical combat prototype featuring:

- **Data‑driven Ability System** using `EAbilityInput` enum routing  
- **AbilityMap** for scalable ability slot assignment  
- **Modular CombatComponent** handling traces, hit detection, and damage  
- **Rage System** with ability‑specific resource costs  
- **Animation‑driven combat** using montages and notifies  
- **Hit reaction pipeline** for enemies  
- **Expandable architecture** designed for future UI, combos, and advanced abilities  

This project is the successor to an earlier prototype and now includes a significantly improved ability routing system and cleaner Character → Component communication.

---

## 🧩 Core Systems

### **Ability Input System**
- Enum‑based ability routing (`EAbilityInput`)
- Character stores a `TMap<EAbilityInput, UAbilityData*>`
- Input functions call `DoAttack(EAbilityInput)`
- Scales to any number of abilities without rewriting code

### **Combat Component**
- Executes ability logic
- Handles traces, hit detection, and damage application
- Reads damage, rage cost, and montage data from `UAbilityData`

### **Ability Data**
- Defines damage, rage cost, montage, trace settings, and more
- Allows new abilities to be added without modifying C++

### **Animation System**
- Uses montages with notifies to trigger hit traces
- Supports different slots and animation sections per ability

---

## 🛠️ Tech Stack

- **Unreal Engine 5**
- **C++ Gameplay Framework**
- **Animation Montages + Notifies**
- **Data Assets (UAbilityData)**
- **Enhanced Input System**

---

## 📚 Documentation

Full documentation is available in the project Wiki:

- Combat System  
- Ability System  
- Health System  
- UI System  
- Project Roadmap  

---

## 🚀 Current Status

**Milestone 1: Core Combat Loop — Complete**  
- Basic + Heavy attacks  
- Rage system  
- Hit detection + damage  
- Animation notifies  
- AbilityData integration  

**Milestone 2: Ability Input System — Complete**  
- Enum‑based routing  
- AbilityMap  
- Scalable DoAttack()  

Now progressing into **Milestone 3: Ability Expansion**.

---

## 📄 License

This project is for educational and portfolio purposes currently.
