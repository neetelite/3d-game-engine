bool
char_is_eol(char c)
{
    bool result = ((c == '\n') || (c == '\r'));
    return(result);
}

bool
char_is_whitespace(char c)
{
    bool result = ((c == ' ') || (c == '\t') ||
                  (c == '\v') || (c == '\f') ||
                  char_is_eol(c));

    return(result);
}

char *
char_skip_whitespace(char *c)
{
	while(char_is_whitespace(*c)) ++c;
	return(c);
}
