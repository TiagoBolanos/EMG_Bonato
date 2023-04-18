/*
  Bonato.h - Determine onsets of EMG pulses.
  Follows the approach by Bonato et al. [Bo98]_.
  Based on the BioSPPy toolbox for biosignal processing
  Created by ... , April 10, 2023.
  Released into the public domain.
*/
#ifndef BONATO_H
#define BONATO_H

#include <driver/adc.h>
#include <math.h>

class Bonato {
  public:
    Bonato(bool stream_raw, bool detect_onset, int pin);
    void calibration();
    void read();
    bool isOnset();
    float getEMG();
    float getTF();
  
  private:
    bool _stream_raw;
    bool _detect_onset;
    int _pin;
    float _EMG;
    float _lEMG;
    int _sumEMG;
    int _sumsqrEMG;
    int _Calibration_len;
    float _calibration_mean;
    float _delta;
    float _mean;
    float _sum_of_squared_deviations;
    float _variance;
    float _tf;
    int _n;
    int _j;
    int _state_duration;
    int _samples_above_fail;
    int _active_state_duration;
    int _fail_size;
    float _threshold;
    bool _onset;
    bool _alarms;
    bool _flag;
};

#endif
