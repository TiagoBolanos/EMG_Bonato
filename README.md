# EMG Onset Detection Library

This library provides a simple implementation of real-time EMG onset detection for Arduino-based platforms. The library consists of two main functions, "Calibration" and "Bonato," enabling the detection of muscle activation based on predetermined parameters that can be easily adjusted by the user.

## Calibration

The "Calibration" function allows the user to obtain a baseline reading of the EMG signal, which is required for subsequent onset detection. This function takes as inputs the number of samples to be used for calibration and the pin number to which the EMG sensor is connected.
`void calibration();` 

## Bonato

The "Bonato" function performs real-time onset detection using the EMG signal obtained from the specified pin. This function classifies the signal as either an onset or an offset based on predetermined parameters, which can be adjusted by the user.
`void Bonato();` 

The following parameters can be modified within the Bonato function:

-   `Threshold`: The minimum amplitude required for an EMG signal to be classified as an onset.
-   `Samples_above_fail`: The number of consecutive samples above the threshold required for an onset detection to be considered valid.
-   `Active_state_duration`: The minimum duration that the EMG signal must remain above the threshold to be classified as an onset.
-   `Fail_size`: The maximum number of consecutive samples that can be below the threshold before an ongoing onset detection is considered to have failed.
-   `Variation`: The variance value of the EMG signal obtained during the calibration process.
-   `Calibration_mean`: The mean value of the EMG signal obtained during the calibration process.

### Example


`#include "EMG_Onset_Detection.h"`

`EMG_Onset_Detection emg;`

`void setup() {
  Serial.begin(115200);
  emg.calibration();
}`

`void loop() {
  emg.Bonato();
} `

## Requirements

This library requires an EMG sensor, such as the MyoWare Muscle Sensor, and an Arduino-based platform. It has been tested on an Arduino Uno board.

## Credits

This library was developed by [Name], [Affiliation], in collaboration with [Name], [Affiliation].

## License

This library is released under the [MIT License](https://opensource.org/licenses/MIT).
