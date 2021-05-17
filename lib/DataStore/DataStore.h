/**
 * @file DataStore.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Objects to store user parameters
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef DataStore_h
#define DataStore_h

#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include "DebugMessage.h"
#include "DataParameter.h"

#ifndef DATASTORE_MAX_PARAMS
#define DATASTORE_MAX_PARAMS 5
#endif


/**
 * @brief Object to store a list of DataParameter
 * 
 */
class DataStore
{
public:
  /**
   * @brief Construct a new Data Store object
   * 
   */
  DataStore();

  /**
   * @brief Destroy the Data Store object
   * 
   */
  ~DataStore();

  /**
   * @brief Add DataParameter
   * 
   * @param parameter DataParameter
   * @return true Success
   * @return false Failure
   */
  bool addParameter(DataParameter *parameter);
  
  /**
   * @brief Change parameter
   * 
   * @param field Key
   * @param value Value
   * @return true Sucess
   * @return false Failure
   */
  bool changeParameter(const char *field, const char *value);
  
  /**
   * @brief Delete parameter
   * 
   * @param field Key
   * @return true Success
   * @return false Failure
   */
  bool deleteParameter(const char *field);
  
  /**
   * @brief Delete parameters starting with fieldPrefix
   * 
   * @param fieldPrefix Prefix
   * @return true Sucess
   * @return false Failure
   */
  bool deleteParameterPrefix(const char *fieldPrefix);

  /**
   * @brief Get value for field
   * 
   * @param field Key
   * @return const char* Value
   */
  const char *getValue(const char *field);

  /**
   * @brief Get the DataParameter object
   * 
   * @param field Key
   * @return DataParameter* Pointer to DataParamenter
   */
  DataParameter *getDataParameter(const char *field);

  /**
   * @brief Get total of DataParameter obejcts
   * 
   * @return int Total of DataParameter obejcts
   */
  int getDataParametersCount();

  /**
   * @brief Print parameters to Serial
   * 
   */
  void printParameters();

  /**
   * @brief Clear all parameters
   * 
   * @return true Success
   * @return false Failure
   */
  bool clearParameters();

  /**
   * @brief Get an array of DataParameter Objects
   * 
   * @return DataParameter** Pointer to an array of DataParameter object
   */
  DataParameter** getParameters();

  /**
   * @brief Get a pointer to DataParameter by position in array
   * 
   * @param parameterPos Position
   * @return DataParameter* 
   */
  DataParameter * getParameterByPos(int parameterPos);

private:
  DataParameter** _parameters;
  int _paramsCount = 0;
  int _max_params;
  DebugMessage _debugMessage;
};

#endif
