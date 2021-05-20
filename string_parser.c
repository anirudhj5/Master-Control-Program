/*
 * string_parser.c
 *
 *  Created on: Nov 25, 2020
 *      Author: gguan
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"

#define _GUN_SOURCE

int count_token (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*/

	if(buf == NULL){
		return 0;
	}
	char *replace;
	char *token;
	int count = 0;

	replace = strdup(buf);
	while(token = strtok_r(replace, delim, &replace)){
		count += 1;
	}
	return count+1;
}

command_line str_filler (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	create command_line variable to be filled
	*	#2.	use function strtok_r to take out \n character then count
	*			number of tokens with count_token function, set num_token.
	*	#3. malloc memory for token array inside command_line variable
	*			base on the number of tokens.
	*	#4. malloc each index of the array with the length of tokens,
	*			fill array with tokens, and fill last spot with NULL.
	*	#5. return the variable.
	*/
	command_line new;

	buf[strcspn(buf, "\n")] = 0;
    int count = count_token(buf, delim);
    int c1 = 0;

    new.command_list = malloc(sizeof(char*)*count);
    new.num_token = count;

    char *replace;
    char *token;

    replace = strdup(buf);

    while(token = strtok_r(replace, delim, &replace)){
        new.command_list[c1] = malloc((strlen(token)+1)*sizeof(char));
        strcpy(new.command_list[c1], token);
        c1++;
    }
	/*for(int i = 0; i < new->num_token-1; i++){
		printf("%s\n", new->command_list[i]);
	}
	*/

	new.command_list[count-1] = NULL;
    return new;
}


void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	*/
	for(int i = 0; i < command->num_token; i++){
		free(command->command_list[i]);
	}
	free(command->command_list);
	command->num_token = 0;

}