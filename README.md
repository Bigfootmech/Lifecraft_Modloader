# Lifecraft Modloader (Project)

This project (WIP) is a mod-loader for the game Lifecraft.

https://store.steampowered.com/app/1839930/Lifecraft/
https://pixbitsduo.itch.io/lifecraft

## Engine

The game devs have written their own proprietary C++ code (engine).

This means the existing modloaders for Unity (or indeed Godot, or Unreal) based games are unlikely to work.
Or may have other crashes due to relying on non-existing systems.

### Existing solutions
Melonloader looks like it could almost work. maybe. using winmm.dll. But I haven't been able to get it to.
BepInEx has no entry (Lifecraft currently has no web, and doesn't use version.dll)
UnrealModLoader uses xinput1_3 but Lifecraft has xinput1_4
and the Godot one requires developer intervention as far as I can tell, to enable it.

### This project

Since there's nothing that currently works, I decided to have a play about with it.
After getting permission from the devs, I'm uploading this as a project, and hopefully jumping-off platform,
so that others can also join in too :).

# Disclaimer

This is meant for changing your own experience in singleplayer sandbox game.

## Don't Misuse
Please do not misuse this for anything illegal (eg: repurpose for hacking other software)
Or to harm anyone else's experience. (eg: playing a joke on your friends if the game gets multiplayer later)

## Care Sus links
"Don't download and run a suspicious .exe off the internet and run it"
applies to this too.

Any mod you load is like an exe effectively. 
For the mentioned modloaders, as well as some other games like Minecraft, and Terraria.

Ultimately, any software can "be hacks" or "do bad things".
From government supplied, to companies, to individuals.

If you don't trust anyone, physically disconnect your machine from the internet, as well as any wifi/bluetooth.
It's the safest way.

Other than that, a reasonable amount of precaution is:
Getting mods that a lot of other people do, and from a big website (steam's workshop, nexusmods, curseforge... etc)
They tend to have screening processes. As well as announcing and helping you get rid of any nasty stuff that was better hidden.

## Stability
Mods can also absolutely break your game. 
Both in general (Skyrim), and because this current loading is particularly hacky.
Back up your saves before trying.

> User\AppData\Roaming\lifecraft
or
> %APPDATA%/lifecraft

You have been warned.


# Inspiration / goal

Unity games already have lots of very well developed tools that enable modding. Eg:

MelonLoader
https://melonloader.net/
https://github.com/LavaGang/MelonLoader

and

BepInEx (powered by Doorstop)
https://docs.bepinex.dev/
https://github.com/BepInEx/BepInEx/
https://github.com/NeighTools/UnityDoorstop
https://docs.bepinex.dev/master/articles/user_guide/troubleshooting.html

Actually, the "bootstrap" method is a copy of how they run inside Unity games.
Modified to work for this game in particular.

## Dll names

Currently, the loading works worse than the above projects, but it's one of the short term goals to improve.
(melonloader "proxies" equivalent, and unnamed at the bottom of troubleshooting for BepInEx)

I've found that on my system, these work ok as dll names: 
- umpdc.dll
- HID.DLL
and maybe
- mscms.dll 

These could change as the game updates.

# Contact / Progress

Please come hang out on the official Lifecraft Discord (#modding channel)
https://discord.gg/TwsNSCHaN8

I'm currently dropping any updates I have to the people there

## Current state

At the moment, the project compiles to a single DLL file, with just my mod in it. And no ability to run others.

The next thing I need to do is to split it up in to separate sections.

ie: architect a multi-module solution in a language I'm not familiar with.
So bear with me.

## Known Bugs:
- Save text is invisible for my "no blinking" mod
- probably some missing functionality or crash from overriding dll functionality. But I haven't encountered it yet.


# Install

- Build the project (have Visual Studio on your PC, and compile the project)
- Rename LifecraftDll.dll to one of the above dll names (eg: umpdc.dll)
- drop it in the Lifecraft bin folder (steamapps\common\Lifecraft\bin\win)

Now if you launch your game, and start it, 
you'll see that if you pause with spacebar, the "game is paused" text no longer flashes.

More ComingSoon(tm)
\#famouslastwords

## Uninstall

- delete the DLL.

"fix game files" won't work, as this is a separate file that's not tracked by Steam or the game.