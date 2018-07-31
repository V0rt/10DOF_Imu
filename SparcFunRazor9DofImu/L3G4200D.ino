#define GYR_ADDRESS (0xD2 >> 1)

void Init_Gyro()
{
  Wire.beginTransmission(GYR_ADDRESS);
  Wire.write(0x20);    //L3G4200D_CTRL_REG1
  Wire.write(0x1f);    //Output data rate = 100Hz, Cut-Off = 25
  Wire.endTransmission();

  Wire.beginTransmission(GYR_ADDRESS);
  Wire.write(0x23);    //L3G4200D_CTRL_REG4
  Wire.write(0x30);    //(00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
  Wire.endTransmission();
}

// Reads the angular rates from the Gyro
void Read_Gyro()
{
  Wire.beginTransmission(GYR_ADDRESS);
  Wire.write(0x28 | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(GYR_ADDRESS, 6);

  while (Wire.available() < 6);

  uint8_t xla = Wire.read();
  uint8_t xha = Wire.read();
  uint8_t yla = Wire.read();
  uint8_t yha = Wire.read();
  uint8_t zla = Wire.read();
  uint8_t zha = Wire.read();

  sen_data.gyro_x_raw = ((int)xha << 8 | xla) / Gyro_Gain_X;  // X axis
  sen_data.gyro_y_raw = ((int)yha << 8 | yla) / Gyro_Gain_Y;  // Y axis
  sen_data.gyro_z_raw = ((int)zha << 8 | zla) / Gyro_Gain_Z;  // Z axis

  //subtract the offset
  sen_data.gyro_x = sen_data.gyro_x_raw - sen_offset.gyro_offset[0];    // X axis
  sen_data.gyro_y = sen_data.gyro_y_raw - sen_offset.gyro_offset[1];    // Y axis
  sen_data.gyro_z = sen_data.gyro_z_raw - sen_offset.gyro_offset[2];    // Z axis

  //change the sign if needed
  sen_data.gyro_x *= SENSOR_SIGN[0];    // X axis
  sen_data.gyro_y *= SENSOR_SIGN[1];    // Y axis
  sen_data.gyro_z *= SENSOR_SIGN[2];    // Z axis

  //To radians / s
  sen_data.gyro_x = ToRad(sen_data.gyro_x);
  sen_data.gyro_y = ToRad(sen_data.gyro_y);
  sen_data.gyro_z = ToRad(sen_data.gyro_z);

  //adjust any random offset
  if (abs(sen_data.gyro_x) <= 0.022) {
    sen_data.gyro_x = 0.0;
  }

  if (abs(sen_data.gyro_y) <= 0.022) {
    sen_data.gyro_y = 0.0;
  }

  if (abs(sen_data.gyro_z) <= 0.022) {
    sen_data.gyro_z = 0.0;
  }
}
