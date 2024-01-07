<p align="center">
  <img alt="Naivis" src="./resources/icons/Naivis512x512.svg" height="240" />
  <p align="center">A young and simple (sometimes naive) geometry visualization tool.</p>
</p>

---

<p align="center">
  <img src="./doc/MainWindow.png" width="96%" />
</p>

# Build
- Clone the repository
  ```sh
  git clone https://github.com/AnthonyK213/Naivis.git
  ``` 
- Install dependencies
  - Install the latest release of [vcpkg](https://github.com/microsoft/vcpkg)
    - Set environment variable `VCPKG_ROOT`
  - [Qt](https://www.qt.io/download-open-source)
    - Set environment variable `QT_DIR`
  - [LuaJIT](https://github.com/LuaJIT/LuaJIT)
    - Set environment variable `LUA_DIR`
  - [OCCT](https://github.com/Open-Cascade-SAS/OCCT) >= 7.6.0 is needed on Linux
    - Set environment variable `OCCT_DIR`
  - [NaiveCGL](https://github.com/AnthonyK213/NaiveCGL)
    - Build & Install
    - Set environment variable `NAIVECGL_DIR`
- Configure with cmake
  ``` sh
  cd Naivis
  mkdir build
  cd build
  cmake ..
  ```
