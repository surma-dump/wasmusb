// #include "emscripten.h"
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-port-result.h>
#include <gphoto2/gphoto2-port-log.h>

int gp_port_new (GPPort **port) {
  *port = malloc(sizeof(GPPort));
  (*port)->type = GP_PORT_USB;

  return GP_OK;
}

int gp_port_free (GPPort *port) {
  return GP_OK;
}

int gp_port_close (GPPort *port) {
  return GP_OK;
}

