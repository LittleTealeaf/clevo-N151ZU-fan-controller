#include <bits/chrono.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <sys/io.h>
#include <unistd.h>
using namespace std;

#define EC_COMMAND_PORT 0x66
#define EC_DATA_PORT 0x62
#define TEMP 0x9E

#define FAN_MIN_VALUE 40 // minimal rotation speed of the fan (0-255)
#define FAN_OFF_TEMP 60  // temp below which fan is off
#define FAN_MAX_TEMP 80  // at which temperature the fan should be maximum ?
#define FAN_START_VALUE                                                        \
  100 // speed (between 0 and 255) at which fan will turn when FAN_OFF_TEMP is
      // reached
#define FAN_PEAK_HOLD_TIME                                                     \
  10000 // when a maximum of fan rotation rate is reached, hold much time (ms)
        // to hold it before allowing to decrease the value

#define REFRESH_RATE 1000 // time to wait between each controller loop (ms)
#define MAX_FAN_SET_INTERVAL                                                   \
  2000 // maximal time between two fan rate send command

static int EcInit() {
  if (ioperm(EC_DATA_PORT, 1, 1) != 0) {
    return 1;
  }

  if (ioperm(EC_COMMAND_PORT, 1, 1) != 0) {
    return 1;
  }

  return 0;
}

static void EcFlush() {
  while ((inb(EC_COMMAND_PORT) & 0x1) == 0x1) {
    inb(EC_DATA_PORT);
  }
}

static void SendCommand(int command) {
  int tt = 0;
  while ((inb(EC_COMMAND_PORT) & 2)) {
    tt++;
    if (tt > 30000) {
      break;
    }
  }

  outb(command, EC_COMMAND_PORT);
}

static void WriteData(int data) {
  while ((inb(EC_COMMAND_PORT) & 2))
    ;

  outb(data, EC_DATA_PORT);
}

static int ReadByte() {
  int i = 1000000;
  while ((inb(EC_COMMAND_PORT) & 1) == 0 && i > 0) {
    i -= 1;
  }

  if (i == 0) {
    return 0;
  } else {
    return inb(EC_DATA_PORT);
  }
}

static void setFanSpeed(int speed) {
  EcInit();
  SendCommand(0x99);
  WriteData(0x01); // ID
  WriteData(speed);
}

static int GetLocalTemp() {
  int index = 1;
  EcInit();
  EcFlush();
  SendCommand(TEMP);
  WriteData(index);
  // ReadByte();
  int value = ReadByte();
  return value;
}

static unsigned int time() {
  chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(
      chrono::system_clock::now().time_since_epoch());
  unsigned int time = ms.count();
  return time;
}

int main(int argc, char *argv[]) {
  // last fan speed value, used to avoid write speed if not necessary
  int lastFanSpeed = 1;

  // last max speed value, used in combination with FAN_PEAK_HOLD_TIME
  int slidingMaxFanSpeed = -1;

  // time at which the last max was reached, used in combination with
  // FAN_PEAK_HOLD_TIME
  unsigned int maxFanSpeedTime = 0;

  // use this to periodically set the temp unconditionally (seuful when wake up
  // from sleep)
  unsigned int lastTimeFanUpdate = 0;

  while (1) {
    int temp = GetLocalTemp();

    // dynamic fan speed is the computed instantaneous speed without hysteries
    int dynamicFanSpeed = round(
        (float)((float)(temp - FAN_OFF_TEMP) / (FAN_MAX_TEMP - FAN_OFF_TEMP) *
                    (255 - FAN_START_VALUE) +
                FAN_START_VALUE));

    if (dynamicFanSpeed < FAN_START_VALUE) {
      dynamicFanSpeed = 0;
    }
    if (dynamicFanSpeed > 255) {
      dynamicFanSpeed = 255;
    }

    if (dynamicFanSpeed > slidingMaxFanSpeed ||
        time() > maxFanSpeedTime + FAN_PEAK_HOLD_TIME) {
      slidingMaxFanSpeed = dynamicFanSpeed;
      maxFanSpeedTime = time();
    }

    if (lastFanSpeed != slidingMaxFanSpeed ||
        lastTimeFanUpdate + MAX_FAN_SET_INTERVAL < time()) {
      setFanSpeed(max(FAN_MIN_VALUE, slidingMaxFanSpeed));
      lastTimeFanUpdate = time();
    }
  }

  //   while (1) {
  //     int temp = GetLocalTemp();
  //     // dynamic fan speed is the computed instantaneous speed, whithout
  //     // hysteresis (FAN_PEAK_HOLD_TIME)
  //     if (dynamicFanSpeed < FAN_START_VALUE)
  //       dynamicFanSpeed = 0;
  //     if (dynamicFanSpeed > 255)
  //       dynamicFanSpeed = 255;
  //
  //     if (dynamicFanSpeed > slidingMaxFanSpeed ||
  //         time() > maxFanSpeedTime +
  //                      FAN_PEAK_HOLD_TIME) { // update max values if max is
  //                                            // overcome or if the time
  //                                            // (FAN_PEAK_HOLD_TIME) is
  //                                            reached
  //       slidingMaxFanSpeed = dynamicFanSpeed;
  //       maxFanSpeedTime = time();
  //     }
  //     if (lastFanSpeed != slidingMaxFanSpeed ||
  //         lastTimeFanUpdate + MAX_FAN_SET_INTERVAL <
  //             time()) { // send value if it changed or if we didn't do it
  //             since
  //                       // more than "MAX_FAN_SET_INTERVAL" seconds.
  //       setFanSpeed(max(FAN_MIN_VALUE, slidingMaxFanSpeed));
  //       lastTimeFanUpdate = time();
  // #ifdef VERBOSE
  //       cout << "T:" << temp << "°C | set fan to "
  //            << round((float)(slidingMaxFanSpeed) / 255 * 100) << "%";
  // #endif
  //     }
  //     cout << endl;
  //     lastFanSpeed = slidingMaxFanSpeed;
  //     usleep(REFRESH_RATE * 1000);
  return 0;
}
