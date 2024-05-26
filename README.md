
## Windows Installation

We use MinGW for building the project, but you can change it depending on where you have SDL installed

Go to project root and run:

Change "C:/lambda_engine_dev_lib" with the installation directory you want it to be

```
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="C:/lambda_engine_dev_lib"
```

Then go to the build directory to build and install it:

In my case I will use mingw32-make to install it

```
mingw32-make install
```
