#ifndef __PCA9685_CPP__
#define __PCA9685_CPP__



#include "pca9685.h"

PCA9685::PCA9685(int bus_ch, int dev_addr) {
  channel = bus_ch;
  address = dev_addr;
  polling_ms = 50;//PCA9685_POLLING_MS;
}

void PCA9685::init(void) {
  initialized = false;
  state = s_detect;
}

int PCA9685::prop_count(void) { return 0; }

bool PCA9685::prop_name(int index, char *name) { return false; }

bool PCA9685::prop_unit(int index, char *unit) { return false; }

bool PCA9685::prop_attr(int index, char *attr) { return false; }

bool PCA9685::prop_read(int index, char *value) { return false; }

bool PCA9685::prop_write(int index, char *value) { return false; }

void PCA9685::process(Driver *drv) {
  I2CDev *i2c = (I2CDev *)drv;

  switch (state) {
  case s_detect:
    polling_tickcnt = get_tickcnt();
    if (i2c->detect(channel, address) == ESP_OK) {
      error = false;
      initialized = true;
      reset();
      state = s_running;
    } else {
      state = s_error;
    }
    break;
  case s_running:
    //
    break;
  case s_error:
    error = true;
    initialized = false;
    tickcnt = get_tickcnt();
    state = s_wait;
    break;
  case s_wait:
    if (error) {
      if (is_tickcnt_elapsed(tickcnt, 1000)) {
        state = s_detect;
      }
    }
    break;
  }
}

void PCA9685::reset() {
  send(PCA9685_MODE1, 0x0);
  send(PCA9685_MODE2, 0x4);
}
//! Set the frequency of PWM
/*!
 \param freq desired frequency. 40Hz to 1000Hz using internal 25MHz oscillator.
 */
void PCA9685::setPWMFreq(int freq) {

  uint8_t prescale_val = (CLOCK_FREQ / 4096 / freq) - 1;
  send(PCA9685_MODE1, 0x10);
  send(PCA9685_PRESCALE, prescale_val);
  send(PCA9685_MODE1, 0x80);             // restart
  send(PCA9685_MODE2, 0x4);             // totem pole (default)
}

//! PWM a single channel
/*!
 \param led channel (1-16) to set PWM value for
 \param value 0-4095 value for PWM
 */
void PCA9685::setPWM(uint8_t led, int value) {
  int OFF = (int)(4095 * value / 100);
  int ON = (int)(4095 - OFF);
 // printf("LED = %d \nOFF = %d \nON = %d", led, OFF, ON);
  setPWM(led, ON, OFF);
  }
//! PWM a single channel with custom on time
/*!
 \param led channel (1-16) to set PWM value for
 \param on_value 0-4095 value to turn on the pulse
 \param off_value 0-4095 value to turn off the pulse
 */
void PCA9685::setPWM(uint8_t led, int on_value, int off_value) {
  send(LED0_ON_L + 4 * led, on_value & 0xFF);
  send(LED0_ON_H + 4 * led, on_value >> 8);
  send(LED0_OFF_L + 4 * led, off_value & 0xFF);
  send(LED0_OFF_H + 4 * led, off_value >> 8);
}


//! Get current PWM value
/*!
 \param led channel (1-16) to get PWM value from
 */
/*
int PCA9685::getPWM(uint8_t led) {
  int ledval = 0;
  ledval = i2c->read_byte(LED0_OFF_H + LED_MULTIPLYER * (led - 1));
  ledval = ledval & 0xf;
  ledval <<= 8;
  ledval += i2c->read_byte(LED0_OFF_L + LED_MULTIPLYER * (led - 1));
  return ledval;
}
*/

//Send Form ikb_1

// Send only, no parameter, no request
bool PCA9685::send(uint8_t command) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, command, true);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	
	return ret == ESP_OK;
}

// Send command and parameter, no request
bool PCA9685::send(uint8_t command, uint8_t parameter) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, command, true);
	i2c_master_write_byte(cmd, parameter, true);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	
	return ret == ESP_OK;
}

bool PCA9685::send(uint8_t command, int parameter) {
  uint8_t para = parameter & 0xFF;
  return send(command, para);
}
/*
// Send command and parameter and request
bool PCA9685::send(uint8_t command, uint8_t parameter, int request_length) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, command, true);
	i2c_master_write_byte(cmd, parameter, true);
	
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, true);
	if (request_length > 1) {
        i2c_master_read(cmd, read_data, request_length - 1, I2C_MASTER_ACK);
    }
	i2c_master_read_byte(cmd, read_data + request_length - 1, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	
	return ret == ESP_OK;
}

// Send command and request, no parameter 
bool PCA9685::send(uint8_t command, int request_length) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, command, true);
	
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, true);
	if (request_length > 1) {
        i2c_master_read(cmd, read_data, request_length - 1, I2C_MASTER_ACK);
    }
	i2c_master_read_byte(cmd, read_data + request_length - 1, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_1, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	
	return ret == ESP_OK;
}
*/
#endif