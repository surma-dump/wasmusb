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
  return malloc(sizeof(CameraAbilities
  ));
}

EMSCRIPTEN_KEEPALIVE
int camera_abilities_get_vendor(CameraAbilities* ca) {
  return ca->usb_vendor;
}

EMSCRIPTEN_KEEPALIVE
int camera_abilities_get_product(CameraAbilities* ca) {
  return ca->usb_product;
}
