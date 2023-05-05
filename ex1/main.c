#include "HackEnrollment.h"
#include "../ex1/ex1/IsraeliQueue.h"

int main(int* argc, int** argv)
{
	FILE* students = fopen("students.txt", "r");
	FILE* courses = fopen("courses.txt", "r");
	FILE* hackers = fopen("hackers.txt", "r");
	FILE* queues = fopen("queues.txt", "r");
	FILE* out = fopen("out.txt", "w");
	EnrollmentSystem sys = createEnrollment(students, courses, hackers);
	sys = readEnrollment(sys, queues);
	hackEnrollment(sys, out);
	return 0;
}