# Physics 101

Just a fun project for me to mess around with some OpenGL and C++. It has been a long time since I used C++.

## Usage

This project uses the git submodule for dependencies. To download, use

```bash
git clone --recursive 
```

or if the repository is already cloned;

```bash
git submodule update --init --recursive
```

To manage third-party dependencies (and for more flexible IDE choices), CMAKE should be used to build the solution, e.g.

```bash
cmake -S . -B build && cmake --build build
```