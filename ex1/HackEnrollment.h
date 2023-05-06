#pragma once
#ifndef HACK_ENROLLMENT_H
#define HACK_ENROLLMENT_H

#define FRIENDSHIP_THRESHOLD 20
#define RIVALRY_THRESHOLD 0

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*The student should have a friendWithHacker end a rivalWithHacker arrays
* containing the ids of the hackers that are his friends and rivals respectively.
*/
typedef struct Student_t {
	char* name;
	int id;
	int* hackerFriends;
	int* hackerRivals;
} *Student;

typedef struct Course_t {
	int courseNumber;
	int size;
} *Course;

typedef struct Hacker_t {
	int id;
	int* courseNumbers;
	int* friendsIds;
	int* rivalsIds;	
} *Hacker;

typedef struct Queue_t{
	int courseNumber;
	int* studentsIds;
} *Queue;

typedef struct EnrollmentSystem_t {
	Student* studentsArray;
	Course* coursesArray;
	Hacker* hackersArray;
	Queue* queuesArray;
} *EnrollmentSystem;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
void hackEnrollment(EnrollmentSystem sys, FILE* out);


#endif /*HACK_ENROLLMENT_H*/

