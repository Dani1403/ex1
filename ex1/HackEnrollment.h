#pragma once
#ifndef HACK_ENROLLMENT_H
#define HACK_ENROLLMENT_H

#define FRIENDSHIP_THRESHOLD 20
#define RIVALRY_THRESHOLD 0

#include <stdio.h>
#include <stdlib.h>

typedef struct Student_t {
	char* name;
	int id;
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

typedef struct EnrollmentSystem_t {
	Student* studentsArray;
	Course* coursesArray;
	Hacker* hackersArray;
}*EnrollmentSystem;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
void hackEnrollment(EnrollmentSystem sys, FILE* out);


#endif /*HACK_ENROLLMENT_H*/

