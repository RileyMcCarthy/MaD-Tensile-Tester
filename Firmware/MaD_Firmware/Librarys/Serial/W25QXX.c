#include "W25Qxx.h"
#include "simpletools.h"

#define HAL_GetTick() _getms()

static int ws25qxxSemaphore = -1;
// static struct __using("jm_spi.spin2") spi;

static void HAL_SPI_Receive(void *arg, uint8_t *pData, uint16_t size, uint32_t Timeout)
{
	for (int i = 0; i < size; i++)
	{
		pData[i] = shift_in(_W25QXX_DI, _W25QXX_CLK, MSBPRE, 8);
		// serial_debug("shift in: %d\n", pData[i]);
		//  pData[i] = spi.shiftin(1, 8);
	}
}

static void HAL_SPI_Transmit(void *arg, uint8_t *pData, uint16_t size, uint32_t Timeout)
{
	for (int i = 0; i < size; i++)
	{
		shift_out(_W25QXX_DO, _W25QXX_CLK, MSBFIRST, 8, pData[i]);
		// spi.shiftout(1, pData[i], 8);
	}
}

static void W25Qx_Enable()
{
	_pinl(_W25QXX_CS_PIN);
}

static void W25Qx_Disable()
{
	_pinh(_W25QXX_CS_PIN);
}

static void BSP_W25Qx_Reset(void)
{
	uint8_t cmd[2] = {RESET_ENABLE_CMD, RESET_MEMORY_CMD};

	W25Qx_Enable();
	/* Send the reset command */
	HAL_SPI_Transmit(NULL, cmd, 2, W25Qx_TIMEOUT_VALUE);
	W25Qx_Disable();
}

/**********************************************************************************
 *  The functionality :  Module initialization
 */
uint8_t BSP_W25Qx_Init(Error *err)
{
	seterror(err, SUCCESS);
	// Allocate semaphore if it does not exist
	if (ws25qxxSemaphore == -1)
	{
		serial_debug("Allocating semaphore\n");
		ws25qxxSemaphore = _locknew();
		if (ws25qxxSemaphore == -1)
		{
			seterror(err, SEM_NOT_AVAILABLE);
			serial_debug("Error allocating semaphore\n");
			return 0;
		}
	}
	// spi.start(_W25QXX_DI, _W25QXX_DO, _W25QXX_CLK, 10);
	BSP_W25Qx_Reset();

	uint8_t ID[4];
	BSP_W25Qx_Read_ID(ID);
	if (ID[0] != 0xEF)
	{
		seterror(err, W25QXX_INVALID_ID);
		serial_debug("Invalid ID\n");
		return 0;
	}

	return BSP_W25Qx_GetStatus();
}

// Locks the device semmpaphore. Returns true if device was available
bool BSP_W25Qx_Lock()
{
	// Check if semaphore is available to lock it
	return _locktry(ws25qxxSemaphore) != 0;
}

void BSP_W25Qx_Unlock()
{
	_lockrel(ws25qxxSemaphore);
}

/**********************************************************************************
 *  The functionality :  Get device status
 */
static uint8_t BSP_W25Qx_GetStatus(void)
{
	uint8_t cmd[] = {READ_STATUS_REG1_CMD};
	uint8_t status;

	W25Qx_Enable();
	/* Send the read status command */
	HAL_SPI_Transmit(NULL, cmd, 1, W25Qx_TIMEOUT_VALUE);
	/* Reception of the data */
	HAL_SPI_Receive(NULL, &status, 1, W25Qx_TIMEOUT_VALUE);
	W25Qx_Disable();
	// serial_debug("Status: %x\n", status);
	/* Check the value of the register */
	if ((status & W25Q128FV_FSR_BUSY) != 0)
	{
		return W25Qx_BUSY;
	}
	else
	{
		return W25Qx_OK;
	}
}

/**********************************************************************************
 *  The functionality :  Write enable
 */
uint8_t BSP_W25Qx_WriteEnable(void)
{
	uint8_t cmd[] = {WRITE_ENABLE_CMD};
	uint32_t tickstart = HAL_GetTick();

	/*Select the FLASH: Chip Select low */
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(NULL, cmd, 1, W25Qx_TIMEOUT_VALUE);
	/*Deselect the FLASH: Chip Select high */
	W25Qx_Disable();
	// serial_debug("W25Qx_WriteEnable: %d\n", HAL_GetTick() - tickstart);
	/* Wait the end of Flash writing */
	while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
	{
		/* Check for the Timeout */
		if ((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
		{
			serial_debug("W25Qx_WriteEnable: Timeout\n");
			return W25Qx_TIMEOUT;
		}
	}
	// serial_debug("W25Qx_WriteEnable: OK\n");
	return W25Qx_OK;
}

/**********************************************************************************
 *  The functionality :  Get the device ID
 */
void BSP_W25Qx_Read_ID(uint8_t *ID)
{
	uint8_t cmd[4] = {READ_ID_CMD, 0x00, 0x00, 0x00};

	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(NULL, cmd, 4, W25Qx_TIMEOUT_VALUE);
	/* Reception of the data */
	HAL_SPI_Receive(NULL, ID, 2, W25Qx_TIMEOUT_VALUE);
	W25Qx_Disable();
}

/**********************************************************************************
 *  The functionality :  Reading data
 *  Input parameters :  Cache array pointer 、 Read the address 、 Number of bytes
 */
uint8_t BSP_W25Qx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t size)
{
	uint8_t cmd[4];

	/* Configure the command */
	cmd[0] = READ_CMD;
	cmd[1] = (uint8_t)(ReadAddr >> 16);
	cmd[2] = (uint8_t)(ReadAddr >> 8);
	cmd[3] = (uint8_t)(ReadAddr);

	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(NULL, cmd, 4, W25Qx_TIMEOUT_VALUE);
	/* Reception of the data */
	HAL_SPI_Receive(NULL, pData, size, W25Qx_TIMEOUT_VALUE);
	W25Qx_Disable();
	return W25Qx_OK;
}

/**********************************************************************************
 *  The functionality :  Writing data
 *  Input parameters :  Cache array pointer 、 Write the address 、 Number of bytes
 */
uint8_t BSP_W25Qx_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t size)
{
	uint8_t cmd[4];
	uint32_t end_addr, current_size, current_addr;
	uint32_t tickstart = HAL_GetTick();

	/* Calculation of the size between the write address and the end of the page */
	current_addr = 0;

	while (current_addr <= WriteAddr)
	{
		// serial_debug("current_addr = %d\n", current_addr);
		current_addr += W25Q128FV_PAGE_SIZE;
	}
	current_size = current_addr - WriteAddr;

	/* Check if the size of the data is less than the remaining place in the page */
	if (current_size > size)
	{
		current_size = size;
	}

	/* Initialize the adress variables */
	current_addr = WriteAddr;
	end_addr = WriteAddr + size;

	/* Perform the write page by page */
	do
	{
		/* Configure the command */
		cmd[0] = PAGE_PROG_CMD;
		cmd[1] = (uint8_t)(current_addr >> 16);
		cmd[2] = (uint8_t)(current_addr >> 8);
		cmd[3] = (uint8_t)(current_addr);
		/* Enable write operations */
		BSP_W25Qx_WriteEnable();
		W25Qx_Enable();
		/* Send the command */
		HAL_SPI_Transmit(NULL, cmd, 4, W25Qx_TIMEOUT_VALUE);

		/* Transmission of the data */
		HAL_SPI_Transmit(NULL, pData, current_size, W25Qx_TIMEOUT_VALUE);
		W25Qx_Disable();
		/* Wait the end of Flash writing */
		while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
		{
			/* Check for the Timeout */
			if ((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
			{
				serial_debug("W25Qx_Write timeout\n");
				return W25Qx_TIMEOUT;
			}
		}

		/* Update the address and size variables for next page programming */
		current_addr += current_size;
		pData += current_size;
		current_size = ((current_addr + W25Q128FV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128FV_PAGE_SIZE;
	} while (current_addr < end_addr);

	return W25Qx_OK;
}

/**********************************************************************************
 *  The functionality :  Sector erase
 *  Input parameters :  Address
 */
uint8_t BSP_W25Qx_Erase_Block(uint32_t Address)
{
	uint8_t cmd[4];
	uint32_t tickstart = HAL_GetTick();
	cmd[0] = SECTOR_ERASE_CMD;
	cmd[1] = (uint8_t)(Address >> 16);
	cmd[2] = (uint8_t)(Address >> 8);
	cmd[3] = (uint8_t)(Address);

	/* Enable write operations */
	BSP_W25Qx_WriteEnable();

	/*Select the FLASH: Chip Select low */
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(NULL, cmd, 4, W25Qx_TIMEOUT_VALUE);
	/*Deselect the FLASH: Chip Select high */
	W25Qx_Disable();

	/* Wait the end of Flash writing */
	while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
	{
		/* Check for the Timeout */
		if ((HAL_GetTick() - tickstart) > W25Q128FV_SECTOR_ERASE_MAX_TIME)
		{
			serial_debug("W25Qx_Erase_Block timeout\n");
			return W25Qx_TIMEOUT;
		}
	}
	return W25Qx_OK;
}

/**********************************************************************************
 *  The functionality :  Chip erase
 */
uint8_t BSP_W25Qx_Erase_Chip(void)
{
	uint8_t cmd[4];
	uint32_t tickstart = HAL_GetTick();
	cmd[0] = CHIP_ERASE_CMD;

	/* Enable write operations */
	BSP_W25Qx_WriteEnable();

	/*Select the FLASH: Chip Select low */
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(NULL, cmd, 1, W25Qx_TIMEOUT_VALUE);
	/*Deselect the FLASH: Chip Select high */
	W25Qx_Disable();

	/* Wait the end of Flash writing */
	while (BSP_W25Qx_GetStatus() == W25Qx_BUSY)
	{
		/* Check for the Timeout */
		if ((HAL_GetTick() - tickstart) > W25Q128FV_BULK_ERASE_MAX_TIME)
		{
			serial_debug("timeout\n");
			return W25Qx_TIMEOUT;
		}
	}
	return W25Qx_OK;
}
