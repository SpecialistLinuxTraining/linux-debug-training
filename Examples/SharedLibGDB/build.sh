# Clean up any previous builds
./clean.sh

# compile shared library "libmylib.so"
gcc -shared -o libmylib.so -fPIC mylib.c

mkdir symbols
# compile shared library "libmylib.so" with debugging symbols
gcc -shared -g -o symbols/libmylib.so.debug -fPIC mylib.c


# Compile application using libmylib.so
gcc -o main main.c -L. -lmylib
