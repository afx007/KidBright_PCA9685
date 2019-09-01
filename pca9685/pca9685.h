#ifndef __PCA9685_H__
#define __PCA9685_H__

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "kidbright32.h"

#include "device.h"
#include "driver.h"
#include "i2c-dev.h"

//#define PCA9685_POLLING_MS 1000

#define PCA9685_SUBADR1 0x2 /**< i2c bus address 1 */
#define PCA9685_SUBADR2 0x3 /**< i2c bus address 2 */
#define PCA9685_SUBADR3 0x4 /**< i2c bus address 3 */

#define PCA9685_MODE1 0x0     /**< Mode Register 1 */
#define PCA9685_MODE2 0x1     /**< Mode Register 2 */
#define PCA9685_PRESCALE 0xFE /**< Prescaler for PWM output frequency */

#define START_LED_REG 0x6
#define LED0_ON_L 0x6  /**< LED0 output and brightness control byte 0 */
#define LED0_ON_H 0x7  /**< LED0 output and brightness control byte 1 */
#define LED0_OFF_L 0x8 /**< LED0 output and brightness control byte 2 */
#define LED0_OFF_H 0x9 /**< LED0 output and brightness control byte 3 */

#define ALLLED_ON_L 0xFA  /**< load all the LEDn_ON registers, byte 0 */
#define ALLLED_ON_H 0xFB  /**< load all the LEDn_ON registers, byte 1 */
#define ALLLED_OFF_L 0xFC /**< load all the LEDn_OFF registers, byte 0 */
#define ALLLED_OFF_H 0xFD /**< load all the LEDn_OFF registers, byte 1 */

#define PCA9685_MULTIPLYER 4

#define CLOCK_FREQ 25000000.0 // 25MHz default osc clock

class PCA9685 : public Device {
private:
  enum { s_detect, s_running, s_error, s_wait } state;
  TickType_t tickcnt, polling_tickcnt;
  void reset(void);
  int _PWM_ON[16];
  int _PWM_OFF[16];
  //bool _setFreq;
  uint8_t _regLED, _MaxLED;

  bool send(uint8_t command) ;
	bool send(uint8_t command, uint8_t parameter) ;
 	bool send(uint8_t command, int parameter) ;
	//bool send(uint8_t command, uint8_t parameter, int request_length) ;
	//bool send(uint8_t command, int request_length) ;

public:
  PCA9685(int bus_ch, int dev_addr);

  void init(void);
  void process(Driver *drv);
  int prop_count(void);
  bool prop_name(int index, char *name);
  bool prop_unit(int index, char *unit);
  bool prop_attr(int index, char *attr);
  bool prop_read(int index, char *value);
  bool prop_write(int index, char *value);

  void setPWMFreq(int freq);
  void setPWM(uint8_t led, int on_value, int off_value);
  void setPWM(uint8_t led, int value);
  int getPWM(uint8_t led);
  //void setMax(uint8_t max);
};

#endif