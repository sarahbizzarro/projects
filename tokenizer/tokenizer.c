
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
struct TokenizerT_ {
	char *separators;
	char *str;
	int index;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * Returns a copy of the given string, replacing any sequences representing
 * special characters with the actual special characters.
 */
char *unescape(char *str)
{
	char *unescaped;
	int len = strlen(str);

	unescaped = (char *) malloc(len*sizeof(char) + 1);
	int j = 0;	// where to insert next character into unescaped string

	int i;
	for (i = 0; i < len; i++){

		char c = str[i];

		if (c == '\\'){
			i++;
			switch (str[i])
			{
				case 'n':
					unescaped[j++] = '\n'; break;
				case 't':
					unescaped[j++] = '\t'; break;
				case 'v':
					unescaped[j++] = '\v'; break;
				case 'b':
					unescaped[j++] = '\b'; break;
				case 'r':
					unescaped[j++] = '\r'; break;
				case 'f':
					unescaped[j++] = '\f'; break;
				case 'a':
					unescaped[j++] = '\a'; break;
				case '\\':
					unescaped[j++] = '\\'; break;
				case '\"':
					unescaped[j++] = '\"'; break;
				case '\0':
					break;
				default:
					unescaped[j++] = str[i]; break;
			}
		} else {
			unescaped[j++] = c;
		}
	}

	unescaped[j] = '\0';

	return unescaped;
}

/*
 * Returns 1 if the given string contains the given character, 0 otherwise.
 */
int contains(char *str, char c)
{
	int len = strlen(str);

	int i;
	for (i = 0; i < len; i++){
		if (str[i] == c) return 1;
	}

	return 0;
}

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
TokenizerT *TKCreate(char *separators, char *ts)
{
	TokenizerT *tokenizer = (TokenizerT *) malloc(sizeof(TokenizerT));

	tokenizer->separators = unescape(separators);
	tokenizer->str = unescape(ts);
	tokenizer->index = 0;

	return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
void TKDestroy(TokenizerT *tk)
{
	free(tk->separators);
	free(tk->str);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
char *TKGetNextToken(TokenizerT *tk)
{
	char *separators = tk->separators;
	char *str = tk->str;
	int len = strlen(str);

	// find where next token begins
	int start = tk->index;
	while(contains(separators, str[start]) && (start < len)){
		start++;
	}

	if (start == len){
		return 0;	// no more tokens :(
	}

	// find where this token ends
	int stop = start + 1;
	while(!contains(separators, str[stop]) && (stop < len)){
		stop++;
	}

	// create just enough space for the token
	int token_len = stop - start;
	char *token = (char *) malloc(sizeof(char)*(token_len + 1));

	// loop back through and fill new string
	int i;
	for (i = 0; i < token_len; i++){
		token[i] = str[start++];
	}

	token[token_len] = '\0';

	tk->index = stop;	// so that next function call will pick up we left off

	return token;
}

/*
 * Prints the given string into the terminal, converting any special characters
 * into their corresponding hexadecimal representations.
 */
void special_print(char *str)
{
	char specials[] = { '\n', '\t', '\v', '\b', '\r', '\f', '\a', '\\', '\"' };
	char *hex[] = { "0x0a", "0x09", "0x0b", "0x08", "0x0d", "0x0c", "0x07", "0x5c", "0x22" };

	int len = strlen(str);

	int i;
	for (i = 0; i < len; i++){
		if (contains(specials, str[i])){
			// print the bracketed hex representation of the special character
			int index = 0;
			while(specials[index] != str[i]){
				index++;
			}
			printf("[%s]", hex[index]);
		} else {
			// just print the character
			printf("%c", str[i]);
		}
	}
}


/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
int main(int argc, char **argv)
{
	if (argc != 3){
		printf("Usage: tokenizer <separators> <str>\n");
		return 1;
	}

	// printf("%s\n", str);		test unescape function

	TokenizerT *tokenizer = TKCreate(argv[1], argv[2]);	// create the Tokenizer
	char *token = TKGetNextToken(tokenizer);

	while (token != 0){
		special_print(token);
		printf("\n");
		free(token);
		token = TKGetNextToken(tokenizer);
	}

	TKDestroy(tokenizer);

	return 0;
}
