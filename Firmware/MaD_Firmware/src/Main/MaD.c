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
    // FILE *jsonFile = fopen("Default.mcp", "r");
    // printf("Loading machine profile from settings file\n");
    MachineProfile *profile = json_to_machine_profile("Default.mcp");

    json_print_machine_profile(profile);
    return profile;
  }

  printf("No machine profile found, creating default\n");
  // Default machine profile does not exist, make a new one
  MachineProfile *profile = (MachineProfile *)malloc(sizeof(MachineProfile)); //= get_machine_profile();
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
  profile->configuration->gearDiameter = 40.0;      // make float
  profile->configuration->gearPitch = 2;            // make float
  profile->configuration->systemIntertia = 0.00121; // make float
  profile->configuration->staticTorque = 0.558;     // make float
  profile->configuration->load = 37.8;              // make float
  char *positionEncoderType = "QuadEncoder";
  profile->configuration->positionEncoderType = (char *)malloc(strlen(positionEncoderType) + 1);
  strcpy(profile->configuration->positionEncoderType, positionEncoderType);
  profile->configuration->positionEncoderStepsPerRev = (4095 * 4); //(LINE_NUM*4steps/rev)/(GEAR_PITCH*GEAR_TEETHmm/rev) = step/mm
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

/*static void test_sd_card()
{
  int sd = mount("/sd", _vfs_open_sdcard());
  if (sd == 0)
  {
    printf("SD Card Mounted\n");
    @ @-314, 15 + 405, 29 @ @ static void test_sd_card()
    {
      printf("Mount error:%d\n", _geterror());
    }
    mkdir("/sd/test_dir");
    chdir("/sd/test_dir");
    char *dirName[50];
    getcwd(dirName, 50);
    printf("Current Directory:%s\n", dirName);
    if (strcmp(dirName, "/sd/test_dir") == 0)
    {
      printf("Directory Change Successful\n");
    }
    else
    {
      printf("Directory Change Failed\n");
    }

    FILE *fp = fopen("test.txt", "w");
    if (fp == NULL)
    {
      printf("Error file could not be opened(Error: %d)\n", _geterror());
      return;
    }
    fprintf(fp, "Hello World\n");
    fclose(fp);
    fp = fopen("test.txt", "r");
    char buffer[100];
    fgets(buffer, 100, fp);
    if (strcmp(buffer, "Hello World\n") == 0)
      @ @-332, 8 + 437, 48 @ @ static void test_sd_card() else
      {
        printf("SD card read/write failed\n");
        return;
      }
    fclose(fp);

    char directory[100] = "/sd";
    while (1)
    {
      DIR *dir = opendir(directory);
      if (dir == NULL)
      {
        printf("Error: %d\n", _geterror());
        return;
      }
      struct dirent *dirent;
      while ((dirent = readdir(dir)) != NULL)
      {
        printf("%s\n", dirent->d_name);
      }
      printf("-----------------------------------\n");
      printf("Please type new directory to enter or .. for back:");
      char newDir[100];
      scanf("%s", newDir);
      if (strcmp(newDir, "..") == 0)
      {
        for (int i = strlen(directory) - 1; i >= 0; i--)
        {
          if (directory[i] == '/')
          {
            directory[i] = '\0';
            break;
          }
        }
      }
      else
      {
        strcat(directory, "/");
        strcat(directory, newDir);
      }
      closedir(dir);
    }

    // unmount("/sd");
  }*/

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
  while (1)
  {
    mcp_update_register(mcp);

    if (mcp_get_pin(mcp, DISTANCE_LIMIT_MIN, DISTANCE_LIMIT_MIN_REGISTER) == 1) // LOWER
    {
      // Error machine out of bounds
      // Update state machine
      printf("lower Limit on\n");
      // state_machine_set(control->stateMachine, PARAM_MOTION_CONDITION, MOTION_LOWER);
    }
    else
    {
      printf("lower Limit off\n");
    }
    if (mcp_get_pin(mcp, DISTANCE_LIMIT_MAX, DISTANCE_LIMIT_MAX_REGISTER) == 1) // UPPER
    {
      // Error machine out of bounds (Upper Limit)
      // Update state machine
      printf("Upper Limit\n");
    }
    else
    {
      printf("Upper Limit off\n");
    }
    _waitms(500);
  }
  return;
}

static float central_difference_order_two(float d1, float d0, float dn1, float h)
{
  return (dn1 - 2 * d0 + d1) / powf(h, 2);
}
static float central_difference_order_three(float d2, float d1, float dn1, float dn2, float h)
{
  return (-0.5 * dn2 + dn1 - d1 + 0.5 * d2) / powf(h, 3);
}

/*static void test_motion_profile()
{
  double d_max = 1;
  double sr = 4;
  double error = 0.005;
  double max_velocity_rpm = 12.21 * 1 * (48 + 3)(48 + 3) / 16; //((float)value / 1000.0) * (60.0 / 80.0);              // um/s to rpm
  double max_acceleration_rpms = 30 * 635.78 * 1;
  double v_max = (0.08) * (max_velocity_rpm) / (60.0);
  double a_max = (0.08) * (max_acceleration_rpms) / (60.0);
  SetPoint *setpoint = create_empty_setpoint();
  double t = 0;
  while (abs(d_max - setpoint->x) > error)
  {
    simulate_profile(setpoint, t, d_max, v_max, a_max, sigmoid, d_max, sr, error);
    printf("%f,%f,%f,%f\n", t + 0.0386, setpoint->x, setpoint->v, setpoint->a);
    t += error * 4;
  }
  return;
}*/

// double *args = float distance, float strain_rate, float error
static double sigmoidTemp(double t, double *args)
{
  // double E = args[2];
  // double A = args[0];
  // double C = args[1] * 4 / abs(A);
  // double D = logf(abs(A) / E - 1) / C;
  // return A / (1 + expf(-1 * C * (t - D)));
}

/*static void dyn4_test()
{
  MCP23017 *mcp = mcp23017_create();
  mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL);
  mcp_set_pin(mcp, CHARGE_PUMP_PIN, CHARGE_PUMP_REGISTER, 0);
  printf("MCP23017 begin\n");
  pause(500);

  DYN4 *dyn4 = dyn4_create();
  dyn4_begin(dyn4, DYN4_RX, DYN4_TX, 0);
  DYN4_Status status;
  while (dyn4_get_status(dyn4, &status) != SUCCESS)
  {
    printf("Error getting status\n");
    _waitms(100);
  }
  dyn4_send_command(dyn4, dyn4_read_drive_config, 0);
  printf("Drive Config:%d\n", dyn4_read_command(dyn4, dyn4_is_gear_number));
  dyn4_send_command(dyn4, dyn4_read_drive_id, 1);
  printf("Drive ID:%d\n", dyn4_read_command(dyn4, dyn4_is_gear_number));
  _waitms(100);
  dyn4_send_command(dyn4, dyn4_read_gear_number, 0);
  int gearnum = dyn4_read_command(dyn4, dyn4_is_gear_number);
  printf("GEAR NUM:%d\n", gearnum);
  dyn4_send_command(dyn4, dyn4_go_abs_pos, 0);
  _waitms(2000);
  Encoder encoder;
  encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000000, 100000000);
  unsigned long startus = _getus();
  double setpoint = 1;
  double strain_rate = 4;
  double error = 0.005;
  double setPosition = 0;
  double encoderPosition = 0;
  while (abs(encoderPosition - setpoint) > 0.01)
  {
    double dt = (_getus() - startus) / 1000000.0;
    //    setPosition = sigmoidTemp(dt, setpoint, strain_rate, error);
    dyn4_send_command(dyn4, dyn4_go_abs_pos, setPosition * 204800.0);
    encoderPosition = -2 * encoder.value() / 204800.0;
    printf("%f,%f,%f\n", dt, setPosition, encoderPosition);
    _waitms(30);
  }
  return;
}*/

void test_dyn4_control()
{
  // Create Test Profile
  TestProfile *testProfile = get_test_profile();
  char *testName = "TestProfile001";
  testProfile->name = (char *)malloc(sizeof(char) * (strlen(testName) + 1));
  strcpy(testProfile->name, testName);
  testProfile->sampleSN = 1021221;

  MotionProfile *profile = (MotionProfile *)malloc(sizeof(MotionProfile)); //= get_machine_profile();
  profile->number = 1;
  profile->setCount = 1;
  profile->sets = (MotionSet *)malloc(sizeof(MotionSet) * profile->setCount);

  /*Creating motion set #1*/
  char *profileName = "setProfile";
  profile->sets[0].name = (char *)malloc(sizeof(char) * (strlen(profileName) + 1));
  strcpy(profile->sets[0].name, profileName);
  profile->sets[0].number = 1;
  profile->sets[0].executions = 2;
  profile->sets[0].quartetCount = 2;
  profile->sets[0].quartets = (MotionQuartet *)malloc(sizeof(MotionQuartet) * profile->sets[0].quartetCount);
  // Creating motion set quartet #1
  char *quartetName11 = "quartetS1Q1";
  profile->sets[0].quartets[0].name = (char *)malloc(sizeof(char) * (strlen(quartetName11) + 1));
  strcpy(profile->sets[0].quartets[0].name, quartetName11);
  profile->sets[0].quartets[0].function = QUARTET_FUNC_SIGMOIDAL;
  profile->sets[0].quartets[0].parameters = (double *)malloc(sizeof(double) * 3);
  profile->sets[0].quartets[0].parameters[0] = 0.5;           // distance
  profile->sets[0].quartets[0].parameters[1] = 4;             // strain rate
  profile->sets[0].quartets[0].parameters[2] = 0.00000488281; // error
  profile->sets[0].quartets[0].distanceMax = profile->sets[0].quartets[0].parameters[0];
  profile->sets[0].quartets[0].dwell = 1000000; // 1000ms
  // Creating motion set quartet #2
  char *quartetName12 = "quartetS1Q2";
  profile->sets[0].quartets[1].name = (char *)malloc(sizeof(char) * (strlen(quartetName12) + 1));
  strcpy(profile->sets[0].quartets[1].name, quartetName12);
  profile->sets[0].quartets[1].function = QUARTET_FUNC_SIGMOIDAL;
  profile->sets[0].quartets[1].parameters = (double *)malloc(sizeof(double) * 3);
  profile->sets[0].quartets[1].parameters[0] = -0.5;          // distance
  profile->sets[0].quartets[1].parameters[1] = 4;             // strain rate
  profile->sets[0].quartets[1].parameters[2] = 0.00000488281; // error
  profile->sets[0].quartets[1].distanceMax = profile->sets[0].quartets[1].parameters[0];
  profile->sets[0].quartets[1].dwell = 1000000; // 1000ms

  /*Run the profile*/
  DYN4 *dyn4 = dyn4_create();
  dyn4_begin(dyn4, DYN4_RX, DYN4_TX, 0);
  dyn4_send_command(dyn4, dyn4_read_drive_config, 0);
  printf("Drive Config:%d\n", dyn4_read_command(dyn4, dyn4_is_gear_number));
  dyn4_send_command(dyn4, dyn4_read_drive_id, 1);
  printf("Drive ID:%d\n", dyn4_read_command(dyn4, dyn4_is_gear_number));
  _waitms(100);
  dyn4_send_command(dyn4, dyn4_go_abs_pos, 0);
  _waitms(2000);

  Encoder encoder;
  encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000000, 100000000);

  int currentSet = 0;
  int currentQuartet = 0;
  int currentPosition = 0; // Theoretical Position (not from encoder)
  printf("Starting profile\n");
  while (currentSet < profile->setCount)
  {
    MotionSet set = profile->sets[currentSet];

    /*Run the motion set*/
    int currentExecution = 0;
    int currentQuartet = 0;
    printf("  Starting set %s\n", set.name);
    while (currentQuartet != set.quartetCount)
    {
      MotionQuartet quartet = set.quartets[currentQuartet];
      float (*f)(float t, double *args) = NULL;
      switch (quartet.function)
      {
      case QUARTET_FUNC_SIGMOIDAL:
        f = sigmoidTemp;
        break;
      }

      /*Run the motion quartet*/
      unsigned long startus = _getus();
      long quartetPosition = 0;
      long setpoint = floor(quartet.distanceMax * 204800.0); // convert m to encoder ticks
      printf("    starting quartet %s,%ld,%d\n", quartet.name, setpoint, currentExecution);
      while (quartetPosition != setpoint)
      {
        double dt = (_getus() - startus) / 1000000.0;           // Time in seconds
        quartetPosition = f(dt, quartet.parameters) * 204800.0; // Quartet position (m) in encoder ticks
        dyn4_send_command(dyn4, dyn4_go_abs_pos, (currentPosition + quartetPosition) / 2);
        printf("      %f,%ld,%ld,%ld\n", dt, setpoint, encoder.value(), quartetPosition);
        _waitms(30);
      }

      // Move motor to quartet end position
      dyn4_send_command(dyn4, dyn4_go_abs_pos, (currentPosition + quartetPosition) / 2);

      // Dwell for quartet dwell time
      _waitus(quartet.dwell);

      // Increment quartet and update execution
      printf("    end of quartet %s\n", quartet.name);
      currentPosition += setpoint;
      currentExecution++;
      if (currentExecution == set.executions)
      {
        currentQuartet++;
        currentExecution = 0;
      }
    }
    // Increment set
    printf("  End of set %s\n", set.name);
    currentSet++;
  }
  printf("End of profile\n");
  // End of motion profile
}

void test_quartet()
{
  printf("Testing quartet\n");
  MotionQuartet *quartet = get_motion_quartet();
  char *name = "/sd/qrtet.qrt";
  quartet->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(quartet->name, name);
  quartet->function = QUARTET_FUNC_SIGMOIDAL;
  quartet->parameters = (float *)malloc(sizeof(float) * 3);
  quartet->parameters[0] = -0.5;          // distance
  quartet->parameters[1] = 4;             // strain rate
  quartet->parameters[2] = 0.00000488281; // error
  quartet->distanceMax = quartet->parameters[0];
  quartet->dwell = 1000000; // 1000ms

  // Save quartet
  printf("Saving quartet\n");
  motion_quartet_to_json(quartet, quartet->name);
  printf("Saved quartet\n");
  // FILE *file = fopen(quartet->name, "w");
  // fprintf(file, "testing if this shit is even working wtf\n");
  // fclose(file);
  FILE *file = fopen(quartet->name, "r");
  if (file == NULL)
  {
    printf("Error opening file\n");
    return;
  }
  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET); /* same as rewind(f); */

  char *string = malloc(fsize + 1);
  fread(string, fsize, 1, file);
  fclose(file);
  printf("Total size of file: %ld\n", fsize);
  string[fsize] = 0;
  printf("JSON:%s\n", string);
}

/**
 * @brief Starts the display, motion control, and all MaD board related tasks
 *
 */
void mad_begin()
{
  printf("Starting...\n");
  mount("/sd", _vfs_open_sdcard());

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
  // image_load_assets(images, display);

  // Load Machine Profile
  loading_overlay_display(display, "Loading Machine Profile", OVERLAY_TYPE_LOADING);

  // Create DYN4 object (currently no communication with DYN4)
  DYN4 *dyn4 = dyn4_create();
  if (dyn4_begin(dyn4, DYN4_RX, DYN4_TX, 0) != SUCCESS)
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
      /// StatusPage *statusPage = status_page_create(display, machineState, &(monitor->data), images);
      // status_page_run(statusPage);
      // status_page_destroy(statusPage);
      TestProfilePage *page = test_profile_page_create(display, images);
      test_profile_page_run(page);
      //  printf("Leaving status page\n");
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
