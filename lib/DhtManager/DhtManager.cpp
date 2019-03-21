#include "DhtManager.h"

DhtManager::DhtManager(int gpio, int dhtType, DebugMessage *debugMessage)
{
    _dht = new DHT(gpio, dhtType);
    _debugMessage = debugMessage;
    _dht->begin();
    debugMessage->debug("Configuring DHT - GPIO: " + String(gpio) + " - Type: " + String(dhtType));
}

float DhtManager::getHumidity()
{
    return _humidity;
}

float DhtManager::getTemperatureCelsius()
{
    return _celsius;
}

float DhtManager::getTemperatureFahrenheit()
{
    return _fahrenheit;
}

void DhtManager::update()
{
    _humidity = _dht->readHumidity();
    _celsius = _dht->readTemperature();
    _fahrenheit = _dht->readTemperature(true);
}

