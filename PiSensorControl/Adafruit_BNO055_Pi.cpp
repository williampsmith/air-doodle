/***************************************************************************
  This is a library for the BNO055 orientation sensor

  Designed specifically to work with the Adafruit BNO055 Breakout.

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products

  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by KTOWN for Adafruit Industries.

  MIT license, all text above must be included in any redistribution
 ***************************************************************************/

/*
 * Updated by Mitchell Oleson for Air Doodle and other projects on the Raspberry Pi.
 */

#include "Adafruit_BNO055_Pi.h"

// Creates a new sensor
Adafruit_BNO055::Adafruit_BNO055(int32_t sensorID, uint8_t address) {
  _sensorID = sensorID;
  _address = address;
}


// PUBLIC FUNCTIONS //

// Sets up the hardware
bool Adafruit_BNO055::begin(adafruit_bno055_opmode_t mode) {
  // Enable I2C
  i2c = wiringPiI2CSetup(_address);

  // Make sure we have the right device
  uint8_t id = read8(BNO055_CHIP_ID_ADDR);
  if (id != BNO055_ID) {
    delay(1000); // hold on for boot
    id = read8(BNO055_CHIP_ID_ADDR);
    if (id != BNO055_ID) {
      return false;  // still not? ok bail
    }
  }

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);

  // Reset
  write8(BNO055_SYS_TRIGGER_ADDR, 0x20);
  while (read8(BNO055_CHIP_ID_ADDR) != BNO055_ID) {
    delay(10);
  }
  delay(50);

  // Set to normal power mode
  write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
  delay(10);

  write8(BNO055_PAGE_ID_ADDR, 0);

  // Set the output units
  /*
  uint8_t unitsel = (0 << 7) | // Orientation = Android
                    (0 << 4) | // Temperature = Celsius
                    (0 << 2) | // Euler = Degrees
                    (1 << 1) | // Gyro = Rads
                    (0 << 0);  // Accelerometer = m/s^2
  write8(BNO055_UNIT_SEL_ADDR, unitsel);
  */

  // Configure axis mapping (see section 3.4)
  /*
  write8(BNO055_AXIS_MAP_CONFIG_ADDR, REMAP_CONFIG_P2); // P0-P7, Default is P1
  delay(10);
  write8(BNO055_AXIS_MAP_SIGN_ADDR, REMAP_SIGN_P2); // P0-P7, Default is P1
  delay(10);
  */
  
  write8(BNO055_SYS_TRIGGER_ADDR, 0x0);
  delay(10);
  // Set the requested operating mode (see section 3.3)
  setMode(mode);
  delay(20);

  return true;
}

// Sets chip operating mode
void Adafruit_BNO055::setMode(adafruit_bno055_opmode_t mode) {
  _mode = mode;
  write8(BNO055_OPR_MODE_ADDR, _mode);
  delay(30);
}

// Use the external 32.768KHz crystal
void Adafruit_BNO055::setExtCrystalUse(boolean usextal) {
  adafruit_bno055_opmode_t modeback = _mode;

  // Switch to config mode (just in case since this is the default)
  setMode(OPERATION_MODE_CONFIG);
  delay(25);
  write8(BNO055_PAGE_ID_ADDR, 0);
  if (usextal) {
    write8(BNO055_SYS_TRIGGER_ADDR, 0x80);
  } else {
    write8(BNO055_SYS_TRIGGER_ADDR, 0x00);
  }
  delay(10);
  // Set the requested operating mode (see section 3.3)
  setMode(modeback);
  delay(20);
}

// Gets the latest system status info
void Adafruit_BNO055::getSystemStatus(uint8_t *system_status, uint8_t *self_test_result, uint8_t *system_error) {
  write8(BNO055_PAGE_ID_ADDR, 0);

  /* System Status (see section 4.3.58)
     ---------------------------------
     0 = Idle
     1 = System Error
     2 = Initializing Peripherals
     3 = System Iniitalization
     4 = Executing Self-Test
     5 = Sensor fusio algorithm running
     6 = System running without fusion algorithms */

  if (system_status != 0) {
    *system_status    = read8(BNO055_SYS_STAT_ADDR);
  }

  /* Self Test Results (see section )
     --------------------------------
     1 = test passed, 0 = test failed

     Bit 0 = Accelerometer self test
     Bit 1 = Magnetometer self test
     Bit 2 = Gyroscope self test
     Bit 3 = MCU self test

     0x0F = all good! */

  if (self_test_result != 0) {
    *self_test_result = read8(BNO055_SELFTEST_RESULT_ADDR);
  }

  /* System Error (see section 4.3.59)
     ---------------------------------
     0 = No error
     1 = Peripheral initialization error
     2 = System initialization error
     3 = Self test result failed
     4 = Register map value out of range
     5 = Register map address out of range
     6 = Register map write error
     7 = BNO low power mode not available for selected operat ion mode
     8 = Accelerometer power mode not available
     9 = Fusion algorithm configuration error
     A = Sensor configuration error */

  if (system_error != 0) {
    *system_error     = read8(BNO055_SYS_ERR_ADDR);
  }

  delay(200);
}

// Gets the chip revision numbers
void Adafruit_BNO055::getRevInfo(adafruit_bno055_rev_info_t* info) {
  uint8_t a, b;

  memset(info, 0, sizeof(adafruit_bno055_rev_info_t));

  // Check the accelerometer revision
  info->accel_rev = read8(BNO055_ACCEL_REV_ID_ADDR);

  // Check the magnetometer revision
  info->mag_rev   = read8(BNO055_MAG_REV_ID_ADDR);

  // Check the gyroscope revision
  info->gyro_rev  = read8(BNO055_GYRO_REV_ID_ADDR);

  // Check the SW revision
  info->bl_rev    = read8(BNO055_BL_REV_ID_ADDR);

  a = read8(BNO055_SW_REV_ID_LSB_ADDR);
  b = read8(BNO055_SW_REV_ID_MSB_ADDR);
  info->sw_rev = (((uint16_t)b) << 8) | ((uint16_t)a);
}

// Gets current calibration state.  Each value should be a uint8_t
// pointer and it will be set to 0 if not calibrated and 3 if
// fully calibrated.
void Adafruit_BNO055::getCalibration(uint8_t* sys, uint8_t* gyro, uint8_t* accel, uint8_t* mag) {
  uint8_t calData = read8(BNO055_CALIB_STAT_ADDR);
  if (sys != NULL) {
    *sys = (calData >> 6) & 0x03;
  }
  if (gyro != NULL) {
    *gyro = (calData >> 4) & 0x03;
  }
  if (accel != NULL) {
    *accel = (calData >> 2) & 0x03;
  }
  if (mag != NULL) {
    *mag = calData & 0x03;
  }
}

// Gets the temperature in degrees celsius
int8_t Adafruit_BNO055::getTemp(void) {
  int8_t temp = (int8_t)(read8(BNO055_TEMP_ADDR));
  return temp;
}

// Gets a vector reading from the specified source
std::vector<double> Adafruit_BNO055::getVector(adafruit_vector_type_t vector_type) {
  std::vector<double> xyz(3);
  uint8_t buffer[6];
  memset (buffer, 0, 6);

  int16_t x, y, z;
  x = y = z = 0;

  // Read vector data (6 bytes)
  readLen((adafruit_bno055_reg_t)vector_type, buffer, 6);

  x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
  y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
  z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);

  // Convert the value to an appropriate range (section 3.6.4)
  // and assign the value to the Vector type
  switch(vector_type) {
    case VECTOR_MAGNETOMETER:
      // 1uT = 16 LSB
      xyz[0] = ((double)x)/16.0;
      xyz[1] = ((double)y)/16.0;
      xyz[2] = ((double)z)/16.0;
      break;
    case VECTOR_GYROSCOPE:
      // 1rps = 900 LSB
      xyz[0] = ((double)x)/900.0;
      xyz[1] = ((double)y)/900.0;
      xyz[2] = ((double)z)/900.0;
      break;
    case VECTOR_EULER:
      // 1 degree = 16 LSB
      xyz[0] = ((double)x)/16.0;
      xyz[1] = ((double)y)/16.0;
      xyz[2] = ((double)z)/16.0;
      break;
    case VECTOR_ACCELEROMETER:
    case VECTOR_LINEARACCEL:
    case VECTOR_GRAVITY:
      // 1m/s^2 = 100 LSB
      xyz[0] = ((double)x)/100.0;
      xyz[1] = ((double)y)/100.0;
      xyz[2] = ((double)z)/100.0;
      break;
  }

  return xyz;
}

// Provides the sensor_t data for this sensor
void Adafruit_BNO055::getSensor(sensor_t *sensor) {
  // Clear the sensor_t object
  memset(sensor, 0, sizeof(sensor_t));

  // Insert the sensor name in the fixed length char array
  strncpy (sensor->name, "BNO055", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_ORIENTATION;
  sensor->min_delay   = 0;
  sensor->max_value   = 0.0F;
  sensor->min_value   = 0.0F;
  sensor->resolution  = 0.01F;
}

// Reads the sensor and returns the data as a sensors_event_t
bool Adafruit_BNO055::getEvent(sensors_event_t *event) {
  // Clear the event
  memset(event, 0, sizeof(sensors_event_t));

  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_ORIENTATION;
  event->timestamp = millis();

  // Get a Euler angle sample for orientation
  std::vector<double> euler = getVector(Adafruit_BNO055::VECTOR_EULER);
  event->orientation.x = euler.x();
  event->orientation.y = euler.y();
  event->orientation.z = euler.z();

  return true;
}

// Reads the sensor's offset registers into a byte array
bool Adafruit_BNO055::getSensorOffsets(uint8_t* calibData) {
    if (isFullyCalibrated()) {
        adafruit_bno055_opmode_t lastMode = _mode;
        setMode(OPERATION_MODE_CONFIG);

        readLen(ACCEL_OFFSET_X_LSB_ADDR, calibData, NUM_BNO055_OFFSET_REGISTERS);

        setMode(lastMode);
        return true;
    }
    return false;
}

// Reads the sensor's offset registers into an offset struct
bool Adafruit_BNO055::getSensorOffsets(adafruit_bno055_offsets_t &offsets_type) {
    if (isFullyCalibrated()) {
        adafruit_bno055_opmode_t lastMode = _mode;
        setMode(OPERATION_MODE_CONFIG);
        delay(25);

        offsets_type.accel_offset_x = (read8(ACCEL_OFFSET_X_MSB_ADDR) << 8) | (read8(ACCEL_OFFSET_X_LSB_ADDR));
        offsets_type.accel_offset_y = (read8(ACCEL_OFFSET_Y_MSB_ADDR) << 8) | (read8(ACCEL_OFFSET_Y_LSB_ADDR));
        offsets_type.accel_offset_z = (read8(ACCEL_OFFSET_Z_MSB_ADDR) << 8) | (read8(ACCEL_OFFSET_Z_LSB_ADDR));

        offsets_type.gyro_offset_x = (read8(GYRO_OFFSET_X_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_X_LSB_ADDR));
        offsets_type.gyro_offset_y = (read8(GYRO_OFFSET_Y_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_Y_LSB_ADDR));
        offsets_type.gyro_offset_z = (read8(GYRO_OFFSET_Z_MSB_ADDR) << 8) | (read8(GYRO_OFFSET_Z_LSB_ADDR));

        offsets_type.mag_offset_x = (read8(MAG_OFFSET_X_MSB_ADDR) << 8) | (read8(MAG_OFFSET_X_LSB_ADDR));
        offsets_type.mag_offset_y = (read8(MAG_OFFSET_Y_MSB_ADDR) << 8) | (read8(MAG_OFFSET_Y_LSB_ADDR));
        offsets_type.mag_offset_z = (read8(MAG_OFFSET_Z_MSB_ADDR) << 8) | (read8(MAG_OFFSET_Z_LSB_ADDR));

        offsets_type.accel_radius = (read8(ACCEL_RADIUS_MSB_ADDR) << 8) | (read8(ACCEL_RADIUS_LSB_ADDR));
        offsets_type.mag_radius = (read8(MAG_RADIUS_MSB_ADDR) << 8) | (read8(MAG_RADIUS_LSB_ADDR));

        setMode(lastMode);
        return true;
    }
    return false;
}


// Writes an array of calibration values to the sensor's offset registers
void Adafruit_BNO055::setSensorOffsets(const uint8_t* calibData) {
    adafruit_bno055_opmode_t lastMode = _mode;
    setMode(OPERATION_MODE_CONFIG);
    delay(25);

    // A writeLen() would make this much cleaner
    write8(ACCEL_OFFSET_X_LSB_ADDR, calibData[0]);
    write8(ACCEL_OFFSET_X_MSB_ADDR, calibData[1]);
    write8(ACCEL_OFFSET_Y_LSB_ADDR, calibData[2]);
    write8(ACCEL_OFFSET_Y_MSB_ADDR, calibData[3]);
    write8(ACCEL_OFFSET_Z_LSB_ADDR, calibData[4]);
    write8(ACCEL_OFFSET_Z_MSB_ADDR, calibData[5]);

    write8(GYRO_OFFSET_X_LSB_ADDR, calibData[6]);
    write8(GYRO_OFFSET_X_MSB_ADDR, calibData[7]);
    write8(GYRO_OFFSET_Y_LSB_ADDR, calibData[8]);
    write8(GYRO_OFFSET_Y_MSB_ADDR, calibData[9]);
    write8(GYRO_OFFSET_Z_LSB_ADDR, calibData[10]);
    write8(GYRO_OFFSET_Z_MSB_ADDR, calibData[11]);

    write8(MAG_OFFSET_X_LSB_ADDR, calibData[12]);
    write8(MAG_OFFSET_X_MSB_ADDR, calibData[13]);
    write8(MAG_OFFSET_Y_LSB_ADDR, calibData[14]);
    write8(MAG_OFFSET_Y_MSB_ADDR, calibData[15]);
    write8(MAG_OFFSET_Z_LSB_ADDR, calibData[16]);
    write8(MAG_OFFSET_Z_MSB_ADDR, calibData[17]);

    write8(ACCEL_RADIUS_LSB_ADDR, calibData[18]);
    write8(ACCEL_RADIUS_MSB_ADDR, calibData[19]);

    write8(MAG_RADIUS_LSB_ADDR, calibData[20]);
    write8(MAG_RADIUS_MSB_ADDR, calibData[21]);

    setMode(lastMode);
}

// Writes to the sensor's offset registers from an offset struct
void Adafruit_BNO055::setSensorOffsets(const adafruit_bno055_offsets_t &offsets_type) {
    adafruit_bno055_opmode_t lastMode = _mode;
    setMode(OPERATION_MODE_CONFIG);
    delay(25);

    write8(ACCEL_OFFSET_X_LSB_ADDR, (offsets_type.accel_offset_x) & 0x0FF);
    write8(ACCEL_OFFSET_X_MSB_ADDR, (offsets_type.accel_offset_x >> 8) & 0x0FF);
    write8(ACCEL_OFFSET_Y_LSB_ADDR, (offsets_type.accel_offset_y) & 0x0FF);
    write8(ACCEL_OFFSET_Y_MSB_ADDR, (offsets_type.accel_offset_y >> 8) & 0x0FF);
    write8(ACCEL_OFFSET_Z_LSB_ADDR, (offsets_type.accel_offset_z) & 0x0FF);
    write8(ACCEL_OFFSET_Z_MSB_ADDR, (offsets_type.accel_offset_z >> 8) & 0x0FF);

    write8(GYRO_OFFSET_X_LSB_ADDR, (offsets_type.gyro_offset_x) & 0x0FF);
    write8(GYRO_OFFSET_X_MSB_ADDR, (offsets_type.gyro_offset_x >> 8) & 0x0FF);
    write8(GYRO_OFFSET_Y_LSB_ADDR, (offsets_type.gyro_offset_y) & 0x0FF);
    write8(GYRO_OFFSET_Y_MSB_ADDR, (offsets_type.gyro_offset_y >> 8) & 0x0FF);
    write8(GYRO_OFFSET_Z_LSB_ADDR, (offsets_type.gyro_offset_z) & 0x0FF);
    write8(GYRO_OFFSET_Z_MSB_ADDR, (offsets_type.gyro_offset_z >> 8) & 0x0FF);

    write8(MAG_OFFSET_X_LSB_ADDR, (offsets_type.mag_offset_x) & 0x0FF);
    write8(MAG_OFFSET_X_MSB_ADDR, (offsets_type.mag_offset_x >> 8) & 0x0FF);
    write8(MAG_OFFSET_Y_LSB_ADDR, (offsets_type.mag_offset_y) & 0x0FF);
    write8(MAG_OFFSET_Y_MSB_ADDR, (offsets_type.mag_offset_y >> 8) & 0x0FF);
    write8(MAG_OFFSET_Z_LSB_ADDR, (offsets_type.mag_offset_z) & 0x0FF);
    write8(MAG_OFFSET_Z_MSB_ADDR, (offsets_type.mag_offset_z >> 8) & 0x0FF);

    write8(ACCEL_RADIUS_LSB_ADDR, (offsets_type.accel_radius) & 0x0FF);
    write8(ACCEL_RADIUS_MSB_ADDR, (offsets_type.accel_radius >> 8) & 0x0FF);

    write8(MAG_RADIUS_LSB_ADDR, (offsets_type.mag_radius) & 0x0FF);
    write8(MAG_RADIUS_MSB_ADDR, (offsets_type.mag_radius >> 8) & 0x0FF);

    setMode(lastMode);
}

// Returns if sensor is calibrated
bool Adafruit_BNO055::isFullyCalibrated(void) {
    uint8_t system, gyro, accel, mag;
    getCalibration(&system, &gyro, &accel, &mag);
    if (system < 3 || gyro < 3 || accel < 3 || mag < 3)
        return false;
    return true;
}


// PRIVATE FUNCTIONS //

// Writes an 8 bit value over I2C
bool Adafruit_BNO055::write8(adafruit_bno055_reg_t reg, byte value) {
  wiringPiI2CWriteReg8(i2c, reg, value);
  return true;
}

// Reads an 8 bit value over I2C
byte Adafruit_BNO055::read8(adafruit_bno055_reg_t reg ) {
  byte value = 0;

  value = wiringPiI2CReadReg8(i2c, reg);

  return value;
}

// Reads the specified number of bytes over I2C
bool Adafruit_BNO055::readLen(adafruit_bno055_reg_t reg, byte * buffer, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    buffer[i] = wiringPiI2CReadReg8(i2c, reg);
  }

  return true;
}
