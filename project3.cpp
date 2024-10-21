#include <iostream>
#include <string>
using namespace std;

class CPUJob {
    public:
        int job_id; // Unique identifier for the job
        int priority; // Priority level of the job (1-10)
        int job_type; // Job type (1-10)
        int cpu_time_consumed; // Total CPU time consumed by the job
        int memory_consumed; // Total memory consumed thus far

    // My functions
        CPUJob(); // default constructor
        CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed); // constructor that assigns a value to the class fields
        CPUJob(const CPUJob& job); // Copy constructor, makes a deep copy
        ~CPUJob();
        void display(); // displays the job that was removed from the queue
};

// Default constructor
CPUJob:: CPUJob() { // assigning all the fields -1
    job_id = -1;
    priority = -1;
    job_type = -1;
    cpu_time_consumed = -1;
    memory_consumed = -1;
}

// Other constructor
CPUJob:: CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed) {
    (*this).job_id = job_id;
    (*this).priority = priority;
    (*this).job_type = job_type;
    (*this).cpu_time_consumed = cpu_time_consumed;
    (*this).memory_consumed = memory_consumed;
}

// Copy constructor, creates a deep copy so that the information is copied and the memory address is not the same
// Individually assigns all of the fields of job to the this's jobPointer 
CPUJob:: CPUJob(const CPUJob& job) {
    job_id = job.job_id;
    priority = job.priority;
    job_type = job.job_type;
    cpu_time_consumed = job.cpu_time_consumed;
    memory_consumed = job.memory_consumed;
}

// Decosntructor
CPUJob:: ~CPUJob() {
    delete this;
}

// Prints and displays all of the fields of the CPUJob pointer variable
void CPUJob:: display() {
     cout << "Job ID: " << job_id 
          << ", Priority: " << priority 
          << ", Job Type: " << job_type 
          << ", CPU Time Consumed: " << cpu_time_consumed 
          << ", Memory Consumed: " << memory_consumed << endl;
}

template <class DT>
class Queue {
    public:
        DT* JobPointer; // Pointer to a job (e.g., CPUJob)
        Queue<DT>* next; // Pointer to the next node in the queue
    
    // My functions
        Queue(); // default contructor
        Queue(DT* job); // constructor that will just assign the jobPointer and keep next null, best for the front object (given by ChatGPT)
        Queue(const Queue<DT>& x); // creates a deep copy of the pointer, so that it does not repeat the same address
        ~Queue(); // Destructor, deletes memory to prevent information from being copied again
};

// Default constructor
template <class DT>
Queue<DT>::Queue() {
    JobPointer = nullptr;
    next = nullptr;
}

// Constructor that only assigns the JobPointer
template <class DT>
Queue<DT>::Queue(DT* job) {
    JobPointer = new DT(*job); // this creates a deep copy for the JobPointer pointer, so that it does not share the same memory address
                               // uses the copy constructor created in the CPUJob class
    next = nullptr;
}

// Destructor, deletes memory to prevent information from being copied again
template <class DT>
Queue<DT>::~Queue() {
    delete JobPointer;
    delete next;
}

// Copy constructor that creates a deep copy, this prevents the same job from being entered into the queue
template <class DT>
Queue<DT>::Queue(const Queue<DT>& x) { 
        JobPointer = new DT((*x.JobPointer)); // creates a deep copy for JobPointer
        next = x.next; 
}

template <class DT>
class NovelQueue {
    public:
        Queue<DT>* front; // Pointer to the front of the queue
        Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
        int size; // Number of elements in the queue

    // My functions
        NovelQueue(); // default constructor
        void enqueue(DT* x); // adds an object to the end of a queue
        CPUJob* dequeue(); // removes the front element of the queue and returns that element
        void display(); // displays all of the elements in the queue
        void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed); // modifies a job's attributes
        void change(int job_id, int field_index, int new_value); // changes a specific element's field into a new value
        void promote(int job_id, int positions); // promotes a specific id a certain amount of positions
        void reorder(int attribute_index);
        int count();
        void listJobs();
        int findId(int job_id);
};

// default construtor of the queue that assigns default values to field variables
template <class DT>
NovelQueue<DT>:: NovelQueue() {
    front = nullptr; // holds the front element of the queue
    NodePtrs = new Queue<DT>*[30]; // makes an empty array
}

// finds the index of where the job_id is within an array using binary search
template <class DT>
int NovelQueue<DT>:: findId(int job_id) {
    int index = -1; // -1 is returned if value is not in the array

    for (int i = 0; i < size; i++) {
        if (((*(*NodePtrs[i]).JobPointer) -> job_id) == job_id) {
            index = i;
            break;
        }
    }
    return index;
}

// adds CPUJob objects to the Queue, specifically adds it to the end of the NodePtrs array
// Asked ChatGPT on how to implement the enqueue function
template <class DT> 
void NovelQueue<DT>:: enqueue(DT* x) {
    int index = -1; // holds the index of where the job_id is
    
    if (size > 0) {
        index = findId((*x) -> job_id); // gets the index
    }
    if (index != -1) { // if index is not -1, that means it already exists in the array
            cout << "Job ID " << (*x) -> job_id << " already exists!" << endl;
    } 
    else {
    // Displaying what will be enqueued onto the queue
        cout << "Enqueued Job:" << endl; 
        (*x) -> display();

    // Creates a new Queue object that will be used to store the information in x into the front pointer and the array
    // This pointer object will make deep copying easier
    // With the Queue copy constructor I made, I made a deep copy that copies the CPUJob fields of x to the JobPointer pointer variable of newNode
    // This prevents these two pointers from sharing the same memory address
        Queue<DT>* newNode = new Queue<DT>(x); 

    // if size is 0, then the array is empty
        if (size == 0) { 
    // copies newNode into front, deep copy
            front = new Queue<DT>(*newNode); 
    // adds front into the array. It is a shallow copy because we want it to have the same memory address to be able to remove
            NodePtrs[size] = front; 
        }
    // goes to this conditional if the array is not empty
        else { 
    // creates a deep copy that will be stored into the array
            NodePtrs[size] = new Queue<DT>(*newNode); 
    // goes to the previous element to assign the newNode to its next pointer field. This allows dequeue to be implemented easier
    // and it shows what the next value will be
    // shallow copy so that the next pointer will point to the memory address of the next element
            NodePtrs[size - 1] -> next = NodePtrs[size];
        }

        size++; // increments and updates size

    // Displaying all of the current elements in the queue
        cout << "Jobs after enqueue:" << endl;
        (*this).display();

        delete newNode; // deletes newNode to free up space and prevent from memory leaks
    }
}

// promotes a chosen job to move up a certain amount of spots in the array
template <class DT>
void NovelQueue<DT>:: promote(int job_id, int positions) {
    int swaps = positions; // holds the original positions value
    int index = findId(job_id); // finds the index of where a specific job is in the array

// This while loop will iterate until the chip is moved in the right place (indicated by position == 0)
    while (positions > 0) { 
    // Swaps the wanted element with the element before it
        Queue<DT>* holder = new Queue<DT>((*NodePtrs[index])); // holder pointer variable used for swapping
        (NodePtrs[index]) = new Queue<DT>(*NodePtrs[index - 1]);
        (NodePtrs[index - 1]) = new Queue<DT>(*holder);
    
    // Reassigns the next values of the swapped jobs
        if (index == 1) { // conditional for when the promoted job is at the front of the array
            front = new Queue<DT>(*NodePtrs[index - 1]); // assigns front queue to be the desired job
            (NodePtrs[index]) -> next = new Queue<DT>(*NodePtrs[index - 1]); // re-assigns the next pointer of the swapped element
        }
        else if (index == size - 1) { // conditional for when the promoted job was originally at the end of the array
            (NodePtrs[index - 1]) -> next = new Queue<DT>(*NodePtrs[index]); // re-assigns the next pointer of the wanted job
            (NodePtrs[index]) -> next = nullptr; // re-assigns the next pointer of the swapped element
        }
        else {
            (NodePtrs[index - 1]) -> next = new Queue<DT>(*NodePtrs[index]); // re-assigns the next pointer of the wanted job
            (NodePtrs[index]) -> next = new Queue<DT>(*NodePtrs[index + 1]); // re-assigns the next pointer of the swapped element
        }
    
        positions--; // decreases positions, so that the while loop will eventually end
        index--; // decreases index and updates it so that it will swap another pair of elements and not the same one
    }
}

// removes and returns the front of the array
// also shifts the remaining element one positions upwards
template <class DT>
CPUJob* NovelQueue<DT>:: dequeue() {
// creates a new CPUJob pointer to temporarily hold the JobPointer CPUJob pointer of front, which is what is being removed
    CPUJob* temp = (*(*front).JobPointer);

// This for loop shifts all of the elements in the array one to the left
    for (int i = 0; i < size - 1; ++i) {
        NodePtrs[i] = new Queue<DT>((*NodePtrs[i + 1]));
    }
    
    delete NodePtrs[size]; // deletes the end of the array to be no duplicates
    front = new Queue<DT>(*NodePtrs[0]); // assigns the front pointer to be the new front or the previously second element
    size--; // decreases and updates the size of the array

    return temp; // returns what is removed
}

// Displays all of the element in the array
template <class DT>
void NovelQueue<DT>:: display() {
    for (int i = 0; i < size; ++i) { 
            cout << "Job ID: " << (*(*NodePtrs[i]).JobPointer) -> job_id 
                << ", Priority: " << (*(*NodePtrs[i]).JobPointer) -> priority 
                << ", Job Type: " << (*(*NodePtrs[i]).JobPointer) -> job_type 
                << ", CPU Time Consumed: " << (*(*NodePtrs[i]).JobPointer) -> cpu_time_consumed 
                << ", Memory Consumed: " << (*(*NodePtrs[i]).JobPointer) -> memory_consumed << endl;
    }
}

// changes a field of a specific job
template <class DT>
void NovelQueue<DT>:: change(int job_id, int field_index, int new_value) {
    int index = findId(job_id); // finds the index of where the Job is in the queue based off of job_id

    if (index == - 1) { // if job does not exist in queue
        cout << "Job with ID " << job_id << " not found in the queue." << endl;
    }
    else {
        // Switch command that is based off of the field_index
        switch (field_index) {
            case 1: // changes priority
                (*(*NodePtrs[index]).JobPointer) -> priority = new_value;
                break;
            
            case 2: // changes job_type
                (*(*NodePtrs[index]).JobPointer) -> job_type = new_value;
                break;

            case 3: // changes cpu_time_consumed
                (*(*NodePtrs[index]).JobPointer) -> cpu_time_consumed = new_value;
                break;

            case 4: // changes memory_consumed
                (*(*NodePtrs[index]).JobPointer) -> memory_consumed = new_value;
                break;
            }

        // Prints what is being changed
            cout << "Changed Job ID " << job_id << " field " << field_index << " to " << new_value << ":" << endl;
            (*(*NodePtrs[index]).JobPointer) -> display();

        // Prints the new queue
            cout << "Jobs after changing field:" << endl;
            (*this).display();
    }


}

// changes all of the field of a specific job
template <class DT>
void NovelQueue<DT>:: modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {
    int index = findId(job_id); // finds index of where the job_id in the array
 
    if (index == -1) { // if job does not exist in queue
        cout << "Job with ID " << job_id << " not found in the queue." << endl;
    } 
    else {
    // Re-assigning the fields
        (*(*NodePtrs[index]).JobPointer) -> priority = new_priority;
        (*(*NodePtrs[index]).JobPointer) -> job_type = new_job_type;
        (*(*NodePtrs[index]).JobPointer) -> cpu_time_consumed = new_cpu_time_consumed;
        (*(*NodePtrs[index]).JobPointer) -> memory_consumed = new_memory_consumed;

    // Displays what is being modified
        cout << "Modified Job ID " << job_id << ":" << endl;
        (*(*NodePtrs[index]).JobPointer) -> display();

    // New queue after modification
        cout << "Jobs after modification:" << endl;
        (*this).display();
    }
}

// Re-orders the array based off a specific attribute
// Insertion sort algorithm assisted by ChatGPT
template <class DT>
void NovelQueue<DT>:: reorder(int attribute_index) {
// Switch cases that sorts the array based off what is wanted to be sorted
// Sorting by insertion sort
// Create deep copies to swap
    switch (attribute_index) {
        case 1: // sorts by job_id
            for (int i = 1; i < size; ++i) { // outer for loops picks what is going to be moved
                int key = (*(*NodePtrs[i]).JobPointer) -> job_id; // holds what is being shifted up
                int id = (*(*NodePtrs[i]).JobPointer) -> job_id; // holds the id of what is being shifted up
                int j = i-1;

                // inner loop does the shifitng
                while (j >= 0 && key < (*(*NodePtrs[j]).JobPointer) -> job_id) { 
                    (*this).promote(id, 1); // calls promotes to shift an element up in the queue
                    j--; // decrements j to get the previous element to compare
                }
            }
            break;
            
        case 2: // sorts by priority
            for (int i = 1; i < size; ++i) { // outer for loops picks what is going to be moved
                int key = (*(*NodePtrs[i]).JobPointer) -> priority; // holds what is being shifted up
                int id = (*(*NodePtrs[i]).JobPointer) -> job_id; // holds the id of what is being shifted up
                int j = i-1;

                // inner loop does the shifitng
                while (j >= 0 && key < (*(*NodePtrs[j]).JobPointer) -> priority) { 
                    (*this).promote(id, 1); // calls promotes to shift an element up in the queue
                    j--; // decrements j to get the previous element to compare
                }
            }
            break;

        case 3: // sorts by job_type
            for (int i = 1; i < size; ++i) { // outer for loops picks what is going to be moved
                int key = (*(*NodePtrs[i]).JobPointer) -> job_type; // holds what is being shifted up
                int id = (*(*NodePtrs[i]).JobPointer) -> job_id; // holds the id of what is being shifted up
                int j = i-1;

                // inner loop does the shifitng
                while (j >= 0 && key < (*(*NodePtrs[j]).JobPointer) -> job_type) { 
                    (*this).promote(id, 1); // calls promotes to shift an element up in the queue
                    j--; // decrements j to get the previous element to compare
                }
            }
            break;

        case 4: // sorts bu cpu_time_consumed
            for (int i = 1; i < size; ++i) { // outer for loops picks what is going to be moved
                int key = (*(*NodePtrs[i]).JobPointer) -> cpu_time_consumed; // holds what is being shifted up
                int id = (*(*NodePtrs[i]).JobPointer) -> job_id; // holds the id of what is being shifted up
                int j = i-1;

                // inner loop does the shifitng
                while (j >= 0 && key < (*(*NodePtrs[j]).JobPointer) -> cpu_time_consumed) { 
                    (*this).promote(id, 1); // calls promotes to shift an element up in the queue
                    j--; // decrements j to get the previous element to compare
                }
            }
            break;

        case 5: // sorts by memory_consumed
            for (int i = 1; i < size; ++i) { // outer for loops picks what is going to be moved
                int key = (*(*NodePtrs[i]).JobPointer) -> memory_consumed; // holds what is being shifted up
                int id = (*(*NodePtrs[i]).JobPointer) -> job_id; // holds the id of what is being shifted up
                int j = i-1;

                // inner loop does the shifitng
                while (j >= 0 && key < (*(*NodePtrs[j]).JobPointer) -> memory_consumed) { 
                    (*this).promote(id, 1); // calls promotes to shift an element up in the queue
                    j--; // decrements j to get the previous element to compare
                }
            }
            break;
    }
}

// Returns how many elements are in the array
template <class DT>
int NovelQueue<DT>:: count() {

    return size;
}

// List elements in the array by Job_id
template <class DT>
void NovelQueue<DT>:: listJobs() {
    cout << "List of jobs sorted by job IDs:" << endl;
    (*this).reorder(1); // calls the reorder method to order it by the job_id
    (*this).display(); // displays the reordered queue
}

int main() {
        int n; // Number of commands
        cin >> n; // Read the number of commands

    // Instantiate a NovelQueue for CPUJob pointers
        NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
        char command; // Variable to store the command type

    // Variables for job attributes
        int job_id, priority, job_type, cpu_time_consumed, memory_consumed;

    // Variables for modifying a job
        int new_priority, new_job_type, new_cpu_time_consumed;
        int new_memory_consumed;
        int field_index, new_value;

    // Variable for the number of positions in the 'Promote' command
        int positions;
        int attribute_index; // Variable for the 'Reorder' command

    /************** Read each command Process ***************/
        for (int i = 0; i < n; ++i) {
            cin >> command; // Read the command type
            switch (command) {
                case 'A': { // Add (Enqueue)
                    cin >> job_id >> priority >> job_type;
                    cin >> cpu_time_consumed >> memory_consumed;
                    CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                    (*myNovelQueue).enqueue(&newJob);

                    break;
                }
                case 'R': { // Remove (Dequeue)
                    CPUJob* removedJob = (*myNovelQueue).dequeue();

                    if (removedJob) {
                        cout << "Dequeued Job: " << endl;
                        (*removedJob).display();
                        cout << "Jobs after dequeue: " << endl;
                        (*myNovelQueue).display();
                    }

                    break;
                }
                case 'M': { // Modify
                    cin >> job_id >> new_priority >> new_job_type;
                    cin >> new_cpu_time_consumed >> new_memory_consumed;
                    (*myNovelQueue).modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);

                    break;
                }
                case 'C': { // Change Job Values
                    cin >> job_id >> field_index >> new_value;
                    (*myNovelQueue).change(job_id, field_index, new_value);

                    break;
                }
                case 'P': { // Promote
                    cin >> job_id >> positions;

                    // Prints what will be promoted
                        cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s):" << endl;
                        int index = (*myNovelQueue).findId(job_id);
                        (*(*(*myNovelQueue).NodePtrs[index]).JobPointer) -> display();

                        (*myNovelQueue).promote(job_id, positions);


                    // Prints the updated list
                        cout << "Jobs after promotion:" << endl;
                        (*myNovelQueue).display();

                    break;
                }
                case 'O': { // Reorder
                    cin >> attribute_index;
                    (*myNovelQueue).reorder(attribute_index);

                    cout << "Reordered Queue by attribute " << attribute_index << ":" << endl;
                    (*myNovelQueue).display();

                    break;
                }
                case 'D': { // Display
                    cout << "Displaying all jobs in the queue:" << endl;
                    (*myNovelQueue).display();

                    break;
                }
                case 'N': { // Count
                    cout << "Number of elements in the queue: " <<
                    (*myNovelQueue).count() << endl;

                    break;
                }
                case 'L': { // List Jobs
                   (*myNovelQueue).listJobs();
             
                   break;
                }
                default: {
                    cout << "Invalid command!" << endl;
                }
            }
        }
        delete myNovelQueue; // Clean up the NovelQueue after all operations
        
        return 0;
};

/***** Debugging plan and LLM Usage ******/
/*

Approach: 

    Based off of the "input1.txt" file, the order I chose to complete each function was determined of the order shown in the file. I
made sure that one function worked before moving to the next one. I started with the 'A' command case while having the other cases muted.
As I made progress with the other functions, I would unmute those functions to slowly piece together the output.
----------------------------------------------------------------------------------------------------------------------------------------
CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed)

LLM Usage: 
    I did not use any LLM Usage. If I did, however, I would have asked "How can I implement the most efficient constructor for this 
class?"

Debugging:
    The implementation of this constructor was fairly easy as I did not face any challenges, but I still wanted to check if I did it 
correctly. Therefore, I create a line of code inside the 'A' case that printed each object's fields every time the command called that
switch case. The code I used was...

    cout << newJob -> job_id << " " 
         << newJob -> priority << " " 
         << newJob -> job_type << " " 
         << newJob -> cpu_time_consumed << " " 
         << newJob -> memory_consumed << endl;

Based off the output that is shown below, I believe that the constructor was implemented correctly. It matches with the data on the 
input file. Because I muted the other cases, the output will stop once the command is not A anymore. 

    101 5 2 150 2000
    102 3 1 100 1024
    103 7 4 250 4096
    104 2 3 300 2048
    105 6 5 200 512
----------------------------------------------------------------------------------------------------------------------------------------
void enqueue(DT* x)

LLM Usage: 
    I asked two questions when implementing this function. The first question was "How can I implement an enqueue function?" From this
prompt, I picked up on how to assign the front and next pointer a pointer value. Somethings, such as a rear pointer, was suggested, but
I did not use it. The next question I asked was "How can I create deep copies of a pointer?" I used this prompt as a base on how to create
the copy constructors for the CPUJob and Queue class and how to reassign a pointer variable without copying the memory address.  
The reason I picked those prompts will be explained in the debugging section.

Debugging:
    The only issue I came upon when implementing this function was when I initially printed the elements in the queue. The elements that was
already in the queue would somehow change to the job that was being added on. 

    Enqueued Job:
    Job ID: 101, Priority: 5, Job Type: 2, CPU Time Consumed: 150, Memory Consumed: 2000
    Jobs after enqueue:
    Job ID: 101, Priority: 5, Job Type: 2, CPU Time Consumed: 150, Memory Consumed: 2000
    Enqueued Job:
    Job ID: 102, Priority: 3, Job Type: 1, CPU Time Consumed: 100, Memory Consumed: 1024
    Jobs after enqueue:
    Job ID: 102, Priority: 3, Job Type: 1, CPU Time Consumed: 100, Memory Consumed: 1024
    Job ID: 102, Priority: 3, Job Type: 1, CPU Time Consumed: 100, Memory Consumed: 1024
    Enqueued Job:
    Job ID: 103, Priority: 7, Job Type: 4, CPU Time Consumed: 250, Memory Consumed: 4096
    Jobs after enqueue:
    Job ID: 103, Priority: 7, Job Type: 4, CPU Time Consumed: 250, Memory Consumed: 4096
    Job ID: 103, Priority: 7, Job Type: 4, CPU Time Consumed: 250, Memory Consumed: 4096
    Job ID: 103, Priority: 7, Job Type: 4, CPU Time Consumed: 250, Memory Consumed: 4096

The first thing I checked was whether the address of the pointer objects were correct. I used the code below to check the addresses.

    for (int i = 0; i < size; ++i) {
        cout << "Front: " << front << " Array element " << i << " : " << *(*NodePtrs[i]).JobPointer << endl;
    }

    Output: 
    Enqueued Job:
    Job ID: 101, Priority: 5, Job Type: 2, CPU Time Consumed: 150, Memory Consumed: 2000
    Front: 0x7fa5a4f05e80 Array element 0 : 0x7fa5a4f05e20
    Enqueued Job:
    Job ID: 102, Priority: 3, Job Type: 1, CPU Time Consumed: 100, Memory Consumed: 1024
    Front: 0x7fa5a4f05e80 Array element 0 : 0x7fa5a4f05e20
    Front: 0x7fa5a4f05e80 Array element 1 : 0x7fa5a4f05ea0
    Enqueued Job:
    Job ID: 103, Priority: 7, Job Type: 4, CPU Time Consumed: 250, Memory Consumed: 4096
    Front: 0x7fa5a4f05e80 Array element 0 : 0x7fa5a4f05e20
    Front: 0x7fa5a4f05e80 Array element 1 : 0x7fa5a4f05ea0
    Front: 0x7fa5a4f05e80 Array element 2 : 0x7fa5a4f05d60

I noticed how the front address is the same and the Job address are different and how the addresses are stored in the way that it should. 
Based off this observation, I knew it was because I was creating shallow copies that only shared its memory address and not deep copies 
that would actually share its actual fields. Thus, I used the LLM to give me examples on how to create deep copies. I made a copy constructor
in the CPUJob class, a copy constructor that only copies a CPUJob pointer object into the JobPointer field for new jobs that are being added 
to the queue the in the Queue class, and a copy constructor that both copies into the JobPointer and the next pointer variables for the next 
pointer variable in the Queue class. I made sure to use the "new" keyword to allocate dynamic memory for a deep copy. After these changes, it 
properly printed the statements.
----------------------------------------------------------------------------------------------------------------------------------------
promote(int job_id, int positions)

LLM Usage: 
    I did not use LLM to complete this function. However, if I were to use it, I would ask "How can I efficiently swap two pointer varaibles?"

Debugging:
    I did not have any serious issues when implementing this function. The only minor issue was the loop trying to access an element in the array
that is not there. Specifically, it would try to access (*(*NodePtrs[-1].JobPointer). I fixed this by adding a conditional statement in the while 
loop that considers this. Whenever index == 1, which means that the element is about to be in the front of the queue, it re-assigns the front.next
pointer variable instead of the element before it's next pointer variable. I know this function is implemented correctly if it prints out the correct 
statements, and it did.
----------------------------------------------------------------------------------------------------------------------------------------
dequeue()

LLM Usage:
    I did not use LLM to complete this function. If I were to use it, however, I would ask "How can I implement a dequeue function?"

Debugging:
    I did not run into any issues implementing this code. I know that this code is implementing correctly because it printed the right
statements.
----------------------------------------------------------------------------------------------------------------------------------------
change(int job_id, int field_index, int new_value)

LLM Usage:
    I did not use LLM to complete this function. If I were to use it, however, I would ask "How can I implement a change function?"

Debugging:
    I did not run into any issues implementing this code. I know that this code is implementing correctly because it printed the right
statements.
----------------------------------------------------------------------------------------------------------------------------------------
modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed)

LLM Usage:
    I did not use LLM to complete this function. If I were to use it, however, I would ask "How can I implement a modifiy function?"

Debugging:
    I did not run into any issues implementing this code. I know that this code is implementing correctly because it printed the right
statements.
----------------------------------------------------------------------------------------------------------------------------------------
NEW ISSUE:
    On the "input1.txt" second attempt at the promotion function. It is outputting "zsh: segmentation fault", which usually means it is 
trying to access something in the queue array that is not there. After looking at my code, I noticed that in my while loop, I did not 
have a conditional that considered if the promoted job was at the end of the queue array. The code that was creating the issue is listed
below.

    else {
            (NodePtrs[index - 1]) -> next = new Queue<DT>(*NodePtrs[index]); // re-assigns the next pointer of the wanted job
            (NodePtrs[index]) -> next = new Queue<DT>(*NodePtrs[index + 1]); // re-assigns the next pointer of the swapped element
    }

Because I neglected this, whenever it swapped values and re-assigned the next pointer field, it would try to access an element in the 
NodePtrs array that is not there with *NodePtrs[index + 1]. Since it is being swapped to the end of the queue, its next should be pointing 
to nothing. Therefore, I added a conditional that accounted for this case.

    else if (index == size - 1) { // conditional for when the promoted job was originally at the end of the array
            (NodePtrs[index - 1]) -> next = new Queue<DT>(*NodePtrs[index]); // re-assigns the next pointer of the wanted job
            (NodePtrs[index]) -> next = nullptr; // re-assigns the next pointer of the swapped element
    }
----------------------------------------------------------------------------------------------------------------------------------------
count()
LLM Usage:
    I did not use LLM to complete this function. If I were to use it, however, I would ask "Is there a more efficient way to get the size
of a queue?"

Debugging:
    I did not run into any issues implementing this code. I know that this code is implementing correctly because it printed the right
statements.
----------------------------------------------------------------------------------------------------------------------------------------
reorder(int attribute_index)

LLM Usage
    I asked ChatGPT "How to implement the insertion sort". This gave me a way to implement the reorder function.

Debugging:
    I did not run into any issues implementing this code. I know that this code is implementing correctly because it printed the right
statements. I just created switch cases. I feel like I could have implemented the function better, but I believe this will suffice.
----------------------------------------------------------------------------------------------------------------------------------------
display()

LLM Usage:
    I did not use LLM to complete this function. If I were to use it, however, I would ask "How can I implement the display function?"

Debugging:
    I did not run into any issues implementing this code. I know that this code is implementing correctly because it printed the right
statements.
----------------------------------------------------------------------------------------------------------------------------------------
listJobs()

LLM Usage:
    I did not use LLM to complete this function. If I were to use it, however, I would ask "How can I implement this function?"

Debugging:
    I did not run into any issues implementing this code. I know that this code is implementing correctly because it printed the right
statements.
----------------------------------------------------------------------------------------------------------------------------------------
Checking input2.txt: 
    Next, I inputted the input2.txt file, and I got wrong statements. I did not consider if a job that is trying to be enqueued is already
in the queue, so I added a conditional in my enqueue function, so solve this.

    int index; // holds the index of where the job_id is

    if (size > 0) {
        index = findId((*x) -> job_id, 0, size); // gets the index

        if (index >= 0) { // if index is not -1, that means it already exists in the array
            cout << "Job ID " << (*x) -> job_id << " already exists!" << endl;
        }  
    }

    The next issue was modifiying an element of the queue. I did not consider not running the function if the job wanted was not there. 
Thus, I added a conditional statement to consider this situation. I checked what the index was by printing it after finding it.
It ended up printing -1. I think this was because that since the job_id is not sorted due to 108 being promoted, it could not find the element. 
To fix this, I changed my binary search method into a regular search method. Now it works.

    The next issue was my reorder method. Instead of manually swapping values, which would create shallow copies, I decided to call the promote method. This
ended up working as I am getting correct statements. I used ChatGPT to give me an idea of how to implement this.


*/




