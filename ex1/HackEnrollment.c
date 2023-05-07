#include "HackEnrollment.h"
#include "../ex1/ex1/IsraeliQueue.h"
#include "ComparisonAndFriendship.h"

IsraeliQueue enqueueHackersInIsraeliQueue(IsraeliQueue israeliQueue, Course course, Hacker* hackerArray, Student* studentArray);
IsraeliQueue enqueueStudentsInIsraeliQueue(IsraeliQueue israeliQueue, Student* studentArr, Queue queue);


int Line_counter(FILE* file){
    if(!file) return 0;
    int i = 0;
    char current;
    while((current = fgetc (file)) != EOF){
        if(current == '\n'){
            i++;
        }
    }
    return i;
}

Student* readStudents(FILE* students) {
	Student* studentsArray = malloc(sizeof(Student) * Line_counter(students));

    if (!studentsArray) return NULL;

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
	Course* coursesArray = malloc(sizeof(Course) * Line_counter(courses));

    if (!coursesArray) return NULL;

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
	Hacker* hackersArray = malloc(sizeof(Hacker) * (Line_counter(hackers) % 4));

    if (!hackersArray) return NULL;

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
	addFriendshipWithHacker(sys->studentsArray, sys->hackersArray);
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
	Hacker* hackersArray = sys->hackersArray;
	Course* coursesArray = sys->coursesArray;
	Queue* queuesArray = sys->queuesArray;
	Student* studentsArray = sys->studentsArray;
	FriendshipFunction* friendshipFunctions = createFrenshipFunctions(hackersArray);
	int course = 0;
	while (coursesArray[course])
	{
		Queue queue = findQueueCorresponding(queuesArray, coursesArray[course]->courseNumber);
		IsraeliQueue newQueue = IsraeliQueueCreate(friendshipFunctions, comparisonFunction, FRIENDSHIP_THRESHOLD, RIVALRY_THRESHOLD);
		newQueue = enqueueStudentsInIsraeliQueue(newQueue, studentsArray, queue);
		newQueue = enqueueHackersInIsraeliQueue(newQueue, coursesArray[course], hackersArray, studentsArray);
	}
	fprintf(out, "cannot satisfy constraints");
	return;
}

Queue findQueueCorresponding(Queue* queuesArray, int course)
{
	int i = 0;
	while (queuesArray[i])
	{
		if (queuesArray[i]->courseNumber == course)
		{
			return queuesArray[i];
		}
	}
	return NULL;
}


//Find the student that are in the actual queue and enqueues them in the new israeliQueue
IsraeliQueue enqueueStudentsInIsraeliQueue(IsraeliQueue israeliQueue, Student* studentArr, Queue queue)
{
	int id = 0;
	while (queue->studentsIds[id])
	{
		int student = 0;
		while (studentArr[student])
		{
			if (studentArr[student]->id == queue->studentsIds[id])
			{
				IsraeliQueue err = IsraeliQueueEnqueue(israeliQueue, studentArr[student]);
				if (err != ISRAELIQUEUE_SUCCESS)
				{
					return NULL;
				}
			}
			student++;
		}
		id++;
	}
	return israeliQueue;
}

//find the hackers that want the course and enqueues them in the israeliqueue corresponding to the course
IsraeliQueue enqueueHackersInIsraeliQueue(IsraeliQueue israeliQueue, Course course, Hacker* hackerArray, Student* studentArray)
{
	int courseNumber = course->courseNumber;
	int hacker = 0;
	while (hackerArray[hacker])
	{
		int askedCourse = 0;
		while (hackerArray[hacker]->courseNumbers[askedCourse])
		{
			if (courseNumber == hackerArray[hacker]->courseNumbers[askedCourse])
			{
				int student = 0;
				while (studentArray[student])
				{
					if (studentArray[student]->id == hackerArray[hacker]->id)
					{
						IsraeliQueueError err = IsraeliQueueEnqueue(israeliQueue, studentArray[student]);
						if (err != ISRAELIQUEUE_SUCCESS)
						{
							return NULL;
						}
					}
				}
			}
		}
	}
	return israeliQueue;
}