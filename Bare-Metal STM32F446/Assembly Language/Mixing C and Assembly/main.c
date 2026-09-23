// Inline & Assembly Function in a C Program are not used 


//	Calling Assembly Subroutine from a C program
//extern	int sum4(int a, int b, int c, int d);
//static char msg[25] = "1234567890";
//extern int my_strlen(char* s);

//	Accessing C variable in Assembly
int counter;

extern void setCount(int c);
extern int getCount();

void increament();

int main(void)
{
	//	Calling Assembly Subroutine from a C program
//	int result = sum4(1,2,3,4);
	
//	int i = 0;
//	i = my_strlen(msg);
	
	//	Accessing C variable in Assembly
	
	int volatile result = 0;
	
	setCount(1);
	increament();
	result = getCount();

	while(1);
}

//void increament()
//{
//	counter++;
//}
