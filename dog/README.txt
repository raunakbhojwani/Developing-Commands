README for dog.c
Raunak Bhojwani, April 2016

Compiling:
	mygcc -o dog dog.c

Usage:/* 
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

Example command lines:
	
  dog
  dog —-help
  dog -n -m mapfile.txt 
  dog -n filename1 - filename2
  dog -n -M mapfile.txt filename1 filename2 - - - filename3

Exit status:
  0 - success
  1 - unable to understand option/mapfile not given to -m or -M
  2 - both options -m and -M indicated
  3 - mapfile does not exist or could not be found
  4 - unable to open file/file does not exist
  5 - error reading file

Assumptions:

- it is assumed that the mapfile is formatted as expected: 26 lines of encryption where the first character is the original and the third character is the cipher
- it is assumed the user will have his or her own files to pass through dog


Non-Assumptions:

- there is no limit to the number of files that the user can pass to dog
- cannot use -m and -M options simultaneously
- when taking from stdin, the user can type whatever they wish to, without any limit or cap

Limitations:

- the program is written to only encrypt and decrypt alphabets, even if maple contains numbers or strings
- cannot encrypt and decrypt in the same function call, the program catches the error

