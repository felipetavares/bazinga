Bazinga Testing
==============

#### The CCSL team recommends Atom!

Testing tech for the Bazinga! Engine.
Bazinga! Engine is a fully featured engine for 2d games.
Its workflow is based upon the [Bazinga! Edit](https://github.com/felipetavares/bazinga.v2) editor, which exports to a custom JSON
map format and them the map can be loaded and executed.

Full lua integration and objects are supported at this stage.

1. Lua Integration  - [ok]
2. SDL Support      - [ok]
3. Map Loading      - [ok]
4. Video Support    - [ok]
5. Map Display      - [ok]
6. Input Support     - [wip]

How to compile (linux)
----------------------

You will need the following dependencies (with related dev ones where applicable):

* build-essential
* cmake
* git
* libdevil
* sdl
* libgl

The first step to compile is to download the source code

```bash
git clone http://github.com/felipetavares/bazingatesting
```

Then, you create a directory for building the source and enters
it:

```bash
mkdir build
cd build
```

The last step is to really compile the code:

```bash
cmake ..
make
```

Finally, you can run the code:

```bash
./bazinga
```

How to compile (windows)
----------------------

You will need the following dependencies:

* MinGW (installed to C:\MinGW)
* cmake
* git

The first step to compile is to download the source code

```bash
git clone http://github.com/felipetavares/bazingatesting
```

Then, you create a directory for building the source and enters
it:

```bash
mkdir build
cd build
cp ..\compile.bat .
```

The last step is to really compile the code:

```bash
compile.bat
```

Finally, you can run the code:

```bash
bazinga.exe
```
