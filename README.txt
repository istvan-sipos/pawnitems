pawnitems
=========

A support pawn gear mod for Dragon's Dogma: Dark Arisen.

Support pawns (the two pawns you hire from the rift) are handled more
strictly than your main pawn. Anything you take off them is sent back to
their owner and lost to you, and they refuse upgraded or otherwise
restricted gear. pawnitems removes both restrictions, so managing a
support pawn's equipment works the same way as it does for your main
pawn.


FEATURES
--------

- Unequipped items stay. Gear you remove from a support pawn stays in
  that pawn's inventory instead of being sent away when you close the
  equipment menu. The "Removing this equipment will send it away"
  confirmation is skipped, since it no longer applies.
- Any gear can be equipped. Support pawns can equip items they normally
  refuse: upgraded gear (such as 3-star or dragonforged) and restricted
  item types. The [Equip] button is no longer greyed out for them.
- Any gear can be given. You can hand that gear to a support pawn with
  [Give], or withdraw it from storage straight into their inventory.
- Vocation rules are untouched. A support pawn still can't equip gear
  their vocation can't use. pawnitems doesn't touch that check, so it
  works alongside dinput8's ignoreEquipVocation option without
  interfering.
- Configurable. Each feature can be switched off in pawnitems.ini.
- Reversible. All changes are made in memory while the game runs;
  nothing is written to the game files. Remove pawnitems.dll and the
  game goes back to its stock behaviour.


REQUIREMENTS
------------

- Dragon's Dogma: Dark Arisen on PC, GOG release. Linux with Wine /
  Proton works too.

  The Steam release isn't supported yet. pawnitems checks the game code
  before changing anything; on another build it changes nothing and
  logs why in pawnitems.log.

- dinput8, which loads add-on DLLs into the game:
  https://github.com/istvan-sipos/dinput8


INSTALLATION
------------

1. Install dinput8 first. Its dinput8.dll and dinput8.ini should sit
   next to DDDA.exe in your game folder.

2. Download the latest pawnitems release archive and extract
   pawnitems.dll and pawnitems.ini into the same folder, next to
   DDDA.exe.

3. Open dinput8.ini in a text editor. Under the [main] section, add
   pawnitems.dll to the addons line:

       addons = pawnitems.dll

   If you already load other addons, separate them with semicolons:

       addons = other.dll;pawnitems.dll

4. Launch the game. A pawnitems.log file appears next to DDDA.exe.
   It should end with these two lines:

       unequip: ready
       restrict: ready

   dinput8.log also records whether the addon loaded.


CONFIGURATION
-------------

pawnitems.ini sits next to pawnitems.dll. Every option defaults to on;
set it to 0 to turn that feature off.

    [main]
    logging = truncate     ; disabled, truncate (reset each launch),
                           ; or append

    [unequip]
    keep_items = 1         ; unequipped items stay in the inventory
    skip_prompt = 1        ; skip the "send it away" confirmation

    [restrict]
    allow_equip = 1        ; support pawns can equip restricted gear
    allow_give = 1         ; restricted gear can be given to them, or
                           ; withdrawn from storage into their inventory


NOTES
-----

- An unequipped item ends up in the support pawn's inventory. Move it
  to yourself or your main pawn before you dismiss the support pawn,
  or it leaves with them.
- A few specific items are blocked for support pawns by the game's
  vocation check rather than the rules above. pawnitems leaves those
  alone. dinput8's ignoreEquipVocation option lifts them along with
  every other vocation restriction.


BUILDING
--------

The mod is plain C, built with the MinGW-w64 32-bit cross compiler.
On Debian / Ubuntu:

    sudo apt-get install gcc-mingw-w64-i686-win32 libz-mingw-w64-dev

Then, from the repository root:

    make pawnitems.dll     build pawnitems.dll
    make                   build and copy it to DDDA_DIR (see Makefile)
    make deploy-ini        copy the default pawnitems.ini to DDDA_DIR

Edit DDDA_DIR at the top of the Makefile to point at your game folder.
deploy-ini is separate from the default target so a customised ini
isn't overwritten on every build.

Source layout:

    src/pawnitems.c   DllMain, ini parsing, logging
    src/patch.c       signature-checked code patching helpers
    src/unequip.c     keep items unequipped from support pawns
    src/restrict.c    equip / give / withdraw restrictions
