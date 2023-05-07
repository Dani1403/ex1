#pragma once
#ifndef COMPARISON_AND_FRIENDSHIP
#define COMPARISON_AND_FRIENDSHIP

#include "HackEnrollment.h"
#include "ex1/IsraeliQueue.h"


int getSizeOfArray(int* array);
int absolute(int num);
int nameDistance(Student student1, Student student2);
int idDistance(Student student1, Student student2);
void addFriendshipWithHacker(Student* students, Hacker* hackers);
bool checkFriendshipFromArray(Student student1, Student student2);
bool checkRivalryFromArray(Student student1, Student student2);
int friendOrRivalWithHacker(Student student1, Student student2);
FriendshipFunction* createFrenshipFunctions(Hacker* hackersArray);
int comparisonFunction(Student student1, Student student2);

#endif //COMPARISON_AND_FRIENDSHIP
