#define IMU_SDA  40
#define IMU_SCL  39
#define IMU_IRQ  -1

#define IMU_CS                      5


void setupIMU() {
  //Using WIRE !!
  if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, IMU_SDA, IMU_SCL)) {
    Serial.println("Failed to find QMI8658 - check your wiring!");
  }

  Serial.print("Device ID:");
  Serial.println(qmi.getChipID(), HEX);

  qmi.configAccelerometer(
    /*
      * ACC_RANGE_2G
      * ACC_RANGE_4G
      * ACC_RANGE_8G
      * ACC_RANGE_16G
      * */
    SensorQMI8658::ACC_RANGE_4G,
    /*
      * ACC_ODR_1000H
      * ACC_ODR_500Hz
      * ACC_ODR_250Hz
      * ACC_ODR_125Hz
      * ACC_ODR_62_5Hz
      * ACC_ODR_31_25Hz
      * ACC_ODR_LOWPOWER_128Hz
      * ACC_ODR_LOWPOWER_21Hz
      * ACC_ODR_LOWPOWER_11Hz
      * ACC_ODR_LOWPOWER_3H
    * */
    SensorQMI8658::ACC_ODR_1000Hz,
    /*
    *  LPF_MODE_0     //2.66% of ODR
    *  LPF_MODE_1     //3.63% of ODR
    *  LPF_MODE_2     //5.39% of ODR
    *  LPF_MODE_3     //13.37% of ODR
    * */
    SensorQMI8658::LPF_MODE_0,
    // selfTest enable
    true);


  qmi.configGyroscope(
    /*
    * GYR_RANGE_16DPS
    * GYR_RANGE_32DPS
    * GYR_RANGE_64DPS
    * GYR_RANGE_128DPS
    * GYR_RANGE_256DPS
    * GYR_RANGE_512DPS
    * GYR_RANGE_1024DPS
    * */
    SensorQMI8658::GYR_RANGE_256DPS,
    /*
      * GYR_ODR_7174_4Hz
      * GYR_ODR_3587_2Hz
      * GYR_ODR_1793_6Hz
      * GYR_ODR_896_8Hz
      * GYR_ODR_448_4Hz
      * GYR_ODR_224_2Hz
      * GYR_ODR_112_1Hz
      * GYR_ODR_56_05Hz
      * GYR_ODR_28_025H
      * */
    SensorQMI8658::GYR_ODR_896_8Hz,
    /*
    *  LPF_MODE_0     //2.66% of ODR
    *  LPF_MODE_1     //3.63% of ODR
    *  LPF_MODE_2     //5.39% of ODR
    *  LPF_MODE_3     //13.37% of ODR
    * */
    SensorQMI8658::LPF_MODE_3,
    // selfTest enable
    true);

  // In 6DOF mode (accelerometer and gyroscope are both enabled),
  // the output data rate is derived from the nature frequency of gyroscope
  qmi.enableGyroscope();
  qmi.enableAccelerometer();

  // Print register configuration information
  qmi.dumpCtrlRegister();

  Serial.println("Read data now...");
}
