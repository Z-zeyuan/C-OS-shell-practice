#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"
//changed size so allow 5 values saperate by ' ' for set
int MAX_ARGS_SIZE = 8;

int help();
int quit();
int badcommand();
int set(char* var, char* value);
int echo(char* value);
int my_ls();
int print(char* var);
int run(char* script);
int badcommandFileDoesNotExist();
int badcommandTooManyTokens();

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;
	if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		return badcommand();
	}


	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size > 7) return badcommandTooManyTokens();
		//combine legal inputs so some special symbles can be allowed to be set
		for ( i=3; i<args_size; i++){
		strcat(command_args[2]," ");
		strcat(command_args[2],command_args[i]);
		}
		return set(command_args[1], command_args[2]);
	
	} else if (strcmp(command_args[0], "echo")==0) {
		//echo
		if (args_size !=2) return badcommand();	
		return echo(command_args[1]);
	
	} else if (strcmp(command_args[0], "my_ls")==0) {
			//my _ls
			if (args_size !=1) return badcommand();	

			return my_ls();
			
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with 鈥淏ye!鈥漒n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
echo VALUE		print value in Var if it is a Var or print VALUE , if variable not found print newline   \n \
my_ls 		Prints the names in this directory . one name per line alphabric order\n    ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}
// For set command only
int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many tokens");
	return 4;
}
// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int set(char* var, char* value){
	//nochanges
	char *link = "=";
	char buffer[1000];
	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);

	mem_set_value(var, value);

	return 0;

}

int echo(char* value){
	//checking variable
	if(*value == '$'){
		//value+1 is value without $
		value+=1;
		//search and print
		if(strcmp(mem_get_value(value),"Variable does not exist"))  {printf("%s\n", mem_get_value(value)); }
			else {printf("\n");}
		
	} else {printf("%s\n", value); }
	return 0;

}

int my_ls(){
	//getting list to hidden file
	int status = system("ls > .tempfile260953312");
	   // Extract the first token
	FILE *temp = fopen(".tempfile260953312", "r");
	//print file by line
	char line[100];
	    while (!feof(temp)) 
	    { 
	    	fgets(line,99,temp);
	        printf ("%s", line); 
	        memset(line, 0, sizeof(line));
	    } 
	    fclose(temp);
	    remove(".tempfile260953312");
	return 0;
}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file
	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	
	//provides a mark for end of file
	while(fgets(line,999,p)!=NULL){
		char *command;
		command = strtok(line, ";");
			while (command != NULL)
			{
				errCode = parseInput(command);	// which calls interpreter()
				command = strtok(NULL, ";");
			}
			memset(line, 0, sizeof(line));
	}
	
    fclose(p);
    
    return errCode;
}
