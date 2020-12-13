#ifndef TEXT_H
#define TEXT_H
#include <stdlib.h>

struct text
{
  size_t capacity;
  size_t size;
  char* data;
};

struct text *newtext();
void freetext(struct text *result);
void double_capacity(struct text *result);
void text_push(struct text *result, char letter);


#endif
