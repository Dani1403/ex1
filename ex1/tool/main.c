#include "ex1/IsraeliQueue.h"
#include "HackEnrollment.h"
#include <stdlib.h>	

int main(int argc, char** argv)
{
	printf("sdd");
	FILE* students = fopen(argv[2], "r");
	FILE* courses = fopen(argv[3], "r");
	FILE* hackers = fopen(argv[4], "r");
	FILE* queues = fopen(argv[5], "r");
	FILE* out = fopen(argv[6], "w");
	EnrollmentSystem sys = createEnrollment(students, courses, hackers);
	sys = readEnrollment(sys, queues);
	hackEnrollment(sys, out);
	fclose(students);
	fclose(courses);
	fclose(hackers);
	fclose(queues);
	fclose(out);
	return 0;
}