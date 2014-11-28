bazingatesting
==============

Testing tech for bazinga engine
Currently coding lua integration

How to compile
--------------

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
