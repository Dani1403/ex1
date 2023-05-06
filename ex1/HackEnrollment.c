#include "HackEnrollment.h"
#include "../ex1/ex1/IsraeliQueue.h"

int getSizeOfArray(int* array);
int abs(int num);
void addFriendshipWithHacker(Student* students, Hacker* hackers);
bool checkFriendshipFromArray(Student student1, Student student2);
bool checkRivalryFromArray(Student student1, Student student2);
FriendshipFunction* createFrenshipFunctions(Hacker* hackersArray);
int comparisonFunction(Student student1, Student student2);
IsraeliQueue* newQueuesArray(EnrollmentSystem sys);


int Line_counter(FILE* file){
    if(!file) return -1;
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
    //TO DO: quel message renvoyer si Line_counter renvoie -1 ???(cad fichier vide)

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
    //TO DO: quel message renvoyer si Line_counter renvoie -1 ???(cad fichier vide)

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
	Hacker* hackersArray = malloc(sizeof(Hacker) * Line_counter(hackers));
    //Verifier avec Daniel si cette technique peut marcher pour le fichier hackers

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
	printf("cannot satisfy constraints");
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
	FriendshipFunction* friendshipFunctions = createFrenshipFunctions(sys->hackersArray);
	for (int i = 0; i < numberOfCourses; i++)
	{
		israeliQueues[i] = IsraeliQueueCreate(friendshipFunctions, comparisonFunction, FRIENDSHIP_THRESHOLD, RIVALRY_THRESHOLD);
	}
	return israeliQueues;
}

FriendshipFunction* createFrenshipFunctions(Hacker* hackersArray) {
	FriendshipFunction* friendshipFunctions = malloc(4 * sizeof(FriendshipFunction));
	friendshipFunctions[0] = &friendOrRivalWithHacker;
	friendshipFunctions[1] = &nameDistance;
	friendshipFunctions[2] = &idDistance;
	friendshipFunctions[3] = NULL;
	return friendshipFunctions;
}

int friendOrRivalWithHacker(Student student1, Student student2) {
	if (checkFriendshipFromArray(student1, student2) || checkFriendshipFromArray(student2, student1))
	{
		return FRIENDSHIP_THRESHOLD;
	}
	else if (checkRivalryFromArray(student1, student2) || checkRivalryFromArray(student2, student1))
	{
		return RIVALRY_THRESHOLD;
	}
	return 0;
}

int abs(int num)
{
	return num < 0 ? -num : num;
}

int nameDistance(Student student1, Student student2) {
	char* name1 = student1->name;
	char* name2 = student2->name;
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

int idDistance(Student student1, Student student2) {
	int id1 = student1->id;
	int id2 = student2->id;
	return abs(id1 - id2);
}

//update the fields of the students if they are friends with a hacker
void addFriendshipWithHacker(Student* students, Hacker* hackers) {
	int student = 0;
	while (students[student])
	{
		int studentId = students[student]->id;
		int hacker = 0;
		while (hackers[hacker])
		{
			int hackerFriend = 0;
			while (hackers[hacker]->friendsIds[hackerFriend])
			{
				if (hackers[hacker]->friendsIds[hackerFriend] == studentId)
				{
					int size = getSizeOfArray(students[student]->hackerFriends);
					int* newHackerFriends = malloc(sizeof(int) * (size + 1));
					for (int i = 0; i < size; i++)
					{
						newHackerFriends[i] = students[student]->hackerFriends[i];
					}
					newHackerFriends[size] = hackers[hacker]->id;
					students[student]->hackerFriends = newHackerFriends;
				}
				hackerFriend++;
			}
			int hackerRival = 0;
			while (hackers[hacker]->rivalsIds[hackerRival])
			{
				if (hackers[hacker]->rivalsIds[hackerRival] == studentId)
				{
					int size = getSizeOfArray(students[student]->hackerRivals);
					int* newHackerRivals = malloc(sizeof(int) * (size + 1));
					for (int i = 0; i < size; i++)
					{
						newHackerRivals[i] = students[student]->hackerRivals[i];
					}
					newHackerRivals[size] = hackers[hacker]->id;
					students[student]->hackerRivals = newHackerRivals;
				}
				hackerRival++;
			}
			hacker++;
		}
		student++;
		}
}

int getSizeOfArray(int* array) {
	int size = 0;
	while (array[size])
	{
		size++;
	}
	return size;
}

bool checkFriendshipFromArray(Student student1, Student student2)
{
	int student2Id = student2->id;
	int i = 0;
	while (student1->hackerFriends[i])
	{
		if (student1->hackerFriends[i] == student2Id)
		{
			return true;
		}
		i++;
	}
	return false;
}

bool checkRivalryFromArray(Student student1, Student student2)
{
	int student2Id = student2->id;
	int i = 0;
	while (student1->hackerRivals[i])
	{
		if (student1->hackerRivals[i] == student2Id)
		{
			return true;
		}
		i++;
	}
	return false;
}

int comparisonFunction(Student student1, Student student2)
{
	return student1->id == student2->id;
}