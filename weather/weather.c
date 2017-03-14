/* 
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

 /* include all the relevant libraries */
 #include <stdio.h>
 #include <stdlib.h>
 #include <ctype.h>
 #include <string.h>
 #include <stdbool.h>
 #include "ezxml.h"
 #include <curl/curl.h>
 #include "mycurl.h"
 #include "mycurl.c"


 int main(int argc, char *argv[])
{
	/* check args, must be the command itself and the four character code */
	if (argc == 2) {

		/* local variables: store the code in station and each of the letters in the letter variables */
	    char *station;
	    station = argv[1];
	    char letter1 = station[0];
	    char letter2 = station[1];
	    char letter3 = station[2];
	    char letter4 = station[3];

	    /* check if the code is exactly 4 characters */
	    if(strlen(station) != 4) {
	    	printf("%s\n", "Error: The code entered does not contain exactly 4 characters.");
	    	exit(1);
	    }

	    /* check if all the letters are uppercase */
	    if(!(isupper(letter1) && isupper(letter2) && isupper(letter3) && isupper(letter4))) {
	    	printf("%s\n", "Error: The code entered as a parameter is not in uppercase.");
	    	exit(2);
	    }

	    else {
		    /* curl stuff */
		    CURL *curl;
		    CURLcode res;
		    struct curlResponse s;

		    /* Format string for the URL we will request */
		    char *APIurl  = "http://w1.weather.gov/xml/current_obs/%s.xml";

		    /* allocate and initialize the output area */
		    init_curlResponse(&s);

		    /* create the URL from the format string. */
		    char newURL[100];
		    sprintf(newURL, APIurl, station);	

		    /* initialize curl */
		    curl = curl_easy_init();

		    if(curl) {
		        /* set the URL we want to visit */
		        curl_easy_setopt(curl, CURLOPT_URL, newURL);
		        
		        /* weather.gov expects USERAGENT option */
		        curl_easy_setopt(curl, CURLOPT_USERAGENT, "cs50");
		                
		        /* set the function curl should call with the result */
		        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		        /* set the buffer into which curl should place the data */
		        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		        /* Perform the request, res will get the return code */
		        res = curl_easy_perform(curl);

		        /* make sure it worked, if not print error and exit */
		        if (res != CURLE_OK) {
				    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				    exit(3);
				}

		        /* check: if the weather station code wasn't found, */
		        /* we get back HTML instead of XML  */
				if (strstr(s.ptr, "<?xml") == NULL) {
					printf("%s\n", "Error: The station code could not be found.");
					exit(4);
				}

				/* otherwise, select the desired output from the results. i did this using the ezxml parser, which was a free download online. */
				else {
					FILE *fp;
					fp = fopen("data.xml", "w");
					fprintf(fp, "%s\n", s.ptr);
					fclose(fp);

					ezxml_t fileOfData = ezxml_parse_file("data.xml");

					printf("credit: %s\n", ezxml_get(fileOfData, "credit", -1)->txt);
					printf("location: %s\n", ezxml_get(fileOfData, "location", -1)->txt);
					printf("station_id: %s\n", ezxml_get(fileOfData, "station_id", -1)->txt);
					printf("observation_time: %s\n", ezxml_get(fileOfData, "observation_time", -1)->txt);
					printf("weather: %s\n", ezxml_get(fileOfData, "weather", -1)->txt);
					printf("temperature_string: %s\n", ezxml_get(fileOfData, "temperature_string", -1)->txt);
					printf("relative_humidity: %s\n", ezxml_get(fileOfData, "relative_humidity", -1)->txt);
					printf("wind_string: %s\n", ezxml_get(fileOfData, "wind_string", -1)->txt);
					printf("visibility_mi: %s\n", ezxml_get(fileOfData, "visibility_mi", -1)->txt);
				}

		        /* always cleanup */
		        curl_easy_cleanup(curl);
		    }
	    }
	}
	/* otherwise, either too few or too many arguments. print error */
	else {
		printf("%s\n", "Error: Incorrent number of parameters. Please enter the function call with one parameter, a four character location code");
		exit(5);
	}
	exit(0);
}