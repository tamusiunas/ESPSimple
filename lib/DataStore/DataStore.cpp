/**
 * @file DataStore.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Object to manage and store parameters used in the project
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "DataStore.h"

DataStore::DataStore() {
    _max_params = DATASTORE_MAX_PARAMS;
    _debugMessage = DebugMessage();
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
    // if params are full increase params by 5 to be faster (so dont need to resize every new parameter)
    _max_params += 5;  

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
  String parametersStr = "";
  for (int parameterPos = 0; parameterPos < _paramsCount; parameterPos++)
  {
    if (_parameters[parameterPos] != NULL)
    {
      parametersStr += "Key: " + String(_parameters[parameterPos]->_field) +
                     " - Value: " + String(_parameters[parameterPos]->_value) + "\n";
    }
  }
  _debugMessage.debug("DataStore keys:\n" + parametersStr);
}

DataParameter** DataStore::getParameters()
{
  return _parameters;
}
