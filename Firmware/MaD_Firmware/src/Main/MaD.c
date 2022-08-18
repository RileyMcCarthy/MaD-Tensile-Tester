#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <dirent.h>
#include "MCP23017.h"
#include "tiny-json.h"
#include "Images.h"
#include <stdint.h>
#include "MotionPlanning.h"
#include "ControlSystem.h"
#include "W25QXX.h"
#include "DYN.h"

static Monitor monitor;
static ControlSystem control;

static Display display;
static MachineProfile machineProfile;
static MachineState machineState;
Images images; // Make images global

// Pages
static StatusPage statusPage;
static ManualPage manualPage;
static AutomaticPage automaticPage;
static CalibratePage calibrateForcePage;
static SettingsPage settingsPage;
static TestProfilePage testProfilePage;
static NavigationPage navigationPage;

static void load_images()
{
  strcpy(images.keyboardImage.name, "keyboard.bin");
  images.keyboardImage.page = 2;
  images.keyboardImage.width = 1026;
  images.keyboardImage.height = 284;
  images.keyboardImage.x0 = 0;
  images.keyboardImage.y0 = SCREEN_HEIGHT - images.keyboardImage.height;
  images.keyboardImage.backgroundColor = NULL;
  images.keyboardImage.colorToReplace = 0;
  images.keyboardImage.replacementColor = 0;
  printf("Name:%s\n", (images.keyboardImage.name));

  strcpy(images.manualPageImage.name, "manual.bin");
  images.manualPageImage.page = 2;
  images.manualPageImage.width = 200;
  images.manualPageImage.height = 200;
  images.manualPageImage.x0 = 0;
  images.manualPageImage.y0 = 0;
  images.manualPageImage.backgroundColor = 0xf800;
  images.manualPageImage.colorToReplace = 0x0000;
  images.manualPageImage.replacementColor = SECONDARYCOLOR;

  strcpy(images.automaticPageImage.name, "auto.bin");
  images.automaticPageImage.page = 2;
  images.automaticPageImage.width = 200;
  images.automaticPageImage.height = 200;
  images.automaticPageImage.x0 = 200;
  images.automaticPageImage.y0 = 0;
  images.automaticPageImage.backgroundColor = 0xf800;
  images.automaticPageImage.colorToReplace = 0;
  images.automaticPageImage.replacementColor = SECONDARYCOLOR;

  strcpy(images.statusPageImage.name, "status.bin");
  images.statusPageImage.page = 2;
  images.statusPageImage.width = 200;
  images.statusPageImage.height = 200;
  images.statusPageImage.x0 = 400;
  images.statusPageImage.y0 = 0;
  images.statusPageImage.backgroundColor = 0xf800;
  images.statusPageImage.colorToReplace = 0;
  images.statusPageImage.replacementColor = SECONDARYCOLOR;

  strcpy(images.calibratePageImage.name, "calib.bin");
  images.calibratePageImage.page = 2;
  images.calibratePageImage.width = 200;
  images.calibratePageImage.height = 200;
  images.calibratePageImage.x0 = 600;
  images.calibratePageImage.y0 = 0;
  images.calibratePageImage.backgroundColor = 0xf800;
  images.calibratePageImage.colorToReplace = 0;
  images.calibratePageImage.replacementColor = SECONDARYCOLOR;

  strcpy(images.filesPageImage.name, "files.bin");
  images.filesPageImage.page = 2;
  images.filesPageImage.width = 200;
  images.filesPageImage.height = 200;
  images.filesPageImage.x0 = 800;
  images.filesPageImage.y0 = 0;
  images.filesPageImage.backgroundColor = 0xf800;
  images.filesPageImage.colorToReplace = 0;
  images.filesPageImage.replacementColor = SECONDARYCOLOR;

  strcpy(images.navigationImage.name, "nav.bin");
  images.navigationImage.page = 2;
  images.navigationImage.width = 100;
  images.navigationImage.height = 100;
  images.navigationImage.x0 = 0;
  images.navigationImage.y0 = 200;
  images.navigationImage.backgroundColor = 0xf800;
  images.navigationImage.colorToReplace = 0x0000;
  images.navigationImage.replacementColor = SECONDARYCOLOR;

  strcpy(images.successImage.name, "check.bin");
  images.successImage.page = 2;
  images.successImage.width = 25;
  images.successImage.height = 25;
  images.successImage.x0 = 100;
  images.successImage.y0 = 200;
  images.successImage.backgroundColor = 0xf800;
  images.successImage.colorToReplace = 0;
  images.successImage.replacementColor = 0;

  strcpy(images.failImage.name, "ex.bin");
  images.failImage.page = 2;
  images.failImage.width = 25;
  images.failImage.height = 25;
  images.failImage.x0 = 100;
  images.failImage.y0 = 225;
  images.failImage.backgroundColor = 0x07e0;
  images.failImage.colorToReplace = 0;
  images.failImage.replacementColor = 0;

  strcpy(images.addImage.name, "add.bin");
  images.addImage.page = 2;
  images.addImage.width = 25;
  images.addImage.height = 25;
  images.addImage.x0 = 100;
  images.addImage.y0 = 250;
  images.addImage.backgroundColor = 0xFFFF;
  images.addImage.colorToReplace = 0;
  images.addImage.replacementColor = 0;

  strcpy(images.garbageImage.name, "garbage.bin");
  images.garbageImage.page = 2;
  images.garbageImage.width = 25;
  images.garbageImage.height = 25;
  images.garbageImage.x0 = 100;
  images.garbageImage.y0 = 275;
  images.garbageImage.backgroundColor = 0xFFFF;
  images.garbageImage.colorToReplace = 0;
  images.garbageImage.replacementColor = 0;

  strcpy(images.fileImage.name, "file.bin");
  images.fileImage.page = 2;
  images.fileImage.width = 87;
  images.fileImage.height = 100;
  images.fileImage.x0 = 150;
  images.fileImage.y0 = 200;
  images.fileImage.backgroundColor = 0xFFFF;
  images.fileImage.colorToReplace = 0;
  images.fileImage.replacementColor = 0;

  strcpy(images.folderImage.name, "folder.bin");
  images.folderImage.page = 2;
  images.folderImage.width = 100;
  images.folderImage.height = 80;
  images.folderImage.x0 = 250;
  images.folderImage.y0 = 200;
  images.folderImage.backgroundColor = 0xFFFF;
  images.folderImage.colorToReplace = 0;
  images.folderImage.replacementColor = 0;

  loading_overlay_display(&display, "Loading Image: nav", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.navigationImage));
  // display_bte_mpu_write_color_expansion(&display, PAGE2_START_ADDR, SCREEN_WIDTH, images.navigationImage.x0, images.navigationImage.y0, images.navigationImage.width, images.navigationImage.height, 0xFFFF, COLOR65K_DARKRED);
  //  display_bte_memory_copy_with_chroma_key(display, pageAddr, SCREEN_WIDTH, image->x0, image->y0, PAGE1_START_ADDR, SCREEN_WIDTH, xpos, ypos, image->width, image->height, image->backgroundColor);

  loading_overlay_display(&display, "Loading Image: manual page", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.manualPageImage));

  loading_overlay_display(&display, "Loading Image: automatic page", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.automaticPageImage));

  loading_overlay_display(&display, "Loading Image: status page", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.statusPageImage));

  loading_overlay_display(&display, "Loading Image: calibrate page", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.calibratePageImage));

  loading_overlay_display(&display, "Loading Image: files page", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.filesPageImage));

  loading_overlay_display(&display, "Loading Image: check status", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.successImage));

  loading_overlay_display(&display, "Loading Image: fail status", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.failImage));

  loading_overlay_display(&display, "Loading Image: add status", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.addImage));

  loading_overlay_display(&display, "Loading Image: garbage status", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.garbageImage));

  loading_overlay_display(&display, "Loading Image: file status", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.fileImage));

  loading_overlay_display(&display, "Loading Image: folder status", OVERLAY_TYPE_LOADING);
  display_load_image(&display, &(images.folderImage));
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
    return;
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

static MotionProfile *static_test_profile()
{
  mkdir("/sd/profiles", 0);
  MotionProfile *profile = (MotionProfile *)malloc(sizeof(MotionProfile));
  strcpy(profile->name, "/sd/profiles/Test1.mp");

  profile->number = 1;
  profile->setCount = 2;

  // Create first set
  strcpy(profile->sets[0].name, "/sd/profiles/Set_1.set");

  profile->sets[0].number = 1;
  profile->sets[0].executions = 2;
  profile->sets[0].quartetCount = 2;

  // Create first quartet
  strcpy(profile->sets[0].quartets[0].name, "/sd/profiles/qrt1.qrt");

  profile->sets[0].quartets[0].function = QUARTET_FUNC_LINE;

  profile->sets[0].quartets[0].parameters[0] = 20;  // Distance
  profile->sets[0].quartets[0].parameters[1] = 200; // Strain rate
                                                    // profile->sets[0].quartets[0].parameters[2] = 0.01; // Error

  profile->sets[0].quartets[0].dwell = 5; // 500ms

  // Create second quartet
  strcpy(profile->sets[0].quartets[1].name, "/sd/profiles/qrt2.qrt");

  profile->sets[0].quartets[1].function = QUARTET_FUNC_LINE;

  profile->sets[0].quartets[1].parameters[0] = -20; // Distance (m)
  profile->sets[0].quartets[1].parameters[1] = 20;  // Strain rate (m/s)
  // profile->sets[0].quartets[1].parameters[2] = 0.01; // Error (m)

  profile->sets[0].quartets[1].dwell = 5; // ms

  // Create second set
  strcpy(profile->sets[1].name, "/sd/profiles/Set_2.set");

  profile->sets[1].number = 2;
  profile->sets[1].executions = 3;
  profile->sets[1].quartetCount = 2;

  // Create first quartet
  strcpy(profile->sets[1].quartets[0].name, "/sd/profiles/qrt3.qrt");

  profile->sets[1].quartets[0].function = QUARTET_FUNC_LINE;

  profile->sets[1].quartets[0].parameters[0] = 10;  // Distance
  profile->sets[1].quartets[0].parameters[1] = 100; // Strain rate
  // profile->sets[1].quartets[0].parameters[2] = 0.01; // Error

  profile->sets[1].quartets[0].dwell = 5; // 500ms

  // Create second quartet
  strcpy(profile->sets[1].quartets[1].name, "/sd/profiles/qrt4.qrt");

  profile->sets[1].quartets[1].function = QUARTET_FUNC_LINE;

  profile->sets[1].quartets[1].parameters[0] = -10; // Distance
  profile->sets[1].quartets[1].parameters[1] = 50;  // Strain rate
  // profile->sets[1].quartets[1].parameters[2] = 0.01; // Error

  profile->sets[1].quartets[1].dwell = 5; // 500ms

  motion_profile_to_json(profile, profile->name);
  motion_set_to_json(&(profile->sets[0]), profile->sets[0].name);
  motion_set_to_json(&(profile->sets[1]), profile->sets[1].name);
  motion_quartet_to_json(&(profile->sets[0].quartets[0]), profile->sets[0].quartets[0].name);
  motion_quartet_to_json(&(profile->sets[0].quartets[1]), profile->sets[0].quartets[1].name);
  motion_quartet_to_json(&(profile->sets[1].quartets[0]), profile->sets[1].quartets[0].name);
  motion_quartet_to_json(&(profile->sets[1].quartets[1]), profile->sets[1].quartets[1].name);
  json_print_motion_profile(profile);

  // MotionProfile profile2;
  // json_to_motion_profile(profile->name, &profile2);
  // json_print_motion_profile(&profile2);

  return NULL;
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
typedef struct __using("jm_fullduplexserial.spin2") FDS;
#define CMD_PING 0  // test communication
#define CMD_DATA 1  // send monitor data
#define CMD_STATE 2 // send machine state
/**
 * @brief Starts the display, motion control, and all MaD board related tasks. Should never exit
 *
 */
void mad_begin()
{
  printf("Starting MAD\n");
  FDS serial;
  serial.start(50, 49, 0, 115200);
  MonitorData data;
  data.forceRaw = 1202;
  data.encoderRaw = 1104;
  data.force = 4.2;
  data.position = 105;
  data.timems = 100;
  data.timeus = 100000;
  uint8_t *ptr = &data;
  while (1)
  {
    printf("Waiting for command\n");
    int cmd = serial.rx();
    switch (cmd)
    {
    case CMD_PING:
      printf("pinging device back\n");
      serial.tx(1); // Send true
      break;
    case CMD_DATA:
      printf("Sending monitor data\n");
      for (int i = 0; i < sizeof(MonitorData); i++)
      {
        serial.tx(ptr[i]);
      }
      break;
    default:
      break;
    }
  }

  while (1)
  {
    int message = serial.rx();
    printf("Got serial message: %d\n", message);
    serial.str("Hello world\n");
    // serial.tx(50);
    _waitms(100);
  }
  if (!start_display())
  {
    printf("Error starting display\n");
    return;
  }
  loading_overlay_display(&display, "Display Initialized!", OVERLAY_TYPE_LOADING);

  // Load machine profile from SD card
  load_machine_profile();

  loading_overlay_display(&display, "Loaded Machine Profile", OVERLAY_TYPE_LOADING);

  // Load Assets from SD card to display memory
  load_images();
  loading_overlay_display(&display, "All Images Loaded", OVERLAY_TYPE_LOADING);

  machine_state_init(&machineState);

  if (monitor_begin(&monitor, &machineState, &(machineProfile.configuration), 10))
  {
    loading_overlay_display(&display, "Monitor Started", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(&display, "Monitor Failed, please reset", OVERLAY_TYPE_LOADING);
    return;
  }

  if (control_begin(&control, &machineProfile, &machineState, &(monitor.data)))
  {
    loading_overlay_display(&display, "ControlSystem Started", OVERLAY_TYPE_LOADING);
    static_test_profile(); // Create dummy test profile
  }
  else
  {
    loading_overlay_display(&display, "ControlSystem Failed, please reset", OVERLAY_TYPE_LOADING);
    return;
  }

  state_machine_set(&machineState, PARAM_SELF_CHARGE_PUMP, true);

  status_page_init(&statusPage, &display, &machineState, &machineProfile, &(monitor.data), &images);
  // manual_page_init(&manualPage, &display, &machineState, &images);
  automatic_page_init(&automaticPage, &display, &images, &machineState, &control, &(monitor.data)); //@TODO: remove structure pointer and pass data only
  // calibrate_force_page_init(&calibrateForcePage, &display, &monitor, &machineProfile, &images); //@TODO: remove structure pointer and pass data only
  // settings_page_init(&settingsPage, &display, &machineProfile, &images);
  test_profile_page_init(&testProfilePage, &display, &images);
  navigation_page_init(&navigationPage, &display, &images);

  printf("Machine propfile size:%d\n", (int)sizeof(machineProfile));
  // Begin main loop
  Page currentPage = PAGE_STATUS;
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
      //      manual_page_run(&manualPage);
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
      bool update = 0; // calibrate_force_page_run(&calibrateForcePage);
      if (update)
      {
        write_machine_profile(&machineProfile);
      }
      printf("Leaving force calibration page\n");
      break;
    }
    case PAGE_SETTINGS:
    {
      printf("Loading settings page...\n");
      /*while (settings_page_run(&settingsPage)) // Keep running settings page until navigation icon selected
      {
        printf("Updating force calibration page:%s\n", machineProfile.name);
        write_machine_profile(&machineProfile);
      }*/
      printf("Leaving settings page\n");
      break;
    }
    case PAGE_TEST_PROFILE:
    {
      test_profile_page_run(&testProfilePage);
      break;
    }
    default:
      break;
    }
    printf("Selecting new page\n");

    currentPage = navigation_page_run(&navigationPage);
  }
}
