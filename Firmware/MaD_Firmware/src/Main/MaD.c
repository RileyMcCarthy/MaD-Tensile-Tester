#include "MaD.h"
#include "JSON.h"
#include "MotionPlanning.h"
#ifdef __MEMORY_CHECK__
#include "leak_detector_c.h"
#endif
static void test_JSON_MachineProfile()
{
  MachineProfile *profile = get_machine_profile();
  //MachineSettings
  profile->name = "Tensile_Test_1";
  profile->number = 1;

  //MachineConfiguration
  char *motorType = "640-DST";
  profile->configuration->motorType = (char *)malloc(strlen(motorType) + 1);
  strcpy(profile->configuration->motorType = "640-DST", motorType);
  profile->configuration->maxMotorTorque = 3.82;    //make float
  profile->configuration->maxMotorRPM = 5000;       //make float
  profile->configuration->gearDiameter = 25.4;      //make float
  profile->configuration->gearPitch = 1.0;          //make float
  profile->configuration->systemIntertia = 0.00121; //make float
  profile->configuration->staticTorque = 0.558;     //make float
  profile->configuration->load = 37.8;              //make float
  char *positionEncoderType = "QuadEncoder";
  profile->configuration->positionEncoderType = (char *)malloc(strlen(positionEncoderType) + 1);
  strcpy(profile->configuration->positionEncoderType, positionEncoderType);
  profile->configuration->positionEncoderScaleFactor = 23;
  char *forceGauge = "DS2-5N";
  profile->configuration->forceGauge = (char *)malloc(strlen(forceGauge) + 1);
  strcpy(profile->configuration->forceGauge, forceGauge);

  //MachinePerformance
  profile->performance->minPosition = 0.01;
  profile->performance->maxPosition = 50.5;
  profile->performance->maxVelocity = 200.5;
  profile->performance->maxAcceleration = 100.5;
  profile->performance->maxForceTensile = 3.5;
  profile->performance->maxForceCompression = 3.1;
  profile->performance->forceGaugeNeutralOffset = 0.5;

  //Profile Struct to JSON
  char *json = machine_profile_to_json(profile);
  printf("Machine Profile JSON:%s\n\n", json);
  //Create JSON using JSON to validate conversion process
  MachineProfile *profile_validate = json_to_machine_profile(json);
  char *json_validate = machine_profile_to_json(profile_validate);

  printf("Machine Profile Validation Json:%s\n\n", json_validate);
  if (strcmp(json, json_validate) == 0)
  {
    printf("Machine Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Machine Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");

  //Free memory
  free(json);
  free(json_validate);
  free_machine_profile(profile);
  free_machine_profile(profile_validate);
}

static void test_json_sample_profile()
{
  //Get sample profile structure
  SampleProfile *profile = get_sample_profile();

  //Populate structure
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

  //Profile Struct to JSON
  char *json = sample_profile_to_json(profile);
  printf("Sample Profile JSON:%s\n\n", json);

  //Create JSON using JSON to validate conversion process
  SampleProfile *profile_validate = json_to_sample_profile(json);
  char *json_validate = sample_profile_to_json(profile_validate);
  printf("Sample Profile Validation Json:%s\n\n", json_validate);

  //Make sure the JSON is the same
  if (strcmp(json, json_validate) == 0)
  {
    printf("Sample Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Sample Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  //free memory
  free(json);
  free(json_validate);
  free_sample_profile(profile);
  free_sample_profile(profile_validate);
}

static void test_json_test_profile()
{
  //Get test profile structure
  TestProfile *profile = get_test_profile();

  //Populate structure
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

  //Profile Struct to JSON
  char *json = test_profile_to_json(profile);
  printf("Test Profile JSON:%s\n\n", json);

  //Create JSON using JSON to validate conversion process
  TestProfile *profile_validate = json_to_test_profile(json);
  char *json_validate = test_profile_to_json(profile_validate);
  printf("Test Profile Validation Json:%s\n\n", json_validate);

  //Make sure the JSON is the same
  if (strcmp(json, json_validate) == 0)
  {
    printf("Test Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Test Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  //free memory
  free_test_profile(profile);
  free_test_profile(profile_validate);
  free(json);
  free(json_validate);
}

static void test_json_motion_quartet()
{
  //Get motion quartet structure
  MotionQuartet *profile = get_motion_quartet();

  //Populate structure
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

  //Profile Struct to JSON
  char *json = motion_quartet_to_json(profile);
  printf("Motion Quartet JSON:%s\n\n", json);

  //Create JSON using JSON to validate conversion process
  MotionQuartet *profile_validate = json_to_motion_quartet(json);
  char *json_validate = motion_quartet_to_json(profile_validate);
  printf("Motion Quartet Validation Json:%s\n\n", json_validate);
  //Make sure the JSON is the same
  if (strcmp(json, json_validate) == 0)
  {
    printf("Motion Quartet JSON Validation Successful\n");
  }
  else
  {
    printf("Motion Quartet JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  //free memory
  free(json);
  free(json_validate);
  free_motion_quartet(profile);
  free_motion_quartet(profile_validate);
}

static void test_json_motion_profile()
{
  //Get motion profile structure
  MotionProfile *profile = get_motion_profile();

  //Populate structure
  char *profileName = "motion_profile_name";
  profile->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(profile->name, profileName);
  profile->number = 2;
  profile->setCount = 2;
  profile->sets = malloc(sizeof(MotionSet) * profile->setCount);

  //Get motion set structure
  MotionSet *set = get_motion_set();

  //Populate structure
  char *profileName = "motion_set_profile_name";
  set->name = (char *)malloc(strlen(profileName) + 1);
  strcpy(set->name, profileName);
  set->number = 3;
  char *profileType = "motion_set_profile_type";
  set->type = (char *)malloc(strlen(profileType) + 1);
  strcpy(set->type, profileType);
  set->executions = 5;
  set->quartetCount = 3;
  set->quartets = (MotionQuartet **)malloc(sizeof(MotionQuartet *) * set->quartetCount);
  char *quartet1File = "quartet1_file_name.quartet";
  char *quartet2File = "quartet2_file_name.quartet";
  char *quartet3File = "quartet3_file_name.quartet";

  set->quartets[0] = malloc(sizeof(char) * (strlen(quartet1File) + 1));
  strcpy(set->quartets[0], quartet1File);
  set->quartets[1] = malloc(sizeof(char) * (strlen(quartet2File) + 1));
  strcpy(set->quartets[1], quartet2File);
  set->quartets[2] = malloc(sizeof(char) * (strlen(quartet3File) + 1));
  strcpy(set->quartets[2], quartet3File);

  //Create another set structure
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
  set2->quartets = (MotionQuartet **)malloc(sizeof(MotionQuartet *) * set2->quartetCount);
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

  //Profile Struct to JSON
  char *json = motion_profile_to_json(profile);
  printf("Motion Profile JSON:%s\n\n", json);
  //Create JSON using JSON to validate conversion process
  MotionProfile *profile_validate = json_to_motion_profile(json);
  char *json_validate = motion_profile_to_json(profile_validate);
  printf("Motion Profile Validation Json:%s\n\n", json_validate);

  //Make sure the JSON is the same
  if (strcmp(json, json_validate) == 0)
  {
    printf("Motion Profile JSON Validation Successful\n");
  }
  else
  {
    printf("Motion Profile JSON Validation Failed\n");
  }
  printf("---------------------------------------------------\n\n");
  //free memory
  free(json);
  free(json_validate);
  free_motion_profile(profile);
  free_motion_profile(profile_validate);
}

static void test_JSON()
{
  //@todo When all tests run, issues occur with the JSON validation. I think i have memory leaks
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

/**
 * @brief Starts the display, motion control, and all MaD board related tasks
 * 
 */
void mad_begin()
{
  printf("Starting...\n");
  //test_motion_planning();
  test_JSON();
#ifdef __MEMORY_CHECK__
  report_mem_leak();
#endif
  while (1)
    ;
  /*while (1)
  {
    _pinh(MAD_DS3231_SDA);
    _waitms(2000);
    _pinl(MAD_DS3231_SDA);
    _waitms(2000);
  }*/
  printf("Starting display...\n");
  DS3231 *rtc = ds3231_create();
  Error status = ds3231_begin(rtc, MAD_DS3231_SCL, MAD_DS3231_SDA);
  Time newTime;
  ds3231_set_time(rtc, &newTime);
  printf("clock started: %d\n", status);
  for (int i = 0; i < 5; i++)
  {
    int res = ds3231_update_time(rtc);
    printf("updating time:%d\n", res);
    printf("time: %d\n", rtc->time.second);
    _waitms(500);
  }

  /* Pages currentPage = PAGE_STATUS;
  while (1)
  {
    switch (currentPage)
    {
    case PAGE_STATUS:
    {
      printf("Loading status page\n");
      StatusPage statusPage;
      statusPage.run(&display);
      printf("Leaving status page\n");
    }
    break;
    case PAGE_MANUAL:
    {
      printf("Loading manual page\n");
      ManualPage manual;
      manual.run(&display, (Motion_Cog *)(&motionCog));
      printf("Leaving manual page\n");
    }
    break;
    case PAGE_AUTOMATIC:
    {
      printf("Loading automatic page\n");
      AutomaticPage automatic;
      automatic.run(&display, (Motion_Cog *)(&motionCog));
    }
    break;
    default:
      break;
    }
    printf("Selecting new page\n");
    NavigationPage nav;
    currentPage = nav.run(&display);
  }*/
}

/**
 * @brief Initializes display object
 * 
 * @return Enum Error: SUCCESS if display connected
 */
/*Error MAD::startDisplay()
{
  Error err;

  //turn on diplay
  if ((err = display.begin(RA8876_XNSCS, RA8876_XNRESET, CLK, DATA, GT9271_INT, BACKLIGHT)) != SUCCESS)
  {
    return err;
  }
  display.displayOn(true);

  //Init display and background
  display.canvasImageStartAddress(PAGE1_START_ADDR);
  display.canvasImageWidth(SCREEN_WIDTH);
  display.activeWindowXY(0, 0);
  display.activeWindowWH(SCREEN_WIDTH, SCREEN_HEIGHT);
  display.drawSquareFill(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, BACKCOLOR);
  return Error::SUCCESS;
}*/
