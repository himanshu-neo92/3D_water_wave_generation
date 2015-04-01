////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Text overlay
//

#include "../../octet.h"
#include "sky_box.h"
#include "inputs.h"
#include "UI.h"
#include "wave_create.h"
#include "colour_create.h"
#include "wave_file.h"
#include "water_wave.h"

/// Create a box with octet
int main(int argc, char **argv) {
  // set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::water_wave app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();
}


