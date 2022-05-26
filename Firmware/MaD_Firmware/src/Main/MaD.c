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

/**
 * @brief writes a machine profile object to a json file in /sd/settings/
 *
 * @param profile machine profile to write
 */
void write_machine_profile(MachineProfile *profile)
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
MachineProfile *load_machine_profile()
{
  if (access("/sd/settings/Default.mcp", F_OK) == 0) // Check for machine profile in filesyste
  {
    printf("Opening existing profile\n");
    MachineProfile *profile = json_to_machine_profile("/sd/settings/Default.mcp"); // Load machine profile from file
    json_print_machine_profile(profile);                                           // Print machine profile to console
    return profile;
  }

  printf("No machine profile found, creating default\n"); // Default machine profile does not exist, make a new one

  MachineProfile *profile = get_machine_profile(); // Get empty machine profile

  // MachineSettings

  char *name = "Tensile_Test_1";
  profile->name = (char *)malloc(strlen(name) + 1);
  strcpy(profile->name, name);

  profile->number = 1;

  // MachineConfiguration

  char *motorType = "640-DST";
  profile->configuration->motorType = (char *)malloc(strlen(motorType) + 1);
  strcpy(profile->configuration->motorType, motorType);

  profile->configuration->maxMotorTorque = 3.82;
  profile->configuration->maxMotorRPM = 5000;
  profile->configuration->gearDiameter = 40.0;
  profile->configuration->gearPitch = 2;
  profile->configuration->systemIntertia = 0.00121;
  profile->configuration->staticTorque = 0.558;
  profile->configuration->load = 37.8;

  char *positionEncoderType = "QuadEncoder";
  profile->configuration->positionEncoderType = (char *)malloc(strlen(positionEncoderType) + 1);
  strcpy(profile->configuration->positionEncoderType, positionEncoderType);

  profile->configuration->positionEncoderStepsPerRev = 4 * 2048; //(LINE_NUM*4steps/rev)/(GEAR_PITCH*GEAR_TEETHmm/rev) = step/mm

  char *forceGauge = "DS2-5N";
  profile->configuration->forceGauge = (char *)malloc(strlen(forceGauge) + 1);
  strcpy(profile->configuration->forceGauge, forceGauge);

  profile->configuration->forceGaugeScaleFactor = 1.0;
  profile->configuration->forceGaugeZeroFactor = 0;

  // MachinePerformance
  profile->performance->minPosition = 0.01;
  profile->performance->maxPosition = 50.5;
  profile->performance->maxVelocity = 200.5;
  profile->performance->maxAcceleration = 100.5;
  profile->performance->maxForceTensile = 3.5;
  profile->performance->maxForceCompression = 3.1;
  profile->performance->forceGaugeNeutralOffset = 0.5;

  write_machine_profile(profile); // Write machine profile to file
  return profile;
}

static MotionProfile *static_test_profile()
{
  char buf[50] = '\0';
  MotionProfile *profile = get_motion_profile();

  char *name = "Tensile_Test_1";
  profile->name = (char *)malloc(strlen(name) + 1);
  strcpy(profile->name, name);

  profile->number = 1;
  profile->setCount = 2;
  profile->sets = (MotionSet *)malloc(sizeof(MotionSet) * profile->setCount);

  // Create first set
  char *set1Name = "Set_1";
  profile->sets[0].name = (char *)malloc(strlen(set1Name) + 1);
  strcpy(profile->sets[0].name, set1Name);

  profile->sets[0].number = 1;
  profile->sets[0].executions = 1;
  profile->sets[0].quartetCount = 2;
  profile->sets[0].quartets = (MotionQuartet *)malloc(sizeof(MotionQuartet) * profile->sets[0].quartetCount);

  // Create first quartet
  char *quartet1Name = "Quartet_1";
  profile->sets[0].quartets[0].name = (char *)malloc(strlen(quartet1Name) + 1);
  strcpy(profile->sets[0].quartets[0].name, quartet1Name);

  profile->sets[0].quartets[0].function = QUARTET_FUNC_SIGMOIDAL;
  update_quartet_function(&(profile->sets[0].quartets[0]));

  profile->sets[0].quartets[0].parameters[0] = 5;     // Distance
  profile->sets[0].quartets[0].parameters[1] = 4;     // Strain rate
  profile->sets[0].quartets[0].parameters[2] = 0.001; // Error

  profile->sets[0].quartets[0].dwell = 500; // 500ms

  // Create second quartet
  char *quartet2Name = "Quartet_2";
  profile->sets[0].quartets[1].name = (char *)malloc(strlen(quartet2Name) + 1);
  strcpy(profile->sets[0].quartets[1].name, quartet2Name);

  profile->sets[0].quartets[1].function = QUARTET_FUNC_SIGMOIDAL;
  update_quartet_function(&(profile->sets[0].quartets[1]));

  profile->sets[0].quartets[1].parameters[0] = -5;    // Distance (m)
  profile->sets[0].quartets[1].parameters[1] = 4;     // Strain rate (m/s)
  profile->sets[0].quartets[1].parameters[2] = 0.001; // Error (m)

  profile->sets[0].quartets[1].dwell = 500; // us

  // Create second set
  char *set2Name = "Set_2";
  profile->sets[1].name = (char *)malloc(strlen(set2Name) + 1);
  strcpy(profile->sets[1].name, set2Name);

  profile->sets[1].number = 2;
  profile->sets[1].executions = 1;
  profile->sets[1].quartetCount = 2;
  profile->sets[1].quartets = (MotionQuartet *)malloc(sizeof(MotionQuartet) * profile->sets[1].quartetCount);

  // Create first quartet
  char *quartet3Name = "Quartet_3";
  profile->sets[1].quartets[0].name = (char *)malloc(strlen(quartet3Name) + 1);
  strcpy(profile->sets[1].quartets[0].name, quartet3Name);

  profile->sets[1].quartets[0].function = QUARTET_FUNC_SIGMOIDAL;
  update_quartet_function(&(profile->sets[1].quartets[0]));

  profile->sets[1].quartets[0].parameters[0] = 5;     // Distance
  profile->sets[1].quartets[0].parameters[1] = 4;     // Strain rate
  profile->sets[1].quartets[0].parameters[2] = 0.001; // Error

  profile->sets[1].quartets[0].dwell = 500; // 500ms

  // Create second quartet
  char *quartet4Name = "Quartet_4";
  profile->sets[1].quartets[1].name = (char *)malloc(strlen(quartet4Name) + 1);
  strcpy(profile->sets[1].quartets[1].name, quartet4Name);

  profile->sets[1].quartets[1].function = QUARTET_FUNC_SIGMOIDAL;
  update_quartet_function(&(profile->sets[1].quartets[1]));

  profile->sets[1].quartets[1].parameters[0] = -5;    // Distance
  profile->sets[1].quartets[1].parameters[1] = 4;     // Strain rate
  profile->sets[1].quartets[1].parameters[2] = 0.001; // Error

  profile->sets[1].quartets[1].dwell = 500; // 500ms
  return profile;
}

/**
 * @brief Creates and initializes the display object
 *
 * @return Display* Display object if successful, NULL otherwise
 */
Display *start_display()
{
  Error err;
  Display *display = (Display *)malloc(sizeof(Display));

  // turn on diplay
  if ((err = display_begin(display, DISPLAY_XNRESET, DISPLAY_XNSCS, DISPLAY_MOSI, DISPLAY_MISO, DISPLAY_SCK, DISPLAY_CLK, DISPLAY_DATA)) != SUCCESS)
  {
    printf("Error starting display:%d\n", err);
    return NULL;
  }

  display_on(display, true);

  // Init display and background
  display_canvas_image_start_address(display, PAGE1_START_ADDR);
  display_canvas_image_width(display, SCREEN_WIDTH);
  display_active_window_xy(display, 0, 0);
  display_active_window_wh(display, SCREEN_WIDTH, SCREEN_HEIGHT);
  display_draw_square_fill(display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
  return display;
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
  Display *display = start_display();
  if (display == NULL)
  {
    printf("Error starting display\n");
    return;
  }
  loading_overlay_display(display, "Display Initialized!", OVERLAY_TYPE_LOADING);

  _waitms(200);

  // Load machine profile from SD card
  MachineProfile *machineProfile = load_machine_profile();
  loading_overlay_display(display, "Loaded Machine Profile", OVERLAY_TYPE_LOADING);

  // Create the images object that contains all information required to draw the images on the display from SD card
  Images *images = create_images();

  // Load Assets from SD card to display memory
  image_load_assets(images, display);
  loading_overlay_display(display, "All Assets Loaded", OVERLAY_TYPE_LOADING);

  // Create DYN4 object and check for communication
  DYN4 *dyn4 = dyn4_create();
  if (dyn4_begin(dyn4, DYN4_RX, DYN4_TX, 0) != SUCCESS)
  {
    loading_overlay_display(display, "DYN4 Comm Error", OVERLAY_TYPE_LOADING);
    _waitms(500);
  }
  else
  {
    loading_overlay_display(display, "DYN4 Connected", OVERLAY_TYPE_LOADING);
  }

  // Start state machine
  MachineState *machineState = machine_state_create();

  if (machineState == NULL)
  {
    loading_overlay_display(display, "State Machine Failed. Please Reset", OVERLAY_TYPE_LOADING);
    return; // Something went wrong, exit.
  }
  loading_overlay_display(display, "State Machine Running", OVERLAY_TYPE_LOADING);

  // Connect to RTC
  DS3231 *rtc = ds3231_create();
  Error status = ds3231_begin(rtc, MAD_DS3231_SCL, MAD_DS3231_SDA);

  if (status != SUCCESS)
  {
    loading_overlay_display(display, "Error connecting to RTC", OVERLAY_TYPE_LOADING);
    machineState->machineCheckParameters.rtcCom = true;
  }
  else
  {
    machineState->machineCheckParameters.rtcCom = true;
    loading_overlay_display(display, "RTC Connected", OVERLAY_TYPE_LOADING);
  }

  // Connect Force Gauge
  ForceGauge *forceGauge = force_gauge_create();
  if (force_gauge_begin(forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX, machineProfile->configuration->forceGaugeScaleFactor, machineProfile->configuration->forceGaugeZeroFactor) != SUCCESS)
  {
    loading_overlay_display(display, "Force gauge failed", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(display, "Force Gauge Connected", OVERLAY_TYPE_LOADING);
  }

  Monitor *monitor = monitor_create();
  if (monitor_begin(monitor, dyn4, forceGauge, machineProfile, 1000))
  {
    loading_overlay_display(display, "Monitor Started", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(display, "Monitor Failed, please reset", OVERLAY_TYPE_LOADING);
    return;
  }

  Control *control = control_create(machineProfile, machineState, dyn4, &(monitor->data));
  if (control_begin(control))
  {
    loading_overlay_display(display, "Control Started", OVERLAY_TYPE_LOADING);
    control->motionProfile = static_test_profile(); // Set dummy test profile
  }
  else
  {
    loading_overlay_display(display, "Control Failed, please reset", OVERLAY_TYPE_LOADING);
    return;
  }
  machineState->selfCheckParameters.chargePump = true;

  // Begin main loop
  Pages currentPage = PAGE_STATUS;
  while (1)
  {
    switch (currentPage)
    {
    case PAGE_STATUS:
    {
      printf("Loading status page\n");
      StatusPage *statusPage = status_page_create(display, machineState, &(monitor->data), images);
      status_page_run(statusPage);
      status_page_destroy(statusPage);

      printf("Leaving status page\n");
      break;
    }
    case PAGE_MANUAL:
    {
      printf("Loading manual page\n");
      ManualPage *manualPage = manual_page_create(display, machineState, images);
      manual_page_run(manualPage);
      manual_page_destroy(manualPage);
      printf("Leaving manual page\n");
      break;
    }
    case PAGE_AUTOMATIC:
    {
      printf("Loading automatic page...\n");
      AutomaticPage *automaticPage = automatic_page_create(display, images, machineState);
      automatic_page_run(automaticPage);
      automatic_page_destroy(automaticPage);
      // TestProfilePage *page = test_profile_page_create(display, images);
      // test_profile_page_run(page);
      printf("Leaving automatic page\n");
      break;
    }
    case PAGE_CALIBRATION:
    {
      printf("Loading force calibration page...\n");
      CalibrateForcePage *calibrateForcePage = calibrate_force_page_create(display, monitor, forceGauge, machineProfile, images);
      bool update = calibrate_force_page_run(calibrateForcePage);
      calibrate_force_page_destroy(calibrateForcePage);
      if (update)
      {
        write_machine_profile(machineProfile);
        forceGauge->interpolationSlope = machineProfile->configuration->forceGaugeScaleFactor;
        forceGauge->interpolationZero = machineProfile->configuration->forceGaugeZeroFactor;
      }
      printf("Leaving force calibration page\n");
      break;
    }
    case PAGE_SETTINGS:
    {
      printf("Loading settings page...\n");
      SettingsPage *settingsPage = settings_page_create(display, machineProfile, images);
      while (settings_page_run(settingsPage)) // Keep running settings page until navigation icon selected
      {
        write_machine_profile(machineProfile);
      }
      settings_page_destroy(settingsPage);
      printf("Leaving settings page\n");
      break;
    }
    case PAGE_TEST_PROFILE:
    {
      TestProfilePage *page = test_profile_page_create(display, images);
      test_profile_page_run(page);
      test_profile_page_destroy(page);
      break;
    }
    default:
      break;
    }
    printf("Selecting new page\n");
    NavigationPage *navigationPage = navigation_page_create(display, images);
    currentPage = navigation_page_run(navigationPage);
    navigation_page_destroy(navigationPage);
  }
}
