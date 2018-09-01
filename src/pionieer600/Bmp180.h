#ifndef PIONIEER_600_BMP180_H
#define PIONIEER_600_BMP180_H

#include <cstdint>

#include <functional>

#include <uv.h>

namespace pionieer600 {

class Bmp180
{
using HandlerT = std::function<void(float, float)>;

struct MeasureResult {
  float temperature;
  float pressure;
};

public:
  Bmp180();
  ~Bmp180();

  void SetupI2C();
  void Calibrate();

  void Measure(HandlerT handler);

  float ReadTemperature();
  float ReadPressure();
  float ReadAltitude();
  float ReadSealevelPressure();

private:
  char I2C_readByte(int reg);
  unsigned short I2C_readU16(int reg);
  short I2C_readS16(int reg);
  void I2C_writeByte(int reg, int val);
  int read_raw_temp();
  int read_raw_pressure();

  short AC1, AC2, AC3, B1, B2, MB, MC, MD;
  unsigned short AC4, AC5, AC6;
  int fd;

  HandlerT handler_;
  MeasureResult result_;

  uv_work_t measure_work_;
};

} // namespace pionieer600

#endif // !PIONIEER_600_BMP180_H
