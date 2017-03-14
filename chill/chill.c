/* 
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
 * stdin: the input template
 * stdout: the modified template
 * stderr: error messages
 *
 * Raunak Bhojwani, April 2016
 */

/* include the libraries that are needed */
 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>

/* function that takes as input two floats then calculates and returns the wind chill using provided formula */
float calculateChill(float T, float V)
  {
    float windChill;
    float exponent;

    /* use pow function from math library to use exponents */
    exponent = pow(V, 0.16);
    windChill = 35.74 + (0.6215 * T)  - (35.75 * exponent) + (0.4275 * T * exponent);

    return windChill;
  }

 int main(int argc, char *argv[])
 {

 	float windChill;

/* check if no paramters passed (except for command itself) */
 	if (argc == 1) {
    /* print header */
 		printf("Temp\tWind\tChill\n");
 		printf("-----\t----\t-----\n");
 		/* use for loop to loop through -10,0,10,20,30,40F of temperature and 5,10,15MPH of wind speed and calculate and output corresponding windchills */
 		for (float temperature = -10; temperature <= 40; temperature += 10) {
 			for (float windVelocity = 5; windVelocity <= 15; windVelocity += 5) {
 				windChill = calculateChill(temperature, windVelocity);
 				printf("%.1f\t%.1f\t%.1f\n", temperature, windVelocity, windChill);
 			}
 			printf("\n");
 		}
 	}
/* check if only one paramter passed (except for command itself) */
 	else if (argc == 2) {
    float temp = atof(argv[1]);
    /* check if temperature is out of bounds */
 		if (temp >= 50) {
 			printf("Temparature must be less than 50 degrees Fahrenheit");
      exit(1);
 		}
 	  
    /* print header */
 		printf("Temp\tWind\tChill\n");
 		printf("-----\t----\t-----\n");
 		/* use for loop to loop through 5,10,15MPH of wind speed and calculate and output corresponding windchills */
 		for (float windVelocity = 5; windVelocity <= 15; windVelocity += 5) {
 				windChill = calculateChill(temp, windVelocity);
 				printf("%.1f\t%.1f\t%.1f\n", temp, windVelocity, windChill);
 			}
 	}

  /* check if two paramter passeds (except for command itself) */
 	else if (argc == 3) {
    float temp = atof(argv[1]);
    float windSpeed = atof(argv[2]);
    /* check if temperature is out of bounds */
 		if (temp >= 50) {
 			printf("Temparature must be less than 50 degrees Fahrenheit");
      exit(1);
 		}
    /* check if wind chill is out of bounds */
 		else if (windSpeed < 0.5){
 			printf("Wind Speed must be greater than or equal to 0.5MPH");
      exit(2);
 		}
    /* print header */
 		printf("Temp\tWind\tChill\n");
 		printf("-----\t----\t-----\n");

 		/* calculate and output corresponding windchills */
		windChill = calculateChill(temp, windSpeed);
		printf("%.1f\t%.1f\t%.1f\n", temp, windSpeed, windChill);
 	}
  /* otherwise, too many arguments have been passed */
 	else {
 		printf("usage: %s \n", "too many arguments provided.");
    	exit(3);
 	}
  exit(0);
 }