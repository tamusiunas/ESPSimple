#include "AmazonAlexa.h"

AmazonAlexa::AmazonAlexa(ESPConfig *espConfig, unsigned long tcpPort)
{
    _fauxmo = new fauxmoESP();
    _fauxmo->setPort(tcpPort);
    _espConfig = espConfig;
}

AmazonAlexa::AmazonAlexa(ESPConfig *espConfig)
{
    _fauxmo = new fauxmoESP();
    _fauxmo->setPort(80);
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

void AmazonAlexa::Enable()
{
    _fauxmo->enable(true);
}

void AmazonAlexa::Disable()
{
    _fauxmo->enable(false);
}