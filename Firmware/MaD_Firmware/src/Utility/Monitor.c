#include "Monitor.h"
#include "IOBoard.h"
#include "ForceGauge.h"
#include "Encoder.h"
#include <stdbool.h>
#include "W25QXX.h"
#include "propeller.h"

#define MONITOR_MEMORY_SIZE 4096 * 2
static long monitor_stack[MONITOR_MEMORY_SIZE];

bool monitorWriteData;

static ForceGauge forceGauge;
static Encoder encoder;

static bool new_test_data;
static MonitorData test_data;
static int test_data_address;

#define size sizeof(MonitorData)

MonitorData data;

static void test_flash()
{
  printf("Flash init\n");

  /*Error flashError;
  BSP_W25Qx_Init(&flashError);
  if (flashError != SUCCESS) {
    printf("Error initializing flash:%d\n", flashError);
    return;
  }*/
  //BSP_W25Qx_Init(NULL);

    if (!BSP_W25Qx_Lock()) {
    printf("Failed to lock flash\n");
    return;
  }
  printf("getting id\n");
  //shift_out( 0x90);
   // shift_out(0x00);
   // shift_out(0x00);
    shift_out(_W25QXX_DO, _W25QXX_CLK, MSBFIRST, 8, 0x90);
    shift_out(_W25QXX_DO, _W25QXX_CLK, MSBFIRST, 8, 0);
    shift_out(_W25QXX_DO, _W25QXX_CLK, MSBFIRST, 8, 0);
    return;
  uint8_t ID[4];
  BSP_W25Qx_Read_ID(ID);
  printf(" W25Qxx ID is : ");
	for(int i=0;i<2;i++)
	{
		printf("0x%02X ",ID[i]);
	}
	printf("\r\n");
  return;
  int eraseBlock = 0;
  //BSP_W25Qx_Erase_Block(lastEraseBlock);
  for (int i=0;i<10;i++) {
    data.forceRaw = i*2;
    data.encoderRaw = i*2;
    data.timems = i*4;
    
    if (size*(i+1)/W25Q64FV_SUBSECTOR_SIZE >= eraseBlock) {
      printf("Erasing flash:%d\n",eraseBlock);
      BSP_W25Qx_Erase_Block(eraseBlock*W25Q64FV_SUBSECTOR_SIZE);
      eraseBlock++;
    }
    BSP_W25Qx_Write(&data,size*i,size);
  }
    return;
  for (int i=0;i<10;i++) {
    BSP_W25Qx_Read(&data,size*i,size);
    printf("0x%04X %d %d %d\n",size*i, data.forceRaw, data.encoderRaw, data.timems);
  }
  BSP_W25Qx_Unlock();
}

MonitorData *monitor_read_data(int addr)
{
  if (addr > 0) test_data_address=addr;
  
  new_test_data = false;
  while(!new_test_data) {}
  
  return &test_data;
}

/*responsible for reading/writing data to buffer/test output*/
static void monitor_cog(Monitor *monitor)
{
  new_test_data = false;
  test_data_address = 0;
    //mount("/da", _vfs_open_sdcardx(40, 42, 41, 39)); // Mount data card using default pins

  Error flashError;
  BSP_W25Qx_Init(&flashError);
  while (flashError != SUCCESS) {
    printf("Error initializing flash:%d\n", flashError);
    BSP_W25Qx_Init(&flashError);
    _waitms(100);
  }

    // Connect Force Gauge
    if (force_gauge_begin(&forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX) == SUCCESS)
    {
        state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, true);
    }else
    {
        state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, false);
    }

    // Set up encoder
    encoder.start(DYN4_ENCODER_A, DYN4_ENCODER_B, -1, false, 0, -100000, 100000);

    int delayCycles = CLKFREQ / monitor->sampleRate;
    printf("Monitor Cog Started at %dHz with delay of:%d\n", monitor->sampleRate,delayCycles);
    FILE *file = NULL;

    monitorWriteData = false;
    bool firstTime = true;
    int flashAddr = 0;
    int eraseBlock = 0;
    while (1)
    {
        /*Delay to run at sampleRate, replace with _waitcnt*/
        uint32_t waitcycles = _cnt() + delayCycles;

        Error err;
        int forceRawTemp = force_gauge_get_raw(&forceGauge, &err); // Get Force
        if (err == SUCCESS)
        {
            monitor->data.forceRaw = forceRawTemp;
        }
        else // Force gauge isnt responding attempt to reconnect
        {
            printf("Force Gauge disconnected, attempting to reconnect\n");
            force_gauge_stop(&forceGauge);
            if (force_gauge_begin(&forceGauge, FORCE_GAUGE_RX, FORCE_GAUGE_TX) == SUCCESS)
            {
                state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, true);
            }else
            {
                state_machine_set(monitor->machineState, PARAM_MACHINE_FORCE_GAUGE_COM, false);
            }
        }

        monitor->data.encoderRaw = encoder.value();

        //monitor->data.positionum = (1000 * steps_to_mm(monitor->encoder->value(), &(monitor->profile->configuration))); // Get Position in um
        monitor->data.timems = _getms();
        monitor->data.force = raw_to_force(monitor->data.forceRaw, monitor->configuration)/1000.0; // Convert Force to N
        monitor->data.position = steps_to_mm(monitor->data.encoderRaw, monitor->configuration); // Convert steps to mm
        if (monitorWriteData)
        {
            
            if (flashAddr == 0) {
                
                while(!BSP_W25Qx_Lock()) printf("flash locked\n"); //Wait for flash to be available, then lock it.
                printf("flash claimed\n");
            }

            if ((flashAddr + size*2)/W25Q64FV_SUBSECTOR_SIZE >= eraseBlock) {
                printf("Erasing flash:%d\n",eraseBlock);
                BSP_W25Qx_Erase_Block(eraseBlock*W25Q64FV_SUBSECTOR_SIZE);
                _waitms(100);
                eraseBlock++;
            }
            BSP_W25Qx_Write(&(monitor->data),flashAddr,sizeof(MonitorData));
            //printf("wrote data to flash at address:%d\n",flashAddr); //not getting to this line
            flashAddr += sizeof(MonitorData);
        }
        else
        {
            if (flashAddr != 0)
            {
                printf("Monitor done writing\n");
                MonitorData temp;
                temp.timems = -1; // Set to invalid value to indicate end of data
                temp.forceRaw = 0;
                temp.encoderRaw = 0;
                BSP_W25Qx_Write(&temp,flashAddr,sizeof(MonitorData));
                MonitorData data;
                int addr = 0;
                do {
                    BSP_W25Qx_Read(&data,addr,sizeof(MonitorData));
                  //  printf("0x%04X %d %d %d\n",addr,data.forceRaw,data.encoderRaw,data.timems);
                    addr += sizeof(MonitorData);
                }
                while(data.timems > -1); // wait for invalid time
                BSP_W25Qx_Unlock();
            }
            flashAddr = 0;
            eraseBlock = 0; //@TODO combine these into different method using flashAddr to det new page
            if (!new_test_data)
            {
              while(!BSP_W25Qx_Lock()) printf("flash locked\n"); // Wait for flash to be available, then lock it.
              
              BSP_W25Qx_Read(&test_data, test_data_address, size);
              test_data_address += size;

              BSP_W25Qx_Unlock();
              new_test_data = true;
            }
        }
        /*if (waitcycles < (uint32_t)getcnt())
            printf("Error: Monitor COG is running too slow:%lu,%lu\n", ((uint32_t)getcnt() - waitcycles),delayCycles);
        else
            waitcnt(waitcycles);*/ //@TODO this delay shouldd only affect the writing of dfata
    }
}

bool monitor_begin(Monitor *monitor, MachineState *machineState, MachineConfiguration *configuration, int sampleRate)
{
    monitor->configuration = configuration;
    monitor->sampleRate = sampleRate;
    monitor->machineState = machineState;
    monitor->cogid = _cogstart_C(monitor_cog, monitor, &monitor_stack[0], sizeof(long) * MONITOR_MEMORY_SIZE);
    if (monitor->cogid != -1)
    {
        return true;
    }
    return false;
}
