#include <math.h>




//for bonato
float EMG = 0;
float lEMG = 0;
int sumEMG = 0;
int sumsqrEMG = 0;
int Calibration_len = 1000;
float calibration_mean = 0.0;
float delta, mean, sum_of_squared_deviations, variance, tf;


int n = 0;
int j = 0;
int state_duration = 0;


int samples_above_fail = 1;     //1
int active_state_duration = 2;  //2
int fail_size = 5;              //5
float threshold = 150.0;        // Set the threshold value 250
bool onset = false;
bool alarms = false;
bool flag = false;

void calibration() {
  float i = 1;
  Serial.println("Calibration starts in 3 seconds");
  delay(3000);
  sumEMG = 0;
  sumsqrEMG = 0;
  while (i <= Calibration_len) {
    EMG = analogRead(36);
    Serial.println(EMG);
    sumEMG += EMG;
    sumsqrEMG += EMG * EMG;
    calibration_mean = sumEMG / i;
    delta = EMG - calibration_mean;
    mean = mean + delta / (i + 1);
    sum_of_squared_deviations = sum_of_squared_deviations + delta * (EMG - mean);
    i++;
    delay(10);
  }
  calibration_mean = sumEMG / Calibration_len;
  variance = ((sumsqrEMG / Calibration_len) - calibration_mean * calibration_mean) * (Calibration_len / (Calibration_len - 1));
  if (variance == 0) { variance = 1; }
  Serial.println(calibration_mean);
  Serial.println(variance);
  Serial.println("Calibration Successful!");
  delay(1000);
}
void setup() {
  Serial.begin(115200);  //initiates serial wired communication at 115200 baud rate
  delay(3000);
  Serial.println("Connected...");
  delay(3000);
  calibration();
}

int count = 0;







void loop() {
  unsigned long time = millis();
  EMG = analogRead(36);



  tf = abs(1 / variance) * ((EMG - calibration_mean) * (EMG - calibration_mean) + (lEMG - calibration_mean) * (lEMG - calibration_mean));
  lEMG = EMG;

  if (onset == true) {
    if (alarms == false) {
      if (tf < threshold) {
        alarms = true;
      }
    } else {
      if (tf < threshold) {
        state_duration += 1;
        if (j > 0) {
          n += 1;
          if (n == samples_above_fail) {
            n = 0;
            j = 0;
          }
        }
        if (state_duration == active_state_duration) {
          onset = false;
          alarms = false;
          flag = false;
          n = 0;
          j = 0;
          state_duration = 0;
        }
      } else {
        j += 1;
        if (j > fail_size) {
          alarms = false;
          n = 0;
          j = 0;
          state_duration = 0;
        }
      }
    }
  } else {
    if (alarms == false) {
      if (tf >= threshold) {
        alarms = true;
      }
    } else {
      if (tf >= threshold) {
        state_duration += 1;
        if (j > 0) {
          n += 1;
          if (n == samples_above_fail) {
            n = 0;
            j = 0;
          }
        }
        if (state_duration == active_state_duration) {
          flag = true;
          onset = true;
          alarms = false;
          n = 0;
          j = 0;
          state_duration = 0;
        }
      } else {
        j += 1;
        if (j > fail_size) {
          alarms = false;
          n = 0;
          j = 0;
          state_duration = 0;
        }
      }
    }
  }


  Serial.print(EMG);
  Serial.print(",");
  Serial.print(tf);
  Serial.print(",");
  Serial.print(300 * flag);
  Serial.println(0);
  delay(5);
}