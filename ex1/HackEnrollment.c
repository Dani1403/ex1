#include "HackEnrollment.h"
#include "ex1/IsraeliQueue.h"
#include "ComparisonAndFriendship.h"


IsraeliQueue enqueueHackersInIsraeliQueue(IsraeliQueue israeliQueue, Course course, Hacker* hackerArray, Student* studentArray);
IsraeliQueue enqueueStudentsInIsraeliQueue(IsraeliQueue israeliQueue, Student* studentArr, Queue queue);


int Line_counter(FILE* file)
{
    if(!file) return 0;
    int i = 0;
    char current;
    while((current = fgetc(file)) != EOF){
        if(current == '\n'){
            i++;
        }
    }
    return i;
}

int Queue_counter(FILE* file) 
{
    if (!file) return 0;
    int count = 0;
    int courseNum, studentsIds;

    while (fscanf(file, "%d %d", &courseNum, &studentsIds) == 2) {
        count++;
    }

    return count;
}


Student* readStudents(FILE* students) {
	Student* studentsArray = (Student*)malloc(sizeof(Student) * Line_counter(students));

    if (!studentsArray) return NULL;
	char* firstName;
	char* lastName;
	int id;
	int gpa;
	int points;
	char* city;
	char* department;
	int i = 0;
	while (fscanf(students, "%d %d %d %s %s %s %s", &id, &points, &gpa, firstName, lastName, city, department) != EOF) {
		studentsArray[i] = (Student)malloc(sizeof(Student));
		studentsArray[i]->firstName = firstName;
		studentsArray[i]->lastName = lastName;
		studentsArray[i]->id = id;
		i++;
	}
	return studentsArray;
}

Course* readCourses(FILE* courses) {
	Course* coursesArray = (Course*)malloc(sizeof(Course) * Line_counter(courses));

    if (!coursesArray) return NULL;

	int courseNum, size;
	int i = 0;
	while (fscanf(courses, "%d %d", &courseNum, &size) != EOF) {
		coursesArray[i] = (Course)malloc(sizeof(Course));
		coursesArray[i]->courseNumber = courseNum;
		coursesArray[i]->size = size;
		i++;
	}
	return coursesArray;
}

Hacker* readHackers(FILE* hackers) {
	Hacker* hackersArray = (Hacker*)malloc(sizeof(Hacker) * (Line_counter(hackers) % 4));

    if (!hackersArray) return NULL;

	int hackerId, courseNumber, friendsIds, rivalsIds;
	int i = 0;
	while (fscanf(hackers, "%d %d %d %d", &hackerId, &courseNumber, &friendsIds, &rivalsIds) != EOF) {
		hackersArray[i] = (Hacker)malloc(sizeof(Hacker));
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
	EnrollmentSystem sys = (EnrollmentSystem)malloc(sizeof(sys));
	sys->coursesArray = readCourses(courses);
	sys->studentsArray = readStudents(students);
	sys->hackersArray = readHackers(hackers);
	addFriendshipWithHacker(sys->studentsArray, sys->hackersArray);
	return sys;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
	Queue* queuesArray = (Queue*)malloc(sizeof(Queue) * Queue_counter(queues));
    if (!queuesArray) return NULL;

	int courseNum, studentsIds;
	int i = 0;
	//TODO : change because the file is not read correctly
	while (fscanf(queues, "%d %d", &courseNum, &studentsIds) != EOF) {
		queuesArray[i] = (Queue)malloc(sizeof(Queue));
		queuesArray[i]->courseNumber = courseNum;
		queuesArray[i]->studentsIds = studentsIds;
		i++;
	}
	sys->queuesArray = queuesArray;
	return sys;
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

Course findCourseCorresponding(Course* coursesArray, int courseNum)
{
	int i = 0;
	while (coursesArray[i])
	{
		if (coursesArray[i]->courseNumber == courseNum)
		{
			return coursesArray[i];
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
				IsraeliQueueError err = IsraeliQueueEnqueue(israeliQueue, studentArr[student]);
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

Queue updateFromIsraeli(IsraeliQueue israeliQueue, Queue queue)
{
	int size = IsraeliQueueSize(israeliQueue);
	int* newStudentsIds = malloc(sizeof(int) * size);
	for (int student = 0; student <= size; student++)
	{
		Student foremostStudent = IsraeliQueueDequeue(israeliQueue);
		newStudentsIds[student] = foremostStudent->id;
	}
	queue->studentsIds = newStudentsIds;
	return queue;
}

int getHackerPosition(Hacker hacker, Queue queue)
{
	int hackerId = hacker->id;
	int i = 0;
	while (queue->studentsIds[i])
	{
		if (queue->studentsIds[i] == hackerId)
		{
			return i + 1;
		}
	}
	return getSizeOfArray(queue->studentsIds);
}

void printQueuesInFile(Queue* queuesArray, FILE* out)
{
	int queue = 0;
	while (queuesArray[queue])
	{
		fprintf(out, "%d ", queuesArray[queue]->courseNumber);
		int position = 0;
		while (queuesArray[queue]->studentsIds[position])
		{
			fprintf(out, "%d", queuesArray[queue]->studentsIds[position]);
			position++;
		}
		fputc('/n', out);
	}
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
		queue = updateFromIsraeli(newQueue, queue);
	}
	int hacker = 0;
	bool flag = true;
	while (hackersArray[hacker] && flag)
	{
		int coursesNotReceived = 0;
		int courseNum = 0;
		while (hackersArray[hacker]->courseNumbers[courseNum]) 
		{
			Course course = findCourseCorresponding(coursesArray, hackersArray[hacker]->courseNumbers[courseNum]);
			Queue queue = findQueueCorresponding(queuesArray, course->courseNumber);
			if (getHackerPosition(hacker, queue) > course->size)
			{
				coursesNotReceived++;
				if (coursesNotReceived == getSizeOfArray(hackersArray[hacker]->courseNumbers))
				{
					fprintf(out, "Cannot satisfy constraints for %d", hackersArray[hacker]->id);
					flag = false;
					break;
				}
			} 
			courseNum++;
		}
		hacker++;
	}
	if (flag)
	{
		printQueuesInFile(queuesArray, out);
	}
	return;
}
