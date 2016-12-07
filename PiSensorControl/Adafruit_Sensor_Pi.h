/*
* Copyright (C) 2008 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software< /span>
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* 
 * Update by K. Townsend (Adafruit Industries) for lighter typedefs, and
 * extended sensor support to include color, voltage and current.
 */

/* 
 * Intentionally modeled after sensors.h in the Android API:
 * https://github.com/android/platform_hardware_libhardware/blob/master/include/hardware/sensors.h 
 */

/*
 * Udpated by Mitchell Oleson for Air Doodle and other projects on the Raspberry Pi
 */

// Constants
#define SENSORS_GRAVITY_EARTH             (9.80665F)              // Earth's gravity in m/s^2
#define SENSORS_GRAVITY_MOON              (1.6F)                  // The moon's gravity in m/s^2
#define SENSORS_GRAVITY_SUN               (275.0F)                // The sun's gravity in m/s^2
#define SENSORS_GRAVITY_STANDARD          (SENSORS_GRAVITY_EARTH)
#define SENSORS_MAGFIELD_EARTH_MAX        (60.0F)                 // Maximum magnetic field on Earth's surface
#define SENSORS_MAGFIELD_EARTH_MIN        (30.0F)                 // Minimum magnetic field on Earth's surface
#define SENSORS_PRESSURE_SEALEVELHPA      (1013.25F)              // Average sea level pressure is 1013.25 hPa
#define SENSORS_DPS_TO_RADS               (0.017453293F)          // Degrees/s to rad/s multiplier
#define SENSORS_GAUSS_TO_MICROTESLA       (100)                   // Gauss to micro-Tesla multiplier

// Types of sensors
typedef enum {
  SENSOR_TYPE_ACCELEROMETER         = (1),   // Gravity + linear acceleration
  SENSOR_TYPE_MAGNETIC_FIELD        = (2),
  SENSOR_TYPE_ORIENTATION           = (3),
  SENSOR_TYPE_GYROSCOPE             = (4),
  SENSOR_TYPE_LIGHT                 = (5),
  SENSOR_TYPE_PRESSURE              = (6),
  SENSOR_TYPE_PROXIMITY             = (8),
  SENSOR_TYPE_GRAVITY               = (9),
  SENSOR_TYPE_LINEAR_ACCELERATION   = (10),  // Acceleration not including gravity
  SENSOR_TYPE_ROTATION_VECTOR       = (11),
  SENSOR_TYPE_RELATIVE_HUMIDITY     = (12),
  SENSOR_TYPE_AMBIENT_TEMPERATURE   = (13),
  SENSOR_TYPE_VOLTAGE               = (15),
  SENSOR_TYPE_CURRENT               = (16),
  SENSOR_TYPE_COLOR                 = (17)
} sensors_type_t;

// Used to return vector in a common format
typedef struct {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
        // Orientation sensors
        struct {
            float roll;    // Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90°<=roll<=90°
            float pitch;   // Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180°<=pitch<=180°)
            float heading; // Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359°
        };
    };
    int8_t status;
    uint8_t reserved[3];
} sensors_vec_t;

// Used to return color vector in common format
typedef struct {
    union {
        float c[3];
        // RGB color space
        struct {
            float r;       // Red component
            float g;       // Green component
            float b;       // Blue component
        };
    };
    uint32_t rgba;         // 24-bit RGBA value
} sensors_color_t;

// Sensor event (36 bytes)
// Used to provide a single sensor event in a common format
typedef struct {
    int32_t version;                          // Must be sizeof(struct sensors_event_t)
    int32_t sensor_id;                        // Unique sensor identifier
    int32_t type;                             // Sensor type
    int32_t reserved0;                        // Reserved
    int32_t timestamp;                        // Time is in milliseconds
    union
    {
        float           data[4];
        sensors_vec_t   acceleration;         // acceleration values are in meter per second per second (m/s^2)
        sensors_vec_t   magnetic;             // magnetic vector values are in micro-Tesla (uT)
        sensors_vec_t   orientation;          // orientation values are in degrees
        sensors_vec_t   gyro;                 // gyroscope values are in rad/s
        float           temperature;          // temperature is in degrees centigrade (Celsius)
        float           distance;             // Distance in centimeters
        float           light;                // Light in SI lux units
        float           pressure;             // Pressure in hectopascal (hPa)
        float           relative_humidity;    // Relative humidity in percent
        float           current;              // Current in milliamps (mA)
        float           voltage;              // Voltage in volts (V)
        sensors_color_t color;                // Color in RGB component values
    };
} sensors_event_t;

// Sensor details (40-bytes)
// Used to describe basic information about a specific sensor
typedef struct {
    char     name[12];                        // Sensor name
    int32_t  version;                         // Version of the hardware + driver
    int32_t  sensor_id;                       // Unique sensor identifier
    int32_t  type;                            // This sensor's type (ex. SENSOR_TYPE_LIGHT)
    float    max_value;                       // Maximum value of this sensor's value in SI units
    float    min_value;                       // Minimum value of this sensor's value in SI units
    float    resolution;                      // Smallest difference between two values reported by this sensor
    int32_t  min_delay;                       // Min delay in microseconds between events. zero = not a constant rate
} sensor_t;

class Adafruit_Sensor {
    public:
        // Constructor(s)
        Adafruit_Sensor() {}
        virtual ~Adafruit_Sensor() {}

        // These must be defined by the subclass
        virtual void enableAutoRange(bool enabled) {};
        virtual bool getEvent(sensors_event_t*) = 0;
        virtual void getSensor(sensor_t*) = 0;
  
    private:
         bool _autoRange;
};