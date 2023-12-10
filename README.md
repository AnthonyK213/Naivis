# Naivis

A young and simple (sometimes naive) geometry visualization tool.

## Build
- Clone the repository
  ```sh
  git clone https://github.com/AnthonyK213/Naivis.git
  ``` 
- Install dependencies
  - Eigen
    - Windows
      - Install the latest release of [vcpkg](https://github.com/microsoft/vcpkg)
        ``` ps1
        git clone https://github.com/microsoft/vcpkg
        .\vcpkg\bootstrap-vcpkg.bat
        ```
      - Set environment variable
        - `VCPKG_ROOT`
    - Unix
      - Debian
        ``` sh
        sudo apt-get install ...
        ```
      - RedHat
        ``` bash
        sudo dnf install eigen3 eigen3-devel
        ```
  - [NaiveCGL](https://github.com/AnthonyK213/NaiveCGL)
    - Build & Install
    - Set environment variable
      - `NAIVECGL_DIR`
  - [Qt](https://www.qt.io/download-open-source)
    - Set environment variable
      - `QT_DIR`
- Build with cmake
  ``` sh
  cd NaiveCGL
  mkdir build
  cd build
  cmake ..
  ```
