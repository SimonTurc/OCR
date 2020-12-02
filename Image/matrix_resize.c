#include <stddef.h>

void matrix_resize(double input[], size_t rows, size_t cols, double output[])
{
  size_t output_size = 28;

  if(rows > output_size && cols > output_size)
    {
      size_t y0 = (rows - output_size) / 2;
      size_t x0 = (cols - output_size) / 2;
      for (size_t x = 0; x < output_size; x++)
	{
	  for(size_t y = 0; y < output_size; y++)
	    {
                output[y * output_size + x] = input[(y+y0) * cols + x+x0];
	    }
	}
    }
  else if(rows <= output_size && cols > output_size)
    {
      size_t y0 = (output_size - rows) / 2;
      size_t x0 = (cols - output_size) / 2;
      for (size_t x = 0; x < output_size; x++)
	{
	  for(size_t y = 0; y < rows; y++)
	    {
                output[(y+y0) * output_size + x] = input[y * cols + x+x0];
	    }
	}
    }
  else if(rows > output_size && cols <= output_size)
    {
      size_t y0 = (rows - output_size) / 2;
      size_t x0 = (output_size - cols) / 2;
      for (size_t x = 0; x < cols; x++)
	{
	  for(size_t y = 0; y < output_size; y++)
	    {
                output[y * output_size + x+x0] = input[(y+y0) * cols + x];
	    }
	}
    }
  else
    {
      size_t y0 = (output_size - rows) / 2;
      size_t x0 = (output_size - cols) / 2;
      for (size_t x = 0; x < cols; x++)
	{
	  for(size_t y = 0; y < rows; y++)
	    {
                output[(y+y0) * output_size + x+x0] = input[y * cols + x];
	    }
	}
    }
}
