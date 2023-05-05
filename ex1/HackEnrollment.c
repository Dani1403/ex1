#include "HackEnrollment.h"
#include "../ex1/ex1/IsraeliQueue.h"

//TODO : change because the file is not read correctly
Student* readStudents(FILE* students) {
	Student* studentsArray = malloc(sizeof(Student) * 100);
	char* studentName;
	int id;
	int i = 0;
	while (fscanf_s(students, "%s %d", &studentName, &id) != EOF) {
		studentsArray[i] = malloc(sizeof(Student));
		studentsArray[i]->name = studentName;
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
	int courseNum, size;
	int i = 0;
	while (fscanf_s(courses, "%d %d", &courseNum, &size) != EOF) {
		coursesArray[i] = malloc(sizeof(Course));
		coursesArray[i]->courseNumber = courseNum;
		coursesArray[i]->size = size;
		i++;
	}
	return coursesArray;
}

Hacker* readHackers(FILE* hackers) {
	Hacker* hackersArray = malloc(sizeof(Hacker) * 100);
	int hackerId, courseNumber, friendsIds, rivalsIds;
	int i = 0;
	while (fscanf_s(hackers, "%d %d %d %d", &hackerId, &courseNumber, &friendsIds, &rivalsIds) != EOF) {
		hackersArray[i] = malloc(sizeof(Hacker));
		hackersArray[i]->id = hackerId;
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
	int courseNum, studentsIds;
	int i = 0;
	//TODO : change because the file is not read correctly
	while (fscanf_s(queues, "%d %d", &courseNum, &studentsIds) != EOF) {
		queuesArray[i] = malloc(sizeof(Queue));
		queuesArray[i]->courseNumber = courseNum;
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
