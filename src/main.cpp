#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 15

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress probeOne;
DeviceAddress probeTwo;

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // zero pad the address if necessary
        if (deviceAddress[i] < 16)
            Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

void printTemperature(DeviceAddress deviceAddress)
{
    float tempC = sensors.getTempC(deviceAddress);
    if (tempC == DEVICE_DISCONNECTED_C)
    {
        Serial.println("Error: Could not read temperature data");
        return;
    }
    Serial.print("Temp C: ");
    Serial.print(tempC);
    Serial.print(" Temp F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
}

void printData(DeviceAddress deviceAddress)
{
    Serial.print("Device Address: ");
    printAddress(deviceAddress);
    Serial.print(" ");
    printTemperature(deviceAddress);
    Serial.println();
}

void setup()
{
    // Start up the logger
    Serial.begin(115200);
    while (!Serial && !Serial.available())
        ;

    Serial.print("Locating devices...");
    sensors.begin();
    delay(100);


    if (!sensors.getAddress(probeOne, 0))
        Serial.println("Unable to find address for Probe 1");
    Serial.print("Probe 1 Address: ");
    printAddress(probeOne);
    Serial.println();

     if (!sensors.getAddress(probeTwo, 1))
        Serial.println("Unable to find address for Probe 2");
    Serial.print("Probe 2 Address: ");
    printAddress(probeTwo);
    Serial.println();
}

void loop(void)
{
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures();
    Serial.println("DONE");

    // print the device information
    printData(probeOne);
    printData(probeTwo);

    delay(1000);
}
