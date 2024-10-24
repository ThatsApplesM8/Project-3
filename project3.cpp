#include <iostream>
using namespace std;

class CPUJob {
public:
    int job_id; // Unique identifier for the job
    int priority; // Priority level of the job (1-10)
    int job_type; // Job type (1-10)
    int cpu_time_consumed; // Total CPU time consumed by the job
    int memory_consumed; // Total memory consumed thus far

    // Constructors
    CPUJob(int id, int p, int type, int cpu_time, int memory) {
        job_id = id;
        priority = p;
        job_type = type;
        cpu_time_consumed = cpu_time;
        memory_consumed = memory;
    }
    void display() const {
        cout << "Job ID: " << job_id << ", Priority: " << priority
             << ", Job Type: " << job_type << ", CPU Time Consumed: " << cpu_time_consumed
             << ", Memory Consumed: " << memory_consumed << endl;
    }
};

template <class DT>
class Queue {
    public:
    DT JobPointer; // Pointer to a job (e.g., CPUJob)
    Queue<DT>* next; // Pointer to the next node in the queue

    Queue(DT job) { // Constructor
        JobPointer = job;
        next = nullptr;    
    }
};

template <class DT>
class NovelQueue {
public:
    Queue<DT>* front; // Pointer to the front of the queue
    Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
    int size; // Number of elements in the queue

    //Constructor
    NovelQueue() {  // Initialize the queue
        front = nullptr;
        size = 0;
    }

    // Methods
    int findJob(int job_id) {   //Binary search to find the job
        int low = 0, high = size - 1;
        while(low <= high) {
            int mid = low + (high - low) / 2;

            if(NodePtrs[mid]->JobPointer->job_id == job_id) {
                return mid;
            } else if(NodePtrs[mid]->JobPointer->job_id < job_id) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return -1;
        
    }
    void display() {    //Display all jobs in the queue
        Queue<DT>* temp = front;  // Use a temp pointer to iterate through the queue
        while (temp != nullptr) {
            temp->JobPointer->display();  // Use '->' to access members of the pointer
            temp = temp->next;
        }
    }

    void nodePtrUpdate() {  //Update the NodePtrs array
        NodePtrs = new Queue<DT>*[size];
        Queue<DT>* temp = front;
        for (int i = 0; i < size; i++) {
            NodePtrs[i] = temp;
            temp = temp->next;
        }

        for(int j = 0; j < size; j++) {     // Bubble Sort

            for(int k = 0; k < size-j-1; k++) {
                if(NodePtrs[k]->JobPointer->job_id > NodePtrs[k+1]->JobPointer->job_id) {
                    Queue<DT>* temp = NodePtrs[k];
                    NodePtrs[k] = NodePtrs[k+1];
                    NodePtrs[k+1] = temp;
                }
            }
        }
    }
    void enqueue(DT job) {  // Add a job to the queue
        // Create a new node for the job
        Queue<DT>* newNode = new Queue<DT>(job);
        

        // If the queue is empty, set front to the new node
        if (front == nullptr || size == 0) {
            front = newNode;
        } else {
            // Otherwise, find the last node and append the new node
            Queue<DT>* temp = front;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
        // Update the NodePtrs array
        nodePtrUpdate();
        // Display all jobs in the queue
    }

    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {
        int index = findJob(job_id);
        if(index == -1) {
            cout << "Job ID " << job_id << " does not exist!" << endl;
        } else {
            NodePtrs[index]->JobPointer->priority = new_priority;
            NodePtrs[index]->JobPointer->job_type = new_job_type;
            NodePtrs[index]->JobPointer->cpu_time_consumed = new_cpu_time_consumed;
            NodePtrs[index]->JobPointer->memory_consumed = new_memory_consumed;
            cout << "Modified Job ID " << NodePtrs[index]->JobPointer->job_id << ":" << endl;
            NodePtrs[index]->JobPointer->display();

            nodePtrUpdate();
        }
    }

    DT dequeue() {
        // If the queue is empty, return nullptr
        if (front == nullptr || size == 0) {
            cout << "Queue is empty!" << endl;
            return nullptr;
        }
        // Otherwise, remove the front node and return the job
        Queue<DT>* temp = front;
        front = front->next;
        size--;
        // Update the NodePtrs array
        nodePtrUpdate();
        return temp->JobPointer;   //Return the job
    }

    void change(int job_id, int field_index, int new_value) {   //Change the field of a job
        int index = findJob(job_id);    //Find the job
        if(index == -1) {   //If the job is not found
            cout << "Job with ID "<< job_id << " not found in the queue." << endl;
            return;
        } else {
            switch(field_index) {   //Change the field based on the field index
                case 1:
                    NodePtrs[index]->JobPointer->priority = new_value;
                    break;
                case 2:
                    NodePtrs[index]->JobPointer->job_type = new_value;
                    break;
                case 3:
                    NodePtrs[index]->JobPointer->cpu_time_consumed = new_value;
                    break;
                case 4:
                    NodePtrs[index]->JobPointer->memory_consumed = new_value;
                    break;
                default:
                    cout << "Invalid field index!" << endl;
            }
            cout << "Changed Job ID " << NodePtrs[index]->JobPointer->job_id << " field " << field_index << " to " << new_value << ":" << endl;
            nodePtrUpdate();    //Update the NodePtrs array
            NodePtrs[index]->JobPointer->display();   //Display the job

            cout << "Jobs after changing field: " << endl;
            for(int i = 0; i < size; i++) {     //Display all jobs in the queue
                NodePtrs[i]->JobPointer->display();
            }
        }
    }

    void promote(int job_id, int positions) {
        int index = findJob(job_id);    //Find the job
        if (index == -1) {  //If the job is not found
            cout << "Job ID " << job_id << " does not exist!" << endl;
            return;
        }

        int newIndex = index - positions;   //Calculate the new index
        Queue<DT>* currentNode = NodePtrs[index];   //Get the node at the current index

        if (newIndex < 0) { // If the new index is less than 0
            // Move the node to the front of the queue
            if (index == 0) {
                // Already at the front
                cout << "Job ID " << job_id << " is already at the front!" << endl;
                return;
            } else {
                // Remove the node from its current position
                if (index > 0) {
                    NodePtrs[index - 1]->next = currentNode->next;
                }

                // Insert the node at the front
                currentNode->next = front;  //Set the next pointer of the current node to the front
                front = currentNode;    //Set the front pointer to the current node

                // Update NodePtrs array
                nodePtrUpdate();

                cout << "Moved Job ID " << currentNode->JobPointer->job_id << " to the front of the queue." << endl;
                currentNode->JobPointer->display();   //Display the job
            }
        } else {
            // Move the node to the new position
            if (index > 0) {
                NodePtrs[index - 1]->next = currentNode->next;
            } else {
                front = currentNode->next;
            }

            if (newIndex == 0) {    //If the new index is 0
                currentNode->next = front;  //Set the next pointer of the current node to the front
                front = currentNode;    //Set the front pointer to the current node
            } else {
                Queue<DT>* newNode = NodePtrs[newIndex - 1];    //Get the node at the new index
                currentNode->next = newNode->next;  //Set the next pointer of the current node to the next pointer of the new node
                newNode->next = currentNode;    //Set the next pointer of the new node to the current node
            }

            // Update NodePtrs array
            nodePtrUpdate();

            cout << "Promoted Job ID " << currentNode->JobPointer->job_id << " by " << positions << " Position(s): " << endl;
            currentNode->JobPointer->display();  //Display the job
        }
    }

    void listJobs() {
        NovelQueue<DT>* tempQueue = new NovelQueue<DT>();   //Create a temporary queue
        for (int i = 0; i < size; i++) {    //Enqueue all jobs in the temporary queue
            tempQueue->enqueue(NodePtrs[i]->JobPointer);    
        }
        tempQueue->reorder(1);  //Reorder the temporary queue by Job ID
        
        for(int j = 0; j < size; j++) { //Display all jobs in the temporary queue
            tempQueue->NodePtrs[j]->JobPointer->display();
        }
    }

    int count() {
        return size;    //Return the number of elements in the queue
    }
    void reorder(int index) {
        int elements = count();
        Queue<DT>* tempFront = front;
        Queue<DT>** temp = new Queue<DT>*[elements];

        // Populate the temporary array with pointers to the nodes
        int idx = 0;
        while (tempFront) {
            temp[idx++] = tempFront;
            tempFront = tempFront->next;
        }

        // Bubble Sort based on the specified index (attribute)
        for (int i = 0; i < elements; i++) {
            for (int j = 0; j < elements - i - 1; j++) {
                switch (index) {
                    case 1: // Sort by Job ID
                        if (temp[j]->JobPointer->job_id > temp[j + 1]->JobPointer->job_id) {
                            Queue<DT>* temp1 = temp[j];
                            temp[j] = temp[j + 1];
                            temp[j + 1] = temp1;
                        }
                        break;
                    case 2: // Sort by Priority
                        if (temp[j]->JobPointer->priority > temp[j + 1]->JobPointer->priority) {
                            Queue<DT>* temp1 = temp[j];
                            temp[j] = temp[j + 1];
                            temp[j + 1] = temp1;
                        }
                        break;
                    case 3: // Sort by Job Type
                        if (temp[j]->JobPointer->job_type > temp[j + 1]->JobPointer->job_type) {
                            Queue<DT>* temp1 = temp[j];
                            temp[j] = temp[j + 1];
                            temp[j + 1] = temp1;
                        }
                        break;
                    case 4: // Sort by CPU Time Consumed
                        if (temp[j]->JobPointer->cpu_time_consumed > temp[j + 1]->JobPointer->cpu_time_consumed) {
                            Queue<DT>* temp1 = temp[j];
                            temp[j] = temp[j + 1];
                            temp[j + 1] = temp1;
                        }
                        break;
                    case 5: // Sort by Memory Consumed
                        if (temp[j]->JobPointer->memory_consumed > temp[j + 1]->JobPointer->memory_consumed) {
                            Queue<DT>* temp1 = temp[j];
                            temp[j] = temp[j + 1];
                            temp[j + 1] = temp1;
                        }
                        break;
                    default:
                        cout << "Invalid index for reorder!" << endl;
                        delete[] temp;
                        return;
                }
            }
        }

        // Update the front pointer and the next pointers of the nodes in the original queue
        front = temp[0];
        for (int i = 0; i < elements - 1; i++) {
            temp[i]->next = temp[i + 1];
        }
        temp[elements - 1]->next = nullptr;

        // Update NodePtrs array
        nodePtrUpdate();

        delete[] temp; // Clean up the temporary array
    }

};

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
                if(myNovelQueue->findJob(job_id) != -1) {
                    cout << "Job ID " << job_id << " already exists!" << endl;
                    break;
                }
                else {
                    CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                    (*myNovelQueue).enqueue(newJob);
                    cout << "Enqueued Job: " << endl;
                    (*newJob).display();
                    cout << "Jobs after enqueue: " << endl;
                    (*myNovelQueue).display();
                }
                break;
            }
            case 'R': { // Remove (Dequeue)
                CPUJob* removedJob = (*myNovelQueue).dequeue();
                if (removedJob) {
                    cout << "Dequeued Job: " << endl;
                    (*removedJob).display();
                    delete removedJob; // Clean up memory after use
                    cout << "Jobs after dequeue: " << endl;
                    (*myNovelQueue).display();
                }
                break;
            }
            case 'M': { // Modify
                cin >> job_id >> new_priority >> new_job_type;
                cin >> new_cpu_time_consumed >> new_memory_consumed;
                (*myNovelQueue).modify(job_id, new_priority, new_job_type,
                new_cpu_time_consumed, new_memory_consumed);
                cout << "Jobs after modification: " << endl;
                (*myNovelQueue).display();
                break;
            }
            case 'C': { // Change Job Values
                cin >> job_id >> field_index >> new_value;
                (*myNovelQueue).change(job_id, field_index, new_value);
                // cout << "Jobs after changing field: " << endl;
                // (*myNovelQueue).display();
                break;
            }
            case 'P': { // Promote
                cin >> job_id >> positions;
                (*myNovelQueue).promote(job_id, positions);
                cout << "Jobs after promotion: " << endl;
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
                cout << "Displaying all jobs in the queue: " << endl;
                (*myNovelQueue).display();
                break;
            }
            case 'N': { // Count
                cout << "Number of elements in the queue: " <<
                (*myNovelQueue).count() << endl;
                break;
            }
            case 'L': { // List Jobs
                cout << "List of jobs sorted by job IDs:" << endl;
                (*myNovelQueue).listJobs();
                break;
            }
            default:
                cout << "Invalid command!" << endl;
        }
    }
    delete myNovelQueue; // Clean up the NovelQueue after all operations
    return 0;
}

/* LLM and GitHub Copilot Documentation

    Prompt: "Describe the pointer implementation of the NovelQueue class and its methods.""
    Rationale:  The NovelQueue class's implementation refrences pointers to its linked-list queues, ensuring properly memory management and assigning.
    The main problem I faced was how the pointers are being implmented into the classes itself, as well as how each reflects towards the methods of the class.
    This concept struggled me to understand, which gave me a bad start to the project. However, I was able to understand the concept of pointers and how they are implemented in the class.
    Incremental Development: I used the knowledge of the pointers and how they are implemented in the class to understand the concept of the NovelQueue class and its methods.
    This ensures the proper necessities of the class and how the implementation works, storing managements, and outputs for future methods that requires pointers in the NovelQueue Class.
    
    Prompt: "Generate a Node pointer array to store the pointers to the nodes in the queue."
    Rationale: The Node pointer array is generated to store the pointers to the nodes in the queue, which is used to update the NodePtrs array.
    I was asked to implement a bubble sort algorithm to sort the NodePtrs array based on the job ID from the given documentation on Canvas to do so.
    However, I was not able to understand the implementation towards Nodes and how they are stored in the array.
    Incremental Development: I used the knowledge of the Node pointer array and how they are stored in the array to ensure that such methods that are implemented in the class main function
    can be used to sort the NodePtrs array based on the job ID, ensuring uses of the nodePtrsUpdate() method to be automated after a method has changes into the NovelQueue itself.

    Prompt: "Could you explain the reasoning as to why my JobPointer is not pointing to the correct job?"
    Rationale: The reason behind this question is because my errors in running the code is that the JobPointer is pointing to the CPUJob* entirely instead of CPUJob object.
    This is a common error that I have faced in the project, which I was not able to understand the concept of the JobPointer and how it is implemented in the class.
    Instead, I removed the asterik (*) from the JobPointer to ensure that the JobPointer is pointing to the correct job.
    Incremental Development: I used the knowledge of the JobPointer and how it is implemented in the class to ensure that the JobPointer is pointing to the correct job, ensuring that the JobPointer is pointing to the CPUJob object
    in reference to the pointer instead of its double pointer (**).

*/

/* Specific Testings/Debugging 
    Display() Method Testings, Debugging, and Results:
    I tested the display() method to ensure that the jobs are displayed correctly in the queue. The display methods within each class for NovelQueue and CPUJob output within each breakpoints, which outputted and verified correctly.
    Each input.txt files have outputted values that was displayed correctly, which ensured that the display() method was working correctly.

    NodePtrUpdate() Method Testings, Debugging, and Results:
    I tested the nodePtrUpdate() method to ensure that the NodePtrs array is updated correctly. The nodePtrUpdate() method within the NovelQueue class for the NodePtrs array outputted within each breakpoints, which outputted and verified correctly.
    Each input.txt files have outputted the updated Node pointers array accordingly along with the display() methods and other methods alongside with it.

    Enqueue() Method Testings, Debugging, and Results:
    I tested the enqueue() method, ensuring that each job has been added accordingly to the queue.
    I faced some breakpoints of the method, as it displays thats its being added to the queue, but the actually queue itself is not adding the job entirely.
    I fixed it by creating a temporary queue and store the job, and generated a while loop to go transverse to the next node, and place it into the queue.
    after the implementation, the enqueue() method outputted the job correctly and added it to the queue.

    Dequeue() Method Testings, Debugging, and Results:
    I tested the dequeue() method to ensure that the job is removed from the queue correctly.
    I faced some breakpoints of the method, as it displays that the job is being removed from the queue, but the job is not being removed entirely.
    I checked the output.txt files and found that the job is not being updated from the queue, leaving the removed job still present in the queue.
    I fixed it by implementing the NodePtrUpdate() method, which would be applied to most of the other methods.
    I retested the dequeue() method breakpoints with each input.txt files, and the job was removed from the queue correctly.

    Modify() Method Testings, Debugging, and Results:
    I tested the modify() method to ensure that the job is modified correctly in the queue.
    I tested the modify() method breakpoints with each input.txt files, and the job was modified correctly in the queue.

    Change() Method Testings, Debugging, and Results:
    I tested the change() method to ensure that the job field is changed correctly in the queue.
    Checking the breakpoints, I verified that for each input.txt files, the job field was changed correctly in the queue.

    Promote() Method Testings, Debugging, and Results:
    I tested the promote() method, and found several major problems regarding the debugging stage
    The promote() job was not being moved to the position that is being promoted; instead it was promoting it to the very top of the queue, which is not that wass aimed for.
    I fixed it by implementing a new index to calculate the new index of the job, and implemented a new node to store the current node, and a new node to store the new node.
    I also implemented a new if statement to check if the new index is less than 0, and if it is, it would move the node to the front of the queue.
    After the implementation, the promote() method outputted the job being promoted to the correct position in the queue.
    I also restested and applied the nodePtrUpdate() method to ensure that the NodePtrs array is updated correctly to the main NovelQueue itself, which I verified did work properly.

    Reorder() Method Testings, Debugging, and Results:
    I tested the reorder() method, and also found some problems of the debugging stage of the output files.
    The switch-case worked fine to implement the bubble sort algorithm, which was working perfectly, however,
    it was the the generated temp queue that was not being updated correctly, which was not being sorted by the attribute index.
    I fixed it by implementing a new temporary array to store the pointers to the nodes, and implemented a new bubble sort algorithm to sort the temporary array based on the attribute index.
    After retesting the reorder() method, the reorder() method outputted the queue being reordered by the attribute index correctly.

    ListJobs() Method Testings, Debugging, and Results:
    I tested the listJobs() method to ensure that the jobs are listed correctly in the queue.
    Since it was being sorted by the job ID, by the instructor suggestion, I implemented the reorder() method to sort the queue by the job ID.
    I tested the breakpoints for each input.txt files, and the listJobs() method outputted the jobs being listed correctly in the queue.

    Count() Method Testings, Debugging, and Results:
    I verified and testes the count() method, which return the number of elements (size) in the queue.
    I tested the count() method breakpoints with each input.txt files, and the count() method outputted the number of elements in the queue correctly.



*/