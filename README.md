# Splitgate Tools

An experimental mod tool for improving the end of life experience for Splitgate 1 in it's semi offline state.

### Compiling
Requires visual studio 2022 build tools (v143) to be compiled.

### Usage
Simply drag the compiled or downloaded `dsound.dll` into the splitgate executable folder.  
Example: `C:\Program Files (x86)\Steam\steamapps\common\Splitgate\PortalWars\Binaries\Win64\`

### Current Features
- Uploading Race stats to `https://sgracers.vercel.app/` with server side validation so others can't modify your times
- Modifying Race countdown length from 5 to any non-negative number
- Toolbar with tabs for maps that exist in the current build to quickly switch to them
- Generally should be universal and work between most 64 bit splitgate builds (untested but was made with that goal in mind (it worked between 8.0.0 to 9.0.0 and the others are not too different))

### Future Ideas
- Forge map downloading using the [Forge Backup](https://github.com/Splitgate/SplitgateForgeBackup) hosted on a web api#
- Forge map uploading

### Early State
As I (Adam) the main developer am back at university I will not be able to focus any time on this so it is open source as it was left off in a working state of course.  
Feel free to make pull requests with any fixes for bugs and new features I will attempt to read those.
