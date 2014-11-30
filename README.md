bazingatesting
==============

#### The CCSL team recommends Atom!

Testing tech for bazinga engine.

Full lua integration and objects are supported at this stage.

1. Lua Integration  - [ok]
2. SDL Support      - [ok]
3. Map loading      - [wip]

How to compile (linux)
----------------------

You will need the following dependencies:

* build-essential
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
