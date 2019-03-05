#include "AmazonAlexa.h"

AmazonAlexa::AmazonAlexa(ESPConfig *espConfig, unsigned long tcpPort)
{
    _fauxmo = new fauxmoESP();
    _fauxmo->setPort(tcpPort);
    _espConfig = espConfig;
}

AmazonAlexa::~AmazonAlexa()
{

}

void AmazonAlexa::Handle()
{
    _fauxmo->handle();
}

void AmazonAlexa::AddDevice(const char *deviceName)
{
    _fauxmo->addDevice(deviceName);
}