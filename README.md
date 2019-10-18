# Standard Win32 Application
This is a framework I've created to aid with the initial setup of a modern Windows application.

## Visual Studio Configuration
The development environment used is Visual Studio 2019, configured such that
* On compilation, all binary executables get placed in the "_Binary" folder
* All intermediate files (.suo, .obj, .pdb, etc) are placed in the "Z_DELETE" folder
  * This folder is always safe to delete
* The "System" folder contains all .cpp, .h, .hpp files for booting up the app, 
which the IDE internally sorts into Headers and Source Files in the Solution Explorer

These configuration settings were set by using the Solution Properties GUI (Alt + Enter) and stored in the included '.vcxproj' and '.vcxproj.filters' files, so feel free to change them as you like!

## Character Set
This was built to support the Unicode character set
