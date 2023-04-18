/*
  Bonato.h - Determine onsets of EMG pulses.
  Follows the approach by Bonato et al. [Bo98]_.
  Based on the BioSPPy toolbox for biosignal processing
  Created by ... , April 10, 2023.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Bonato.h"

Bonato::Bonato(bool stream, bool onset, int pin) {
  this->stream = stream;
  this->onset = onset;
  this->pin = pin;
  EMG = 0;
  lEMG = 0;
  tf = 0;
  variance = 0;
  calibration_mean = 0;
  threshold = 0;
  active_state_duration = 0;
  fail_size = 0;
  samples_above_fail = 0;
  alarms = false;
  flag = false;
  n = 0;
  j = 0;
  state_duration = 0;
}


void Bonato::calibrate() {
  int sum = 0;
  
  // Read EMG values for calibration
  for (int i = 0; i < calibration_samples; i++) {
    sum += analogRead(pin);
    delay(1);
  }
  
  // Compute mean and variance
  calibration_mean = sum / calibration_samples;
  sum = 0;
  
  for (int i = 0; i < calibration_samples; i++) {
    int x = analogRead(pin) - calibration_mean;
    sum += x * x;
    delay(1);
  }
  
  variance = sum / calibration_samples;
}

void Bonato::process() {
  EMG = analogRead(pin);
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
  }
}

void Bonato::setThreshold(float threshold) {
  this->threshold = threshold;
}

void Bonato::setActiveStateDuration(int active_state_duration) {
  this->active_state_duration = active_state_duration;
}

void Bonato::setFailSize(int fail_size) {
  this->fail_size = fail_size;
}

void Bonato::setSamplesAboveFail(int samples_above_fail) {
  this->samples_above_fail = samples_above_fail;
}


float Bonato::getThreshold() {
  return threshold;
}

int Bonato::getActiveStateDuration() {
  return active_state_duration;
}

int Bonato::getFailSize() {
  return fail_size;
}

int Bonato::getSamplesAboveFail() {
  return samples_above_fail;
}

float Bonato::getVariance() {
  return variance;
}

float Bonato::getCalibrationMean() {
  return calibration_mean;
}
void Bonato::startStream() {
  streaming = true;
  Serial.println("Streaming started");
}

void Bonato::stopStream() {
  if (streaming) {
    Serial.end();
    streaming = false;
  }
}


void Bonato::setOnset(bool onset) {
  this->onset = onset;
}


bool Bonato::getOnset() {
  return onset;
}

float Bonato::getEMG() {
  return EMG;
}

float Bonato::getTF() {
  return tf;
}


void Bonato::printData() {
  Serial.print("EMG: ");
  Serial.print(getEMG());
  Serial.print(" | ");
  Serial.print("TF: ");
  Serial.print(getTF());
  Serial.print(" | ");
  Serial.print("Onset: ");
  Serial.print(getOnset());
  Serial.print(" | ");
  Serial.print("Threshold: ");
  Serial.print(getThreshold());
  Serial.print(" | ");
  Serial.print("Active State Duration: ");
  Serial.print(getActiveStateDuration());
  Serial.print(" | ");
  Serial.print("Fail Size: ");
  Serial.print(getFailSize());
  Serial.print(" | ");
  Serial.print("Samples Above Fail: ");
  Serial.print(getSamplesAboveFail());
  Serial.print(" | ");
  Serial.print("Variance: ");
  Serial.print(getVariance());
  Serial.print(" | ");
  Serial.print("Calibration Mean: ");
  Serial.println(getCalibrationMean());
}
