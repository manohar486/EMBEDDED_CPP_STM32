#ifndef ALL_SENSOR_H
#define ALL_SENSOR_H

class VitalSensor {
public:
    virtual float read() = 0;
    virtual ~VitalSensor() {}
};

// ---- Temperature ----
class TemperatureSensor : public VitalSensor {
public:
    float read() override;
};

// ---- Heart Rate ----
class HeartRateSensor : public VitalSensor {
public:
    float read() override;

};

// ---- SpO2 ----
class PulseOxiMeter: public VitalSensor {
public:
    float read() override;

};

#endif
