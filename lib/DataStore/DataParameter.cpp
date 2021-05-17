/**
 * @file DataParameter.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Object with parameters used in the project
 * @version 0.1
 * @date 2021-05-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "DataParameter.h"

DataParameter::DataParameter()
{
  _field = NULL;
  _value = NULL;
}

DataParameter::DataParameter(const char *field, const char *value)
{
  _field = strdup(field);
  strcpy(_field,field);
  _value = strdup(value);
  strcpy(_value, value);
}

DataParameter::DataParameter(String field, String value)
{
  _field = strdup(field.c_str());
  strcpy(_field,field.c_str());
  _value = strdup(value.c_str());
  strcpy(_value, value.c_str());
}

DataParameter::~DataParameter()
{
  delete _field;
  delete _value;
}

const char* DataParameter::getField()
{
  return _field;
}

const char* DataParameter::getValue()
{
  return _value;
}


void DataParameter::setField(const char *field)
{
  free(_field);
  _field = strdup(field);
  strcpy(_field,field);
}

void DataParameter::setValue(const char *value)
{
  free(_value);
  _value = strdup(value);
}
