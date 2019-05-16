#include <stdio.h>
#define BUFFER_SIZE 512
int main(int argc, char *argv[])
{
    //checking for the arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover fileName\n");
        return 1;
    }

    char *infile = argv[1];
    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    unsigned char buffer[BUFFER_SIZE];
    FILE *img = NULL;

    int jpg_found = 0, file_count = 0;

    while((fread(buffer, BUFFER_SIZE, 1,inptr)) == 1)
    {
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if(jpg_found == 1)
                //found previously opened jpeg
                fclose(img);
            else
                jpg_found = 1;


            char filename[10];
            sprintf(filename, "%03d.jpg",file_count);
            img = fopen(filename, "a");
            file_count++;

        }
        if(jpg_found == 1)
        {
            fwrite(&buffer, BUFFER_SIZE, 1, img);
        }
    }
    fclose(img);
    fclose(inptr);
}
