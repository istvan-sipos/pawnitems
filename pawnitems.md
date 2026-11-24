# Pawn Items Modification Plan

## Game Context
DDDA (Dragon's Dogma: Dark Arisen) implements a pawn system where:
- Players have a primary pawn (main pawn) that is fully customizable
- Players can change the main pawn's gear, skills, etc.
- The party can also hire two support pawns
- Support pawns are static with restrictions on what items can be equipped
- When removing equipment from support pawns, items are lost permanently
- The game UI displays the following messages for these actions:
  - "You cannot give a support pawn this item."
  - "Removing this equipment will send it away. You will not be able to re-equip it."

## Desired Behavior
Modify the game to make support pawns behave like the main pawn:
- Un-equipped items should stay in the inventory instead of being lost
- Support pawns should be able to receive any gear (no restrictions)

## Implementation Strategy
Using Ghidra MCP server, we need to identify and patch functions that handle:
1. Pawn type differentiation logic (how support vs main pawns are distinguished)
2. Item restriction checking for pawn equipment
3. Equipment removal logic that causes permanent loss of items
4. Inventory management for pawn equipment

This requires analyzing the game's pawn management code, particularly around inventory and equipment systems where these restrictions are implemented.