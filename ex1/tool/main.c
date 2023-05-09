#include "../IsraeliQueue.h"
#include "../Node.h"
#include "HackEnrollment.h"
#include <stdlib.h>	

void hackEnrollmentProgram(char* students, char* courses, char* hackers, char* queues, char* out)
{
	FILE* studentsFile = fopen(students, "r");
	FILE* coursesFile = fopen(courses, "r");
	FILE* hackersFile = fopen(hackers, "r");
	FILE* queuesFile = fopen(queues, "r");
	FILE* outFile = fopen(out, "w");
	EnrollmentSystem sys = createEnrollment(studentsFile, coursesFile, hackersFile);
	sys = readEnrollment(sys, queuesFile);
	hackEnrollment(sys, outFile);
	fclose(studentsFile);
	fclose(coursesFile);
	fclose(hackersFile);
	fclose(queuesFile);
	fclose(outFile);
}	

int main(int argc, char** argv)
{
	char* students = NULL;
	char* courses = NULL;
	char* hackers = NULL;
	char* queues = NULL;
	char* out = NULL;
	if (argc == 7)
	{
		students = argv[2];
		courses = argv[3];
		hackers = argv[4];
		queues = argv[5];
		out = argv[6];
	}
	else 
	{
		students = argv[1];
		courses = argv[2];
		hackers = argv[3];
		queues = argv[4];
		out = argv[5];
	}
	hackEnrollmentProgram(students, courses, hackers, queues, out);
	return 0;
}