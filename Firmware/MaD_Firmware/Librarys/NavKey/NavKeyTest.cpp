/**
* This is the main Blank Simple C++ Project program file.
*/
#include "i2cNavKey.h"

#include <propeller.h> // Propeller-specific functions

const int IntPin = 22; /* Definition of the interrupt pin*/
//Class initialization with the I2C addresses
i2cNavKey navkey(0b0010010); /* Default address when no jumper are soldered */

uint8_t pwm, fade = 0;

void UP_Button_Pressed(i2cNavKey *p)
{
    print("Button UP Pressed!\n");
}

void DOWN_Button_Pressed(i2cNavKey *p)
{
    print("Button DOWN Pressed!\n");
}

void LEFT_Button_Pressed(i2cNavKey *p)
{
    print("Button LEFT Pressed!\n");
}

void RIGHT_Button_Pressed(i2cNavKey *p)
{
    print("Button RIGHT Pressed!\n");
}

void CENTRAL_Button_Pressed(i2cNavKey *p)
{
    print("Button Central Pressed!\n");
}

void CENTRAL_Button_Double(i2cNavKey *p)
{
    print("Button Central Double push!\n");
}

void Encoder_Rotate(i2cNavKey *p)
{
    print("%d\n", p->readCounterInt());
}

void testNavKey()
{
    // Add your code here
    print("**** I2C navkey V2 basic example ****\n");
    /*
      INT_DATA= The register are considered integer.
      WRAP_ENABLE= The WRAP option is enabled
      DIRE_RIGHT= navkey right direction increase the value
      IPUP_ENABLE= INT pin have the pull-up enabled.
  */

    navkey.pins(28, 29);
    navkey.reset();
    navkey.begin(i2cNavKey::INT_DATA | i2cNavKey::WRAP_ENABLE | i2cNavKey::DIRE_RIGHT | i2cNavKey::IPUP_ENABLE);

    navkey.writeCounter((int32_t)0); /* Reset the counter value */
    navkey.writeMax((int32_t)10);    /* Set the maximum threshold*/
    navkey.writeMin((int32_t)-10);   /* Set the minimum threshold */
    navkey.writeStep((int32_t)1);    /* Set the step to 1*/

    navkey.writeDoublePushPeriod(30); /*Set a period for the double push of 300ms */

    navkey.onUpPush = UP_Button_Pressed;
    navkey.onDownPush = DOWN_Button_Pressed;
    navkey.onRightPush = RIGHT_Button_Pressed;
    navkey.onLeftPush = LEFT_Button_Pressed;
    navkey.onCentralPush = CENTRAL_Button_Pressed;
    navkey.onCentralDoublePush = CENTRAL_Button_Double;
    navkey.onChange = Encoder_Rotate;

    navkey.autoconfigInterrupt(); /* Enable the interrupt with the attached callback */

    print("ID CODE: %u\n", navkey.readIDCode());
    print("Board Version: %u\n", navkey.readVersion());

    while (1)
    {
        navkey.updateStatus();
        pause(10);
    }
}
