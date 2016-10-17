#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

using namespace std;
void helpFunction(void);


class signal{
	private:
		int file;
		int numEl;
		double* array;//pointer to data
		double mean;
		double maxVal;
		double meanFinder(int,double*);//private method for finding the mean (can only be accessed by objects of this class)
		void save_signal(const char*);
		void offset(double);
		void scale(double);
		void normalize(void);
		void center(void);
		void display(void);
	public:
		void workWithData(signal);//implemented this method because signal 1 will be created differently for different scenarios
		signal (void);//default constructor
		signal (int);//constructor for a file number being inputted 
		signal (const char*);//constructor for the file name
		~signal(void);//destructor
		
};
signal::~signal()
{	
	delete array;//deletes the array which i allocated in the constructors 
}
signal::signal(void)
{
	int fileNo;
	int i =0;
	char fileString[50];
	cout <<"NOTE:In the current constructor, ONLY the raw data files may be accessed\nYou did not provide a file name to work with or a file number, please enter a number 1 - 14\n" << endl;
	cin >> fileNo;
	while(fileNo < 1 || fileNo > 14)
	{
		cout << "Try that again, user..\n" << endl;
		cin >> fileNo;
	}
	if(fileNo > 10) sprintf(fileString,"Raw_data_%d.txt",fileNo);
	else sprintf(fileString,"Raw_data_0%d.txt",fileNo);
	FILE* fp = fopen(fileString,"r");
	if(fp == NULL)
	{
		cout << "Could not open the file, terminating\n" << endl;
		return;
	}
	fscanf(fp,"%d %lf",&numEl,&maxVal);
	array = new double[numEl];
	double* arrStart = array;
	while(i < numEl)
	{
		int temp;
		fscanf(fp,"%d\n",&temp);
		*array = temp;
		array++;
		i++;
	}
	array = arrStart;
	mean = meanFinder(numEl,array);
	array = arrStart;
}
signal::signal(const char* fileName)
{
	char fileString[50];
	int i = 0;
	sprintf(fileString,"%s",fileName);
	FILE* fp = fopen(fileString,"r");
	if(fp == NULL)
	{
		cout << "cant open file, terminating" << endl;
		return;
	}
	fscanf(fp,"%d %lf",&numEl,&maxVal);
	array = new double[numEl];
	double* arrStart = array;
	while(i < numEl)
	{
		int temp;
		fscanf(fp,"%d\n",&temp);
		*array = temp;
		array++;
		i++;
	}
	array = arrStart;
	mean = meanFinder(numEl,array);
	array = arrStart;
}
signal::signal(int fileNo)
{
	cout << "NOTE:In the current constructor, ONLY the raw data files may be accessed\n" << endl;
	char fileString[50];
	int i =0;
	if(fileNo > 10) sprintf(fileString,"Raw_data_%d.txt",fileNo);
	else sprintf(fileString,"Raw_data_0%d.txt",fileNo);
	FILE* fp = fopen(fileString,"r");
	if(fp == NULL)
	{
		cout << "cant open file, terminating" << endl;
		return	;
	}
	fscanf(fp,"%d %lf",&numEl,&maxVal);
	array = new double[numEl];
	double* arrStart = array;
	while(i < numEl)
	{
		int temp;
		fscanf(fp,"%d\n",&temp);
		*array = temp;
		array++;
		i++;
	}
	array = arrStart;
	mean = meanFinder(numEl,array);
	array = arrStart;
}
int main (int argc,char* argv[])
{
	int explicitFile = 0;//this is a truth value 
	int fileNo;
	int i =1;
	char* newNameString;
	char* explicitFileName;
	if (argc == 1)
	{
		signal sig1;//default constructor being called
		//sig1.workWithData(sig1);
		//return 1;
	}
	else
	{
		while(i < argc)//this loops steps through each command line argument and checks its syntax
				//please note the truth value integers being set once a certain tag has been found
		{
			if((argv[i][0] == '-') && (argv[i][1] == 'h'))//checks to see if the help menu is needed, terminates program immediately
			{	
				helpFunction();
				return 1;
			}
			else if((argv[i][0] == '-') && (argv[i][1] == 'f'))
			{
				if(i >= (argc-1))
				{
					printf("WARNING: Please be sure to append a file name");
				}
				else
				{
					i++;
					if(argv[i][0] == '-')//checks to see if they forgot to append a number before the next tag
					{
						printf("WARNING: Need a value appended after tag -f\n");
						i--;
					}
					explicitFileName = argv[i];
					explicitFile = 1;
				}
			}	
			else if((argv[i][0] == '-') && (argv[i][1] == 'n'))//file number tag
			{
				if(i >= (argc-1))//ensures we dont run off the end of the argv[] array
				{
					printf("WARNING: Please be sure to append a file number\n");
				}
				else if(explicitFile ==1)
				{
					printf("WARNING: You cannot explicitly open two files at once, please dont try that again\n");
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
			else	//this error check basically says that the program did not find a tag, simply bumps past the argument and gives the user a warning
			{
				printf("WARNING: Your argument:\n %s\n has no valid context within the scope of the program, this error will be ignored and the program will continue\n",argv[i]);
			}
			i++;
		}
	}
	if(explicitFile ==1)
	{
		signal sig1(explicitFileName);
		//sig1.workWithData(sig1);
		//return 1;
	}
	else
	{
		signal sig1(fileNo);
		//sig1.workWithData(sig1);
		//return 1;
	}	
	int userInput =1;//****code from workWithData begins here 
	double offsetVal;
	double scaleVal;
	char saveFileString[50];
	cout << "Alrighty there user!!! We actually got your file open (believe it or not)\nWhat do you want to do with it now? there are several things to choose from" << endl;
	while(userInput != 0)
	{
		cout << "\n\n0: Exit the program\n1: Scale the data\n2: Offset the data\n3: Normalize the data\n4: Center the data\n5: Save the current signal to a file\n6: Display information on the current signal\n" <<endl;
		cin >> userInput;
		switch(userInput)
		{
			case 0:
				return 1;
				break;
			case 1: 
				cout<<"Enter a scale value" <<endl;
				cin>>scaleVal;
				cout<<"Scaling data..."<<endl;
				sig1.scale(scaleVal);//this is where i get the compile error. somehow sig1 is out of scope??? 
				break;
			case 2:
				cout<<"Enter an offset value"<<endl;
				cin>>offsetVal;
				cout<<"Offsetting data..."<<endl;
				sig1.offset(offsetVal);//this will cause the compile error if we omit the scale call........ and so on.......
				break;
			case 3: 
				cout<<"Normalizing data..."<<endl;
				sig1.normalize();
				break;
			case 4:
				cout<<"Centering data..."<<endl;
				sig1.center();
				break;
			case 5:
				cout << "where do you want to save this signal?" << endl;
				cin >> saveFileString;
				cout<<"Saving signal..."<<endl;
				sig1.save_signal(saveFileString);
				break;
			case 6: 
				sig1.display();
				break;
			default:
				break;
		}
	}//****ends here 
	return 1;
}
//void signal::workWithData(signal sig1)//this is where that block of code used to be, however i DO NOT want to have to use the workwithData method, i dont understand this error 
//{
	
//}
void signal::save_signal(const char* fileName)
{
	double* start = array;
	int i =1;
	FILE* fp = fopen(fileName,"w");
	fprintf(fp,"%d\t%lf\n",numEl,maxVal);
	while(i <= numEl)
	{
		fprintf(fp,"%lf\n",*array);
		array++;
		i++;
	}
	array = start;
	fclose(fp);
}
void signal::center(void)
{
	int i = 1;
	double* start = array;
	while(i < numEl)
	{
		*array = *array - mean;
		array++;
		i++;
	}
	maxVal = maxVal - mean;
	array = start;
	mean = meanFinder(numEl,array);
	array = start;
}
void signal::normalize(void)
{
	int i =1;
	double* start = array;
	while(i < numEl)
	{
		*array = *array / maxVal;
		array++;
		i++;
	}
	maxVal = maxVal / maxVal;
	array = start;
	mean = meanFinder(numEl,array);
	array = start;
}
void signal::offset(double offsetVal)
{
	int i = 1;
	double* start = array;
	maxVal = maxVal + offsetVal;
	while(i < numEl)
	{
		*array = *array + offsetVal;
		array++;
		i++;
	}
	array = start;
	mean = meanFinder(numEl,array);
	array = start;
}
void signal::scale(double scaleVal)
{
	int i = 1;
	double* start = array;
	maxVal =  maxVal * scaleVal;
	while(i < numEl)
	{
		*array = *array * scaleVal;
		array++;
		i++;
	}
	array = start;
	mean = meanFinder(numEl,array);
	array = start;
}
void signal::display(void)
{
	cout << "Num of elements is currently: " << numEl << "\nMax value is currently: " << maxVal << "\nMean is currently: " << mean << endl;
}	
void helpFunction(void)//void function to print out a help menu
{ 
	printf("WELCOME!\nHere in lab5.c, we want to make it easy to access and manipulate the information located within these select files!\nTo execute this program correctly, run ./lab4 along with any arguments appended\nEXAMPLE:./lab4 -n 3 -o 5.3\nThe tags must be proceeded by a valid value in which to perform the operation.\n\n\n-n <file_choice>\tSelects the file in which you want to work with, note valid files range from 1 to 11\n-o <offset_value>\tOffsets each value in the file that you have selected, by the offset value proceeding the tag\n-s <scale_value>\tScales each value in the file that you have selected, by the scaling value proceeding the tag\n-S\t\t\tProvides a new file in which you will find the mean value of the data file you have selected, as well as the maximum value present in the file\n-C\t\t\tCenters the signal in the file you have chosen and writes an output file\n-N\t\t\tNormalizes the signal in the file you have chosen and writes an output file\n-r <new_file_name>\tRenames the current file you have chosen, be sure to proceed the tag with the NEW desired name of the file\n-h\t\t\tDisplays this help menu\n\n\n");
	return;
}
double signal::meanFinder(int numEl,double* array)
{
	double mean;
	int i =1;
	double total;
	while(i < numEl)
	{
		total = total + *array;
		array++;
		i++;
	}
	mean = total / numEl;	
	return mean;
}
	
