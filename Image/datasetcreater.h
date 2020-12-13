#ifndef DATASETCREATER_H
#define DATASETCREATER_H

void createdataset(double *matrix)
{
    FILE *file = fopen("datasetbis.h", "a+");
    if (file == NULL) 
    {
        printf("Error!");
        exit(1);
    }
    fprintf(file,"{");
    for (int j=0; j<784; j++) 
    {
        if (j == 783)
        {
            fprintf(file,"%1.f", matrix[j]);
        }
        else
        {
            fprintf(file,"%1.f,", matrix[j]);
        }
        if ((j+1) % 28 == 0 && j <783) 
            fprintf(file,"\n");
    }
    fprintf(file,"},\n");
    fclose(file);

}

void shift_left(double *matrix)
{
    for (size_t i = 0; i < 784; i++)
    {
        matrix[(i-3) % 28 + (28*(i/28))] = matrix[i];
    }
}

void shift_right(double *matrix)
{
    for (size_t i = 783 ; i>0; i--)
    {
        matrix[(i+3) % 28 + (28*(i/28))] = matrix[i];
    }
}

double *shift_up(double *matrix)
{
    double *tmp = matrix;
    for (size_t i = 0; i < 784; i++)
    {
        tmp[(i -84)%784] = matrix[i];
    }
    return tmp;
}

double *shift_down(double *matrix)
{
    double *tmp = matrix;
    for (size_t i = 783 ; i>0; i--)
    {
        tmp[ (i +84)%784] = matrix[i];
    }
    return tmp;
}

#endif
