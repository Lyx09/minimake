#ifndef COMMON_H
#define COMMON_H

#define RC_ERROR    0x2
#define RC_SUCCESS  0x0

#define TRUE    0x1
#define FALSE   0x0

int chr_in_str(const char *s, const char c);

void trim(char *s, const char* trim_char);

#endif /* ! COMMON_H */
