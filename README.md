<p align="center">
  <img alt="Naivis" src="./resources/icons/Naivis512x512.svg" height="180" />
  <p align="center">A geometry visualization tool.</p>
</p>

---

<p align="center">
  <img src="./doc/MainWindow.png" width="96%" />
</p>

# Build
- Clone the repository
  ``` sh
  git clone https://github.com/AnthonyK213/Naivis.git
  ```
- Install dependencies
  - Install the latest release of [vcpkg](https://github.com/microsoft/vcpkg)
    - Set environment variable `VCPKG_ROOT`
  - Install `pkg-config`
    - For [windows](https://stackoverflow.com/questions/1710922/how-to-install-pkg-config-in-windows)
  - [Libclang](https://github.com/llvm/llvm-project)
    - Set environment variable `LIBCLANG_DIR`
  - [Qt6](https://www.qt.io/download-open-source)
    - Set environment variable `QT_DIR`
    - For linux, [`LINUXDEPLOYQT`](https://github.com/probonopd/linuxdeployqt) also should be set.
  - [OCCT](https://github.com/Open-Cascade-SAS/OCCT) >= 7.6.0 is needed on Linux,
    since the vcpkg port on Linux is not usable.
    - Build & Install (on Linux)
    - Set environment variable `OCCT_DIR` (on Linux)
- Build [LuaOCCT-binder](https://github.com/AnthonyK213/LuaOCCT-binder) in `build/deps`.
- Configure with cmake, make sure the source files in `build/binder-gen` are generated successfully.
- Build all.
