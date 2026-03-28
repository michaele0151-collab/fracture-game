# Fracture — UE5 Setup Guide

## After pulling, do this in UE5 (one time setup):

### Step 1 — Recompile C++
Right-click `Fracture.uproject` → **Generate Visual Studio project files**
Then open in VS → Build → **Build Solution**
OR just open UE5 → it will prompt to recompile → click Yes.

### Step 2 — Create Input Assets
In the Content Browser, create a folder: `Content/Input`

Create these assets:
1. **Right-click → Input → Input Mapping Context** → name it `IMC_Default`
2. **Right-click → Input → Input Action** → name it `IA_Move` (Value Type: Axis2D)
3. **Right-click → Input → Input Action** → name it `IA_Look` (Value Type: Axis2D)
4. **Right-click → Input → Input Action** → name it `IA_Jump` (Value Type: Digital)
5. **Right-click → Input → Input Action** → name it `IA_Sprint` (Value Type: Digital)

Open `IMC_Default` and add mappings:
- `IA_Move` → W (Y+), S (Y-), A (X-), D (X+)
- `IA_Look` → Mouse X (X axis), Mouse Y (Y axis, negate)
- `IA_Jump` → Space Bar
- `IA_Sprint` → Left Shift

### Step 3 — Create Player Blueprint
1. Content Browser → `Content/Blueprints`
2. Right-click → Blueprint Class → search `FractureCharacter` → select it
3. Name it `BP_FractureCharacter`
4. Open it → Details panel → Input section:
   - Set `Default Mapping Context` → `IMC_Default`
   - Set `Move Action` → `IA_Move`
   - Set `Look Action` → `IA_Look`
   - Set `Jump Action` → `IA_Jump`
   - Set `Sprint Action` → `IA_Sprint`
5. Add a skeletal mesh to the Mesh component (use UE5 mannequin for now)
6. Compile → Save

### Step 4 — Create Game Mode Blueprint
1. Content Browser → `Content/Blueprints`
2. Right-click → Blueprint Class → search `FractureGameMode` → select it
3. Name it `BP_FractureGameMode`
4. Open it → Default Pawn Class → set to `BP_FractureCharacter`
5. Compile → Save

### Step 5 — Set Project Game Mode
Edit → Project Settings → Maps & Modes:
- Default GameMode → `BP_FractureGameMode`

### Step 6 — Create Test Level
File → New Level → Empty Level
- Add a large plane (floor)
- Add `BP_FractureCharacter` to the level (or set as default pawn and use Player Start)
- Add a Player Start actor
- Add some fog: Place Actors → search `Exponential Height Fog` → drag in
- Set Sky Light to dark/night mood
- Hit Play → WASD to move, mouse to look, Shift to sprint

### Step 7 — Create Enemy Blueprint
1. Content Browser → `Content/Blueprints`
2. Right-click → Blueprint Class → search `FractureEnemy` → select it
3. Name it `BP_HollowKnight`
4. Add a skeletal mesh (mannequin for placeholder)
5. Place in the level in the distance

---
*Ada handles all C++ code. Michael handles UE5 editor setup, compile, and play testing.*
