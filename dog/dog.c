/* 
 * dog.c - provide some functionality of cat with some extra features
 * 
 * usage: dog [—-help]
 * usage: dog [-n] [-m mapfile] [-M mapfile] [-] [filename]...
 * where:
 *   —-help is the option that shows this README file for more information
 *   -n is the option that enables numbered lines for the output
 *   -m mapfile is the option that takes a filename as an argument and uses it to encrypt its inputs with a simple substitution cipher
 *   -M mapfile is very similar to -m but it uses the filename to decrypt its inputs
 *   only one of -m and -M can be used
 * output:
 *   read the input from the files or from stdin and depending on the options indicated, output onto stdout. If -n is indicated, include 
 *   numbered lines. If -m or -M indicated, encrypt/decrypt the input before output to stdout
 *   
 * stdin: the files indicated. if special file ‘-‘ indicated, or no files indicated, stdin is used as input
 * stdout: stdout
 * stderr: error messages
 *
 * Raunak Bhojwani, April 2016
 */

 /* include relevant files */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>

 /* declare function prototype */
bool isNewLine(char);

int main(int argc, char *argv[])
{
  /* local variables */
  int nFlag = 0;
  int mFlag = 0;
  int MFlag = 0;
  int helpFlag = 0;
  char *mFilename = NULL;
  int option;
  bool start = true;
  int lineNum = 1;

  /* check for --help option first, if found open the README.txt file */
  	for (int i = 1; i < argc; i++) {
  		if (strcmp(argv[i], "--help") == 0) {
  			char x;
  			FILE *helpFile;
  			helpFile = fopen("README.txt", "r");
  			if (helpFile) {
  				while ((x = fgetc(helpFile)) != EOF) {
  					 printf("%c",x);
  				}
  			}
  			exit(0);
  		}
  	}

  	/* use switch case to indicate which options have been indicated by the user */
	while ((option = getopt(argc, argv, "nm:M:")) != -1) {
	  	switch(option) {
	  		case 'n':
	  			nFlag = 1;
	  			break;
	  		case 'm':
	  			mFlag = 1;
	  			mFilename = optarg;
	  			break;
	  		case 'M':
	  			MFlag = 1;
	  			mFilename = optarg;
	  			break;
	  		 case '?':
	  		 /* intercept any errors */
		  	 	if (optopt == 'm' || optopt == 'M') {
		  	 		printf("Option -%c requires an argument.\n", optopt);
		  	 		exit(1);
		  	 	}
		  	 	else if (isprint(optopt)) {
		  	 		printf("Unknown option `-%c'.\n", optopt);
		  	 		exit(1);
		  	 	}
		  	 	else {
		  	 		printf("Unknown option character `\\x%x'.\n", optopt);
		  	 		exit(1);
		  	 	}
		  	 default:
		  	 	break;
		  	 	
	  	}
	}

	/* if both -m and -M have been indicated, flag an error */
	if ((mFlag == 1) && (MFlag == 1)) {
	  	fprintf (stderr, "Cannot process both -m and -M options. Please choose one.");
	  	exit(2);
	}

	/* no options indicated */
	if ((nFlag == 0) && (mFlag == 0) && (MFlag == 0)) {							
	  	
	  	/* check if any files indicated, if not take input from stdin */
	  	if (argv[optind] == NULL) {
	  		char x;
	  		while ((x = fgetc(stdin)) != EOF) {
				printf("%c", x);
			}
	  	}

	  	while (argv[optind] != NULL) {
	  		/* check if any specialfile indicated, if not take input from stdin */
	  		if (strcmp(argv[optind], "-") == 0) {
	  			char x;
	  			while ((x = fgetc(stdin)) != EOF) {
	  				printf("%c", x);
	  			}
	  		}
	  		else {
	  			/* if any files indicated, open file and check if it exists */
	  			char x;
	  			FILE *fp;
	  			fp = fopen(argv[optind], "r");
	  			if (fp == NULL) {
	  				printf("Error: Could not find the file you have specified");
	  				exit(4);
	  			}
	  			/* read file by character and print to stdout */
	  			while ((x = fgetc(fp)) != EOF) {
	  				printf("%c", x);
	  			}
	  			fclose(fp);
	  		}
	  		optind ++;		// increment optind to go to next file
	  	}
	}

	/* -m and -n options indicated */
	else if ((nFlag == 1) && (mFlag == 1) && (MFlag == 0)) {						
	  	/* open mapfile and check if it exists, if not, throw an error */
	  	FILE *mapFile;
	  	mapFile = fopen(mFilename, "r");

	  	if (mapFile == NULL) {
	  		printf("Error: the mapfile indicated cannot be opened");
	  		exit(3);
	  	}
	  	else{
	  		/* local variables */
	  		char original[100]={0};
		  	char cipher[100]={0};
		  	char line[10];
		  	int i = 0;

		  	/* read the mapfile by line and save the characters in two arrays: original and cipher */
	  		while (fgets(line, 10, mapFile) != NULL) {
	  			if (((line[0] >= 'a' && line[0] <= 'z') || (line[0] >= 'A' && line[0] <= 'Z')) && ((line[2] >= 'a' && line[2] <= 'z') || (line[2] >= 'A' && line[2] <= 'Z')) ){
		  			original[i] = line[0];
		  			cipher[i] = line[2];
		  			i++;
		  		}
	  		}
	  		fclose(mapFile);

	  		/* if no files indicated, read from stdin, convert using the two arrays created and output to stdout */
		  	if (argv[optind] == NULL) {
		  		char x;
		  		bool newLineLast = false;
		  		int mapFileLines = 26;
		  		while ((x = fgetc(stdin)) != EOF) {
		  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
		  				int foundAt;
		  				/* find index at which the x character is stored in the original array, if it is stored at all*/
						for (foundAt = 0; original[foundAt]!= x; foundAt++) {
			  			}
						if (foundAt < mapFileLines) {
							/* check if it is the start of a file, or is it is a new line in the file and accordingly add line numbers */
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", cipher[foundAt]);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", cipher[foundAt]);
							}	
						}
						else {
							/* take care of new lines and line numbers even if character is not found in original array */
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", x);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", x);
							}
						}
					}
					else {	
						/* take care of new lines and line numbers even if character is not a letter*/
						if (start) {
		  					start = false;
		  					printf("%d ", lineNum);
		  					printf("%c", x);
		  					lineNum++;
		  				}
		  				else if (isNewLine(x)) {
		  					printf("\n");
		  					newLineLast = true;
		  				}
						else {
							if (newLineLast) {
								printf("%d ", lineNum);
								lineNum++;
								newLineLast = false;
							}
							printf("%c", x);
						}
					}	
				}
		  	}

		  	
		  	while (argv[optind] != NULL) {
		  		/* if special file indicated, read from stdin, convert using the two arrays created and output to stdout */
		  		if (strcmp(argv[optind], "-") == 0) {
		  			char x;
		  			bool newLineLast = false;
		  			int mapFileLines = 26;
			  		while ((x = fgetc(stdin)) != EOF) {
			  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
			  				/* find index at which the x character is stored in the original array, if it is stored at all*/
							for (foundAt = 0; original[foundAt]!= x; foundAt++) {
				  			}
							if (foundAt < mapFileLines) {
								/* check if it is the start of a file, or is it is a new line in the file and accordingly add line numbers */
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", cipher[foundAt]);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", cipher[foundAt]);
								}	
							}
							else {
								/* take care of new lines and line numbers even if character is not found in original array */
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", x);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", x);
								}
							}
						}
						else {	
							/* take care of new lines and line numbers even if character is not a letter*/
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", x);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", x);
							}
						}	
					}
					/* set start to true to make sure the program is ready for new files */
					start = true;
		  		}

		  		/* if files indicated, read files by character, convert using the two arrays created and output to stdout */
		  		else {
		  			char x;
		  			int mapFileLines = 26;
		  			bool newLineLast = false;

		  			FILE *fp;
		  			fp = fopen(argv[optind], "r");
		  			if (fp == NULL) {
	  					printf("Error: Could not find the file you have specified");
	  					exit(4);
	  				}
		  			while ((x = fgetc(fp)) != EOF) {
		  				if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
							for (foundAt = 0; original[foundAt]!= x; foundAt++) {
				  			}
				  			/* check if it is the start of a file, or is it is a new line in the file and accordingly add line numbers */
							if (foundAt < mapFileLines) {
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", cipher[foundAt]);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", cipher[foundAt]);
								}	
							}
							else {
								/* take care of new lines and line numbers even if character is not found in original array */
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", x);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", x);
								}
							}
						}
						else {
						/* take care of new lines and line numbers even if character is not a letter*/	
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", x);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", x);
							}
						}
		  			}
		  			/* update local variables so that if more files are opened, program runs smoothly */
		  			start = true;
		  			lineNum--;
		  			fclose(fp);
		  		}
		  		optind ++;
	  		}
		}	
	}


	/* -M and -n option */
	else if ((nFlag == 1) && (mFlag == 0) && (MFlag == 1)) {
	  	 /* open mapfile and check if it exists, if not throw it an error */
	  	FILE *mapFile;
	  	mapFile = fopen(mFilename, "r");

	  	if (mapFile == NULL) {
	  		printf("Error: the mapfile indicated cannot be opened");
	  		exit(3);
	  	}

	  	else{
	  		/* local variables */
	  		char original[100]={0};
		  	char cipher[100]={0};
		  	char line[10];
		  	int i = 0;

		  	/* read mapfile and create two arrays, like done in the -m option*/
	  		while (fgets(line, 10, mapFile) != NULL) {
	  			if (((line[0] >= 'a' && line[0] <= 'z') || (line[0] >= 'A' && line[0] <= 'Z')) && ((line[2] >= 'a' && line[2] <= 'z') || (line[2] >= 'A' && line[2] <= 'Z')) ){
		  			original[i] = line[0];
		  			cipher[i] = line[2];
		  			i++;
		  		}
	  		}
	  		fclose(mapFile);

	  		/* if no files indicated, read from stdin and use the arrays to decrypt the input before printing to stdout */
		  	if (argv[optind] == NULL) {
		  		char x;
		  		bool newLineLast = false;
		  		int mapFileLines = 26;
		  		while ((x = fgetc(stdin)) != EOF) {
		  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
		  				int foundAt;
						for (foundAt = 0; cipher[foundAt]!= x; foundAt++) {
			  			}
						if (foundAt < mapFileLines) {
							/* check if it is the start of a file, or is it is a new line in the file and accordingly add line numbers */
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", original[foundAt]);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", original[foundAt]);
							}	
						}
						else {
							/* take care of new lines and line numbers even if character is not found in original array */
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", x);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", x);
							}
						}
					}
					else {	
						/* take care of new lines and line numbers even if character is not a letter*/
						if (start) {
		  					start = false;
		  					printf("%d ", lineNum);
		  					printf("%c", x);
		  					lineNum++;
		  				}
		  				else if (isNewLine(x)) {
		  					printf("\n");
		  					newLineLast = true;
		  				}
						else {
							if (newLineLast) {
								printf("%d ", lineNum);
								lineNum++;
								newLineLast = false;
							}
							printf("%c", x);
						}
					}	
				}
		  	}

		  	/* if special file indicated, read from stdin before decrypting using the original and cipher arrays and printing to stdout */
		  	while (argv[optind] != NULL) {
		  		if (strcmp(argv[optind], "-") == 0) {
		  			char x;
		  			bool newLineLast = false;
		  			int mapFileLines = 26;
			  		while ((x = fgetc(stdin)) != EOF) {
			  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
							for (foundAt = 0; cipher[foundAt]!= x; foundAt++) {
				  			}
							if (foundAt < mapFileLines) {
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", original[foundAt]);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", original[foundAt]);
								}	
							}
							else {
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", x);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", x);
								}
							}
						}
						else {	
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", x);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", x);
							}
						}	
					}
					start = true;
		  		}
		  		/* if file indicated, read from file before decrypting using the original and cipher arrays and printing to stdout */
		  		else {
		  			char x;
		  			int mapFileLines = 26;
		  			bool newLineLast = false;

		  			FILE *fp;
		  			fp = fopen(argv[optind], "r");
		  			if (fp == NULL) {
		  				printf("Error: Could not find the file you have specified");
		  				exit(4);
		  			}
		  			while ((x = fgetc(fp)) != EOF) {
		  				if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
							for (foundAt = 0; cipher[foundAt]!= x; foundAt++) {
				  			}
							if (foundAt < mapFileLines) {
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", original[foundAt]);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", original[foundAt]);
								}	
							}
							else {
								if (start) {
				  					start = false;
				  					printf("%d ", lineNum);
				  					printf("%c", x);
				  					lineNum++;
				  				}
				  				else if (isNewLine(x)) {
				  					printf("\n");
				  					newLineLast = true;
				  				}
								else {
									if (newLineLast) {
										printf("%d ", lineNum);
										lineNum++;
										newLineLast = false;
									}
									printf("%c", x);
								}
							}
						}
						else {	
							if (start) {
			  					start = false;
			  					printf("%d ", lineNum);
			  					printf("%c", x);
			  					lineNum++;
			  				}
			  				else if (isNewLine(x)) {
			  					printf("\n");
			  					newLineLast = true;
			  				}
							else {
								if (newLineLast) {
									printf("%d ", lineNum);
									lineNum++;
									newLineLast = false;
								}
								printf("%c", x);
							}
						}
		  			}
		  			start = true;
		  			lineNum--;
		  			fclose(fp);
		  		}
		  		optind ++;
	  		}
		}	
	}
	/* write files with line numbers */
	else if ((nFlag == 1) && (mFlag == 0) && (MFlag == 0)) {								
	  	/* -n option */
	  	
		if (argv[optind] == NULL) {
	  		char x;
  			bool newLineLast = false;
  			while ((x = fgetc(stdin)) != EOF) {
  				/* check if it is the start of a file or if a new line was entered and accordingly insert line numbers */
  				if (start) {
  					start = false;
  					printf("%d ", lineNum);
  					printf("%c", x);
  					lineNum++;
  				}
  				else if (isNewLine(x)) {
  					printf("\n");
  					newLineLast = true;
  				}
				else {
					if (newLineLast) {
						printf("%d ", lineNum);
						lineNum++;
						newLineLast = false;
					}
					printf("%c", x);
				}
  			}
	  	}

	  	while (argv[optind] != NULL) {
	  		if (strcmp(argv[optind], "-") == 0) {
	  			char x;
	  			bool newLineLast = false;
	  			while ((x = fgetc(stdin)) != EOF) {
	  				if (start) {
	  					start = false;
	  					printf("%d ", lineNum);
	  					printf("%c", x);
	  					lineNum++;
	  				}
	  				else if (isNewLine(x)) {
	  					printf("\n");
	  					newLineLast = true;
	  				}
					else {
						if (newLineLast) {
							printf("%d ", lineNum);
							lineNum++;
							newLineLast = false;
						}
						printf("%c", x);
					}
	  			}
	  			start = true;
	  		}
	  		else {
	  			char x;
	  			bool newLineLast = false;
	  			FILE *fp;
	  			fp = fopen(argv[optind], "r");
	  			if (fp == NULL) {
	  				printf("Error: Could not find the file you have specified");
	  				exit(4);
	  			}
	  			/* if file indicated, open and read file and output to std out with line numbers*/
	  			while ((x = fgetc(fp)) != EOF) {
	  				if (ferror(fp)) {
	  					printf("Error in reading from file.\n");
	  					fclose(fp);
	  					exit(5);
	  				}
	  				if (start) {
	  					start = false;
	  					printf("%d ", lineNum);
	  					printf("%c", x);
	  					lineNum++;
	  				}
  					else if (isNewLine(x)) {
	  					printf("\n");
	  					newLineLast = true;
	  				}
					else {
						if (newLineLast) {
							printf("%d ", lineNum);
							lineNum++;
							newLineLast = false;
						}
						printf("%c", x);
					}
	  			}
	  			start = true;
	  			lineNum--;
	  			fclose(fp);
	  		}
	  		optind ++;
	  	}
	}

	/* create the original and cipher arrays and then use it to encrypt the input before pinting to stdout */
	else if ((nFlag == 0) && (mFlag == 1) && (MFlag == 0)) {										// -m
	  	/* -m options */

		/* check if mapfile exists, if not print an error */
	  	FILE *mapFile;
	  	mapFile = fopen(mFilename, "r");

	  	if (mapFile == NULL) {
	  		printf("Error: the mapfile indicated cannot be opened");
	  		exit(3);
	  	}
	  	else{
	  		char original[100]={0};
		  	char cipher[100]={0};
		  	char line[10];
		  	int i = 0;

		  	/* create original and cipher arrays */
	  		while (fgets(line, 10, mapFile) != NULL) {
	  			if (((line[0] >= 'a' && line[0] <= 'z') || (line[0] >= 'A' && line[0] <= 'Z')) && ((line[2] >= 'a' && line[2] <= 'z') || (line[2] >= 'A' && line[2] <= 'Z')) ){
		  			original[i] = line[0];
		  			cipher[i] = line[2];
		  			i++;
		  		}
	  		}
	  		fclose(mapFile);

	  		
		  	if (argv[optind] == NULL) {
		  		char x;
		  		int mapFileLines = 26;
		  		while ((x = fgetc(stdin)) != EOF) {
		  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
		  				int foundAt;
						for (foundAt = 0; original[foundAt]!= x; foundAt++) {
			  			}
						if (foundAt < mapFileLines) {
								printf("%c", cipher[foundAt]);
						}
						else {
							printf("%c", x);
						}
					}
					else {
							printf("%c", x);
					}	
				}
		  	}

		  	while (argv[optind] != NULL) {
		  		if (strcmp(argv[optind], "-") == 0) {
		  			char x;
		  			int mapFileLines = 26;
			  		while ((x = fgetc(stdin)) != EOF) {
			  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
							for (foundAt = 0; original[foundAt]!= x; foundAt++) {
				  			}
							if (foundAt < mapFileLines) {
								printf("%c", cipher[foundAt]);
							}
							else {
								printf("%c", x);
							}
						}
						else {
							printf("%c", x);
						}	
					}
		  		}

		  		else {
		  			char x;
		  			int mapFileLines = 26;
		  			FILE *fp;
		  			fp = fopen(argv[optind], "r");
		  			if (fp == NULL) {
		  				printf("Error: Could not find the file you have specified");
		  				exit(4);
		  			}
		  			while ((x = fgetc(fp)) != EOF) {
		  				if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
							for (foundAt = 0; original[foundAt]!= x; foundAt++) {
				  			}
							if (foundAt < mapFileLines) {
								printf("%c", cipher[foundAt]);
							}
							else {
								printf("%c", x);
							}
						}
						else {
							printf("%c", x);
						}
		  			}
		  			fclose(fp);
		  		}
		  		optind ++;
	  		}
		}
	}

	/* create the original and cipher arrays and then use it to decrypt the input before pinting to stdout */
	else {																						
	  	/* -M options */

	  	FILE *mapFile;
	  	mapFile = fopen(mFilename, "r");

	  	if (mapFile == NULL) {
	  		printf("Error: the mapfile indicated cannot be opened");
	  		exit(3);
	  	}
	  	else{
	  		char original[100]={0};
		  	char cipher[100]={0};
		  	char line[10];
		  	int i = 0;

	  		while (fgets(line, 10, mapFile) != NULL) {
	  			if (((line[0] >= 'a' && line[0] <= 'z') || (line[0] >= 'A' && line[0] <= 'Z')) && ((line[2] >= 'a' && line[2] <= 'z') || (line[2] >= 'A' && line[2] <= 'Z')) ){
		  			original[i] = line[0];
		  			cipher[i] = line[2];
		  			i++;
		  		}
	  		}
	  		fclose(mapFile);

		  	if (argv[optind] == NULL) {
		  		char x;
		  		int mapFileLines = 26;
		  		while ((x = fgetc(stdin)) != EOF) {
		  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
		  				int foundAt;
						for (foundAt = 0; cipher[foundAt]!= x; foundAt++) {
			  			}
						if (foundAt < mapFileLines) {
								printf("%c", original[foundAt]);
						}
						else {
							printf("%c", x);
						}
					}
					else {
							printf("%c", x);
					}	
				}
		  	}

		  	while (argv[optind] != NULL) {
		  		if (strcmp(argv[optind], "-") == 0) {
		  			char x;
		  			int mapFileLines = 26;
			  		while ((x = fgetc(stdin)) != EOF) {
			  			if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
							for (foundAt = 0; cipher[foundAt]!= x; foundAt++) {
				  			}
							if (foundAt < mapFileLines) {
								printf("%c", original[foundAt]);
							}
							else {
								printf("%c", x);
							}
						}
						else {
							printf("%c", x);
						}	
					}
		  		}

		  		else {
		  			char x;
		  			int mapFileLines = 26;
		  			FILE *fp;
		  			fp = fopen(argv[optind], "r");
		  			if (fp == NULL) {
		  				printf("Error: Could not find the file you have specified");
		  				exit(4);
		  			}
		  			while ((x = fgetc(fp)) != EOF) {
		  				if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') ){
			  				int foundAt;
							for (foundAt = 0; cipher[foundAt]!= x; foundAt++) {
				  			}
							if (foundAt < mapFileLines) {
								printf("%c", original[foundAt]);
							}
							else {
								printf("%c", x);
							}
						}
						else {
							printf("%c", x);
						}
		  			}
		  			fclose(fp);
		  		}
		  		optind ++;
	  		}
		}
	}
	/* if successful, exit with a status of 0 */
	exit(0);
}

/* function to check if the currect character being read is /n (a new line) or the end of a file */
bool isNewLine(char current) 
{
	if ((current == EOF) || (current == '\n')) {
		return true;
	}
	return false;
}


