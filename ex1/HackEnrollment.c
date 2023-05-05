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
	printf("cannot satisy constraints");
	return;
}

/*
* reads the enrollment system and returns an array of israeli queues corresponding
*/
IsraeliQueue* newQueuesArray(EnrollmentSystem sys) {
	Queue* queuesArray = sys->queuesArray;
	int numberOfCourses = 0;
	while (queuesArray[numberOfCourses])
	{
		numberOfCourses++;
	}
	IsraeliQueue* israeliQueues = malloc(sizeof(IsraeliQueue) * numberOfCourses);
	for (int i = 0; i < numberOfCourses; i++)
	{
		israeliQueues[i] = IsraeliQueueCreate();
		IsraeliQueueError improvedQueue = IsraeliQueueImprovePositions(israeliQueues[i]);
		if (improvedQueue != ISRAELIQUEUE_SUCCESS)
		{
			return NULL;
		}
	}
}

FriendshipFunction* createFrenshipFunctions(Hacker* hackersArray) {
	FriendshipFunction* friendshipFunctions = malloc(3 * sizeof(FriendshipFunction));
}

int friendWithHacker(int studentId, Hacker* hackersArray) {
	int hacker = 0;
	while (hackersArray[hacker])
	{
		int hackerId = hackersArray[hacker]->id;
		int friend = 0, rival = 0;
		while (hackersArray[hacker]->friendsIds[friend])
		{
			if (hackersArray[hacker]->friendsIds[friend] == studentId)
			{
				return FRIENDSHIP_THRESHOLD;
			}
		}
		while (hackersArray[hacker]->rivalsIds[rival])
		{
			if (hackersArray[hacker]->rivalsIds[rival] == studentId)
			{
				return -FRIENDSHIP_THRESHOLD;
			}
		}
	}
}

int abs(int num)
{
	return num < 0 ? -num : num;
}

int nameDistance(char* name1, char* name2) {
	int sum1 = 0, sum2 = 0, i = 0;
	while (name1[i])
	{
		sum1 += name1[i];
		i++;
	}
	i = 0;
	while (name2[i])
	{
		sum2 += name2[i];
		i++;
	}
	return abs(sum1 - sum2);
}

int idDistance(int id1, int id2) {
	return abs(id1 - id2);
}