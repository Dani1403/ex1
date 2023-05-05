#include "HackEnrollment.h"
#include "../ex1/ex1/IsraeliQueue.h"

//TODO : change because the file is not read correctly
Student* readStudents(FILE* students) {
	Student* studentsArray = malloc(sizeof(Student) * 100);
	char* name;
	int id;
	int i = 0;
	while (fscanf(students, "%s %d", &name, &id) != EOF) {
		studentsArray[i] = malloc(sizeof(Student));
		studentsArray[i]->name = name;
		studentsArray[i]->id = id;
		i++;
	}
	return studentsArray;
}

Course* readCourses(FILE* courses) {
	Course* coursesArray = malloc(sizeof(Course) * 100);
	if (!coursesArray)
	{
		return NULL;
	}
	int courseNumber, size;
	int i = 0;
	while (fscanf(courses, "%d %d", &courseNumber, &size) != EOF) {
		coursesArray[i] = malloc(sizeof(Course));
		coursesArray[i]->courseNumber = courseNumber;
		coursesArray[i]->size = size;
		i++;
	}
	return coursesArray;
}

Hacker* readHackers(FILE* hackers) {
	Hacker* hackersArray = malloc(sizeof(Hacker) * 100);
	int id, courseNumber, friendsIds, rivalsIds;
	int i = 0;
	while (fscanf(hackers, "%d %d %d %d", &id, &courseNumber, &friendsIds, &rivalsIds) != EOF) {
		hackersArray[i] = malloc(sizeof(Hacker));
		hackersArray[i]->id = id;
		hackersArray[i]->courseNumbers = courseNumber;
		hackersArray[i]->friendsIds = friendsIds;
		hackersArray[i]->rivalsIds = rivalsIds;
		i++;
	}
	return hackersArray;
}



EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
	EnrollmentSystem sys = malloc(sizeof(sys));
	sys->coursesArray = readCourses(courses);
	sys->studentsArray = readStudents(students);
	sys->hackersArray = readHackers(hackers);
	return sys;
}



EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
	Queue* queuesArray = malloc(sizeof(Queue) * 100);
	int courseNumber, studentsIds;
	int i = 0;
	//TODO : change because the file is not read correctly
	while (fscanf(queues, "%d %d", &courseNumber, &studentsIds) != EOF) {
		queuesArray[i] = malloc(sizeof(Queue));
		queuesArray[i]->courseNumber = courseNumber;
		queuesArray[i]->studentsIds = studentsIds;
		i++;
	}
	sys->queuesArray = queuesArray;
	return sys;
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
	//write function body 
	return;
}
