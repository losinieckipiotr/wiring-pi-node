#include <cmath>

#include "../wiringPi/WiringPi.h"

#include "Bmp180.h"

using namespace std;

using namespace wiringPi;
using namespace pionieer600;

//i2c address
constexpr auto BMP180_Address =         0x77;

//Operating Modes
constexpr auto BMP180_ULTRALOWPOWER =   0;
constexpr auto BMP180_STANDARD =        1;
constexpr auto BMP180_HIGHRES =         2;
constexpr auto BMP180_ULTRAHIGHRES =    3;

//BMP185 Registers
constexpr auto BMP180_CAL_AC1 =         0xAA;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_AC2 =         0xAC;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_AC3 =         0xAE;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_AC4 =         0xB0;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_AC5 =         0xB2;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_AC6 =         0xB4;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_B1 =          0xB6;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_B2 =          0xB8;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_MB =          0xBA;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_MC =          0xBC;  //Calibration data (16 bits)
constexpr auto BMP180_CAL_MD =          0xBE;  //Calibration data (16 bits)
constexpr auto BMP180_CONTROL =         0xF4;
constexpr auto BMP180_TEMPDATA =        0xF6;
constexpr auto BMP180_PRESSUREDATA =    0xF6;

//Commands
constexpr auto BMP180_READTEMPCMD =     0x2E;
constexpr auto BMP180_READPRESSURECMD = 0x34;

constexpr auto OSS = BMP180_ULTRALOWPOWER; // the most table measure ?

Bmp180::Bmp180()
{

}

Bmp180::~Bmp180()
{

}

void Bmp180::SetupI2C()
{
  fd = WiringPi::I2CSetupAdr(BMP180_Address);
}

void Bmp180::Calibrate()
{
  AC1 = I2C_readS16(BMP180_CAL_AC1);
  AC2 = I2C_readS16(BMP180_CAL_AC2);
  AC3 = I2C_readS16(BMP180_CAL_AC3);
  AC4 = I2C_readU16(BMP180_CAL_AC4);
  AC5 = I2C_readU16(BMP180_CAL_AC5);
  AC6 = I2C_readU16(BMP180_CAL_AC6);
  B1 = I2C_readS16(BMP180_CAL_B1);
  B2 = I2C_readS16(BMP180_CAL_B2);
  MB = I2C_readS16(BMP180_CAL_MB);
  MC = I2C_readS16(BMP180_CAL_MC);
  MD = I2C_readS16(BMP180_CAL_MD);
}

void Bmp180::Measure(HandlerT handler)
{
  handler_ = handler; // save handler

  measure_work_.data = this; // pass this ptr to worker

  uv_queue_work(uv_default_loop(), &measure_work_, [](uv_work_t *req)
  {
    const auto self = static_cast<Bmp180*>(req->data);

    // do measure
    const auto temp = self->ReadTemperature();
    const auto press = self->ReadPressure();

    // save results
    self->result_.temperature = temp;
    self->result_.pressure = press;
  },
  [](uv_work_t *req, int status)
  {
    auto self = static_cast<Bmp180*>(req->data);

    // call handler
    self->handler_(self->result_.temperature, self->result_.pressure);
  });
}

float Bmp180::ReadTemperature()
{
  float T;
  int UT, X1, X2, B5;
  UT = read_raw_temp();
  X1 = ((UT - AC6)*AC5) >> 15;
  X2 = (MC << 11) / (X1 + MD);
  B5 = X1 + X2;
  T = ((B5 + 8) >> 4) / 10.0f;
  return T;
}

float Bmp180::ReadPressure()
{
  int P;
  int UT, UP, X1, X2, X3, B3, B5, B6;
  unsigned int B4;
  long long B7;
  UT = read_raw_temp();
  UP = read_raw_pressure();

  X1 = ((UT - AC6)*AC5) >> 15;
  X2 = (MC << 11) / (X1 + MD);
  B5 = X1 + X2;

  //Pressure Calculations
  B6 = B5 - 4000;
  X1 = (B2 * (B6 * B6) >> 12) >> 11;
  X2 = (AC2 * B6) >> 11;
  X3 = X1 + X2;
  B3 = (((AC1 * 4 + X3) << OSS) + 2) / 4;
  X1 = (AC3 * B6) >> 13;
  X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  B4 = (AC4 * (X3 + 32768)) >> 15;
  B7 = (UP - B3) * (50000 >> OSS);
  if (B7 < 0x80000000LL) { P = (B7 * 2) / B4; }
  else { P = (B7 / B4) * 2; }
  X1 = (P >> 8) * (P >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * P) >> 16;
  P = P + ((X1 + X2 + 3791) >> 4);
  return P / 100.0f;

}

float Bmp180::ReadAltitude()
{
  float pressure, altitude;
  float sealevel_pa = 101325.0;
  pressure = (float)ReadPressure();
  altitude = 44330.0f * (1.0f - pow(pressure / sealevel_pa, (1.0f / 5.255f)));
  return altitude;
}

float Bmp180::ReadSealevelPressure()
{
  float altitude_m = 0.0;
  float pressure, p0;
  pressure = (float)ReadPressure();
  p0 = pressure / pow(1.0f - altitude_m / 44330.0f, 5.255f);
  return p0;
}

char Bmp180::I2C_readByte(int reg)
{
  return (char)WiringPi::I2CReadReg8(fd, reg);
}

unsigned short Bmp180::I2C_readU16(int reg)
{
  int MSB, LSB;
  MSB = I2C_readByte(reg);
  LSB = I2C_readByte(reg + 1);
  int value = (MSB << 8) + LSB;
  return (unsigned short)value;
}

short Bmp180::I2C_readS16(int reg)
{
  int result;
  result = I2C_readU16(reg);
  if (result > 32767)result -= 65536;
  return (short)result;
}

void Bmp180::I2C_writeByte(int reg, int val)
{
  WiringPi::I2CWriteReg8(fd, reg, val);
}

int Bmp180::read_raw_temp()
{
  int raw;
  I2C_writeByte(BMP180_CONTROL, BMP180_READTEMPCMD);
  WiringPi::Delay(5);  //5ms;
  raw = I2C_readByte(BMP180_TEMPDATA) << 8;
  raw += I2C_readByte(BMP180_TEMPDATA + 1);
  return raw;
}

int Bmp180::read_raw_pressure()
{
  int MSB, LSB, XLSB, raw;
  I2C_writeByte(BMP180_CONTROL, BMP180_READPRESSURECMD + (OSS << 6));
  switch (OSS)
  {
  case BMP180_ULTRALOWPOWER:
    WiringPi::Delay(5); break;
  case BMP180_HIGHRES:
    WiringPi::Delay(14); break;
  case BMP180_ULTRAHIGHRES:
    WiringPi::Delay(26); break;
  default:
    WiringPi::Delay(8);
  }
  MSB = I2C_readByte(BMP180_PRESSUREDATA);
  LSB = I2C_readByte(BMP180_PRESSUREDATA + 1);
  XLSB = I2C_readByte(BMP180_PRESSUREDATA + 2);
  raw = ((MSB << 16) + (LSB << 8) + XLSB) >> (8 - OSS);
  return raw;
}
