#include "ComparisonAndFriendship.h"

int absolute(int num)
{
	return num < 0 ? -num : num;
}

int getSizeOfArray(int* array) {
	int size = 0;
	while (array[size])
	{
		size++;
	}
	return size;
}

int nameDistance(Student student1, Student student2) {
	char* name1 = student1->firstName;
	char* name2 = student2->firstName;
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

FriendshipFunction* createFrenshipFunctions(Hacker* hackersArray) {
	FriendshipFunction* friendshipFunctions = malloc(4 * sizeof(FriendshipFunction));
	if (!friendshipFunctions)
	{
		return NULL;
	}
	friendshipFunctions[0] = &friendOrRivalWithHacker;
	friendshipFunctions[1] = &nameDistance;
	friendshipFunctions[2] = &idDistance;
	friendshipFunctions[3] = NULL;
	return friendshipFunctions;
}

int comparisonFunction(Student student1, Student student2)
{
	return student1->id - student2->id;
}