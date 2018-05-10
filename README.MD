# SimplyCpp

SimplyCpp is a very simple to use IDE for C++ Language, that is intended for students who are very new to the field of programming. This is programmed because they need to focus more on the syntax rather than understanding the complex build tools and IDEs.

![ScreenShot Windows](http://i.imgur.com/qUVqmdI.png)

I'm remaking the editor in C++ wxWidgets because wxAUI gives more control than standard WinForms, and also allows for the application to be compiled and run on a Mac.

![Screenshot Mac](https://i.imgur.com/rWe3t5A.png)

**Development on macOS is experimental, and currently only editing the source files are supported. This is being worked upon and will be out soon.**

This program uses [TDM-GCC-32](http://tdm-gcc.tdragon.net/) as the compiler, which is a modified (patched) version of MinGW compiler for windows and is included in the installer.

## Building Instructions

This application relies on the wxWidgets library to create the user interface. So you first need to install wxWidgets.

### For Windows:

  - Download the source code distribution of wxWindows and extract it to a convenient location.
  - Then open the Visual Studio developer console, navigate to the directory and configure the library.
  - Build the library.
  - Clone this project and run CMake to generate the project files for IDE.
  
 ### For macOS:

  - wxWidgets on macOS requires some patches to work, so get Homebrew to apply them for you.
  - Open a Terminal and run these commands.
  
        brew install wxmac --devel --with-static
        mkdir build
        cd build
        cmake -G "Xcode" .. -DwxWidgets_CONFIG_EXECUTABLE=/usr/local/Cellar/wxmac/3.1.1/bin/wx-config
        
  - Open the `build/SimplyCpp.xcodeproj` file in XCode and start hacking.
