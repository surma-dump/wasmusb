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

EM_JS(int, gp_port_open, (GPPort* port), {
  const sabView = new Int32Array(new SharedArrayBuffer(4));
  sabView[0] = -1;
  mainUI.usb_port_open(sabView);
  Atomics.wait(sabView, 0, -1);
  return 0; // GP_OK
});

int gp_system_mkdir(const char* name) {
  return GP_OK;
}

int gp_port_set_timeout(GPPort* port, int timeout) {
  return GP_OK;
}

EM_JS(int, gp_port_get_settings, (GPPort* port, GPPortSettings* settings), {
  const sabView = new Int32Array(new SharedArrayBuffer(4));
  sabView[0] = -1;
  mainUI.usb_port_get_max_packet_size(sabView);
  Atomics.wait(sabView, 0, -1);
  console.log(sabView);
  Module._port_settings_set_max_packet_size(settings, sabView[0]);
  return 0; // GP_OK
});

EMSCRIPTEN_KEEPALIVE
int port_settings_set_max_packet_size(GPPortSettings* settings, int ps) {
  settings->usb.maxpacketsize = ps;
}

EM_JS(int, gp_port_write, (GPPort* port, const char* data, int size), {
  const lock = new Int32Array(new SharedArrayBuffer(4));
  lock[0] = -1;
  mainUI.usb_port_write(new Uint8Array(Module.HEAP8.buffer, data, size), lock);
  Atomics.wait(lock, 0, -1);
  // Return bytes written
  return lock[0];
});

EM_JS(int, gp_port_read, (GPPort* port, char* data, int size), {
  const sab = new SharedArrayBuffer(size + 4);
  const lock = new Int32Array(sab, 0, 1);
  const dataView = new Uint8Array(sab, 4);
  lock[0] = -1;
  mainUI.usb_port_read(dataView, lock);
  Atomics.wait(lock, 0, -1);
  Module.HEAP8.set(dataView, data);

  // Return bytes read
  return lock[0];
});

EM_JS(int, gp_port_usb_msg_class_write, (GPPort* port, int request, int value, int index, char* data, int size), {
  const lock = new Int32Array(new SharedArrayBuffer(4));
  lock[0] = -1;
  mainUI.usb_port_msg_class_write(request, value, index, new Uint8Array(Module.HEAP8.buffer, data, size), lock);
  Atomics.wait(lock, 0, -1);
  // Return bytes written
  return lock[0];
});

struct _GPPortPrivateCore {
	char error[2048];	/**< Internal kept error message. */

	// struct _GPPortInfo info;	/**< Internal port information of this port. */
	// GPPortOperations *ops;	/**< Internal port operations. */
	// lt_dlhandle lh;		/**< Internal libtool library handle. */
};


const char * gp_port_get_error (GPPort *port)
{
	if (port && port->pc && strlen (port->pc->error))
    printf("ERROR: %s\n", port->pc->error);
  else
    printf("ERROR: With no description\n");

	return "";
}
