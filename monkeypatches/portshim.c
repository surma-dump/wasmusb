#include "emscripten.h"
#include <gphoto2/gphoto2-abilities-list.h>
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-port-result.h>
#include <gphoto2/gphoto2-port-log.h>
#include "ptp_shim.h"

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

int gp_port_usb_find_device(GPPort *port, int id_vendor, int id_product) {
  return GP_OK;
}

void lt_dlinit() {}

int lt_dlopenext(const char* name) {
  return 1;
}

void* lt_dlsym(void* handle, const char* name) {
  if(strcmp(name, "camera_init") == 0) {
    return (void*) camera_init;
  }
}

void lt_dlclose(void* handle) {}

void lt_dlexit() {}


struct _CameraAbilitiesList {
	int count;
	int maxcount;
	CameraAbilities *abilities;
};

void fill_the_list(CameraAbilitiesList *list) {
  CameraText text;

  camera_id(&text);
  camera_abilities(list);

  int new_count = gp_abilities_list_count (list);

  for (int x = 0; x < new_count; x++) {
    // CameraAbility ability;
    // gp_abilities_list_get_abilities(list, x, &ability);
    strcpy (list->abilities[x].id, text.text);
    strcpy (list->abilities[x].library, "ptp2");
  }
}

EMSCRIPTEN_KEEPALIVE
CameraAbilities* malloc_camera_abilities() {
  return malloc(sizeof(CameraAbilities));
}

EMSCRIPTEN_KEEPALIVE
CameraText* malloc_camera_text() {
  return malloc(sizeof(CameraText));
}

EMSCRIPTEN_KEEPALIVE
int camera_abilities_get_vendor(CameraAbilities* ca) {
  return ca->usb_vendor;
}

EMSCRIPTEN_KEEPALIVE
int camera_abilities_get_product(CameraAbilities* ca) {
  return ca->usb_product;
}

EMSCRIPTEN_KEEPALIVE
int camera_set_abilities(Camera* cam, CameraAbilities* ca) {
  return gp_camera_set_abilities(cam, *ca);
}

EMSCRIPTEN_KEEPALIVE
int print_camera_text(CameraText* ct) {
  printf("%s\n", ct->text);
}
