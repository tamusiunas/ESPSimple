/**
 * @file DataStore.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Objects to store user parameters
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "DataStore.h"

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


DataStore::DataStore() {
    _max_params = DATASTORE_MAX_PARAMS;
    _parameters = (DataParameter**)malloc(_max_params * sizeof(DataParameter*));
}

DataStore::~DataStore()
{
    if (_paramsCount > 0)
    {
      free(_parameters);
      _parameters = NULL;
    }
  }

DataParameter * DataStore::getParameterByPos(int parameterPos)
{
  if (parameterPos < _paramsCount)
    return (_parameters[parameterPos]);
  else
    return (NULL);
}

bool DataStore::addParameter(DataParameter *parameter) {

  if(_paramsCount + 1 > _max_params)
  {
    _max_params += DATASTORE_MAX_PARAMS;  // rezise the params array

    DataParameter** new_parameters = (DataParameter**)realloc(_parameters, _max_params * sizeof(DataParameter*));
    if (new_parameters != NULL) {
      _parameters = new_parameters;
    } else {
      return false;
    }
  }
  _parameters[_paramsCount] = parameter;
  _paramsCount++;
  return true;
}

int DataStore::getDataParametersCount()
{
  return _paramsCount;
}

bool DataStore::changeParameter(const char *field, const char *value)
{
  bool is_found = false;
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {

    if (_parameters[parameterPos] != NULL)
    {
      if (strcmp(_parameters[parameterPos]->_field,field) == 0)
      {
        _parameters[parameterPos]->setValue(value);
        is_found = true;
      }
    }
  }
  return is_found;
}

const char* DataStore::getValue(const char *field)
{
  const char* parameter = "";
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {
    if (_parameters[parameterPos] != NULL)
    {
      if (strcmp(_parameters[parameterPos]->_field,field) == 0)
      {
        parameter = _parameters[parameterPos]->_value;
      }
    }
  }
  return parameter;
}

bool DataStore::clearParameters()
{
  bool clearSuccess = false;
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {
    if (_parameters[parameterPos] != NULL)
    {
      delete _parameters[parameterPos];
      _parameters[parameterPos]=NULL;
      clearSuccess = true;
    }
  }
  _paramsCount = 0;
  return clearSuccess;
}

bool DataStore::deleteParameter(const char *field)
{
  bool deleteSuccess = false;
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {
    if (_parameters[parameterPos] != NULL)
    {
      if (strcmp(_parameters[parameterPos]->_field,field) == 0)
      {
        delete _parameters[parameterPos];
        _parameters[parameterPos]=NULL;
        deleteSuccess = true;
      }
    }
  }
  return deleteSuccess;
}

bool DataStore::deleteParameterPrefix(const char *fieldPrefix)
{
  bool deleteSuccess = false;
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {
    if (_parameters[parameterPos] != NULL)
    {
      if (String(_parameters[parameterPos]->_field).indexOf(fieldPrefix) == 0)
      {
        delete _parameters[parameterPos];
        _parameters[parameterPos]=NULL;
      }
    }
  }
  return deleteSuccess;
}

DataParameter* DataStore::getDataParameter(const char *field)
{
  DataParameter *dataParameter;
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {
    if (_parameters[parameterPos] != NULL)
    {
      if (strcmp(_parameters[parameterPos]->_field,field) == 0)
      {
        dataParameter = _parameters[parameterPos];
        return dataParameter;
      }
    }
  }
  return NULL;
}

void DataStore::printParameters()
{
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {
    if (_parameters[parameterPos] != NULL)
    {
      Serial.println("\nfield: " + String(_parameters[parameterPos]->_field) +
                     "\nvalue: " + String(_parameters[parameterPos]->_value));
    }
  }
}

DataParameter** DataStore::getParameters()
{
  return _parameters;
}
