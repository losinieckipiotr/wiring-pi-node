#include "WiringPi.h"

#ifndef FAKE_WP
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <pcf8574.h>
#else
#include "FakeWiringPi.hpp"
#endif // FAKE_WP

using namespace wiringPi;

constexpr auto PCF8574_Address = 0x20;
constexpr auto EXTEND_BASE = 64;

bool WiringPi::wiringPiWasInit = false;

int WiringPi::Setup()
{
  if (wiringPiWasInit)
    return 1;
  else
  {
    int retVal = wiringPiSetup();
    wiringPiWasInit = true;
    return retVal;
  }
}

void WiringPi::SetPinMode(int pin, int mode)
{
  pinMode(pin, mode);
}

void WiringPi::DigitalWrite(int pin, int value)
{
  digitalWrite(pin, value);
}

int WiringPi::DigitalRead(int pin)
{
  return digitalRead(pin);
}

int WiringPi::SPISetup(int channel, int speed)
{
  int retVal = wiringPiSPISetup(channel, speed);
  return retVal;
}

int WiringPi::SPIDataRW(int channel, unsigned char* data, int len)
{
  return wiringPiSPIDataRW(channel, data, len);
}

int WiringPi::I2CSetupAdr(const int devId)
{
  int retval = wiringPiI2CSetup(devId);
  return retval;
}

int WiringPi::I2CRead(int fd)
{
  return wiringPiI2CRead(fd);
}

int WiringPi::I2CReadReg8(int fd, int reg)
{
  return wiringPiI2CReadReg8(fd, reg);
}

int WiringPi::I2CReadReg16(int fd, int reg)
{
  return wiringPiI2CReadReg16(fd, reg);
}

int WiringPi::I2CWrite(int fd, int data)
{
  return wiringPiI2CWrite(fd, data);
}

int WiringPi::I2CWriteReg8(int fd, int reg, int data)
{
  return wiringPiI2CWriteReg8(fd, reg, data);
}

int WiringPi::I2CWriteReg16(int fd, int reg, int data)
{
  return wiringPiI2CWriteReg16(fd, reg, data);
}

void WiringPi::Delay(unsigned int del)
{
  delay(del);
}

void WiringPi::DelayMicros(unsigned int howLong)
{
  delayMicroseconds(howLong);
}

int WiringPi::PCF8574Setup()
{
  return pcf8574Setup(EXTEND_BASE, PCF8574_Address);
}

void WiringPi::pullUpDn(int pin, int pud)
{
  pullUpDnControl(pin, pud);
}
