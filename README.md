# sztOS

```
                                            __            ________    _________
                           _______________/  |_          \_____  \  /   _____/
                          /  ___/\___   /\   __\  ______  /   |   \ \_____  \ 
                          \___ \  /    /  |  |   /_____/ /    |    \/        \
                         /____  >/_____ \ |__|           \_______  /_______  /
                              \/       \/                        \/        \/ 
```
**Simple, zippy, transparent operating system.**
# About
<p align="center">
  <img src="/src/gui/app/resources/LogoSZTOS.png" width="250" title="Logo">
</p>

An operating system simulator containing: 
* index based filesystem, 
* paged memory management, 
* SRT scheduler, 
* mutex lock based synchronization mechanisms,
* windows-style processes management,
* windows-style gui,
* assembler programs interpreter.

Gui is based on the [SFML](https://github.com/SFML/SFML) library with a console window pulled from [SelbaWard](https://github.com/Hapaxia/SelbaWard/wiki).

# How to build in Visual Studio 2019
1. Open `~\src\gui\app\app\sln` in Visual Studio.
1. Open project properties by pressing the `Project` button in upper left corner, and then `app Properties`.
1. Make sure `Configuration` is set to `All Configurations`.
1. In `C/C++` click on `General`. 
1. Add the absolute path to SFML's include folder (`~\src\gui\SFML`) in `Additional Include Directories` on the top right.
1. In `Linker` click on `General`.
1. Add the absolute path to SFML's lib folder (`~\src\gui\SFML\lib`) in `Additional Library Directories` on the right.
1. Make sure that you build the for the x64 platform and you're good to go.

# Authors
- Artur Jackowiak 
- Bartosz Kosmala
- Grzegorz Konowalczyk
- Hubert Knioła
- Jordan Kondracki
- Mateusz Kuźniak
- Roch Goszczyński
> 2019 SEM 3
