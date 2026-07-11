Prerequisites
A C++17 compiler. 
CMake 3.16+
Qt6 (Widgets module) — required for the GUI (src/gui/MainWindow).


Installing Qt6 (MSYS2 / MinGW route, recommended on Windows)
If you don't already have MSYS2, install it from msys2.org first. Then:

Open the "MSYS2 MinGW x64" terminal (Start menu → search mingw64) — not the plain "MSYS2 MSYS" one.
Update the package database:
pacman -Syu
If it asks you to close and reopen the terminal, do so, then run pacman -Syu once more.

Install Qt6:
pacman -S mingw-w64-x86_64-qt6-base

Building
Using CLion: open the repository root,it will detect CMakeLists.txt automatically. Build and run the Project2EZM target.

From the command line:

mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
Running
Run the built executable from the repository root (it loads data.csv via a relative path):

./Project2EZM
This prints benchmark timings (hash map vs. heap build/query time at 10k/25k/50k/100k+ rows) to the console, then opens the GUI for interactive day/hour/temperature queries.
