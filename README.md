# mono-assembly-injector

Inject assemblies into mono embedded processes like UnityEngine Games

## Features

- Does not lock the injected assembly so you can recompile and inject again without closing the target program
- Simple

## Usage

`injector.exe -dll my.dll -target hearthstone.exe -namespace MyProg -class Loader -method Load`

The unload method should just destroy your callbacks, static objects etc.

`injector.exe -dll my.dll -target hearthstone.exe -namespace MyProg -class Loader -method Unload`

Recompile and go again!

__Important__: You need to specify AssemblyVersion like `[assembly: AssemblyVersion("1.0.*")]` so it changes for every build else mono will use an old assembly version from cache.  

## Licenses
mono-assembly-injector is licensed under the MIT License. Dependencies are under their respective licenses.
