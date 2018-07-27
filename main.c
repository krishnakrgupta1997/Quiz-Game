#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct Question{
	char category[5];
	int QuesID;
	char question[128];
	char option1[128], option2[128], option3[128], option4[128];
	char answer;
};

int choice, AllotedMarks=0;
char name[25],ansArray[10];
int randomNumbersHash[30]={0};
char StartQuiz();
int TotalMarks=0, cat1Marks=0, cat2Marks=0, cat3Marks=0;
 
 /**
 * gcc implementation for getch() without echo
 */
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
}
 
void menuController(){
	system("clear");
	printf("Welcome to Simple Quiz Game\n");
	printf("\nYour Options\n");
	printf("============");
	printf("\n\t1) Start The Quiz\n");
	printf("\n\t2) Exit\n");
	scanf("%d",&choice);
	if(choice == 2)exit(0);
	char c=StartQuiz();
	if(c == 'N') menuController();
}
char StartQuiz(){
	system("clear");                               //If running in Windows OS, substitute "clear" with "cls".
	printf("Welcome to Simple Quiz Game\n");
	printf("\nPlayer Name (max.25 characters) :\n");
	scanf("%s",name);
	char c;
	printf("\nReady (Y/N) : ");
	scanf(" %c",&c);
	if(islower(c))
		c=toupper(c);
	return c;
}
void GenerateRandom(){
	int i;
	srand(time(0));
	for (i = 0; i < 10; i++) {
		int num = rand() %29;		   		
		while(randomNumbersHash[num] == 1){
			num = rand() % 29;
		}
		randomNumbersHash[num]= 1;
	}
}
void Quiz(char* path,int ID){

	system("clear");
	Question question;
	FILE *fp;
	    fp = fopen (path,"r");
	    if (fp!=NULL)
	    {
		//scanning into struct object
		fgets(question.question, 128, fp);
		fgets(question.option1, 128, fp);
		fgets(question.option2, 128, fp);
		fgets(question.option3, 128, fp);
		fgets(question.option4, 128, fp);
		fgets(question.category, 5, fp);
		fscanf(fp, " %c", &question.answer);
		//fscanf(fp, "%d", &QuesID);
		fclose(fp);
	    }
		 // Removing carriage return and new line from strings
	    for (int i = 0; i < 128; i++) {
		if(question.question[i] == 13 || question.question[i] == 10) question.question[i] = 0;
		if(question.option4[i] == 13 || question.option4[i] == 10) question.option4[i] = 0;
		if(question.option3[i] == 13 || question.option3[i] == 10) question.option3[i] = 0;
		if(question.option2[i] == 13 || question.option2[i] == 10) question.option2[i] = 0;
		if(question.option1[i] == 13 || question.option1[i] == 10) question.option1[i] = 0;
		if(question.category[i] == 13 || question.category[i] == 10) question.category[i] = 0;
	    }
	    char ans,lock;
	    printf("Question ID: %d\n", ID);
	    printf("Question Category: %s\n", question.category);
	    printf("Question Text: \n%s\n", question.question);
	    printf("\nAvailable Answers:\n");
	    printf("%s\n", question.option1);
	    printf("%s\n", question.option2);
	    printf("%s\n", question.option3);
	    printf("%s\n", question.option4);
	    printf("\nMarks Alloted: 1\n");
	    printf("\nEnter the correct answer: ");
	    scanf(" %c",&ans);
	    printf("\nLock it(Y/N): ");
	    scanf(" %c",&lock);
	    if(islower(lock)) lock=toupper(lock);
	    if(lock == 'N')
	    	Quiz(path,ID);
	    if(islower(ans)) ans=toupper(ans);
	    ansArray[ID-1] = ans;
	    char cat1[5]="cat1", cat2[5]= "cat2", cat3[5]= "cat3";
	    if(ans == question.answer) {
	    	TotalMarks += 1;
	    	/*if(question.category == cat1) cat1Marks++;
	    	else if(question.category == cat2) cat2Marks++;	
	    	else if(question.category == cat3) cat3Marks++;*/
	    	int cmp;
	    	cmp= strcmp(question.category,"cat1");
	    	if(cmp==0)cat1Marks++;
	    	else{
	    		cmp= strcmp(question.category,"cat1");
	    		if(cmp==0)cat1Marks++;
	    		else{
	    			cmp= strcmp(question.category,"cat2");
	    			if(cmp==0)cat2Marks++;
	    			else{
	    				cmp= strcmp(question.category,"cat3");
	    				if(cmp==0)cat3Marks++;
	    			}
	    		}
	    	} 
	    }	
	    
}
void RunQuiz(){
	system("clear");
	
	int i,TotalMarks=0;
	int number=-1,j;
	char path[32];
	GenerateRandom();
	for(j=0;j<10;j++){
		for(i=number+1;i<30;i++){
			if(randomNumbersHash[i] == 1){
				number=i;
				break;
			}	
		}	
		snprintf(path, 32, "questions/%d.txt", number+1);
	
	
	//reading from file
	Quiz(path,j+1);    
	   
    }
}
void DisplayResult(){
	Question question;
	system("clear");
	printf("Congratulations %s. You have completed the quiz\n",name);
	printf("\nYou have scored %d out of 10",TotalMarks);
	printf("\nHere are your answers ...");
	char path[32];
	int i,j,number=-1;
	for(j=0;j<10;j++){
		for(i=number+1;i<30;i++){
			if(randomNumbersHash[i] == 1){
				number=i;
				break;
			}	
		}	
		snprintf(path, 32, "questions/%d.txt", number+1);
		FILE *fp;
		fp = fopen (path,"r");
		
		if (fp!=NULL)
	    {
		//scanning into struct object
		fgets(question.question, 128, fp);
		fgets(question.option1, 128, fp);
		fgets(question.option2, 128, fp);
		fgets(question.option3, 128, fp);
		fgets(question.option4, 128, fp);
		fgets(question.category, 5, fp);
		fscanf(fp, " %c", &question.answer);
		//fscanf(fp, "%d", &QuesID);
		fclose(fp);
	    }
		 // Removing carriage return and new line from strings
	    for (int i = 0; i < 128; i++) {
		if(question.question[i] == 13 || question.question[i] == 10) question.question[i] = 0;
		if(question.option4[i] == 13 || question.option4[i] == 10) question.option4[i] = 0;
		if(question.option3[i] == 13 || question.option3[i] == 10) question.option3[i] = 0;
		if(question.option2[i] == 13 || question.option2[i] == 10) question.option2[i] = 0;
		if(question.option1[i] == 13 || question.option1[i] == 10) question.option1[i] = 0;
		if(question.category[i] == 13 || question.category[i] == 10) question.category[i] = 0;
	    }
		
		printf("\nQuestion Category: %s",question.category);
		printf("\nQuestion Number: %d", j+1);
		printf("\n%s\n",question.question);
		printf("\nCorrect Answer: %c\n",question.answer);
		printf("\nYour Answer: %c\n",ansArray[j]);
		int tmp=0;
		if(question.answer == ansArray[j]) tmp=1;
		printf("\nMarks Awarded: %d\n\n",tmp);
		
	}
	printf("\nCategory Wise Marks ...\n");
	printf("\nCat1  :  %d",cat1Marks);
	printf("\nCat2  :  %d",cat2Marks);
	printf("\nCat3  :  %d\n\n",cat3Marks);
	printf("\n Your total final score: %d out of 100\n\n",TotalMarks*10);
	
	printf("\nPress any key to go to Main Menu ..");
	getch();
}

int main(){
	//printf(ANSI_COLOR_RED);
	while(1){
		AllotedMarks=0;
		TotalMarks=0; cat1Marks=0; cat2Marks=0; cat3Marks=0;
		for(int i=0;i<30;i++){
			randomNumbersHash[i]=0;
		}
		system("clear");
		char c;
		menuController();
		RunQuiz();
		DisplayResult();
	}
}

