# Dexnav
A simplistic gen III implementation of ORAS's Dexnav feature

## Building

Requirements (Windows):
- DevkitARM (4.3, 4.5, or 4.7 https://devkitpro.org/wiki/Getting_Started/devkitARM windows installer here)
- Python (Atleast 3.4 https://www.python.org/downloads/)
- Armips(Latest: https://buildbot.orphis.net/armips/)
- Visual C++ runtime DLL https://www.microsoft.com/en-ca/download/details.aspx?id=48145

Make sure you have an *environment variable* for `DEVKITARM`, `python` and `armips`. If not, create them.

After cloning the repository, delete the deps folder if you're not planning to build the dependancies. Extract the `deps.zip` file. I've taken the liberty of building them so you don't have to.

Next, create a directory called `roms` and place a clean FireRed US version 1.0 ROM inside and rename it to `BPRE0.gba`.
Now we can build the project by running `make`.

If the last line is `Done.` then everything succeeded.


## Preview
Videoes are with low search levels, so HUD is unrevealing.
Grass vid https://streamable.com/k4ot7
Cave vid https://streamable.com/50qix
Water vid https://streamable.com/gcr42


Credits:
- https://github.com/Touched project structure
- https://github.com/pret string related tools being used
- https://github.com/Zarel showdown assets such as descriptions and table data
- All contributors!

