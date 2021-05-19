/**
 * @file DataParameter.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Headers for DataParameter.cpp
 * @version 0.1
 * @date 2021-05-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef DataParameter_h
#define DataParameter_h

#include <stdlib.h>
#include <string.h>
#include "DebugMessage.h"

/**
 * @brief Object to store key and value
 * 
 */
class DataParameter
{
public:
  /**
   * @brief Construct a new Data Parameter object
   * 
   */
  DataParameter();

  /**
   * @brief Construct a new Data Parameter object
   * 
   * @param field Key
   * @param value Value
   */
  DataParameter(const char *field, const char *value);

  /**
   * @brief Construct a new Data Parameter object
   * 
   * @param field Key
   * @param value Value
   */
  DataParameter(String field, String value);

  /**
   * @brief Destroy the Data Parameter object
   * 
   */
  ~DataParameter();

  /**
   * @brief Get the Field object
   * 
   * @return const char* Key
   */
  const char *getField();

  /**
   * @brief Get the Value object
   * 
   * @return const char* Value
   */
  const char *getValue();

  /**
   * @brief Set the Field object
   * 
   * @param field Field
   */
  void setField(const char *field);

  /**
   * @brief Set the Value object
   * 
   * @param value Value
   */
  void setValue(const char *value);

private:
  char *_field;
  char *_value;
  friend class DataStore;
};

#endif