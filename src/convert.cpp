#include "convert.h"

float convert_celsius_to_fahrenheit(float celsius)
{
  return ((celsius * 9) / 5.0) + 32;
}

float convert_fahrenheit_to_celsius(float fahrenheit)
{
  return ((fahrenheit - 32) * 5) / 9.0;
}

float convert_kilometers_to_miles(float kilometers)
{
  return kilometers / 1.619344;
}

float convert_miles_to_kilometers(float miles)
{
  return miles * 1.619344;
}