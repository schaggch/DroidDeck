# Dark Messiah community loader

Dark Messiah of Might and Magic (Steam app 2100) starts its 32-bit executable through an old SecuROM wrapper. Under ARM64 Proton/FEX, the wrapper terminates before the Source engine loads with an illegal instruction at `0x004F2110` in the protected `nunc` section. Full SMC validation, single-block translation, full TSO and hiding the hypervisor bit do not change that address.

The public [Dark Messiah Mod Launcher](https://github.com/KingDavidW/DarkMessiah-ModLauncher-Files) supplies a supported replacement launch path in `bin/wos_dmmm.dll`, but its patched `mm.exe` retains the same SecuROM wrapper. This directory builds a minimal PE32 loader that calls the community launcher's exported `LauncherMain` directly. It contains no game or community-project binaries.

Build it with:

```sh
./build.sh
```

Install the complete community launcher package into the game's directory first. Back up the installed `mm.exe`, then put the generated `dark-messiah-community-loader.exe` there as `mm.exe`. Steam still owns and launches the title normally; the loader requires the community package's `bin/wos_dmmm.dll` and adjacent Qt files.

For this title, full x87 precision (`FEX_X87REDUCEDPRECISION=0`) is recommended. The loader does not require the expensive full-SMC or one-instruction-block diagnostics used to isolate the original failure.
