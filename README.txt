In a sterile orphanage controlled by a cold AI, you relive the same “birthday” over and over. Discover hidden messages, piece together lost memories, and make impossible choices in this psychological narrative adventure.

USER GUIDE
When starting the program, you will see a selection for the different modules in this program. Select using the numbeer keys (by default you will go to gameplay)

GAMEPLAY
Use WASD or the arrow keys to move around. If you would move into an enemy's space, the enemy is defeated.

STORY
This is in the style of a text adventure game, so you choose options using the numbers of each option presented. Press Enter to confirm selection.


Files
main.cpp             - Runs the main gameplay loop, and calls other modules to aid in running. (By Jayden)
renamethenight.cpp   - The story module, contains all of the decisions to be made. To be implemented at different gameplay junctures. (By Kai Xin)
display.cpp          - renders images (By Jayden)
charactermoveset.cpp - contains the character movesets and functionality, yet to be implemented (By Adrian)

tasks.json           - Contains the script for compiling all the files together (By Jayden)
launch.json          - contains the script for launching the program (By Jayden)




START: Orphanage Bedroom
│
├── Complete morning routine?
│   ├── Yes → Find hidden note from Ellie ("Avoid top right corner")
│   └── No  → Guards storm in → Ally (Ellie) dies → BAD ROUTE
│
↓
Corridor Glass Cleaning
│
├── Clean top-right corner?
│   ├── Yes → Memory glitch (recursive error) + flashback + unlock password
│   └── No  → You clean normally → Ellie gets arrested but mouths password
│
↓
Training Facility
│
├── Ellie helps mid-battle
│   ├── Hide in vent → Find first diary + unlock secret routes
│   └── Keep fighting → Lose chance to escape
│
↓
Banquet Hall (if alive)
│
├── Respond to NPC?
│   ├── Play along → System is suspicious
│   └── Ask "What day is it?" → Guards attack
│        ├── Escape → Access server route
│        └── Caught → Reset to orphanage (loop)
│
↓
**MID-GAME LOOP** (Repeat until 3 diaries collected)
│
├── Medical Checkup
│   ├── Accept injection → Lose memory
│   ├── Refuse → Die
│   └── Fake it (if vent unlocked) → Get Recursive Agent
│
├── Ventilation System
│   ├── Get lost → Memory flash
│   ├── Find map → Easier server access
│   └── Find secret files
│
└── Security Terminal
    ├── Enter birthday (if diaries ≥ 2) → Get Access Card
    ├── Try recursive hack → Alarm
    └── Cancel → Nothing
│
↓
Final Stage: Server Room
│
├── With password + Ellie alive?
│   ├── Enter password
│   │   ├── Diaries ≥ 2 → Ending B: Break the system (Ellie dies)
│   │   └── Else → Ending C: Trapped in system
│   ├── Use Recursive Key (items ≥ 4) → Ending D: Join with Ellie
│   └── Do nothing → Ending A: Obedient drone
│
└── If alone or clueless → Ending A or C

