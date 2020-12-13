#include <err.h>
#include "text.h"


struct text *newtext()
{
  struct text *result = malloc(sizeof(struct text));
  if(result == NULL)
    errx(1,"Not enough memory");
  result->capacity = 66;
  result ->size = 0;
  result->data = malloc(sizeof(char)*result->capacity);
  if(result->data == NULL)
    errx(1, "Not enough memory");
  return result;
}


void freetext(struct text *result)
{
  free(result->data);
  free(result);
}


void double_capacity(struct text *result)
{
  result->capacity = result->capacity*2;
  result->data = realloc(result->data, result->capacity*sizeof(char));
}


void text_push(struct text *result, char letter)
{
  if(result->size == result->capacity)
    double_capacity(result);
  result->data[result->size] = letter;
  result->size += 1;
}
