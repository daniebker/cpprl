## How to setup

- Make sure you have the correct tools ready.
  - For example on Windows:
    Make sure you install the "Desktop development with C++" workload from the latest [Visual Studio Community Installer](https://visualstudio.microsoft.com/vs/community/),
    install the latest [CMake](https://cmake.org/download/),
    and install [Visual Studio Code](https://code.visualstudio.com/download).
- [Create a new repository from ths template](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/creating-a-repository-on-github/creating-a-repository-from-a-template#creating-a-repository-from-a-template). **You do not use the GitHub fork button unless you're contributing.**
- [Clone your new repository](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository-from-github/cloning-a-repository).
- Make sure to initialize the `vcpkg` [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules). If the `vcpkg` folder is empty or missing then the build process will fail.
  - The command to do this is: `git submodule update --init`
- `mkdir build`
- `cd build`
- Init the project from the terminal using `cmake ../`
- `cd ../`
- from the terminal run `cmake --build . --config Debug --target all -j 12 # to build the project`
- Load this project in VS Code and install any recommended plugins.
  Most importantly: _C/C++_, _C++ Intellisense_, and _CMake Tools_.
- Using the CMake Tools plugin you can now build and run this project. Dependencies will automatically be installed via Vcpkg.
  - On Windows, when CMake Tools asks for a toolkit you should select `Visual Studio Community ... - amd64`.
  - When CMake Tools asks for a project select `libtcod-vcpkg-template`.
- The project will now build and run with F5.
  Actions which pass without fail will provide archived executables to test with, these are temporary and are downloaded from the passing action under _automated-builds_.
  You should edit the `CMakeLists.txt` script as needed to expand your project.

Now that you know the template works you'll want to take care the following:

- Make sure you have the correct tools ready.
  - For example on Windows:
    You'll need to add these libraries to [CMakeLists.txt](CMakeLists.txt) to link them.

* Change `libtcod-vcpkg-template` in [CMakeLists.txt](CMakeLists.txt) and [.github/workflows/cmake.yml](.github/workflows/cmake.yml) to the name of your game.
  You may want to edit other parts of the CMake script as well, such as the included source files.

- [Create a new repository from ths template](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/creating-a-repository-on-github/creating-a-repository-from-a-template#creating-a-repository-from-a-template). **You do not use the GitHub fork button unless you're contributing.**
- [Clone your new repository](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository-from-github/cloning-a-repository).
- Make sure to initialize the `vcpkg` [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules). If the `vcpkg` folder is empty or missing then the build process will fail.
  - The command to do this is: `git submodule update --init`
- Init the project from the terminal using ~cmake .~
- Load this project in VS Code and install any recommended plugins.
  Most importantly: _C/C++_, _C++ Intellisense_, and _CMake Tools_.
- Using the CMake Tools plugin you can now build and run this project. Dependencies will automatically be installed via Vcpkg.
  - On Windows, when CMake Tools asks for a toolkit you should select `Visual Studio Community ... - amd64`.
  - When CMake Tools asks for a project select `libtcod-vcpkg-template`.
- The project will now build and run with F5.

## Building Web Locally

Install `emsdk` [by following the official instructions](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions)

Generate the cmake build with the following command:

```sh
emcmake cmake -S . -B build_web -G Ninja \
          -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
          -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
          -DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
          -DCMAKE_BUILD_TYPE="production"
```

If the command fails with emcmake not found then you probably forgot to source the `emsdk_end.sh` file.

Run `cmake --build build_web`

To launch a web server run `npx serve ./build_web/bin`

This will launch a server where you will be able to play the game.

## Resources

- [cpp implementation of the tutorials](https://gitlab.com/libtcod-tutorials/libtcod-tutorials)
- [Getting Started Docs](https://libtcod.readthedocs.io/en/latest/guides/getting-started.html)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [Python tutorial](https://rogueliketutorials.com/tutorials/tcod/v2/)
- [HexDecimals tcod engine 2022 edition](https://github.com/HexDecimal/tcod-cpp-engine-2022)
