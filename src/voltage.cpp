#include <languages.h>
#include <units.h>
#include <constants.h>
#include <memory.h>
#include <pins.h>

void updateVoltageValues()
{
    int sensorValue = analogRead(VOLTAGE_PIN);
    float voltage = sensorValue * (3.3 / 4095.0) * memory.config.voltageDivider;
    state.voltage = voltage;
    Serial.print(voltage);
}