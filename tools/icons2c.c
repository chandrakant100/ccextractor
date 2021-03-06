#include <stdio.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>

#define MAX_FILE_PATH 500

/*
	CCExtractor Binary to C Array Converter
	First Compile the C program to an executable (say icons2c)
    Usage: icons2c DIRECTORY [ > OUTPUTFILE]
    icons2c creates C arrays from binary files.
	Use this tool to generate the C arrays for icons and other GUI resources to be used in src/GUI/ccextractorGUI.c
	C arrays generated by this tool should be placed in src/GUI/icon_data.c
	Copy the code from the output file obtained by running this tool and paste (or replace) in the above mentioned file
	Check the following example to know how to generate C array for new resource(s).
    Example:
    icons2c ../icon > temp.c               		# Creates 'temp.c' from the contents of files in ../icon directory
*/

int main(int argc, char** argv) {
    assert(argc == 2);
    char* directory = argv[1];
	
	DIR *d;
	FILE *f;
    struct dirent *dir;
	printf("//Scanning directory \"%s\" for resources.\n\n", directory);
    d = opendir(directory);
    if (d)
    {
		int count = 0;
        while ((dir = readdir(d)) != NULL)
        {
            char* fn = dir->d_name;
			if (dir->d_type == DT_REG) 
			{
				char file[MAX_FILE_PATH];
				count++;
				printf("//Converting file: %s\n", fn);
				strcpy(file, directory);
				strcat(file, "/");
				strcat(file, fn);
				f = fopen(file, "rb");
				char *name = strtok(fn, ".");
				printf("char %s_icon_data[] = {\n", name);
				unsigned long n = 0;
				while(!feof(f)) {
					unsigned char c;
					if(fread(&c, 1, 1, f) == 0) break;
					printf("0x%.2X,", (int)c);
					++n;
					if(n % 10 == 0) printf("\n");
				}
				fclose(f);
				printf("};\n\n");
			}
        }
		closedir(d);
		printf("//Total %d icons converted successfully.\n",count);
    }
	else 
	{
		printf("//Sorry, Given directory couldn\'t be opened\n");
	}
}