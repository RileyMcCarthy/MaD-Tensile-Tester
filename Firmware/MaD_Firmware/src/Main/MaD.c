#include "MaD.h"
#include "JSON.h"

static void test_JSON()
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

  //Create JSON using JSON to validate conversion process
  MachineProfile *profile_validate = json_to_machine_profile(json);
  char *json_validate = machine_profile_to_json(profile_validate);

  printf("\n\nvalidation_json:%s\n", json_validate);
  if (strcmp(json, json_validate) == 0)
  {
    printf("JSON Validation Successful\n");
  }
  else
  {
    printf("JSON Validation Failed\n");
  }
}

/**
 * @brief Starts the display, motion control, and all MaD board related tasks
 * 
 */
void mad_begin()
{
  printf("Starting...\n");
  test_JSON();
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
