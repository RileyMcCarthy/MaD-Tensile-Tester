#include "MaD.h"
#include "JSON.h"
#include <stdio.h>
#include <dirent.h>
#include "MotionPlanning.h"
#include "MCP23017.h"
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif

// Takes file pointer and prints its content character by character
static void printFile(FILE *file)
{
  int position = ftell(file);
  fseek(file, 0, SEEK_SET);
  int c;
  while ((c = fgetc(file)) != EOF)
  {
    printf("%c", (char)c);
  }
  fseek(file, position, SEEK_SET);
}

// Takes file pointer and compares its content character by character
static bool compareFile(FILE *file1, FILE *file2)
{
  char c1, c2;
  while ((c1 = fgetc(file1)) != EOF && (c2 = fgetc(file2)) != EOF)
  {
    if (c1 != c2)
      return false;
  }
  return true;
}

static void test_JSON_MachineProfile()
{
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

  // MachinePerformance
  profile->performance->minPosition = 0.01;
  profile->performance->maxPosition = 50.5;
  profile->performance->maxVelocity = 200.5;
  profile->performance->maxAcceleration = 100.5;
  profile->performance->maxForceTensile = 3.5;
  profile->performance->maxForceCompression = 3.1;
  profile->performance->forceGaugeNeutralOffset = 0.5;

  // Profile Struct to JSON
  mkdir("/sd/MProfile", 0);
  chdir("/sd/MProfile");
  FILE *jsonFile = fopen("test.mp", "w+");
  if (jsonFile == NULL)
  {
    printf("Error opening machine profile file!\n");
    return;
  }
  machine_profile_to_json(profile, jsonFile);
  printf("Machine Profile JSON:");
  printFile(jsonFile);
  printf("\n\n");

  // Create JSON using JSON to validate conversion process
  fseek(jsonFile, 0, SEEK_SET);
  MachineProfile *profile_validate = json_to_machine_profile(jsonFile);
  FILE *jsonValidateFile = fopen("test_val.mp", "w+");
  if (jsonFile == NULL)
  {
    printf("Error opening machine profile validate file!\n");
    return;
  }
  machine_profile_to_json(profile_validate, jsonValidateFile);

  printf("Machine Profile Validation Json:");
  printFile(jsonValidateFile);
  printf("\n\n");

  if (compareFile(jsonFile, jsonValidateFile) == 0)
  {
    printf("Machine Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Machine Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  // Free memory
  fclose(jsonFile);
  fclose(jsonValidateFile);
  free_machine_profile(profile);
  free_machine_profile(profile_validate);
}

static void test_json_sample_profile()
{
  // Get sample profile structure
  SampleProfile *profile = get_sample_profile();

  // Populate structure
  char *profileName = "profile_name";
  profile->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(profile->name, profileName);
  profile->number = 5;
  profile->length = 5.3;
  profile->stretchMax = 12.1;
  profile->maxVelocity = 2.3;
  profile->maxAcceleration = 1.2;
  profile->maxJerk = 0.3;
  profile->maxForceTensile = 0.5;
  profile->maxForceCompression = 0.1;

  // Profile Struct to JSON
  mkdir("/sd/SProfile", 0);
  chdir("/sd/SProfile");
  FILE *jsonFile = fopen("test.sp", "w+");
  sample_profile_to_json(profile, jsonFile);
  printf("Sample Profile JSON:");
  printFile(jsonFile);
  printf("\n\n");

  // Create JSON using JSON to validate conversion process
  fseek(jsonFile, 0, SEEK_SET);
  SampleProfile *profile_validate = json_to_sample_profile(jsonFile);
  FILE *jsonValidateFile = fopen("test_val.sp", "w+");
  sample_profile_to_json(profile_validate, jsonValidateFile);
  printf("Sample Profile Validation Json:");
  printFile(jsonValidateFile);
  printf("\n\n");

  // Make sure the JSON is the same
  if (compareFile(jsonFile, jsonValidateFile) == 0)
  {
    printf("Sample Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Sample Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  // free memory
  fclose(jsonFile);
  fclose(jsonValidateFile);
  free_sample_profile(profile);
  free_sample_profile(profile_validate);
}

static void test_json_test_profile()
{
  // Get test profile structure
  TestProfile *profile = get_test_profile();

  // Populate structure
  char *profileName = "test_profile_name";
  profile->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(profile->name, profileName);
  char *machineProfileFileName = "machine_profile_name.machine";
  profile->machineProfileFileName = (char *)malloc(strlen(machineProfileFileName) + 1);
  strcpy(profile->machineProfileFileName, machineProfileFileName);
  char *sampleProfileFileName = "sample_profile_name.sample";
  profile->sampleProfileFileName = (char *)malloc(strlen(sampleProfileFileName) + 1);
  strcpy(profile->sampleProfileFileName, sampleProfileFileName);
  profile->sampleSerialNumber = 124532;

  // Profile Struct to JSON
  mkdir("/sd/TProfile", 0);
  chdir("/sd/TProfile");
  FILE *jsonFile = fopen("test.tp", "w+");
  test_profile_to_json(profile, jsonFile);
  printf("Test Profile JSON:\n");
  printFile(jsonFile);
  printf("\n\n");

  // Create JSON using JSON to validate conversion process
  fseek(jsonFile, 0, SEEK_SET);
  TestProfile *profile_validate = json_to_test_profile(jsonFile);
  FILE *jsonValidateFile = fopen("test_val.tp", "w+");
  test_profile_to_json(profile_validate, jsonValidateFile);
  printf("Test Profile Validation Json:\n");
  printFile(jsonValidateFile);
  printf("\n\n");

  // Make sure the JSON is the same
  if (compareFile(jsonFile, jsonValidateFile) == 0)
  {
    printf("Test Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Test Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  // free memory
  fclose(jsonFile);
  fclose(jsonValidateFile);
  free_test_profile(profile);
  free_test_profile(profile_validate);
}

static void test_json_motion_quartet()
{
  // Get motion quartet structure
  MotionQuartet *profile = get_motion_quartet();

  // Populate structure
  char *profileName = "quartet_profile_name";
  profile->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(profile->name, profileName);
  char *profileType = "quartet_profile_type";
  profile->type = (char *)malloc(strlen(profileType) + 1);
  strcpy(profile->type, profileType);
  profile->distance = 5.3;
  profile->velocity = 2.1;
  profile->acceleration = 1.2;
  profile->jerk = 0.3;
  profile->dwell = 0.5;

  // Profile Struct to JSON
  mkdir("/sd/mquartet", 0);
  chdir("/sd/mquartet");
  FILE *jsonFile = fopen("test.mq", "w+");
  motion_quartet_to_json(profile, jsonFile);
  printf("Motion Quartet JSON:\n");
  printFile(jsonFile);
  printf("\n\n");

  // Create JSON using JSON to validate conversion process
  fseek(jsonFile, 0, SEEK_SET);
  MotionQuartet *profile_validate = json_to_motion_quartet(jsonFile);
  FILE *jsonValidateFile = fopen("test_val.mq", "w+");
  motion_quartet_to_json(profile_validate, jsonValidateFile);
  printf("Motion Quartet Validation Json:\n");
  printFile(jsonValidateFile);
  printf("\n\n");
  // Make sure the JSON is the same
  if (compareFile(jsonFile, jsonValidateFile) == 0)
  {
    printf("Motion Quartet JSON Validation Successful\n");
  }
  else
  {
    printf("Motion Quartet JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  // free memory
  fclose(jsonFile);
  fclose(jsonValidateFile);
  free_motion_quartet(profile);
  free_motion_quartet(profile_validate);
}

static void test_json_motion_profile()
{
  // Get motion profile structure
  MotionProfile *profile = get_motion_profile();

  // Populate structure
  char *profileName = "motion_profile_name";
  profile->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(profile->name, profileName);
  profile->number = 2;
  profile->setCount = 2;
  profile->sets = malloc(sizeof(MotionSet) * profile->setCount);

  // Get motion set structure
  MotionSet *set = get_motion_set();

  // Populate structure
  profileName = "motion_set_profile_name";
  set->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(set->name, profileName);
  set->number = 3;
  char *profileType = "motion_set_profile_type";
  set->type = (char *)malloc(strlen(profileType) + 1);
  strcpy(set->type, profileType);
  set->executions = 5;
  set->quartetCount = 3;
  set->quartets = (char **)malloc(sizeof(char *) * set->quartetCount);
  char *quartet1File = "quartet1_file_name.quartet";
  char *quartet2File = "quartet2_file_name.quartet";
  char *quartet3File = "quartet3_file_name.quartet";

  set->quartets[0] = malloc(sizeof(char) * (strlen(quartet1File) + 1));
  strcpy(set->quartets[0], quartet1File);
  set->quartets[1] = malloc(sizeof(char) * (strlen(quartet2File) + 1));
  strcpy(set->quartets[1], quartet2File);
  set->quartets[2] = malloc(sizeof(char) * (strlen(quartet3File) + 1));
  strcpy(set->quartets[2], quartet3File);

  // Create another set structure
  MotionSet *set2 = get_motion_set();
  profileName = "motion_set_profile_name2";
  set2->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(set2->name, profileName);
  set2->number = 4;
  profileType = "motion_set_profile_type2";
  set2->type = (char *)malloc(strlen(profileType) + 1);
  strcpy(set2->type, profileType);
  set2->executions = 6;
  set2->quartetCount = 3;
  set2->quartets = (char **)malloc(sizeof(char *) * set2->quartetCount);
  quartet1File = "quartet1_file_name2.quartet";
  quartet2File = "quartet2_file_name2.quartet";
  quartet3File = "quartet3_file_name2.quartet";
  set2->quartets[0] = malloc(sizeof(char) * (strlen(quartet1File) + 1));
  strcpy(set2->quartets[0], quartet1File);
  set2->quartets[1] = malloc(sizeof(char) * (strlen(quartet2File) + 1));
  strcpy(set2->quartets[1], quartet2File);
  set2->quartets[2] = malloc(sizeof(char) * (strlen(quartet3File) + 1));
  strcpy(set2->quartets[2], quartet3File);

  profile->sets[0] = set;
  profile->sets[1] = set2;

  // Profile Struct to JSON
  mkdir("/sd/mopro", 0);
  chdir("/sd/mopro");
  FILE *jsonFile = fopen("test.mp", "w+");
  motion_profile_to_json(profile, jsonFile);
  printf("Motion Profile JSON:\n");
  printFile(jsonFile);
  printf("\n\n");

  // Create JSON using JSON to validate conversion process
  fseek(jsonFile, 0, SEEK_SET);
  MotionProfile *profile_validate = json_to_motion_profile(jsonFile);
  FILE *jsonValidateFile = fopen("test_val.mp", "w+");
  motion_profile_to_json(profile_validate, jsonValidateFile);
  printf("Motion Profile Validation Json:\n\n");
  printFile(jsonValidateFile);
  printf("\n\n");

  // Make sure the JSON is the same
  if (compareFile(jsonFile, jsonValidateFile) == 0)
  {
    printf("Motion Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Motion Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  // free memory
  fclose(jsonFile);
  fclose(jsonValidateFile);
  free_motion_profile(profile);
  free_motion_profile(profile_validate);
}

static void test_JSON()
{
  int sd = mount("/sd", _vfs_open_sdcard());

  if (sd == 0)
  {
    printf("SD Card Mounted\n");
  }
  else
  {
    printf("Mount error:%d\n", _geterror());
  }
  test_JSON_MachineProfile();
  test_json_sample_profile();
  test_json_test_profile();
  test_json_motion_quartet();
  test_json_motion_profile();
}

static void test_motion_planning()
{
  motion_profile(2, 1, 1, 3, 1000);
}

static void test_sd_card()
{
  int sd = mount("/sd", _vfs_open_sdcard());
  if (sd == 0)
  {
    printf("SD Card Mounted\n");
  }
  else
  {
    printf("Mount error:%d\n", _geterror());
  }
  mkdir("/sd/test_dir", 0);
  chdir("/sd/test_dir");
  char dirName[50];
  getcwd(dirName, 50);
  printf("Current Directory:%s\n", dirName);
  if (strcmp((const char *)dirName, "/sd/test_dir") == 0)
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
  {
    printf("SD card read/write successful\n");
  }
  else
  {
    printf("SD card read/write failed\n");
    return;
  }
  fclose(fp);
  return;
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
}

static void test_ds3231()
{
  DS3231 *rtc = ds3231_create();
  Error status = ds3231_begin(rtc, MAD_DS3231_SCL, MAD_DS3231_SDA);
  Time newTime;
  // ds3231_set_time(rtc, &newTime);
  printf("clock started: %d\n", status);
  for (int i = 0; i < 5; i++)
  {
    int res = ds3231_update_time(rtc);
    printf("updating time:%d\n", res);
    printf("time: %d\n", rtc->time.second);
    pause(500);
  }
}

static void test_display()
{
  Error err;
  Display display;
  // turn on diplay
  if ((err = display_begin(&display, DISPLAY_XNRESET, DISPLAY_XNSCS, DISPLAY_MOSI, DISPLAY_MISO, DISPLAY_SCK, DISPLAY_CLK, DISPLAY_DATA)) != SUCCESS)
  {
    printf("Error starting display:%d\n", err);
    return;
  }
  printf("Display begans\n");
  display_on(&display, true);
  printf("Display on\n");

  // Init display and background
  display_canvas_image_start_address(&display, PAGE1_START_ADDR);
  display_canvas_image_width(&display, SCREEN_WIDTH);
  display_active_window_xy(&display, 0, 0);
  display_active_window_wh(&display, SCREEN_WIDTH, SCREEN_HEIGHT);
  display_draw_square_fill(&display, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);

  char buf[50];
  strcpy(buf, "Operational States");
  int operationalStartX = SCREEN_WIDTH / 6 - strlen(buf) * 8;
  int operationalStartY = 40;
  display_draw_string(&display, operationalStartX, operationalStartY, buf);
  display_draw_line(&display, operationalStartX, operationalStartY + 30, operationalStartX + strlen(buf) * 16, operationalStartY + 30, MAINTEXTCOLOR);
  display_draw_square(&display, 20, 30, SCREEN_WIDTH / 3 - 10, SCREEN_HEIGHT - 20, SECONDARYTEXTCOLOR);

  keyboard_get_input(&display, "Name: ");
  return;

  Image keyboard;
  strcpy(keyboard.name, "/sd/keyboard.bin");
  keyboard.page = 1;
  keyboard.width = 1026;
  keyboard.height = 284;
  keyboard.x0 = 0;
  keyboard.y0 = SCREEN_HEIGHT - keyboard.height;
  keyboard.backgroundColor = NULL;
  display_load_image(&display, keyboard);
}

static bool filesystem_begin()
{
  int sd = mount("/sd", _vfs_open_sdcard());
  if (sd == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
  //@TODO mount data SD card also
}

static void test_mcp23017()
{
  MCP23017 *mcp = mcp23017_create();
  mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL);

  printf("MCP23017 begin\n");

  mcp_set_direction(mcp, 4, 0); // zero is output
  printf("Direction:%d\n", mcp23017_get_direction(mcp, 4));

  mcp_set_direction(mcp, 4, 0);
  printf("Output:%d\n", mcp_get_pin(mcp, 4));
  pause(500);
  mcp_set_pin(mcp, 4, 1);
  printf("Output:%d\n", mcp_get_pin(mcp, 4));
  pause(500);
  mcp23017_destroy(mcp);
  return;
}

static void dyn4_test()
{
  MCP23017 *mcp = mcp23017_create();
  mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL);
  mcp_set_direction(mcp, 4, 0);         // zero is output
  mcp_set_pin(mcp, CHARGE_PUMP_OUT, 0); // Enable charge pump, output is inverted

  printf("MCP23017 begin\n");
  pause(500);

  DYN4 *dyn4 = dyn4_create();
  dyn4_begin(dyn4, DYN4_RX, DYN4_TX, DYN4_ADDR);
  while (1)
  {
    dyn4_send_command(dyn4, 0x0a, 100);
    printf("position:%d\n", dyn4->encoder.value());
    _waitms(10);
  }
}

static void force_test()
{
  // Connect Force Gauge
  ForceGauge *forceGauge = force_gauge_create();
  force_gauge_begin(forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX, -666, 2048402);
  // loading_overlay_display(display, "Force Gauge Connected", OVERLAY_TYPE_LOADING);
  while (1)
  {
    int force = force_gauge_get_force(forceGauge);
    printf("Force: %d\n", force);
  }
}

static MachineProfile *Load_MachineProfile()
{
  // Check for machine profile in filesystem
  if (chdir("/sd/Machine") == 0)
  {
    FILE *jsonFile = fopen("Default.mch", "r");
    if (jsonFile != NULL)
    {
      MachineProfile *profile = json_to_machine_profile(jsonFile);
      return profile;
    }
  }

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

  // MachinePerformance
  profile->performance->minPosition = 0.01;
  profile->performance->maxPosition = 50.5;
  profile->performance->maxVelocity = 200.5;
  profile->performance->maxAcceleration = 100.5;
  profile->performance->maxForceTensile = 3.5;
  profile->performance->maxForceCompression = 3.1;
  profile->performance->forceGaugeNeutralOffset = 0.5;

  // Profile Struct to JSON
  mkdir("/sd/Machine", 0);
  chdir("/sd/Machine");
  FILE *jsonFile = fopen("Default.mch", "w+");
  if (jsonFile == NULL)
  {
    printf("Error opening machine profile file!\n");
    return NULL;
  }

  // Create JSON using JSON to validate conversion process
  profile = json_to_machine_profile(jsonFile);

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
#ifdef __MEMORY_CHECK__
  printf("MEMORY CHECK ENABLED\n");
#endif
  printf("Starting...\n");

  // Begin the display
  Display *display = start_display();
  if (display == NULL)
  {
    printf("Error starting display\n");
    return;
  }
  loading_overlay_display(display, "Display Initialized!", OVERLAY_TYPE_LOADING);

  // Begin SD filesystem
  if (filesystem_begin())
  {
    loading_overlay_display(display, "SD Card Mounted", OVERLAY_TYPE_LOADING);
  }
  else
  {
    loading_overlay_display(display, "SD Card Mounting Error", OVERLAY_TYPE_LOADING);
  }

  // Load Assets from SD card
  image_load_assets(display);

  // Load Machine Profile
  MachineProfile *machineProfile = Load_MachineProfile();

  // Start NavKey
  NavKey *navkey = start_navkey();

  // Initialize IO Expansion(MCP23017)
  MCP23017 *mcp = mcp23017_create();
  mcp23017_begin(mcp, GPIO_ADDR, GPIO_SDA, GPIO_SCL); // issues using i2c slow with delay not saving, change to new i2c
                                                      //  mcp_set_direction(mcp, 4, MCP23017_OUTPUT);         // zero is output
  loading_overlay_display(display, "MCP23017 Connected", OVERLAY_TYPE_LOADING);

  // Create DYN4 object (currently no communication with DYN4)
  DYN4 *dyn4 = dyn4_create();
  dyn4_begin(dyn4, DYN4_RX, DYN4_TX, DYN4_ADDR);
  loading_overlay_display(display, "DYN4 Connected", OVERLAY_TYPE_LOADING);

  // Connect to IMU
  // Start state machine (needs IO expansion)
  MachineState *machineState = state_machine_run();
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
  force_gauge_begin(forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX, -666, 2048402);
  loading_overlay_display(display, "Force Gauge Connected", OVERLAY_TYPE_LOADING);

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
      StatusPage *statusPage = status_page_create(display, machineState);
      status_page_run(statusPage);
      status_page_destroy(statusPage);
      printf("Leaving status page\n");
      break;
    case PAGE_MANUAL:
      printf("Loading manual page\n");
      ManualPage *manualPage = manual_page_create(display, machineState);
      manual_page_run(manualPage);
      manual_page_destroy(manualPage);
      printf("Leaving manual page\n");
      break;
    case PAGE_AUTOMATIC:
    {
      printf("Loading automatic page...\n");
      AutomaticPage *automaticPage = automatic_page_create(display);
      automatic_page_run(automaticPage);
      automatic_page_destroy(automaticPage);
      printf("Leaving automatic page\n");
    }
    break;
    default:
      break;
    }
    printf("Selecting new page\n");
    NavigationPage *navigationPage = navigation_page_create(display);
    currentPage = navigation_page_run(navigationPage);
    navigation_page_destroy(navigationPage);
  }
#ifdef __MEMORY_CHECK__
  report_mem_leak();
#endif
}
