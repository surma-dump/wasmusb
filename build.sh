#!/bin/bash

cp portshim.c libgphoto2
cd libgphoto2
docker run --rm -v $(pwd):/src -v ~/.homebrew:/homebrewlol trzeci/emscripten \
  emcc -O0 \
    -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' \
    -s ASSERTIONS=1 \
    -I . -I libgphoto2_port -I libgphoto2/libgphoto2_port \
     -Wno-error -Wno-implicit-function-declaration \
    -I /homebrewlol/include \
    -D _GPHOTO2_INTERNAL_CODE \
    -D LOCALEDIR="\"lol\"" \
    -D CAMLIBS="\"lol\"" \
    -s EXPORTED_FUNCTIONS='["_gp_context_new","_gp_camera_new","_malloc","_free"]' \
    libgphoto2/ahd_bayer.c \
    libgphoto2/bayer.c \
    libgphoto2/exif.c \
    libgphoto2/gamma.c \
    libgphoto2/gphoto2-abilities-list.c \
    libgphoto2/gphoto2-camera.c \
    libgphoto2/gphoto2-context.c \
    libgphoto2/gphoto2-file.c \
    libgphoto2/gphoto2-filesys.c \
    libgphoto2/gphoto2-list.c \
    libgphoto2/gphoto2-result.c \
    libgphoto2/gphoto2-setting.c \
    libgphoto2/gphoto2-version.c \
    libgphoto2/gphoto2-widget.c \
    libgphoto2/jpeg.c \
    libgphoto2_port/libgphoto2_port/gphoto2-port-log.c \
    portshim.c
mv a.out* ../
