#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <dirent.h>
#include "MCP23017.h"
#include "tiny-json.h"
#include "Images.h"
#include "DYN4.h"

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
    MachineProfile *profile = json_to_machine_profile(jsonFile);

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

static void test_mcp23017()
{
  MCP23017 *mcp = mcp23017_create();
  mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL);

  printf("MCP23017 begin\n");
  int pin = 4;
  mcp_set_direction(mcp, pin, DIRA, 1); // zero is output
  printf("Direction:%d\n", mcp_get_direction(mcp, pin, DIRA));
  pause(500);
  mcp_set_direction(mcp, pin, DIRA, 0);
  printf("direction:%d\n", mcp_get_direction(mcp, pin, DIRA));
  pause(500);
  mcp_set_pin(mcp, pin, DIRA, 0);
  printf("Output:%d\n", mcp_get_pin(mcp, pin, DIRA));
  pause(500);
  mcp_set_pin(mcp, pin, DIRA, 1);
  printf("Output:%d\n", mcp_get_pin(mcp, pin, DIRA));
  pause(500);
  // mcp23017_destroy(mcp);
  return;
}

static void dyn4_test()
{
  MCP23017 *mcp = mcp23017_create();
  mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL);
  mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
  printf("MCP23017 begin\n");
  pause(500);

  DYN4 *dyn4 = dyn4_create();
  dyn4_begin(dyn4, DYN4_RX, DYN4_TX, DYN4_ADDR);
  DYN4_Status status;
  while (dyn4_get_status(dyn4, &status) != SUCCESS)
  {
    printf("Error getting status\n");
    _waitms(100);
  }

  printf("status:alarm(%d),onrange(%d),motorFree(%d)motorBusy(%d)\n", status.alarm, status.onRange, status.motorFree, status.motorBusy);
  low(11);
  while (dyn4_get_status(dyn4, &status) != SUCCESS)
  {
    printf("Error getting status\n");
    _waitms(100);
  }
  printf("status:alarm(%d),onrange(%d),motorFree(%d)motorBusy(%d)\n", status.alarm, status.onRange, status.motorFree, status.motorBusy);

  dyn4_send_command(dyn4, 0x0a, 100);

  while (1)
  {
    printf("position:%d\n", dyn4->encoder.value());
    while (dyn4_get_status(dyn4, &status) != SUCCESS)
    {
      printf("Error getting status\n");
      _waitms(100);
    }
    printf("status:alarm(%d),onrange(%d),motorFree(%d)motorBusy(%d)\n", status.alarm, status.onRange, status.motorFree, status.motorBusy);
    _waitms(1000);
  }
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
  image_load_assets(images, display);

  // Load Machine Profile
  loading_overlay_display(display, "Loading Machine Profile", OVERLAY_TYPE_LOADING);

  // Create DYN4 object (currently no communication with DYN4)
  DYN4 *dyn4 = dyn4_create();
  if (dyn4_begin(dyn4, DYN4_RX, DYN4_TX, DYN4_ADDR) != SUCCESS)
  {
    loading_overlay_display(display, "DYN4 Error", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(display, "DYN4 Connected", OVERLAY_TYPE_LOADING);
  }

  // Connect to IMU
  // Start state machine (needs IO expansion)
  MachineState *machineState = machine_state_create();

  if (machineState == NULL)
  {
    loading_overlay_display(display, "State Machine Failed. Please Reset", OVERLAY_TYPE_LOADING);
    return;
  }
  loading_overlay_display(display, "State Machine Running", OVERLAY_TYPE_LOADING);

  // Connect to RTC
  DS3231 *rtc = ds3231_create();
  Error status = ds3231_begin(rtc, MAD_DS3231_SCL, MAD_DS3231_SDA);

  if (status != SUCCESS)
  {
    loading_overlay_display(display, "Error connecting to RTC", OVERLAY_TYPE_LOADING);
    machineState->machineCheckParameters.rtcCom = false;
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
  if (monitor_begin(monitor, dyn4, forceGauge, 100))
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
      printf("Loading status page\n");
      StatusPage *statusPage = status_page_create(display, machineState, &(monitor->data), images);
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
    case PAGE_SETTINGS:
      printf("Loading settings page...\n");
      SettingsPage *settingsPage = settings_page_create(display, machineProfile, images);
      while (settings_page_run(settingsPage)) // Keep running settings page until navigation icon selected
      {
        write_machine_profile(machineProfile);
      }
      settings_page_destroy(settingsPage);
      printf("Leaving settings page\n");
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
