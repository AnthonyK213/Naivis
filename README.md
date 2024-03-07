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
  - [Libclang](https://github.com/llvm/llvm-project)
    - Set environment variable `LIBCLANG_DIR`
  - [Qt6](https://www.qt.io/download-open-source)
    - Set environment variable `QT_DIR`
    - For linux, [`LINUXDEPLOYQT`](https://github.com/probonopd/linuxdeployqt) also should be set.
  - [OCCT](https://github.com/Open-Cascade-SAS/OCCT) >= 7.6.0
    - [Build OCCT](https://dev.opencascade.org/doc/overview/html/build_upgrade__building_occt.html)
    - Set environment variable `OCCT_DIR`
- Build [LuaOCCT-binder](https://github.com/AnthonyK213/LuaOCCT-binder) in `build/deps`.
- Configure with cmake, make sure the source files in `build/binder-gen` are generated successfully.
- Build all.
