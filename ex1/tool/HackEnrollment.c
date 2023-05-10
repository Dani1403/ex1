#include "HackEnrollment.h"

//helper functions
int getSizeOfArray(int* array);
int absolute(int num);
char* strdup(const char* str);

IsraeliQueue enqueueHackersInIsraeliQueue(IsraeliQueue israeliQueue, Course course, Hacker* hackerArray, Student* studentArray);
IsraeliQueue enqueueStudentsInIsraeliQueue(IsraeliQueue israeliQueue, Student* studentArr, Queue queue);

// Comparison And Friendship Functions
int nameDistance(void* s1, void* s2);
int idDistance(void* s1, void* s2);
void addFriendshipWithHacker(Student* students, Hacker* hackers);
bool checkFriendshipFromArray(Student student1, Student student2);
bool checkRivalryFromArray(Student student1, Student student2);
int friendOrRivalWithHacker(void* s1, void* s2);
int comparisonFunction(void* s1, void* s2);

char* strdup(const char* str)
{
    char* copy = malloc(strlen(str) + 1);
    if (copy != NULL) {
         strcpy(copy, str);
    }
    return copy;
}

int absolute(int num)
{
    return num < 0 ? -num : num;
}

int getSizeOfArray(int* array)
{
    int size = 0;
    while (array[size])
    {
        size++;
    }
    return size;
}

int nameDistance(void* s1, void* s2)
{
    Student student1 = (Student)s1;
    Student student2 = (Student)s2;
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

int idDistance(void* s1, void* s2)
{
    Student student1 = (Student)s1;
    Student student2 = (Student)s2;
    int id1 = student1->id;
    int id2 = student2->id;
    return abs(id1 - id2);
}

// update the fields of the students if they are friends with a hacker
void addFriendshipWithHacker(Student* students, Hacker* hackers)
{
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

int friendOrRivalWithHacker(void* s1, void* s2)
{
    Student student1 = (Student)s1;
    Student student2 = (Student)s2;
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

int comparisonFunction(void* s1, void* s2)
{
    Student student1 = (Student)s1;
    Student student2 = (Student)s2;
    return student1->id - student2->id;
}

///////Free functions

void freeHackerArray(Hacker* hackerArr)
{
    int hacker = 0;
    while (hackerArr[hacker])
    {
        free(hackerArr[hacker]->courseNumbers);
        free(hackerArr[hacker]->friendsIds);
        free(hackerArr[hacker]->rivalsIds);
        free(hackerArr[hacker]);
    }
    free(hackerArr);
}

void freeStudentArray(Student* studentArr)
{
    int student = 0;
    while (studentArr[student])
    {
        free(studentArr[student]->firstName);
        free(studentArr[student]->lastName);
        free(studentArr[student]->city);
        free(studentArr[student]->department);
        free(studentArr[student]->hackerFriends);
        free(studentArr[student]->hackerRivals);
        free(studentArr[student]);
        student++;
    }
    free(studentArr);
}

void freeCourseArray(Course* courseArr)
{
    int course = 0;
    while (courseArr[course])
    {
        free(courseArr[course]);
    }
    free(courseArr);
}

void freeQueueArray(Queue* queueArr)
{
    int queue = 0;
    while (queueArr[queue])
    {
        free(queueArr[queue]->studentsIds);
        free(queueArr[queue]);
    }
    free(queueArr);
}

void freeEnrollmentSystem(EnrollmentSystem sys)
{
    freeStudentArray(sys->studentsArray);
    freeHackerArray(sys->hackersArray);
    freeCourseArray(sys->coursesArray);
    freeQueueArray(sys->queuesArray);
    free(sys);
}

void freeArray(void** array, int currentIndex)
{
    while (currentIndex >= 0)
    {
        free(array[currentIndex]);
        currentIndex--;
    }
    free(array);
}

////////////////////////// Create Enrollment 

//Create Array of Students from file
Student* readStudentsFromFile(FILE* studentFile)
{
    int numOfStudents = 0;
    Student* studentArray = NULL;
    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), studentFile))
    {
        char firstName[LINE_SIZE], lastName[LINE_SIZE], city[LINE_SIZE], department[LINE_SIZE];
        int id, totalCredits, gpa;
        sscanf(line, "%d %d %d %255s %255s %255s %255s", &id, &totalCredits, &gpa, firstName, lastName, city, department);
        Student student = malloc(sizeof(Student));
        student->firstName = strdup(firstName);
        student->lastName = strdup(lastName);
        student->id = id;
        student->totalCredits = totalCredits;
        student->gpa = gpa;
        student->city = strdup(city);
        student->department = strdup(department);
        student->hackerFriends = NULL;
        student->hackerRivals = NULL;
        numOfStudents++;
        studentArray = realloc(studentArray, numOfStudents * sizeof(Student));
        studentArray[numOfStudents - 1] = student;
    }
    return studentArray;
}

//Create Array of Hackers from file

Hacker* readHackersFromFile(FILE* hackerFile)
{
    int numOfHackers = 0;
    Hacker* hackerArray = NULL;
    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), hackerFile))
    {
        return -1;
    }
    int counter = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            counter++;
        }
    }
    return counter;
}


//returns the first word of a line
char* getWord(char* line)
{
    char* word = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (!word)
    {
        return NULL;
    }
    int i = 0;
    while (word[i] != ' ' && word[i] != '\0')
    {
        i++;
    }
    word[i] = '\0';
    return word;
}

bool isEmpty(FILE* file)
{
    if (file == NULL)
        return true;
    long pos = ftell(file);
    bool isEmpty = fgetc(file) == EOF;
    fseek(file, pos, SEEK_SET);
    return isEmpty;
}

char* readLine(FILE* file)
{
    if (!file)
    {
        return NULL;
    }
    long pos = ftell(file);
    int counter = 0;
    char c;
    do
    {
        c = fgetc(file);
        counter++;
    } while (c != '\n' && c != EOF);

    fseek(file, pos, SEEK_SET);
    char* line = (char*)malloc(sizeof(char) * (counter + 1));
    if (!line)
    {
        return NULL;
    }
    char* temp = fgets(line, counter + 1, file);
    if (temp == NULL)
    {
        free(line);
        return NULL;
    }
    line[counter - 1] = 0;
    line[counter] = 0;
    return line;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isGoodId(const char* str)
{
    int i = 0;
    while (isDigit(str[i]))
    {
        i++;
    }
    if (i != IDLEN)
    {
        return false;
    }
    return true;
}

int parseInt(char* str)
{
    int result = 0;
    int i = 0;
    while (isDigit(str[i]))
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

float parseFloat(char* str)
{
    float result = 0;
    int i = 0;
    while (isDigit(str[i]))
    {
        result = result * 10 + (float)(str[i] - '0');
        i++;
    }
    if (str[i] == '.')
    {
        i++;
        float factor = (float)0.1;
        while (isDigit(str[i]))
        {
            result += (float)(str[i] - '0') * factor;
            factor /= 10;
            i++;
        }
    }
    return result;
}

char* readString(char* str, int* i)
{
    int start = *i;
    while (str[*i] != ' ' && str[*i] != '\n')
    {
        (*i)++;
    }
    char* result = malloc(*i - start + 1);
    strncpy(result, &str[start], *i - start);
    result[*i - start] = '\0';
    return result;
}

Student* createStudentFromLine(char* line)
{
    Student student = (Student)malloc(sizeof(Student));

    char* token = strtok(line, " ");
    if (token != NULL)
    {
        student->id = strtol(token, NULL, 10);
    }

    token = strtok(NULL, " ");
    if (token != NULL)
    {
        student->totalCredits = strtol(token, NULL, 10);
    }

    token = strtok(NULL, " ");
    if (token != NULL)
    {
        student->gpa = strtof(token, NULL);
    }

    token = strtok(NULL, " ");
    if (token != NULL)
    {
        student->firstName = strdup(token);
    }

    token = strtok(NULL, " ");
    if (token != NULL)
    {
        student->lastName = strdup(token);
    }

    token = strtok(NULL, " ");
    if (token != NULL)
    {
        student->city = strdup(token);
    }

    token = strtok(NULL, "\n");
    if (token != NULL)
    {
        student->department = strdup(token);
    }

    return student;
}

Student* studentEnrollment(FILE* students, int linesInStudentFile)
{
    Student* arrayOfPtrStudent = malloc((linesInStudentFile) * sizeof(Student));
    if (!arrayOfPtrStudent)
    {
        return NULL;
    }
    initArray((void**)arrayOfPtrStudent, linesInStudentFile);
    int i = 0;
    while (!isEmpty(students))
    {
        char* line = readLine(students);
        arrayOfPtrStudent[i] = malloc(sizeof(Student));
        initStudent(arrayOfPtrStudent[i]);
        if (!arrayOfPtrStudent[i])
        {
            freeArray((void**)arrayOfPtrStudent, i);
            return NULL;
        }
        arrayOfPtrStudent[i] = createStudentFromLine(line);
        i++;
    }
    return arrayOfPtrStudent;
}

int* readIntArray(char* str, int* length)
{
    int start = 0;
    int count = 0;
    while (str[start] != '\n')
    {
        if (isDigit(str[start]))
        {
            count++;
        }
        start++;
    }
    int* result = malloc(count * sizeof(int));
    int i = 0;
    while (str[*length] != '\n')
    {
        if (isDigit(str[*length]))
        {
            result[i] = parseInt(str + *length);
            i++;
            while (isDigit(str[*length]))
            {
                (*length)++;
            }
        }
        (*length)++;
    }
    (*length)++;
    return result;
}

Hacker* createHackerFromLine(char* line)
{

    int id = parseInt(line);
    if (id < MINID || id > MAXID)
    {
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
    hacker->rivalsIds = enemiesId;

    return hacker;
}

Hacker* hackerEnrollment(FILE* hackers, int numOfStudents)
{
    // Créer le tableau de pointeurs de Hacker
    Hacker* hackerArray = malloc(numOfStudents * sizeof(Hacker));
    int i = 0;

    // Lire chaque ligne du fichier et créer un objet Hacker correspondant
    char line[BUFFER];
    while (fgets(line, BUFFER, hackers))
    {
        Hacker hacker = createHackerFromLine(line);
        // est ce quon passe bien a l'autre
        if (hacker != NULL)
        {
            hackerArray[i] = hacker;
            i++;
        }
    }
    return hackerArray;
}

//create Array of Courses from file

Course* readCoursesFromFile(FILE* coursesFile)
{
    int numOfCourses = 0;
    Course* courseArray = NULL;
    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), coursesFile))
    {
        int courseNum, sizeOfCourse;
        sscanf(line, "%d %d", &courseNum, &sizeOfCourse);
        Course course = malloc(sizeof(Course));
        course->courseNumber = courseNum;
        course->size = sizeOfCourse;
        numOfCourses++;
        courseArray = realloc(courseArray, numOfCourses * sizeof(Course));
        courseArray[numOfCourses - 1] = course;
    }
    return courseArray;
}

////////////////////////////////createEnrollement///////////////////////////////////////

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers, FILE* queues)
{
    if (students == NULL || courses == NULL || hackers == NULL || queues == NULL)
    {
        return NULL;
    }

    EnrollmentSystem sys = malloc(sizeof(*sys));
    if (!sys)
    {
        return NULL;
    }
    system->studentsArray = readStudentsFromFile(students);
    if (!system->studentsArray)
    {
        freeStudentArray(sys->studentsArray);
        free(sys);
        return NULL;
    }
    system->coursesArray = readCoursesFromFile(courses);
    if (!system->coursesArray)
    {
        freeStudentArray(sys->studentsArray);
        freeCourseArray(sys->coursesArray);
        free(sys);
        return NULL;
    }
    system->hackersArray = readHackersFromFile(hackers);
    if (!system->hackersArray)
    {
        freeEnrollmentSystem(sys);
        return NULL;
    }

    return sys;
}


/////////////////////////////////////////readEnrollment////////////////////////////////////////////////////////
Queue* readQueuesFromFile(FILE* queuesFile)
{
    int numQueues = 0;
    Queue* queues = NULL;
    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), queuesFile))
    {
        int courseNumber;
        sscanf(line, "%d", &courseNumber);
        int numStudents = 0;
        int* studentIds = NULL;
        char* token = strtok(line, " ");
        while (token != NULL)
        {
            if (numStudents == 0)
            {
                numStudents++;
            }
            else
            {
                numStudents++;
                studentIds = realloc(studentIds, numStudents * sizeof(int));
                studentIds[numStudents - 2] = atoi(token);
            }
            token = strtok(NULL, " ");
        }
        Queue queue = malloc(sizeof(struct Queue_t));
        queue->courseNumber = courseNumber;
        queue->studentsIds = studentIds;
        numQueues++;
        queues = realloc(queues, numQueues * sizeof(Queue));
        queues[numQueues - 1] = queue;
    }
    return queues;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    Queue* queueArray = readQueuesFromFile(queues);
    sys->queuesArray = queueArray;
    return sys;
}

//////////hack enrollment

Queue findQueueCorresponding(Queue* queuesArray, int course)
{
    if (queuesArray == NULL || course < 0)
    {
        return NULL;
    }
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

Student findStudentFromId(Student* studentArray, int id) {
    if (id == 0 || studentArray == NULL) {
        return NULL;
    }
    int student = 0;
    while (studentArray[student]) {
        if ((studentArray[student]->id) == id) {
            return studentArray[student];
        }
        student++;
    }
    return NULL;
}
Course findCourseCorresponding(Course* coursesArray, int courseNum)
{
    if (coursesArray == NULL || courseNum < 0)
    {
        return NULL;
    }
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

// Find the student that are in the actual queue and enqueues them in the new israeliQueue
IsraeliQueue enqueueStudentsInIsraeliQueue(IsraeliQueue israeliQueue, Student* studentArr, Queue queue)
{
    int id = 0;
    while (queue->studentsIds[id])
    {
        Student studentToEnqueue = findStudentFromId(studentArr, queue->studentsIds[id]);
        IsraeliQueueError enqueue = IsraeliQueueEnqueue(israeliQueue, studentToEnqueue);
        if (enqueue != ISRAELIQUEUE_SUCCESS)
        {
            return NULL;
        }
        id++;
    }
    return israeliQueue;
}

// find the hackers that want the course and enqueues them in the israeliqueue corresponding to the course
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
                Student hackerToEnqueue = findStudentFromId(studentArray, (hackerArray[hacker])->id);
                if (IsraeliQueueContains(israeliQueue, hackerToEnqueue))
                {
                    break;
                }
                IsraeliQueueError enqueue = IsraeliQueueEnqueue(israeliQueue, hackerToEnqueue);
                if (enqueue != ISRAELIQUEUE_SUCCESS)
                {
                    return NULL;
                }
            }
            askedCourse++;
        }
        hacker++;
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
        fputc('\n', out);
    }
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    Hacker* hackersArray = sys->hackersArray;
    Course* coursesArray = sys->coursesArray;
    Queue* queuesArray = sys->queuesArray;
    Student* studentsArray = sys->studentsArray;
    addFriendshipWithHacker(studentsArray, hackersArray);
    int course = 0;
    while (coursesArray[course])
    {
        Queue queue = findQueueCorresponding(queuesArray, coursesArray[course]->courseNumber);
        IsraeliQueue newQueue = IsraeliQueueCreate(NULL, comparisonFunction, FRIENDSHIP_THRESHOLD, RIVALRY_THRESHOLD);
        IsraeliQueueAddFriendshipMeasure(newQueue, &nameDistance);
        IsraeliQueueAddFriendshipMeasure(newQueue, &idDistance);
        IsraeliQueueAddFriendshipMeasure(newQueue, &friendOrRivalWithHacker);
        newQueue = enqueueStudentsInIsraeliQueue(newQueue, studentsArray, queue);
        newQueue = enqueueHackersInIsraeliQueue(newQueue, coursesArray[course], hackersArray, studentsArray);
        queue = updateFromIsraeli(newQueue, queue);
        IsraeliQueueDestroy(newQueue);
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
            if (getHackerPosition(hackersArray[hacker], queue) > course->size)
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
