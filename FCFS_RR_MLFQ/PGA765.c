#include <stdio.h>
#include <stdlib.h>

// Node structures for different scheduling algorithms
struct Node
{
    int processNum;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    struct Node* next;
};

struct Node2{
    int processNum;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    struct Node2* next;
};

struct NodeFCFS{
        int processNum;
        int arrivalTime;
        int burstTime;
        int waitingTime;
        int completionTime;
        int turnaroundTime;
        struct NodeFCFS* next;
};

// Function to create a new Node (for Round Robin and Multi-Level Feedback Queue)
struct Node* createNode(int arrivalT, int burstT, int procNum){
    // Allocate memory and initialize values
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->processNum = procNum;
    newNode->arrivalTime = arrivalT;
    newNode->burstTime = burstT;
    newNode->remainingTime = burstT;
    newNode->next = NULL;
    return newNode;
}

// Function to create a new Node2 (for storing results)
struct Node2* createNode2(int arrivalT, int burstT, int procNum, int completionT){
    // Allocate memory and initialize values including calculated times
    struct Node2* newNode2 = (struct Node2*)malloc(sizeof(struct Node2));
    newNode2->processNum = procNum;
    newNode2->arrivalTime = arrivalT;
    newNode2->burstTime = burstT;
    newNode2->completionTime = completionT;
    newNode2->turnaroundTime = completionT - arrivalT;
    newNode2->waitingTime = completionT - arrivalT - burstT;
    newNode2->next = NULL;

    return newNode2;
}

// Function to calculate average waiting time for a list of Node2
float calculateAverageWaitingTime(struct Node2* head){
    // Iterate through the list and calculate the average
    struct Node2* currentNode = head;
    int sum = 0;
    int count = 0;
    float waitingTimeAverage = 0.0;

    while (currentNode!=NULL)
    {
        count++;
        sum += currentNode->waitingTime;
        currentNode = currentNode->next;
    }

    waitingTimeAverage = (float)sum/count;
    return waitingTimeAverage;
}

// Function to calculate average turnaround time for a list of Node2
float calculateAverageTurnaroundTime(struct Node2* head){
    // Iterate through the list and calculate the average
    struct Node2* currentNode = head;
    int sum = 0;
    int count = 0;
    float averageTurnaroundTime = 0.0;

    while (currentNode!=NULL)
    {
        count++;
        sum += currentNode->turnaroundTime;
        currentNode = currentNode->next;
    }

    averageTurnaroundTime = (float)sum/count;
    return averageTurnaroundTime;
}

// Function to calculate throughput for a list of Node2
float calculateThroughput(struct Node2* head, int NumOfProcesses){
    // Calculate the total time and divide the number of processes by it
    float throughput;
    struct Node2* currentNode;
    int minAT = head->arrivalTime, maxCT = head->completionTime, totalTime;

    currentNode = head;

    while (currentNode!=NULL)
    {
        if (currentNode->arrivalTime < minAT)
        {
            minAT = currentNode->arrivalTime;
        }
        if (currentNode->completionTime > maxCT)
        {
            maxCT = currentNode->completionTime;
        }
        
        currentNode = currentNode->next;
    }
    
    totalTime = maxCT - minAT;

    throughput = (float) NumOfProcesses/totalTime;

    return throughput;

}

// Function to sort Nodes based on arrival time (bubble sort)
struct Node* bubbleSort(struct Node* head){
    // Perform bubble sort on the linked list
    int isSwapped;
    struct Node* temp;
    do{
        isSwapped = 0;
        struct Node* tail2 = head;
        struct Node* previousNode = NULL;

        while(tail2 -> next != NULL) {
            struct Node* nextNode = tail2 -> next;

            if(tail2->arrivalTime > nextNode->arrivalTime) {
                if(previousNode != NULL){
                    previousNode->next = nextNode;
                }else{
                    head = nextNode;
                }

                tail2->next = nextNode->next;
                nextNode->next = tail2;

                temp = tail2;
                tail2 = nextNode;
                nextNode = temp;

                isSwapped = 1;
            }
            previousNode = tail2;
            tail2 = tail2->next;
        }
    }while(isSwapped);

    return head;
}

// Function to add a Node to the end of a queue
struct Node* enqueue(struct Node* tail, struct Node* Node){
    // Add Node to the end of the queue and return the new tail

    tail->next = Node;
    tail = tail->next;

    return tail;
}

// Function to remove a Node from the front of a queue
struct Node* dequeue(struct Node** head){
        // Remove the head Node from the queue and return it

    struct Node* node;
    struct Node* temp;

    node = *head;

    temp = *head;
    temp = temp->next;

    *head = temp;
    node->next = NULL;

    return node;
}

// Function to process nodes for Multi-Level Feedback Queue scheduling
struct Node2* Processing(struct Node* head, struct Node2* head2){
    struct Node* RRhead = createNode(head->arrivalTime,head->burstTime,head->processNum);
    struct Node* RRtail = RRhead;
    struct Node* temp;
    struct Node* FCFShead = NULL;
    struct Node* FCFStail = NULL;
    struct Node2* tail2;
    struct Node* RRCheck;
    struct Node* FCFSCheck;
    int currentT = 0;
    int quantum;



    temp = head->next;
    free(head);
    head = temp;

CheckQuantum:

    printf("What is the time quantum?\n");
    scanf("%d", &quantum);

    if (quantum <= 0)
    {
        printf("Invalid time quantum!!!\nPlease input valid time quantum(more than 0)\n");
        goto CheckQuantum;
    }

    if (RRhead->arrivalTime > currentT)
    {
        currentT = RRhead->arrivalTime;
    }
    

    do
    {

        printf("Current Time: %d\n", currentT);
        printf("The Round Robin queue\n");
        printf("|---------------------------------------|\n");
        printf("| Process   | Arrival time | Burst time |\n");
        printf("|-----------|--------------|------------|\n");

        RRCheck = RRhead;
        while(RRCheck != NULL) {
        printf("|%-11d|%-14d|%-12d|\n", RRCheck->processNum, RRCheck->arrivalTime, RRCheck->burstTime);
        RRCheck = RRCheck->next;
        }

        printf("The FCFS queue\n");
        printf("|---------------------------------------|\n");
        printf("| Process   | Arrival time | Burst time |\n");
        printf("|-----------|--------------|------------|\n");

        FCFSCheck = FCFShead;
        while(FCFSCheck != NULL) {
        printf("|%-11d|%-14d|%-12d|\n", FCFSCheck->processNum, FCFSCheck->arrivalTime, FCFSCheck->burstTime);
        FCFSCheck = FCFSCheck->next;
        }

        if (RRhead != NULL && RRhead->remainingTime > quantum)
        {
            RRhead->remainingTime = RRhead->remainingTime - quantum;
            currentT += quantum;
            while (head != NULL && head->arrivalTime <= currentT)
            {
                printf("head Arrival time: %d", head->arrivalTime);
                RRtail = enqueue(RRtail,dequeue(&head));
            }

            if (FCFShead == NULL)
            {
                FCFShead = dequeue(&RRhead);
                FCFStail = FCFShead;
            }else
            {
                FCFStail = enqueue(FCFStail,dequeue(&RRhead));
            }
        }else if(RRhead != NULL && RRhead->remainingTime <=  quantum)
        {
            currentT += RRhead->remainingTime;
            while (head != NULL && head->arrivalTime <= currentT)
            {
                RRtail = enqueue(RRtail,dequeue(&head));
            }

            if (head2==NULL)
            {
                head2 = createNode2(RRhead->arrivalTime,RRhead->burstTime,RRhead->processNum,currentT);
                tail2 = head2;
                free(dequeue(&RRhead));
            }else
            {
                tail2->next = createNode2(RRhead->arrivalTime,RRhead->burstTime,RRhead->processNum,currentT);
                tail2 = tail2->next;
                free(dequeue(&RRhead));
            }

        }else if (RRhead == NULL && FCFShead != NULL)
        {
            currentT += FCFShead->remainingTime;
            FCFShead->remainingTime = 0;
            if (head2==NULL)
            {
                head2 = createNode2(FCFShead->arrivalTime,FCFShead->burstTime,FCFShead->processNum,currentT);
                tail2 = head2;
                free(dequeue(&FCFShead));
            }else
            {
                tail2->next = createNode2(FCFShead->arrivalTime,FCFShead->burstTime,FCFShead->processNum,currentT);
                tail2 = tail2->next;
                free(dequeue(&FCFShead));
            }

            while (head != NULL && head->arrivalTime <= currentT)
            {
                RRtail = enqueue(RRtail,dequeue(&head));
            }
        }else if (head != NULL && RRhead == NULL && FCFShead == NULL)
        {
            currentT = head->arrivalTime;
            RRhead = dequeue(&head);
            RRtail = RRhead;
        }
    } while (head != NULL || RRhead != NULL || FCFShead != NULL);
    
    return head2;
}

// Main function to execute the Multi-level Feedback Queue (MLFQ) scheduling
void MLFQ(){
    // Get user input, sort processes, and process them using Multi-Level Feedback Queue scheduling
    // Print the results including average times and throughput

    int num=0;
    int arrivalT, burstT;

    struct Node* head;
    struct Node* tail2;

    struct Node2* head2 = NULL;

numCheck:

    printf("How many processes do you want to execute? \n");
    scanf("%d", &num);

    if (num <= 0)
            {
                printf("Invalid number of processes!!!\nPlease input valid number of processes(> 0)\n");
                goto numCheck;
            }

    for (int i = 1; i <= num; i++)
    {
        if(i==1)
        {
ATCheck:
            printf("Enter arrival time for process %d: ", i);
            scanf("%d", &arrivalT);

            if (arrivalT < 0)
            {
                printf("Invalid arrival time!!!\nPlease input valid arrival time(=> 0)\n");
                goto ATCheck;
            }
            
BTCheck:
            printf("Enter burst time for process %d: ", i);
            scanf("%d", &burstT);

            if (burstT <= 0)
            {
                printf("Invalid burst time!!!\nPlease input valid burst time(> 0)\n");
                goto BTCheck;
            }


            head = createNode(arrivalT, burstT, i);
            tail2 = head;
        }
        else
        {
ATCheck2:
            printf("Enter arrival time for process %d: ", i);
            scanf("%d", &arrivalT);

            if (arrivalT < 0)
            {
                printf("Invalid arrival time!!!\nPlease input valid arrival time(=> 0)\n");
                goto ATCheck2;
            }

BTCheck2:
            printf("Enter burst time for process %d: ", i);
            scanf("%d", &burstT);

            if (burstT <= 0)
            {
                printf("Invalid burst time!!!\nPlease input valid burst time(> 0)\n");
                goto BTCheck2;
            }

            tail2->next = createNode(arrivalT, burstT, i);
            tail2 = tail2->next;
        }
    }

    head = bubbleSort(head);
    

    printf("|---------------------------------------|\n");
    printf("| Process   | Arrival time | Burst time |\n");
    printf("|-----------|--------------|------------|\n");

    tail2 = head;
    while(tail2 != NULL) {
        printf("|%-11d|%-14d|%-12d|\n", tail2->processNum, tail2->arrivalTime, tail2->burstTime);
        tail2 = tail2->next;
    }
    
    head2 = Processing(head,head2);


    printf("|------------------------------------------------------------------------------------------|\n");
    printf("| Process   | Arrival time | Burst time | Completion Time | Turnaround Time | Waiting time |\n");
    printf("|-----------|--------------|------------|-----------------|-----------------|--------------|\n");

    struct Node2* currentNode2 = head2;
    while(currentNode2 != NULL){
        printf("|%-11d|%-14d|%-12d|%-17d|%-17d|%-14d|\n", currentNode2->processNum, currentNode2->arrivalTime, currentNode2->burstTime, currentNode2->completionTime, currentNode2->turnaroundTime, currentNode2->waitingTime);
        currentNode2 = currentNode2->next;
    }

    printf("|------------------------------------------------------------------------------------------|\n");
    printf("\nAverage Waiting Time: %.2f\n", calculateAverageWaitingTime(head2));
    printf("Throughput: %.2f\n", calculateThroughput(head2, num));
    printf("Average Turnaround Time: %.2f\n\n", calculateAverageTurnaroundTime(head2));
}

// Algorithm function used in Round Robin scheduling
struct Node2* algorithm(struct Node* head, struct Node2* head2){
    // Core Round Robin scheduling logic with time quantum
    int quantum;
    int currentTime=0;

    struct Node2* currentNode;
    struct Node* temp;
    struct Node* queueHead;
    struct Node* queueTail;

CheckQuantum2:

    printf("What is the time quantum?\n");
    scanf("%d", &quantum);

    if (quantum <= 0)
    {
        printf("Invalid time quantum!!!\nPlease input valid time quantum(more than 0)\n");
        goto CheckQuantum2;
    }
    
    
    currentTime = head->arrivalTime;

    queueHead = createNode(head->arrivalTime,head->burstTime,head->processNum);
    queueTail = queueHead;

    temp = head->next;
    free(head);
    head = temp;

    while (queueHead != NULL)
    {
        if (queueHead->remainingTime > quantum)
        {
            queueHead->remainingTime = queueHead->remainingTime - quantum;
            currentTime += quantum;
            while (head != NULL && head->arrivalTime <= currentTime)
            {
                queueTail->next = createNode(head->arrivalTime,head->burstTime,head->processNum);
                queueTail = queueTail->next;
                if (head->next!=NULL)
                {
                    temp = head->next;
                    free(head);
                    head = temp;
                }else
                {
                    free(head);
                    head = NULL;
                }
            }
            
            queueTail->next = queueHead;
            queueHead = queueHead->next;
            queueTail = queueTail->next;
            queueTail->next = NULL;
            if (queueHead==queueTail)
            {
                queueHead->next = NULL;
            }
            
        }else if(queueHead->remainingTime <=  quantum)
        {
            currentTime += queueHead->remainingTime;
            while (head != NULL && head->arrivalTime <= currentTime)
            {
                queueTail->next = createNode(head->arrivalTime,head->burstTime,head->processNum);
                queueTail = queueTail->next;
                if (head->next!=NULL)
                {
                    temp = head->next;
                    free(head);
                    head = temp;
                }else
                {
                    free(head);
                    head = NULL;
                }
            }
            if (head2==NULL)
            {
                head2 = createNode2(queueHead->arrivalTime,queueHead->burstTime,queueHead->processNum,currentTime);
                currentNode = head2;
            }else
            {
                currentNode->next = createNode2(queueHead->arrivalTime,queueHead->burstTime,queueHead->processNum,currentTime);
                currentNode = currentNode->next;
            }
            if (queueHead->next == NULL)
            {
                if (head!=NULL)
                {
                    queueHead->next = createNode(head->arrivalTime,head->burstTime,head->processNum);
                    queueTail = queueHead->next;
                }
                if (queueTail->arrivalTime >= currentTime)
                {
                    currentTime = queueTail->arrivalTime;
                }
            }
            temp = queueHead->next;
            free(queueHead);
            queueHead = temp;    
        }
    }
    return head2;
}

// Main function to execute Round Robin (RR) scheduling
void RR(){
    // Get user input, sort processes, and process them using Round Robin scheduling
    // Print the results including average times and throughput
    int num=0;
    int arrivalT, burstT;

    struct Node* head;
    struct Node* currentNode;

    struct Node2* head2 = NULL;

numCheck3:

    printf("How many processes do you want to execute? \n");
    scanf("%d", &num);

    if (num <= 0)
            {
                printf("Invalid number of processes!!!\nPlease input valid number of processes(> 0)\n");
                goto numCheck3;
            }

    for (int i = 1; i <= num; i++)
    {
        if(i==1)
        {
ATCheck3:
            printf("Enter arrival time for process %d: ", i);
            scanf("%d", &arrivalT);

            if (arrivalT < 0)
            {
                printf("Invalid arrival time!!!\nPlease input valid arrival time(=> 0)\n");
                goto ATCheck3;
            }
            
BTCheck3:
            printf("Enter burst time for process %d: ", i);
            scanf("%d", &burstT);

            if (burstT <= 0)
            {
                printf("Invalid burst time!!!\nPlease input valid burst time(> 0)\n");
                goto BTCheck3;
            }

            head = createNode(arrivalT, burstT, i);
            currentNode = head;
        }
        else
        {
ATCheck4:
            printf("Enter arrival time for process %d: ", i);
            scanf("%d", &arrivalT);

            if (arrivalT < 0)
            {
                printf("Invalid arrival time!!!\nPlease input valid arrival time(=> 0)\n");
                goto ATCheck4;
            }

BTCheck4:
            printf("Enter burst time for process %d: ", i);
            scanf("%d", &burstT);

            if (burstT <= 0)
            {
                printf("Invalid burst time!!!\nPlease input valid burst time(> 0)\n");
                goto BTCheck4;
            }

            currentNode->next = createNode(arrivalT, burstT, i);
            currentNode = currentNode->next;
        }
    }

    head = bubbleSort(head);
    

    printf("|---------------------------------------|\n");
    printf("| Process   | Arrival time | Burst time |\n");
    printf("|-----------|--------------|------------|\n");

    currentNode = head;
    while(currentNode != NULL) {
        printf("|%-11d|%-14d|%-12d|\n", currentNode->processNum, currentNode->arrivalTime, currentNode->burstTime);
        currentNode = currentNode->next;
    }

    head2 = algorithm(head, head2);

    printf("|------------------------------------------------------------------------------------------|\n");
    printf("| Process   | Arrival time | Burst time | Completion Time | Turnaround Time | Waiting time |\n");
    printf("|-----------|--------------|------------|-----------------|-----------------|--------------|\n");
    
    struct Node2* currentNode2 = head2;
    while(currentNode2 != NULL){
        printf("|%-11d|%-14d|%-12d|%-17d|%-17d|%-14d|\n", currentNode2->processNum, currentNode2->arrivalTime, currentNode2->burstTime, currentNode2->completionTime, currentNode2->turnaroundTime, currentNode2->waitingTime);
        currentNode2 = currentNode2->next;
    }

    printf("|------------------------------------------------------------------------------------------|\n");
    printf("\nAverage Waiting Time: %.2f\n", calculateAverageWaitingTime(head2));
    printf("Throughput: %.2f\n", calculateThroughput(head2, num));
    printf("Average Turnaround Time: %.2f\n\n", calculateAverageTurnaroundTime(head2));


}

// Function to create a new NodeFCFS (for First Come First Served scheduling)
struct NodeFCFS* createNodeFCFS(int arrivalT, int burstT, int procNum) {
    // Allocate memory and initialize values
    struct NodeFCFS* newNode = (struct NodeFCFS*)malloc(sizeof(struct NodeFCFS));
    newNode -> processNum = procNum;
    newNode -> arrivalTime = arrivalT;
    newNode -> burstTime = burstT;
    newNode -> next = NULL;
    return newNode;
}

// Function to calculate waiting time for First Come First Served scheduling
void calculateWaitTimeFCFS(struct NodeFCFS* head){
    // Calculate waiting time for each process in First Come First Served manner
    struct NodeFCFS* currentNode = head;
    struct NodeFCFS* previousNode = NULL;

    currentNode->waitingTime = 0;
    currentNode->completionTime = currentNode->arrivalTime + currentNode->burstTime + currentNode->waitingTime;

    previousNode = currentNode;
    currentNode = currentNode->next;

    while (currentNode!=NULL)
    {
    if(currentNode->arrivalTime<previousNode->completionTime){
        currentNode->waitingTime = previousNode->completionTime - currentNode->arrivalTime;
    }else{
        currentNode->waitingTime = 0;
    }

    currentNode->completionTime = currentNode->arrivalTime + currentNode->burstTime + currentNode->waitingTime;

    previousNode = currentNode;
    currentNode = currentNode->next;
    }
}

// Function to calculate turnaround time for First Come First Served scheduling
void calculateTurnaroundFCFS(struct NodeFCFS* head){
    // Calculate turnaround time for each process
    struct NodeFCFS* currentNode = head;

    while(currentNode!=NULL)
    {
        currentNode->turnaroundTime = currentNode->waitingTime+currentNode->burstTime;
        currentNode = currentNode->next;
    }
}

// Function to calculate average waiting time for First Come First Served scheduling
float calculateAverageWaitingTimeFCFS(struct NodeFCFS* head){
    // Iterate through the list and calculate the average waiting time
    struct NodeFCFS* currentNode = head;
    int sum = 0;
    int count = 0;
    float waitingTimeAverage = 0.0;

    while (currentNode!=NULL)
    {
        count++;
        sum += currentNode->waitingTime;
        currentNode = currentNode->next;
    }

    waitingTimeAverage = (float)sum/count;
    return waitingTimeAverage;
}

// Function to calculate average turnaround time for First Come First Served scheduling
float calculateAverageTurnaroundTimeFCFS(struct NodeFCFS* head){
    // Iterate through the list and calculate the average turnaround time
    struct NodeFCFS* currentNode = head;
    int sum = 0;
    int count = 0;
    float averageTurnaroundTime = 0.0;

    while (currentNode!=NULL)
    {
        count++;
        sum += currentNode->turnaroundTime;
        currentNode = currentNode->next;
    }

    averageTurnaroundTime = (float)sum/count;
    return averageTurnaroundTime;
}

// Function to calculate throughput for First Come First Served scheduling
float calculateThroughputFCFS(struct NodeFCFS* head, int NumOfProcesses){
    // Calculate the total time and divide the number of processes by it for First Come First Served
    float throughput;
    struct NodeFCFS* currentNode;
    int minAT = head->arrivalTime, maxCT = head->completionTime, totalTime;

    currentNode = head;

    while (currentNode!=NULL)
    {
        if (currentNode->arrivalTime < minAT)
        {
            minAT = currentNode->arrivalTime;
        }
        if (currentNode->completionTime > maxCT)
        {
            maxCT = currentNode->completionTime;
        }
        
        currentNode = currentNode->next;
    }
    
    totalTime = maxCT - minAT;

    throughput = (float) NumOfProcesses/totalTime;

    return throughput;

}

// Function to sort NodeFCFS based on arrival time (bubble sort)
struct NodeFCFS* bubbleSort1(struct NodeFCFS* head){
    // Perform bubble sort on the linked list based on arrival time

    int isSwapped;
    struct NodeFCFS* temp;
    do{
        isSwapped = 0;
        struct NodeFCFS* currentNode = head;
        struct NodeFCFS* previousNode = NULL;

        while(currentNode -> next != NULL) {
            struct NodeFCFS* nextNode = currentNode -> next;

            if(currentNode->arrivalTime > nextNode->arrivalTime) {
                if(previousNode != NULL){
                    previousNode->next = nextNode;
                }else{
                    head = nextNode;
                }

                currentNode->next = nextNode->next;
                nextNode->next = currentNode;

                temp = currentNode;
                currentNode = nextNode;
                nextNode = temp;

                isSwapped = 1;
            }
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
    }while(isSwapped);

    return head;
}

// Function to sort NodeFCFS based on process number (bubble sort)
struct NodeFCFS* bubbleSort2(struct NodeFCFS* head){
    // Perform bubble sort on the linked list based on process number
    int isSwapped;
    struct NodeFCFS* temp;
    do{
        isSwapped = 0;
        struct NodeFCFS* currentNode = head;
        struct NodeFCFS* previousNode = NULL;

        while(currentNode -> next != NULL) {
            struct NodeFCFS* nextNode = currentNode -> next;

            if(currentNode->processNum > nextNode->processNum) {
                if(previousNode != NULL){
                    previousNode->next = nextNode;
                }else{
                    head = nextNode;
                }

                currentNode->next = nextNode->next;
                nextNode->next = currentNode;

                temp = currentNode;
                currentNode = nextNode;
                nextNode = temp;

                isSwapped = 1;
            }
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
    }while(isSwapped);

    return head;
}

// Main function to execute First-Come, First-Served (FCFS) scheduling
void FCFS(){
    // Get user input, sort processes, and calculate times using FCFS scheduling
    // Print the results including average times and throughput
    int num = 0;
    int arrivalT, burstT;
    
    struct NodeFCFS* head;
    struct NodeFCFS* currentNode;

    
    printf("How many processes do you want to execute? \n");
    scanf("%d", &num);

    for(int i=1; i<=num; i++){
        if (i==1)
        {
            printf("Enter arrival time for process %d: ", i);
            scanf("%d", &arrivalT);

            printf("Enter burst time for process %d: ", i);
            scanf("%d", &burstT);
            head = createNodeFCFS(arrivalT, burstT, i);
            currentNode = head;
        }
        else
        {
        printf("Enter arrival time for process %d: ", i);
        scanf("%d", &arrivalT);

        printf("Enter burst time for process %d: ", i);
        scanf("%d", &burstT);

        currentNode->next = createNodeFCFS(arrivalT, burstT, i);
        currentNode = currentNode->next;

        }
        
        
    }

    head = bubbleSort1(head);

    calculateWaitTimeFCFS(head);

    calculateTurnaroundFCFS(head);

    head = bubbleSort2(head);

    printf("|------------------------------------------------------------------------------------------|\n");
    printf("| Process   | Arrival time | Burst time | Completion time | Waiting time | Turnaround time |\n");
    printf("|-----------|--------------|------------|-----------------|--------------|-----------------|\n");

    currentNode = head;
    while(currentNode != NULL) {
        printf("|%-11d|%-14d|%-12d|%-17d|%-14d|%-16d |\n", currentNode->processNum, currentNode->arrivalTime, currentNode->burstTime, currentNode->completionTime, currentNode->waitingTime, currentNode->turnaroundTime);
        currentNode = currentNode->next;
    }

    printf("|------------------------------------------------------------------------------------------|\n");
    printf("\nAverage Waiting Time: %.2f\n", calculateAverageWaitingTimeFCFS(head));
    printf("Throughput: %.2f\n", calculateThroughputFCFS(head, num));
    printf("Average Turnaround Time: %.2f\n\n", calculateAverageTurnaroundTimeFCFS(head));

}

int main(){

int selection = 0;
    // Main loop to select the scheduling algorithm

while (1)
{
    // Get user input and call the respective scheduling function

    printf("What would you like to do?\n1.FCFS\n2.RR\n3.MLFQ\n4.Exit\n");
    scanf("%d", &selection);

    switch (selection)
    {
    case 1:
        FCFS();
        break;
    
    case 2:
        RR();
        break;

    case 3:
        MLFQ();
        break;

    case 4:
        exit(1);
    default:
        break;
    }
}

}
