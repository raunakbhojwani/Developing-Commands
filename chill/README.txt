README for chill.c
Raunak Bhojwani, April 2016

Compiling:
	mygcc -o chill chill.c -lm

Usage:/* 
 * chill.c - calculate the windchill based on current temperature and wind speed
 * 
 * usage: chill [temperature] [wind speed]
 * where:
 *   temperature is the temperature at which windchill is to be calculated
 *   wind speed is the velocity of the wind at which windchill is to be calculated
 * output:
 *   read the temperature and wind speed and calculate using the formula  Wind Chill = 35.74 + 0.6215T - 35.75(V^0.16) + 0.4275T(V^0.16)
 *   and print to stdout the temperature, wind speed and corresponding windchill. If no windspeed given, give windchill for 5, 10, 15mph and if no temp
 *   given, give windchill for -10, 0, 10, 20, 30, 40F. 
 * stdin: stdin
 * stdout: stdout
 * stderr: error messages
 *
 * Raunak Bhojwani, April 2016
 */

Example command lines:

  chill
  chill 32
  chill 5 20 

Exit status:
  0 - success
  1 - temperature is greater than or equal 50F
  2 - wind speed is less than 0.5MPH
  3 - too many arguments provided

Assumptions:

- it is assumed that the user passes only integers, floats or doubles as arguments. 
  if characters or strings as passed as parameters, the program may or may not convert 
  it to a float , which will not lead to desired behavior.

"Non"-Assumptions:

- temperature and wind speed are integers, or floats

Limitations:

- the program will not be able to calculate windchill for very large numbers,
  for example the wind speed of 20000000000000000000000000000000000000000000000000000000000000000000
  is interpreted as infinite and so leads to undesired behavior