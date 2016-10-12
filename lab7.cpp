#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

using namespace std;
class signal{
	private:
		int file;
		int numEl;
		double* array;
		double mean;
		double maxVal;
	public:
		
};
void helpFunction(void);

int main (int argc,char* argv[])
{
	int offsetYes = 0;//these are the truth values that are set while reading the command line arguments
	int scaleYes = 0;
	int statsYes = 0;
	int normalYes = 0;
	int centerYes = 0;
	int renameYes = 0;
	int fileNo;
	int i =1;
	double offsetVal;
	double scaleVal;
	char* newNameString;
	cout << "Num arguments: " << argc << endl;
	if (argc == 1)
	{
		cout << "cant do anything, exiting" << endl;
		return 1;
	}
	while(i < argc)//this loops steps through each command line argument and checks its syntax
			//please note the truth value integers being set once a certain tag has been found
	{
		if((argv[i][0] == '-') && (argv[i][1] == 'h'))//checks to see if the help menu is needed, terminates program immediately
		{
			helpFunction();
			return 1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'n'))//file number tag
		{
			if(i >= (argc-1))//ensures we dont run off the end of the argv[] array
			{
				printf("WARNING: Please be sure to append a file number\n");
			}
			else
			{
				i++;
				if(argv[i][0] == '-')//checks to see if they forgot to append a number before the next tag
				{
					printf("WARNING: Need a value appended after tag -n\n");
					i--;
				}
				if(atoi(argv[i]) < 1 || atoi(argv[i]) > 11)//range for valid files
				{
					printf("WARNING: You did not enter a valid file number with your -n tag, program must terminate\n");
					return 1;//will seg fault if we try to open a data file that dont exist
				}
				else
				{
					fileNo = atoi(argv[i]);
					printf("You want to work with file %d\n",fileNo);
				}
			}
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'o'))
		{
			if(i >= (argc-1))//ensures we dont run off the end of the argv[] array
			{
				printf("WARNING: Did not provide an offset value\n");
			}
			else
			{
				i++;
				if(argv[i][0] == '-')
				{
					printf("WARNING: Need a value appended after tag -o\n");
					i--;
				}
				else
				{
					offsetVal = (double)atof(argv[i]);//atof() because the offset value can be an decimal number
					printf("You want to offset the data by %lf\n",offsetVal);
				}
			}
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 's'))
		{
			if(i >= (argc-1))//ensures we dont run off the end of the argv[] array
			{
				printf("WARNING: did not provide an scale value\n");
			}
			else
			{
				i++;
				if(argv[i][0] == '-')
				{
					printf("WARNING: Need a value appended after tag -s\n");
					i--;
				}
				else
				{
				scaleVal = (double)atof(argv[i]);
				printf("You want to scale the data by %lf\n",scaleVal);
				scaleYes=1;
				}
			}
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'C'))//these tags do not require a value or input, and requires less error checking for the argument
		{
			printf("You want to center the data\n");
			centerYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'N'))\
		{
			printf("You want to normalize the data\n");
			normalYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'S'))
		{
			printf("You want to gather the statistics of our data\n");
			statsYes=1;
		}
		else if((argv[i][0] == '-') && (argv[i][1] == 'r'))
		{
			if(i >= (argc-1))
			{
				printf("WARNING: Did not provide a file name after -r tag\n");
				i--;
			}
			else
			{
				i++;//gets the new name of the file that the user wants to write
				newNameString = argv[i];
				printf("You want to change the name of a file to %s\n",newNameString);
				renameYes=1;
			}
		}
		else	//this error check basically says that the program did not find a tag, simply bumps past the argument and gives the user a warning
		{
		printf("WARNING: Your argument:\n %s\n has no valid context within the scope of the program, this error will be ignored and the program will continue\n",argv[i]);
		}
		i++;
	}
	return 1;
}
void helpFunction(void)//void function to print out a help menu
{
	printf("WELCOME!\nHere in lab5.c, we want to make it easy to access and manipulate the information located within these select files!\nTo execute this program correctly, run ./lab4 along with any arguments appended\nEXAMPLE:./lab4 -n 3 -o 5.3\nThe tags must be proceeded by a valid value in which to perform the operation.\n\n\n-n <file_choice>\tSelects the file in which you want to work with, note valid files range from 1 to 11\n-o <offset_value>\tOffsets each value in the file that you have selected, by the offset value proceeding the tag\n-s <scale_value>\tScales each value in the file that you have selected, by the scaling value proceeding the tag\n-S\t\t\tProvides a new file in which you will find the mean value of the data file you have selected, as well as the maximum value present in the file\n-C\t\t\tCenters the signal in the file you have chosen and writes an output file\n-N\t\t\tNormalizes the signal in the file you have chosen and writes an output file\n-r <new_file_name>\tRenames the current file you have chosen, be sure to proceed the tag with the NEW desired name of the file\n-h\t\t\tDisplays this help menu\n\n\n");
	return;
}
	
