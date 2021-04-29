#include "MaD.h"
#include "JSON.h"
#include "Keyboard.h"
#include "Motion.h"
#include "BufferlessSerial.h"

unsigned int stack[STACK_SIZE];
volatile Motion_Cog motionCog;

/**
 * @brief Starts the display, motion control, and all MaD board related tasks
 * 
 */
void MAD::begin()
{
  pause(100);
  printf("Starting display...\n");
  /*BLSerial serial;
  uart_start(&serial, 14, 12, 2, 9600);
  pause(2000);
  uart_write(&serial, 0x55);*/

  startDisplay();
  printf("Finished\n");
  printf("Updating machine state\n");
  printf("Finished\n");
  //end of State testing configuration

  //setup real time clock
  printf("starting rtc\n");
  //clock.begin(&display);
  printf("finished\n");

  //setup gpio
  printf("Starting gpio expansion...\n");

  printf("Finished\n");

  //load assets
  //sd_mount(MAD_SD_DO, MAD_SD_CLK, MAD_CD_DI, MAD_CD_CS);
  printf("Loading images\n");
  //loadAssets();
  printf("loading dyn4\n");

  //ForceGauge_begin(&forceGauge, FORCE_RX, FORCE_TX);
  //forceGauge.begin(FORCE_RX, FORCE_TX);
  printf("Getting keyboard input\n");
  //dyn4_send_command(&dyn4, 0x0c, 1000);
  // motion.dyn4 = &dyn4;
  //motion.forceGauge = &forceGauge;
  extern unsigned int _load_start_Motion_cog[];
  //int *cogAddr = cog_run(runMotion, 200);
  // printf("Cog successfully started:%d\n", cog_num(cogAddr));
  ForceGauge forceGauge;
  ForceGauge_begin(&forceGauge, 15, 16);
  int prevTime = 0;
  while (1)
  {
    printf("Force(%d): %u,TIME:%d\n", forceGauge.counter, forceGauge.force, CNT - prevTime);
    prevTime = CNT;
    pause(100);
  }
  pause(100);
  while (1)
  {
    printf("position:%d, force:%d,time:%d\n", motionCog.position, motionCog.force, motionCog.time);
  }
  // getKeyboardInput(buf);
  //start status page
  Pages newPage = Pages::PAGE_STATUS;
  while (1)
  {
    switch (newPage)
    {
    case Pages::PAGE_STATUS:
    {
      printf("Loading status page\n");
      StatusPage statusPage;
      statusPage.run(&display);
      printf("Leaving status page\n");
    }
    break;
    case Pages::PAGE_MANUAL:
    {
      printf("Loading manual page\n");
      ManualPage manual;
      manual.run(&display, (Motion_Cog *)(&motionCog));
      printf("Leaving manual page\n");
    }
    break;
    case Pages::PAGE_AUTOMATIC:
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
    newPage = nav.run(&display);
  }
  //Keyboard keyboard;
  // keyboard.getKeyboardInput(&display, buffer);
  //runStatusPage();
  //pause(1000);

  /* MachineSettings *settings = getMachineSettings();
  sd_mount(MAD_SD_DO, MAD_SD_CLK, MAD_CD_DI, MAD_CD_CS);
  settings->name = (char *)malloc(sizeof(char) * 20);
  strcpy(settings->name, "Tensile V1.0");
  settings->number = 1;
  printf("Name:%s\n", settings->name);

  settings->configuration->motorType = (char *)malloc(sizeof(char) * 20);
  strcpy(settings->configuration->motorType, "640-DST");
  settings->configuration->positionEncoderType = (char *)malloc(sizeof(char) * 20);
  strcpy(settings->configuration->positionEncoderType, "Servo Encoder");
  settings->configuration->forceGauge = (char *)malloc(sizeof(char) * 20);
  strcpy(settings->configuration->forceGauge, "Zique DS2-5N");

  char *settingsJSON = machineSettingsToJSON(settings);
  freeMachineSettings(settings);
  printf("JSON:%s\n", settingsJSON);
  char *name = (char *)malloc(sizeof(char) * strlen(settings->name) + 15);
  sprintf(name, "Machine_%s", settings->name);
  FILE *settingsFile = fopen(name, "w");
  fwrite(settingsJSON, sizeof(char) * (strlen(settingsJSON) + 10), 1, settingsFile);
  fclose(settingsFile);
  settingsFile = fopen(name, "r");
  char data;

  while ((fread(&data, 1, 1, settingsFile)) == 1)
  {
    printf("%c", data);
  }*/
}
/**
 * @brief Initializes display object
 * 
 * @return Enum Error: SUCCESS if display connected
 */
Error MAD::startDisplay()
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
}
