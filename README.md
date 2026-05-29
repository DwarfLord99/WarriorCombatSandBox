# Warrior Combat Sandbox — Player Combat System (WIP)

A third‑person action combat prototype built in Unreal Engine 5, focused on creating a clean, extensible, data‑driven melee combat system.
This project is actively evolving, and this README documents the systems implemented so far and the technical challenges solved along the way.

# 🎮 Project Overview
This prototype establishes the foundation for a full player‑side combat system, including:

Third‑person character movement and animation

Data‑driven ability and resource system

Modular combat component

Weapon‑based melee hit detection

Damage, health, and death handling

Debugging and iteration workflow

Future updates will expand this into a complete combat loop with combos, hit reactions, VFX, and enemy AI.

# 🧍 Character & Animation Setup
Built using KayKit’s mannequin assets and UE5’s Enhanced Input system

Features:

Independent follow camera

Player‑aligned rotation

Idle/Walk blendspace driven by velocity

Attack montage integration

Jumping is functional; jump animation will be added later

This provides a flexible base for future movement and combat features.

# ⚔️ Data‑Driven Ability System
Implemented a modular Ability Data System using Unreal Data Assets.

Each ability defines:

Damage

Rage cost or generation

Cooldown

Animation montage

Additional metadata for future mechanics

This allows rapid iteration and supports multiple future classes (warrior, rogue, mage, etc.) without modifying core code.

# 🧩 Combat Component Architecture
The CombatComponent handles all player combat logic:

Reads ability data and executes the selected attack

Locks movement during attack animations

Manages rage generation/spending

Coordinates with animation notifies for hit detection

Clean separation of gameplay logic from animation and weapon code

This architecture is designed for scalability and reuse across multiple character types.

# 💥 Damage, Health, and Interaction Systems
To standardize how actors receive damage:

IDamageable Interface
Provides a universal entry point for applying damage

Allows any actor (NPCs, destructibles, bosses) to participate in combat

HealthComponent
Tracks current/max health

Applies damage and healing

Broadcasts health changes

Fires a death event when HP reaches zero

Blueprint actors simply forward the interface call to their HealthComponent.

@ 🗡️ Weapon‑Based Hit Detection
The melee hit detection system evolved through several iterations:

Initial Prototype
Forward‑vector trace triggered by a custom C++ AnimNotify

Worked before a weapon mesh was added

Challenges Encountered
Forward trace no longer matched weapon swing

Weapon skeletal mesh had no skin weights, causing socket transforms to behave incorrectly

Sweep traces visually intersected targets but didn’t register hits due to collision channel mismatches

Multiple hits were registered per swing

Final Implementation
Switched to a Static Mesh weapon with proper sockets

Created a C++ AnimNotifyState spanning the entire attack animation

Performed continuous sweep traces using weapon base/tip sockets

Added a per‑swing hit registry (TSet) to ensure only one damage application per target

This results in accurate, animation‑aligned melee hit detection and fair gameplay behavior.

# 🔧 Debugging & Problem‑Solving Highlights
Several complex issues were identified and resolved during development:

Socket transforms not updating  
→ Caused by unskinned skeletal mesh; fixed by switching to static mesh.

Trace visually intersecting but not hitting  
→ Capsule collision wasn’t blocking the Visibility channel.

CombatComponent not applying damage  
→ Missing wiring between NotifyState → CombatComponent → WeaponActor.

Multiple hits per swing  
→ Solved with a per‑swing hit registry.

These solutions demonstrate strong understanding of Unreal’s animation pipeline, collision system, and component‑based architecture.

# 📌 Current Status
The following systems are fully functional:

Player attack execution

Data‑driven ability logic

Weapon‑based sweep tracing

Single‑hit filtering

Damage application

Health reduction and death events

This forms the core of a scalable melee combat system.

# 🚀 Planned Features
Upcoming additions include:

Hit reactions and stagger animations

Weapon trails and VFX

Camera shake and hitstop

Enemy AI integration

Additional player classes and abilities

UI elements (health bars, damage numbers)
