README for weather.c
Raunak Bhojwani, April 2016

Compiling:
	mygcc -o weather weather.c -lcurl ezxml.o

Usage:/* 
 * weather.c - extract data from the national weather.gov website and parse it for relevant information about a certain location
 * 
 * usage: weather CODE
 * where:
 *   CODE is the four character, all upper case station code that corresponds to one of the government’s weather stations
 * output:
 *   read the CODE and use the curl library to access the appropriate URL for the corresponding station
 *   if it is a valid url, parse the data available to extract relevant information
 *   such as: credit, location, station_id, observation_time, weather, temperature_string, relative_humidity, wind_string, and visibility_mi
 * stdin: stdin
 * stdout: stdout
 * stderr: error messages
 *
 * Raunak Bhojwani, April 2016
 */

Example command lines:

  weather KMWN
  weather KBML
  weather KCON

Exit status:
  0 - success
  1 - code entered does not contain exactly 4 characters
  2 - code entered is not in uppercase
  3 - curl_easy_perform failed
  4 - code entered could not be found on database
  5 - incorrect number of parameters.

Assumptions:

- a lot of the functionality of curl is assumed in this program. it is assumed that the station 
  code must be exactly 4 characters long and be completely uppercase. if this is not the case, an
  error will be found.
- i am assuming that you have access to all the files i have submitted in the tarball. my use of the
  ezxml parser meant that i had to make some files in my weather directory. you shouldn’t have a problem
  with this, but if you do please download the ezxml parser and use the ‘make’ command in the weather directory
  before compiling the code.


"Non"-Assumptions:

- the four letter code entered must correspond to a code on the weather.gov database, or else an error will be printed.


Limitations:

- the program uses only the w1.weather.gov database, so it is not an exhaustive source of weather information.
  accuracy of the information is entirely dependent on the accuracy of the information of the database.