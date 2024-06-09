# agsworks

This is a minimal AGS Plugin to bring Steam and GOG functionality. Any additional store required please open an issue so it can be discussed.

**NOTE:** NDA guarded integrations cannot be added here as this is a public source code.

This plugin will attempt to be compatible with AGS2Client, AGSteam and AGSGalaxy, with new features added through a simpler api.

An effort should be done to support at minimum

- achievements,
- look into steamdeck stuff for steam

Additional features may be store specific, and will be added later.

For Compatibility, it may also support leaderboards, stats, and friends.

## Building

Development is done mostly using CLion and opening it as a CMake project. You can alternatively also just build this with CMake.

Additionally, a VS 2019 compatible solution is available, and you optionally open it in Visual Studio to build and edit the library.

A Linux and macOS compatible Makefile is also provided.

**NOTE:** both the VS Project and the Makefile Project assume you have cloned both this repository and ags repositories in the same directory:

```
.
└── git
    ├── ags
    └── agsworks
```
