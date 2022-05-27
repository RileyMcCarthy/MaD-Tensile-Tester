#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <dirent.h>
#include "MCP23017.h"
#include "tiny-json.h"
#include "Images.h"
#include "DYN4.h"
#include <stdint.h>
#include "MotionPlanning.h"

static Display display;
static MachineProfile machineProfile;
static DYN4 dyn4;
static MachineState machineState;
static DS3231 clock;
static ForceGauge forceGauge;
static Images images;
static Monitor monitor;
static Control control;

// Pages
static StatusPage statusPage;
static ManualPage manualPage;
static AutomaticPage automaticPage;
static CalibrateForcePage calibratePage;
static SettingsPage settingsPage;
static TestProfilePage testProfilePage;

static void load_images()
{
  strcpy(images.keyboardImage.name, "keyboard.bin");
  images.keyboardImage.page = 2;
  images.keyboardImage.width = 1026;
  images.keyboardImage.height = 284;
  images.keyboardImage.x0 = 0;
  images.keyboardImage.y0 = SCREEN_HEIGHT - images.keyboardImage.height;
  images.keyboardImage.backgroundColor = NULL;
  printf("Name:%s\n", (images.keyboardImage.name));

  strcpy(images.manualPageImage.name, "manual.bin");
  images.manualPageImage.page = 2;
  images.manualPageImage.width = 200;
  images.manualPageImage.height = 200;
  images.manualPageImage.x0 = 0;
  images.manualPageImage.y0 = 0;
  images.manualPageImage.backgroundColor = 0xf800;

  strcpy(images.automaticPageImage.name, "auto.bin");
  images.automaticPageImage.page = 2;
  images.automaticPageImage.width = 200;
  images.automaticPageImage.height = 200;
  images.automaticPageImage.x0 = 200;
  images.automaticPageImage.y0 = 0;
  images.automaticPageImage.backgroundColor = 0xf800;

  strcpy(images.statusPageImage.name, "status.bin");
  images.statusPageImage.page = 2;
  images.statusPageImage.width = 200;
  images.statusPageImage.height = 200;
  images.statusPageImage.x0 = 400;
  images.statusPageImage.y0 = 0;
  images.statusPageImage.backgroundColor = 0xf800;

  strcpy(images.calibratePageImage.name, "calib.bin");
  images.calibratePageImage.page = 2;
  images.calibratePageImage.width = 200;
  images.calibratePageImage.height = 200;
  images.calibratePageImage.x0 = 600;
  images.calibratePageImage.y0 = 0;
  images.calibratePageImage.backgroundColor = 0xf800;

  strcpy(images.filesPageImage.name, "files.bin");
  images.filesPageImage.page = 2;
  images.filesPageImage.width = 200;
  images.filesPageImage.height = 200;
  images.filesPageImage.x0 = 800;
  images.filesPageImage.y0 = 0;
  images.filesPageImage.backgroundColor = 0xf800;

  strcpy(images.navigationImage.name, "nav.bin");
  images.navigationImage.page = 2;
  images.navigationImage.width = 100;
  images.navigationImage.height = 100;
  images.navigationImage.x0 = 0;
  images.navigationImage.y0 = 200;
  images.navigationImage.backgroundColor = 0xf800;

  strcpy(images.successImage.name, "check.bin");
  images.successImage.page = 2;
  images.successImage.width = 25;
  images.successImage.height = 25;
  images.successImage.x0 = 100;
  images.successImage.y0 = 200;
  images.successImage.backgroundColor = 0xf800;

  strcpy(images.failImage.name, "ex.bin");
  images.failImage.page = 2;
  images.failImage.width = 25;
  images.failImage.height = 25;
  images.failImage.x0 = 100;
  images.failImage.y0 = 225;
  images.failImage.backgroundColor = 0x07e0;

  loading_overlay_display(display, "Loading Image: nav", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.navigationImage);

  loading_overlay_display(display, "Loading Image: manual page", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.manualPageImage);

  loading_overlay_display(display, "Loading Image: automatic page", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.automaticPageImage);

  loading_overlay_display(display, "Loading Image: status page", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.statusPageImage);

  loading_overlay_display(display, "Loading Image: calibrate page", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.calibratePageImage);

  loading_overlay_display(display, "Loading Image: files page", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.filesPageImage);

  loading_overlay_display(display, "Loading Image: check status", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.successImage);

  loading_overlay_display(display, "Loading Image: fail status", OVERLAY_TYPE_LOADING);
  display_load_image(display, images.failImage);
}

/**
 * @brief writes a machine profile object to a json file in /sd/settings/
 *
 * @param profile machine profile to write
 */
static void write_machine_profile(MachineProfile *profile)
{
  mkdir("/sd/settings", 0);
  printf("Writing machine profile to settings file\n");
  machine_profile_to_json(profile, "/sd/settings/Default.mcp");
}

/**
 * @brief Checks /sd/settings/ for a machine profile and loads it into the machine profile object, otherwise creates default profile
 *
 * @return MachineProfile* pointer to the machine profile object that was loaded or created
 *
 */
static void load_machine_profile()
{
  if (access("/sd/settings/Default.mcp", F_OK) == 0) // Check for machine profile in filesyste
  {
    printf("Opening existing profile\n");
    json_to_machine_profile(&machineProfile, "/sd/settings/Default.mcp"); // Load machine profile from file
    json_print_machine_profile(&machineProfile);                          // Print machine profile to console
  }

  printf("No machine profile found, creating default\n"); // Default machine profile does not exist, make a new one
  // MachineSettings

  strcpy(machineProfile.name, "Tensile_Test_1");

  machineProfile.number = 1;

  // MachineConfiguration
  strcpy(machineProfile.configuration.motorType, "640-DST");

  machineProfile.configuration.maxMotorTorque = 3.82;
  machineProfile.configuration.maxMotorRPM = 5000;
  machineProfile.configuration.gearDiameter = 40.0;
  machineProfile.configuration.gearPitch = 2;
  machineProfile.configuration.systemIntertia = 0.00121;
  machineProfile.configuration.staticTorque = 0.558;
  machineProfile.configuration.load = 37.8;

  strcpy(machineProfile.configuration.positionEncoderType, "QuadEncoder");

  machineProfile.configuration.positionEncoderStepsPerRev = 4 * 2048; //(LINE_NUM*4steps/rev)/(GEAR_PITCH*GEAR_TEETHmm/rev) = step/mm

  strcpy(machineProfile.configuration.forceGauge, "DS2-5N");

  machineProfile.configuration.forceGaugeScaleFactor = 1.0;
  machineProfile.configuration.forceGaugeZeroFactor = 0;

  // MachinePerformance
  machineProfile.performance.minPosition = 0.01;
  machineProfile.performance.maxPosition = 50.5;
  machineProfile.performance.maxVelocity = 200.5;
  machineProfile.performance.maxAcceleration = 100.5;
  machineProfile.performance.maxForceTensile = 3.5;
  machineProfile.performance.maxForceCompression = 3.1;
  machineProfile.performance.forceGaugeNeutralOffset = 0.5;

  write_machine_profile(&machineProfile); // Write machine profile to file
}

static MotionProfile static_test_profile()
{
  MotionProfile profile;
  strcpy(profile.name, "Tensile_Test_1");

  profile.number = 1;
  profile.setCount = 2;

  // Create first set
  strcpy(profile.sets[0].name, "Set_1");

  profile.sets[0].number = 1;
  profile.sets[0].executions = 1;
  profile.sets[0].quartetCount = 2;

  // Create first quartet
  strcpy(profile.sets[0].quartets[0].name, "Quartet_1");

  profile.sets[0].quartets[0].function = QUARTET_FUNC_SIGMOIDAL;

  profile.sets[0].quartets[0].parameters[0] = 5;     // Distance
  profile.sets[0].quartets[0].parameters[1] = 4;     // Strain rate
  profile.sets[0].quartets[0].parameters[2] = 0.001; // Error

  profile.sets[0].quartets[0].dwell = 500; // 500ms

  // Create second quartet
  strcpy(profile.sets[0].quartets[1].name, "Quartet_2");

  profile.sets[0].quartets[1].function = QUARTET_FUNC_SIGMOIDAL;

  profile.sets[0].quartets[1].parameters[0] = -5;    // Distance (m)
  profile.sets[0].quartets[1].parameters[1] = 4;     // Strain rate (m/s)
  profile.sets[0].quartets[1].parameters[2] = 0.001; // Error (m)

  profile.sets[0].quartets[1].dwell = 500; // us

  // Create second set
  strcpy(profile.sets[1].name, "Set_2");

  profile.sets[1].number = 2;
  profile.sets[1].executions = 1;
  profile.sets[1].quartetCount = 2;

  // Create first quartet
  strcpy(profile.sets[1].quartets[0].name, "Quartet_3");

  profile.sets[1].quartets[0].function = QUARTET_FUNC_SIGMOIDAL;

  profile.sets[1].quartets[0].parameters[0] = 5;     // Distance
  profile.sets[1].quartets[0].parameters[1] = 4;     // Strain rate
  profile.sets[1].quartets[0].parameters[2] = 0.001; // Error

  profile.sets[1].quartets[0].dwell = 500; // 500ms

  // Create second quartet
  strcpy(profile.sets[1].quartets[1].name, "Quartet_4");

  profile.sets[1].quartets[1].function = QUARTET_FUNC_SIGMOIDAL;

  profile.sets[1].quartets[1].parameters[0] = -5;    // Distance
  profile.sets[1].quartets[1].parameters[1] = 4;     // Strain rate
  profile.sets[1].quartets[1].parameters[2] = 0.001; // Error

  profile.sets[1].quartets[1].dwell = 500; // 500ms
  return profile;
}

/**
 * @brief Creates and initializes the display object
 *
 * @return Display* Display object if successful, NULL otherwise
 */
static bool start_display()
{
  Error err;

  // turn on diplay
  if ((err = display_begin(&display, DISPLAY_XNRESET, DISPLAY_XNSCS, DISPLAY_MOSI, DISPLAY_MISO, DISPLAY_SCK, DISPLAY_CLK, DISPLAY_DATA)) != SUCCESS)
  {
    printf("Error starting display:%d\n", err);
    return false;
  }

  display_on(&display, true);

  // Init display and background
  display_canvas_image_start_address(&display, PAGE1_START_ADDR);
  display_canvas_image_width(&display, SCREEN_WIDTH);
  display_active_window_xy(&display, 0, 0);
  display_active_window_wh(&display, SCREEN_WIDTH, SCREEN_HEIGHT);
  display_draw_square_fill(&display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
  return true;
}

/**
 * @brief Starts the display, motion control, and all MaD board related tasks. Should never exit
 *
 */
void mad_begin()
{
  printf("Starting...\n");
  mount("/sd", _vfs_open_sdcard()); // Mount SD card using default pins

  //  Begin the display
  if (!start_display())
  {
    printf("Error starting display\n");
    return;
  }
  loading_overlay_display(&display, "Display Initialized!", OVERLAY_TYPE_LOADING);

  _waitms(200);

  // Load machine profile from SD card
  load_machine_profile();
  loading_overlay_display(&display, "Loaded Machine Profile", OVERLAY_TYPE_LOADING);

  // Load Assets from SD card to display memory
  load_images();
  loading_overlay_display(&display, "All Images Loaded", OVERLAY_TYPE_LOADING);

  // Create DYN4 object and check for communication
  if (dyn4_begin(&dyn4, DYN4_RX, DYN4_TX, 0) != SUCCESS)
  {
    loading_overlay_display(&display, "DYN4 Comm Error", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(&display, "DYN4 Connected", OVERLAY_TYPE_LOADING);
  }

  // Start state machine
  if (machineState == NULL)
  {
    loading_overlay_display(&display, "State Machine Failed. Please Reset", OVERLAY_TYPE_LOADING);
    return; // Something went wrong, exit.
  }
  loading_overlay_display(&display, "State Machine Running", OVERLAY_TYPE_LOADING);

  // Connect to RTC
  Error status = ds3231_begin(&rtc, MAD_DS3231_SCL, MAD_DS3231_SDA);

  if (status != SUCCESS)
  {
    loading_overlay_display(&display, "Error connecting to RTC", OVERLAY_TYPE_LOADING);
    state_machine_set(&machineState, PARAM_MACHINE_RTC_COM, true);
  }
  else
  {
    state_machine_set(&machineState, PARAM_MACHINE_RTC_COM, true);
    loading_overlay_display(&display, "RTC Connected", OVERLAY_TYPE_LOADING);
  }

  // Connect Force Gauge
  if (force_gauge_begin(&forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX, machineProfile->configuration->forceGaugeScaleFactor, machineProfile->configuration->forceGaugeZeroFactor) != SUCCESS)
  {
    loading_overlay_display(&display, "Force gauge failed", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(&display, "Force Gauge Connected", OVERLAY_TYPE_LOADING);
  }

  monitor_init(&monitor, dyn4, forceGauge, machineProfile, 1000);
  if (monitor_begin(&monitor))
  {
    loading_overlay_display(&display, "Monitor Started", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(&display, "Monitor Failed, please reset", OVERLAY_TYPE_LOADING);
    return;
  }

  control_init(control, machineProfile, machineState, dyn4, &(monitor->data));
  if (control_begin(&control))
  {
    loading_overlay_display(&display, "Control Started", OVERLAY_TYPE_LOADING);
    control.motionProfile = static_test_profile(); // Set dummy test profile
  }
  else
  {
    loading_overlay_display(display, "Control Failed, please reset", OVERLAY_TYPE_LOADING);
    return;
  }
  state_machine_set(&machineState, PARAM_SELF_CHARGE_PUMP, true);

  status_page_init(&statusPage, display, machineState, &(monitor->data), images);
  manual_page_init(&manualPage, display, machineState, images);
  automatic_page_init(&automaticPage, display, images, machineState);
  calibrate_force_page_init(&calibratePage, display, monitor, forceGauge, machineProfile, images);
  settings_page_init(&settingsPage, display, machineProfile, images);
  test_profile_page_init(&testProfilePage, display, images);
  navigation_page_init(&navigationPage, display, images);

  // Begin main loop
  Page currentPage = PAGE_SETTINGS;
  while (1)
  {
    switch (currentPage)
    {
    case PAGE_STATUS:
    {
      printf("Loading status page\n");
      status_page_run(&statusPage);
      printf("Leaving status page\n");
      break;
    }
    case PAGE_MANUAL:
    {
      printf("Loading manual page\n");
      manual_page_run(&manualPage);
      printf("Leaving manual page\n");
      break;
    }
    case PAGE_AUTOMATIC:
    {
      printf("Loading automatic page...\n");
      automatic_page_run(&automaticPage);
      printf("Leaving automatic page\n");
      break;
    }
    case PAGE_CALIBRATION:
    {
      printf("Loading force calibration page...\n");
      bool update = calibrate_force_page_run(&calibrateForcePage);
      if (update)
      {
        write_machine_profile(&machineProfile);
        forceGauge->interpolationSlope = machineProfile->configuration->forceGaugeScaleFactor;
        forceGauge->interpolationZero = machineProfile->configuration->forceGaugeZeroFactor;
      }
      printf("Leaving force calibration page\n");
      break;
    }
    case PAGE_SETTINGS:
    {
      printf("Loading settings page...\n");
      while (settings_page_run(&settingsPage)) // Keep running settings page until navigation icon selected
      {
        write_machine_profile(&machineProfile);
      }
      printf("Leaving settings page\n");
      break;
    }
    case PAGE_TEST_PROFILE:
    {
      test_profile_page_run(&page);
      break;
    }
    default:
      break;
    }
    printf("Selecting new page\n");

    currentPage = navigation_page_run(navigationPage);
  }
}
