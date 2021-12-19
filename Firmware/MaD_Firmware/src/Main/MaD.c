#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <dirent.h>
#include "MCP23017.h"
#include "Images.h"

static void write_machine_profile(MachineProfile *profile)
{
  mkdir("/sd/settings", 0);
  chdir("/sd/settings");
  FILE *file = fopen("Default.mcp", "w");
  if (file != NULL)
  {
    printf("Writing machine profile to settings file\n");
    machine_profile_to_json(profile, file);
  }
  fclose(file);
}

static MachineProfile *load_machine_profile()
{
  // Check for machine profile in filesystem
  chdir("/sd/settings");
  // FILE *jsonFile = fopen("Default.mcp", "r+");
  if (access("Default.mcp", F_OK) == 0)
  {
    FILE *jsonFile = fopen("Default.mcp", "r");
    printf("Loading machine profile from settings file\n");
    MachineProfile *profile = get_machine_profile(); // json_to_machine_profile(jsonFile);
    json_print_machine_profile(profile);
    fclose(jsonFile);
    return profile;
  }

  printf("No machine profile found, creating default\n");
  // Default machine profile does not exist, make a new one
  MachineProfile *profile = get_machine_profile();
  // MachineSettings
  char *name = "Tensile_Test_1";
  profile->name = (char *)malloc(strlen(name) + 1);
  strcpy(profile->name, name);
  profile->number = 1;
  // MachineConfiguration
  char *motorType = "640-DST";
  profile->configuration->motorType = (char *)malloc(strlen(motorType) + 1);
  strcpy(profile->configuration->motorType, motorType);
  profile->configuration->maxMotorTorque = 3.82;    // make float
  profile->configuration->maxMotorRPM = 5000;       // make float
  profile->configuration->gearDiameter = 25.4;      // make float
  profile->configuration->gearPitch = 1.0;          // make float
  profile->configuration->systemIntertia = 0.00121; // make float
  profile->configuration->staticTorque = 0.558;     // make float
  profile->configuration->load = 37.8;              // make float
  char *positionEncoderType = "QuadEncoder";
  profile->configuration->positionEncoderType = (char *)malloc(strlen(positionEncoderType) + 1);
  strcpy(profile->configuration->positionEncoderType, positionEncoderType);
  profile->configuration->positionEncoderScaleFactor = 23;
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

  write_machine_profile(profile);
  return profile;
}

static Display *start_display()
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

static NavKey *start_navkey()
{
  NavKey *navkey = navkey_create(I2C_SCL, I2C_SDA, I2C_ADDR);
  navkey_reset(navkey);
  navkey_begin(navkey, INT_DATA | WRAP_ENABLE | DIRE_RIGHT | IPUP_ENABLE);

  navkey_write_counter(navkey, (int32_t)0);  /* Reset the counter value */
  navkey_write_max(navkey, (int32_t)10000);  /* Set the maximum threshold*/
  navkey_write_min(navkey, (int32_t)-10000); /* Set the minimum threshold */
  navkey_write_step(navkey, (int32_t)100);   /* Set the step to 1*/

  navkey_write_double_push_period(navkey, 30); /*Set a period for the double push of 300ms */
  return navkey;
}

/**
 * @brief Starts the display, motion control, and all MaD board related tasks
 *
 */
void mad_begin()
{
  printf("Starting...\n");
  mount("/sd", _vfs_open_sdcard());

  _waitms(1000);
  //  Begin the display
  Display *display = start_display();
  if (display == NULL)
  {
    printf("Error starting display\n");
    return;
  }

  loading_overlay_display(display, "Display Initialized!", OVERLAY_TYPE_LOADING);

  MachineProfile *machineProfile = load_machine_profile();

  Images *images = create_images();

  // Load Assets from SD card
  printf("Testingh:%s\n", images->statusPageImage->name);
  image_load_assets(images, display);
  printf("after load%s\n", images->statusPageImage->name);

  // Start NavKey
  NavKey *navkey = start_navkey();

  // Initialize IO Expansion(MCP23017)
  MCP23017 *mcp = mcp23017_create();
  mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL); // issues using i2c slow with delay not saving, change to new i2c
                                                      //  mcp_set_direction(mcp, 4, MCP23017_OUTPUT);         // zero is output
  loading_overlay_display(display, "MCP23017 Connected", OVERLAY_TYPE_LOADING);

  // Load Machine Profile
  loading_overlay_display(display, "Loading Machine Profile", OVERLAY_TYPE_LOADING);

  // Create DYN4 object (currently no communication with DYN4)
  DYN4 *dyn4 = dyn4_create();
  dyn4_begin(dyn4, DYN4_RX, DYN4_TX, DYN4_ADDR);
  loading_overlay_display(display, "DYN4 Connected", OVERLAY_TYPE_LOADING);

  // Connect to IMU
  // Start state machine (needs IO expansion)
  MachineState *machineState = state_machine_run();
  _waitms(100);

  if (machineState == NULL)
  {
    loading_overlay_display(display, "State Machine Failed. Please Reset", OVERLAY_TYPE_LOADING);
    return;
  }
  machineState->selfCheckParameters.chargePumpOK = true;
  loading_overlay_display(display, "State Machine Running", OVERLAY_TYPE_LOADING);

  // Connect to RTC
  DS3231 *rtc = ds3231_create();
  Error status = ds3231_begin(rtc, MAD_DS3231_SCL, MAD_DS3231_SDA);

  if (status != SUCCESS)
  {
    loading_overlay_display(display, "Error connecting to RTC", OVERLAY_TYPE_LOADING);
    machineState->selfCheckParameters.rtcReady = false;
  }
  else
  {
    machineState->selfCheckParameters.rtcReady = true;
    loading_overlay_display(display, "RTC Connected", OVERLAY_TYPE_LOADING);
  }

  // Connect Force Gauge
  ForceGauge *forceGauge = force_gauge_create();
  if (force_gauge_begin(forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX, -666, 2048402) == SUCCESS)
  {
    loading_overlay_display(display, "Force Gauge Connected", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(display, "Force Gauge Failed to Connect", OVERLAY_TYPE_LOADING);
  }

  // Start motion control (needs state machine,encoder, dyn4, rtc, forcegauge), responsible for constantly
  // gather information from encoder,dyn4,rtc,forcegauge, and update/log data
  // rename motioncog to monitor
  // motion_run(mcp, dyn4, forceGauge, 100); // returns null if failed to start cog

  // Begin main loop
  Pages currentPage = PAGE_STATUS;
  while (1)
  {
    switch (currentPage)
    {
    case PAGE_STATUS:
      printf("Loading status page\n");
      StatusPage *statusPage = status_page_create(display, machineState, images);
      status_page_run(statusPage);
      status_page_destroy(statusPage);
      printf("Leaving status page\n");
      break;
    case PAGE_MANUAL:
      printf("Loading manual page\n");
      ManualPage *manualPage = manual_page_create(display, machineState, images);
      manual_page_run(manualPage);
      manual_page_destroy(manualPage);
      printf("Leaving manual page\n");
      break;
    case PAGE_AUTOMATIC:
      printf("Loading automatic page...\n");
      AutomaticPage *automaticPage = automatic_page_create(display, images);
      automatic_page_run(automaticPage);
      automatic_page_destroy(automaticPage);
      printf("Leaving automatic page\n");
      break;
    case PAGE_CALIBRATION:
      printf("Loading force calibration page...\n");
      CalibrateForcePage *calibrateForcePage = calibrate_force_page_create(display, machineState, forceGauge, machineProfile, images);
      bool update = calibrate_force_page_run(calibrateForcePage);
      calibrate_force_page_destroy(calibrateForcePage);
      if (update)
        write_machine_profile(machineProfile);
      printf("Leaving force calibration page\n");
      break;
    default:
      break;
    }

    printf("Selecting new page\n");
    NavigationPage *navigationPage = navigation_page_create(display, images);
    currentPage = navigation_page_run(navigationPage);
    navigation_page_destroy(navigationPage);
  }
}
