#!/bin/bash

# buck build //apps/opengl:main

# ./main

g++ pixel_operation.cpp -g -std=c++11 -D DEBUG=1 -I../../build/include -I../../src -L../../build/bin -lHalide -o ./pixel_operation.a
DYLD_LIBRARY_PATH=../../build/bin ./pixel_operation.a

g++ -g -c -std=c++11 -D DEBUG=1 gl_context.cpp -I/usr/local/include -o ./libgl_context.a
g++ -g -c -std=c++11 -D DEBUG=1 gl_util.cpp -I/usr/local/include -o ./libgl_util.a

g++ -g -std=c++11 -D DEBUG=1 main.cpp ./pixel_operation.o \
-L./ -lgl_context -lgl_util \
-I../../build/include \
-I../../test/performance \
-L/usr/local/opt/jpeg-turbo/lib -lturbojpeg \
-framework OpenGL -L/usr/local/lib -lglfw3 -lglew -I/usr/local/include \
-o ./main

./main