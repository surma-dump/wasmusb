importScripts('https://cdn.jsdelivr.net/npm/comlinkjs@2.3.0/comlink.global.min.js');
importScripts('/a.out.js');
const mainUI = Comlink.proxy(self);
(async function () {
  function doPP(call) {
    const pp = Module._malloc(4);
    call(pp);
    const p = new Uint32Array(Module.HEAPU32.buffer, pp, 1)[0];
    const thing = new Uint32Array(Module.HEAPU32.buffer, p);
    return {pp, p, thing};
  }
  await new Promise(resolve => Module.onRuntimeInitialized = resolve);
  const {p: cameraP} = doPP(pp => Module._gp_camera_new(pp));
  const contextP = Module._gp_context_new();
  const {p: abilitiesListP} = doPP(pp => Module._gp_abilities_list_new(pp));
  Module._fill_the_list(abilitiesListP);

  const devices = [];
  const ca = Module._malloc_camera_abilities();
  for(let i = 0; i < Module._gp_abilities_list_count(abilitiesListP); i++) {
    Module._gp_abilities_list_get_abilities(abilitiesListP, i, ca);
    const vendorId = Module._camera_abilities_get_vendor(ca);
    const productId = Module._camera_abilities_get_product(ca);
    devices.push({vendorId, productId});
  }

  const device = await mainUI.getUsbPermission(devices);

  const cameraIdx = devices.findIndex(desc => desc.vendor === device.vendorId && desc.product === device.productID);
  Module._gp_abilities_list_get_abilities(abilitiesListP, cameraIdx, ca);
  Module._camera_set_abilities(cameraP, ca);

  Module._gp_camera_init(cameraP, contextP);
  const previewP = Module._camera_get_preview(cameraP, contextP);
})();
