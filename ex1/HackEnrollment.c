#include "HackEnrollment.h"
#include "ex1/IsraeliQueue.h"
#include "ComparisonAndFriendship.h"


IsraeliQueue enqueueHackersInIsraeliQueue(IsraeliQueue israeliQueue, Course course, Hacker* hackerArray, Student* studentArray);
IsraeliQueue enqueueStudentsInIsraeliQueue(IsraeliQueue israeliQueue, Student* studentArr, Queue queue);

void initArray(void** arr, int length) {
    int i = 0;
    while (i < length + 1) {
        arr[i] = NULL;
        i++;
    }
}

void initStudent(Student s) {
    s->firstName = 0;
    s->lastName = 0;
    s->city = 0;
    s->department = 0;
}

void freeArray(void** arr, int currIndex) {
    while (currIndex >= 0) {
        free(arr[currIndex]);
        currIndex--;
    }
    free(arr);
}

int nbOfLinesInFile(FILE* f) {
    if (!f) {
        return -1;
    }
    int counter = 0;
    char c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            counter++;
        }
    }
    return counter;
}

void destroyHacker(void* hacker) {
    if (hacker == NULL) {
        return;
    }
    //destroy lists of friend et enemy
    free(hacker);
}

void destroyStudent(Student student) {
    if (student == NULL) {
        return;
    }
    free(student->hackerFriends);
    free(student->hackerRivals);
    free(student);
}

void deleteStudentArray(Student* studentArr, int index) {
    int i = 0;
    while (i < index) {
        free(studentArr[i]->firstName);
        free(studentArr[i]->lastName);
        free(studentArr[i]->city);
        free(studentArr[i]->department);
        free(studentArr[i]);
        i++;
    }
    free(studentArr);
}

//reads a string and returns the first word before a space or the end of the string
char* getWord(char* line) {
    char* word = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (!word) {
        return NULL;
    }
    int i = 0;
    while (word[i] != ' ' && word[i] != '\0') {
        i++;
    }
    word[i] = '\0';
    return word;
}


bool isEmpty(FILE* file) {
    if (file == NULL) return true;
    long pos = ftell(file);
    bool isEmpty = fgetc(file) == EOF;
    fseek(file, pos, SEEK_SET);
    return isEmpty;
}

char* readLine(FILE* file) {
    if (!file) {
        return NULL;
    }
    long pos = ftell(file);
    int counter = 0;
    char c;
    do {
        c = fgetc(file);
        counter++;
    } while (c != '\n' && c != EOF);

    fseek(file, pos, SEEK_SET);
    char* line = (char*)malloc(sizeof(char) * (counter + 1));
    if (!line) {
        return NULL;
    }
    char* temp = fgets(line, counter + 1, file);
    if (temp == NULL) {
        free(line);
        return NULL;
    }
    line[counter - 1] = 0;
    line[counter] = 0;
    return line;
}


bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isGoodId(const char* str) {
    int i = 0;
    while (isDigit(str[i])) {
        i++;
    }
    if (i != IDLEN) {
        return false;
    }
    return true;
}


int parseInt(char* str) {
    int result = 0;
    int i = 0;
    while (isDigit(str[i])) {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

float parseFloat(char* str) {
    float result = 0;
    int i = 0;
    while (isDigit(str[i])) {
        result = result * 10 + (float)(str[i] - '0');
        i++;
    }
    if (str[i] == '.') {
        i++;
        float factor = (float)0.1;
        while (isDigit(str[i])) {
            result += (float)(str[i] - '0') * factor;
            factor /= 10;
            i++;
        }
    }
    return result;
}

char* readString(char* str, int* i) {
    int start = *i;
    while (str[*i] != ' ' && str[*i] != '\n') {
        (*i)++;
    }
    char* result = malloc(*i - start + 1);
    strncpy(result, &str[start], *i - start);
    result[*i - start] = '\0';
    return result;
}





////////////////////////////studentEnrollement///////////////////////////////////////////////////

Student* createStudentFromLine(char* line) {
    Student student = (Student)malloc(sizeof(Student));

    char* token = strtok(line, " ");
    if (token != NULL) {
        student->id = strtol(token, NULL, 10);
    }

    token = strtok(NULL, " ");
    if (token != NULL) {
        student->totalCredits = strtol(token, NULL, 10);
    }

    token = strtok(NULL, " ");
    if (token != NULL) {
        student->gpa = strtof(token, NULL);
    }

    token = strtok(NULL, " ");
    if (token != NULL) {
        student->firstName = strdup(token);
    }

    token = strtok(NULL, " ");
    if (token != NULL) {
        student->lastName = strdup(token);
    }

    token = strtok(NULL, " ");
    if (token != NULL) {
        student->city = strdup(token);
    }

    token = strtok(NULL, "\n");
    if (token != NULL) {
        student->department = strdup(token);
    }

    return student;
}

Student* studentEnrollment(FILE* students, int linesInStudentFile) {
    Student* arrayOfPtrStudent = malloc((linesInStudentFile) * sizeof(Student));
    if (!arrayOfPtrStudent) {
        return NULL;
    }
    initArray((void**)arrayOfPtrStudent, linesInStudentFile);
    int i = 0;
    while (!isEmpty(students)) {
        char* line = readLine(students);
        arrayOfPtrStudent[i] = malloc(sizeof(Student));
        initStudent(arrayOfPtrStudent[i]);
        if (!arrayOfPtrStudent[i]) {
            freeArray((void**)arrayOfPtrStudent, i);
            return NULL;
        }
        arrayOfPtrStudent[i] = createStudentFromLine(line);
        i++;
    }
    return arrayOfPtrStudent;
}
////////////////////////////hackerEnrollement///////////////////////////////////////////////////


int* readIntArray(char* str, int* length) {
    int start = 0;
    int count = 0;
    while (str[start] != '\n') {
        if (isDigit(str[start])) {
            count++;
        }
        start++;
    }
    int* result = malloc(count * sizeof(int));
    int i = 0;
    while (str[*length] != '\n') {
        if (isDigit(str[*length])) {
            result[i] = parseInt(str + *length);
            i++;
            while (isDigit(str[*length])) {
                (*length)++;
            }
        }
        (*length)++;
    }
    (*length)++; // passer le '\n'
    return result;
}

Hacker* createHackerFromLine(char* line) {

    int id = parseInt(line);
    if (id < MINID || id > MAXID) {
        return NULL;
    }

    // Lire les autres champs
    int i = IDLEN + 1; // index après l'ID et le '\n'
    int* desiredCourses = readIntArray(line + i, &i);
    int* friendsId = readIntArray(line + i, &i);
    int* enemiesId = readIntArray(line + i, &i);

    // Créer l'objet Hacker
    Hacker hacker = malloc(sizeof(Hacker));
    hacker->id = id;
    hacker->courseNumbers = desiredCourses;
    hacker->friendsIds = friendsId;
    hacker->rivalsIds= enemiesId;

    return hacker;
}

Hacker* hackerEnrollment(FILE* hackers, int numOfStudents) {
    // Créer le tableau de pointeurs de Hacker
    Hacker* hackerArray = malloc(numOfStudents * sizeof(Hacker));
    int i = 0;

    // Lire chaque ligne du fichier et créer un objet Hacker correspondant
    char line[BUFFER];
    while (fgets(line, BUFFER, hackers)) {
        Hacker hacker = createHackerFromLine(line);
        //est ce quon passe bien a l'autre
        if (hacker != NULL) {
            hackerArray[i] = hacker;
            i++;
        }
    }
    return hackerArray;
}

//////////////////////////////courseEnrollement///////////////////////////////////////


Course* createCourseFromLine(char* line) {
    if (strlen(line) < 3) {
        return NULL;
    }
    int i = 0;
    Course course = malloc(sizeof(Course));
    course->courseNumber = parseInt(line + i);
    while (line[i] != ' ') {
        i++;
    }
    i++;
    course->size = parseInt(line + i);
    return course;
}

Course* courseEnrollment(FILE* courses, int linesInCourseFile) {
    Course* courseArray = malloc(linesInCourseFile * sizeof(Course));
    if (!courseArray) {
        return NULL;
    }
    initArray((void**)courseArray, linesInCourseFile);
    int i = 0;
    char line[BUFFER];
    while (fgets(line, BUFFER, courses)) {
        Course course = createCourseFromLine(line);
        if (course != NULL) {
            courseArray[i] = course;
            i++;
        }
    }

    return courseArray;
}

////////////////////////////////createEnrollement///////////////////////////////////////



EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers) {
    if (students == NULL || courses == NULL || hackers == NULL) {
        return NULL;
    }
    EnrollmentSystem system = malloc(sizeof(*system));
    if (!system) {
        return NULL;
    }

    system->studentsArray = studentEnrollment(students, nbOfLinesInFile(students));
    if (!system->studentsArray) {
        free(system);
        return NULL;
    }

    system->coursesArray = courseEnrollment(courses, nbOfLinesInFile(courses));
    if (!system->coursesArray) {
        // free student
        free(system);
        return NULL;
    }

    system->hackersArray = hackerEnrollment(hackers, nbOfLinesInFile(hackers));
    if (!system->hackersArray) {
        //free students
        freeArray((void**)system->coursesArray, nbOfLinesInFile(courses) - 1);
        free(system);
        return NULL;
    }
    return system;
}


/////////////////////////////////////////readEnrollment////////////////////////////////////////////////////////

Queue createQueueFromLine(char* line) {
    int i = 0;
    Queue queue = malloc(sizeof(*queue));
    queue->courseNumber = parseInt(line + i);
    i++;
    queue->studentsIds = readIntArray(line + i, &i);
}
    

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues) {
    int linesInQueue = nbOfLinesInFile(queues);
    if (linesInQueue < 0) {
        return NULL;
    }
    if (linesInQueue == 0) {
        return sys;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, queues)) {
        Queue queue = createQueueFromLine(line);
    }
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
