/* Ticketing System for Theme Park */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define NAME_LENGTH 30
#define ID_LENGTH 20
#define TICKETNO_LENGTH 30

typedef struct visitData_s{
    char *SrNo;
    char *ticketNo;
    char *Name;
    char *ID;
} visitData;

typedef struct ticket{  
    visitData visitor;
    struct ticket *nextPtr;
    struct ticket *prevPtr;     //added this to make it a doubly linked list struct
} Ticket;


FILE *ptrToOutput;      //declaring pointer to output.txt

//functions to interact with Tickets.txt and output.txt
Ticket *ReadNextFromFile(Ticket *start, FILE *pFile);   //read to check if there exists information already from Tickets.txt
Ticket *ReadListIn(Ticket *start);              
void printToTxtFile(Ticket *start);         //prints information into Tickets.txt

//functions to interact with the linked list within the program
void printLinkedList(Ticket *start);        //prints the list
void printSorted(Ticket *start);            //prints the list (sorted alphabetically)
void print_reverse(Ticket* start);
Ticket* MakeNewTicket(Ticket *prev);        //function to create a ticket node
Ticket* addTicketToStart(Ticket *start);    //function to add the created node to start of list
Ticket* addTicketToEnd(Ticket *start);      //function to add the created node to the end of lise
Ticket *printNameDuplicate(Ticket *startPtr, char*input);   //function to print names in case two or more same names are found when delete is called
Ticket *printIDDuplicate(Ticket *startPtr, char*input);     //function to print ids in case two or more same ids are found when delete is called
Ticket *searchListbyName(Ticket *start);        //search linked-list for information by name
Ticket *searchListbyID(Ticket *start);          //search linked-list for information by ID
Ticket *searchListbyTicketNo(Ticket *start);    //search linked-list for information by Ticket No.
Ticket *deleteFromListbyName(Ticket *startPtr); //delete ticket from linked-list by name
Ticket *deleteFromListbyID(Ticket *startPtr);   //delete ticket from linked-list by id
Ticket *deleteFromListbyTicketNo(Ticket *startPtr); //delete ticket from linked-list by ticket.no
void FreeList(Ticket *start);                   //frees the memory allocated
void printTicketsSold();                    //function to print tickets sold

//functions to change string formats...
void stringuppercase(char *arr);                //make the whole string uppercase
void trim_nl (char *arr, int length);           //trims the newline character
void spaceReplace (char *arr, int length);      //replaces 'space' with '_'
void underscoreReplace (char *arr, int length); //replaces '_' with 'space'
char *generateTicketNo(char *name, int length); //makes the ticket no. formatting

void printMenu(){
    printf("Insert '1'      - to print the lists of visitors\n");
    printf("Insert '2'      - to add visitor info.\n");
    printf("Insert '3'      - to search for visitor\n");
    printf("Insert '4'      - to delete visitor info by name.\n");
    printf("Insert '5'      - to delete visitor info by ID.\n");
    printf("Insert '6       - to delete visitor info by ticket no.\n");
    printf("Insert '7'      - to print tickets sold.\n");
    printf("Insert 'quit'   - to exit program\n\n");
}

int main (){
    Ticket *start = NULL;
    start = ReadListIn(start);  //read the file to get the information ready
    char command[16];
    char input[16];
    fflush(stdin);
    printf("\n*****************************************\nWelcome to ThemePark Ticketing System\n*****************************************\n");
    printMenu();
    printf("Instruction: ");
    while(fgets( input, 16, stdin)) {
        sscanf(input,"%s", command);
        if(strncmp(command, "quit",4) == 0){        //quit sequence
            printf("\n\nExiting...\n");
            break;
        } 

        else if(strncmp(command, "menu",4) == 0){   //print menu sequence
            puts("");
            printMenu();
        } 
        
        else if(strncmp(command, "1",1) == 0){      // 1. display visitor list sequence
            char choice = '0';
            puts("");
            printf("Print selected\nEnter 'A' to display original\nEnter 'B' to display in alphabetical order.\nInput: ");
            scanf("%c",&choice);
            fflush(stdin);
            if(toupper(choice) == 'A'){
                printLinkedList(start);
            }else if(toupper(choice) == 'B'){
                printSorted(start);
            }else{
                printLinkedList(start);
            }
        } 
        
        else if(strncmp(command, "2",1) == 0){      // 2. add new visitor sequence
            char str1[100]="\0";
            char str2[100]="\0";
            char totalvisitor[100]="\0";
            FILE *ptrFILE;
            ptrFILE = fopen("Tickets.txt","r");
            int visitorcount = 0;
            if(ptrFILE != NULL){
                fscanf(ptrFILE,"%s %s %s\n",str1,str2,totalvisitor);
                visitorcount = atoi(totalvisitor);
                puts("");
            }   
            if(visitorcount<40){
                start = addTicketToEnd(start);
                printToTxtFile(start);
            } else if(visitorcount == 40){
                printf("Tickets have been sold out, there are already 40 visitors.\n");
            }
            fclose(ptrFILE);
        } 
        
        else if(strncmp(command, "3",1) == 0){      // 3. search for visitor sequence
            char choice = '0';
            puts("");
            printf("Enter 'A' to search by Name\nEnter 'B' to search by ID\nEnter 'C' to search by Ticket No.\nChoice: ");
            scanf("%c",&choice);
            fflush(stdin);
            if(toupper(choice) == 'A'){             //search by name
                printf("Name: ");
                searchListbyName(start);
            }else if(toupper(choice) == 'B'){       //search by id
                printf("ID: ");
                searchListbyID(start);
            }else if(toupper(choice) == 'C'){       //search by ticket no.
                printf("Ticket No.: ");
                searchListbyTicketNo(start);
            }else{
                printf("Error: command (%c) cannot be executed\n",choice);   
            }
        } 
        
        else if(strncmp(command, "4",1) == 0){      // 4. delete by name sequence
            start = deleteFromListbyName(start);
            printToTxtFile(start);
        } 
        
        else if(strncmp(command, "5",1) == 0){      // 5. delete by id sequence
            start = deleteFromListbyID(start);
            printToTxtFile(start);
        } 

        else if(strncmp(command, "6",1) == 0){      // 6. delete by ticket no. sequence
            start = deleteFromListbyTicketNo(start);
            printToTxtFile(start);
        } 
        
        else if(strncmp(command, "7",1) == 0){      // 7. display ticket(s) sold sequence
            printTicketsSold();
        }
    printf("\nInsert 'menu'   - to show lists of commands.\n*******************************************************\nInstruction: ");
    }
    FreeList(start);
    return 0;
}

void trim_nl (char *arr, int length){			//to remove the \n character after using fgets
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}
void spaceReplace (char *arr, int length){      //replace space char with underscore char used to change name input, ex: Yasir Hafeez -> Yasir_Hafeez
    int i;
    for(i = 0; i< length; i++){  
        if(arr[i] == ' '){
            arr[i] = '_';  
        }
    }  
}
void underscoreReplace (char *arr, int length){ //replace underscore with space
    int i;
    for(i = 0; i< length; i++){  
        if(arr[i] == '_'){
            arr[i] = ' ';  
        }
    }  
}

void stringuppercase(char *arr) //makes string into uppercase
{
	int i;
    	for(i=0;arr[i];i++)  
        {
          if(arr[i]>=97 && arr[i]<=122)
          arr[i]-=32;
 	    }	
}

char *generateTicketNo(char *name, int length)  //makes the ticket no. format
{   
    trim_nl(name,NAME_LENGTH);
    char oriName[NAME_LENGTH];
    strcpy(oriName,name);
    underscoreReplace(oriName, NAME_LENGTH);
    int check_name_length = strlen(oriName);
    char temp[NAME_LENGTH] = "\0";
    char placeholder[NAME_LENGTH];
    strcpy(placeholder,oriName);
    int spaceChar_needed = 6-check_name_length;     //check how many spaces to append to the left
    if(check_name_length<6){
        for(int i=0; i<spaceChar_needed; i++)
        {
            strcat(temp," ");   //create string with appropriate number of spaces suitable
        }
        strcat(temp,placeholder);   // x numbers of spaces (temp) + name(placeholder)
    }
    else{
        strcpy(temp,oriName);
    }
    int a = temp[0]; int b = temp[1]; int c = temp[2]; int d = temp[3]; int e = temp[4]; int f = temp[5];
    char buffer[50];
    char *p;
    p = buffer;
    sprintf(buffer,"%d%d%d%d%d%d",a,b,c,d,e,f);
    return p;
}

Ticket *addTicketToStart(Ticket *startPtr){         //adds node to an empty list
    Ticket *newTicket = MakeNewTicket(NULL);
    if(startPtr != NULL)
    return newTicket;
}

Ticket *addTicketToEnd(Ticket *startPtr){           //adds node to end of list
    Ticket *returnPtr = startPtr;
    Ticket *newTicket = NULL;
    if(startPtr == NULL){       //list is empty
        newTicket = addTicketToStart(startPtr);
        returnPtr = newTicket;  //points to the address of the new node
    }
    else{                       //list is not empty
        Ticket *indexTicket = startPtr;
        while(indexTicket->nextPtr != NULL){
            indexTicket = indexTicket->nextPtr;     //traverse the list until it reaches the tail node
        }
        newTicket = MakeNewTicket(indexTicket);
        indexTicket->nextPtr = newTicket;           //previous last element's next points to the newly allocated space, making the newly allocated the new end of list
        newTicket->nextPtr = NULL;                  //end ticket's nextPtr points to null
        newTicket->prevPtr = indexTicket;           //end ticket's prevptr points to previous last ticket
    }
    return returnPtr;
}

Ticket* MakeNewTicket(Ticket *prev){            // creates a new doubly linked list node
    char name_input[100]="\0";
    char id_input[100]="\0";
    char isLocal = '0';
    //asks user to input necessary information
    while(1){
        fflush(stdin);
        printf("Enter Name: ");             //gets name input
        fgets(name_input,100,stdin);
        trim_nl(name_input,100);
        spaceReplace(name_input,100);
        if(strlen(name_input)<=NAME_LENGTH){        //name cannot be more than 30 chars long
            break;
        }printf("Error: Name is too long, please re-enter name.\n");    
        strcpy(name_input,"\0");
    }
    while(1){
        fflush(stdin);
        printf("Are you local? (Y/N) ");
        scanf("%c",&isLocal);
        if((isLocal == 'Y') || (isLocal == 'y')){
            break;
        }else if((isLocal == 'N') || (isLocal == 'n')){
            break;
        }printf("Error interpreting input. Please enter 'Y' for local or 'N' for international.\n");
        isLocal = '0';
    }
    while(1){
        fflush(stdin);
        int length = 0;
        if((isLocal == 'Y') || (isLocal == 'y')){
            length = NAME_LENGTH -2;                    //ID length max is 20, 'I' + 'C' + 18 numbers = 20
        }if((isLocal == 'N') || (isLocal == 'n')){      
            length = NAME_LENGTH - 4;                   //ID length max is 20, 'P' + 'A' + 'S' + 'S' + 16 numbers = 20
        }
        printf("Enter ID: ");
        fgets(id_input,100,stdin);
        if(strlen(id_input)<=length){
            break;
        }printf("Error: ID is too long, please re-enter ID.\n");
        strcpy(id_input,"\0");
    }
    char name[NAME_LENGTH] = "\0";
    strncpy(name,name_input,NAME_LENGTH);
    char id[ID_LENGTH] = "\0";
    if((isLocal == 'Y') || (isLocal == 'y')){
        strcat(id, "IC");
        strcat(id,id_input);
    }else if((isLocal == 'N') || (isLocal == 'n')){
        strcat(id, "PASS");
        strcat(id,id_input);
    }
    trim_nl(id,ID_LENGTH);
    printf("The information of the visitor %s %s is recorded.\n",name,id);
    ptrToOutput = fopen("output.txt","a");
    fprintf(ptrToOutput,"The information of the visitor %s %s is recorded.\n",name,id);
    fclose(ptrToOutput);
    fflush(stdin);
    char serialNumber[3];
    char ticketnum[TICKETNO_LENGTH]="\0";
    int serial = 1;
    if(prev == NULL){       //list starts from here
        sprintf(serialNumber,"0%d",serial);
    }else{  
        serial = (atoi(prev->visitor.SrNo) + 1);
        if (serial >9){
            sprintf(serialNumber,"%d",serial);
        }else if(serial <=9){
            sprintf(serialNumber,"0%d",serial);
        }
    }
    char *pticket = generateTicketNo(name_input,NAME_LENGTH);
    strcat(ticketnum, pticket);
    strcat(ticketnum, serialNumber);

    visitData data = {
        .Name = name_input,
        .ID = id,
        .SrNo = serialNumber,
        .ticketNo = ticketnum
    };

    Ticket *newTicket = malloc(sizeof(Ticket));                 //allocates necessary memory to store data
    newTicket->visitor.ID = malloc(strlen(data.ID)+1);          // malloc strlen + 1 to include '\0'
    newTicket->visitor.Name = malloc(strlen(data.Name)+1);
    newTicket->visitor.ticketNo = malloc(strlen(data.ticketNo)+1);
    newTicket->visitor.SrNo = malloc(strlen(data.SrNo)+1);
    strcpy(newTicket->visitor.ID,data.ID);
    strcpy(newTicket->visitor.Name,data.Name);
    strcpy(newTicket->visitor.ticketNo,data.ticketNo);
    strcpy(newTicket->visitor.SrNo,data.SrNo);
    newTicket->prevPtr = NULL;
    newTicket->nextPtr = NULL;
    return newTicket;
}

void printLinkedList(Ticket *start) {       //Function to print out the linked list as is
    Ticket *curr = start;
    Ticket *counter = start;
    int VisitorCount = 0;
    while(counter != NULL){     //traverse the linked list to check how many items in list
        VisitorCount++;
        counter = counter->nextPtr;
    }
    printf("Total Visitors: %d\n",VisitorCount);
    while (curr != NULL) {
        printf("%s %s %s %s\n", curr->visitor.SrNo, curr->visitor.ticketNo, curr->visitor.Name, curr->visitor.ID);
        curr = curr->nextPtr;
    }
}

void printSorted(Ticket *start){        //Function to print the linked list in a sorted way
    
    Ticket *temp = NULL;        //Create a temporary linked list to store and sort the data
    Ticket *current = start;    
    while (current != NULL) // Traverse the original linked list and insert each node into the temporary linked list
    {
        char tempname[100] = "\0";
        char tempid[100] = "\0";
        char tempsrno[100] = "\0";
        char tempticketno[100] = "\0";
        Ticket *node = (Ticket*)malloc(sizeof(Ticket));     // Create a new node for the current data
        visitData data = {
            .Name = tempname,
            .ID = tempid,
            .SrNo = tempsrno,
            .ticketNo = tempticketno
        };
        strcpy(tempsrno, current->visitor.SrNo);
        strcpy(tempticketno, current->visitor.ticketNo);
        strcpy(tempname, current->visitor.Name);
        strcpy(tempid, current->visitor.ID);
        node->visitor.ID = malloc(strlen(data.ID)+1);
        node->visitor.Name = malloc(strlen(data.Name)+1);
        node->visitor.ticketNo = malloc(strlen(data.ticketNo)+1);
        node->visitor.SrNo = malloc(strlen(data.SrNo)+1);
        strcpy(node->visitor.SrNo, data.SrNo);
        strcpy(node->visitor.ticketNo, data.ticketNo);
        strcpy(node->visitor.Name, data.Name);
        strcpy(node->visitor.ID, data.ID);
        node->nextPtr = NULL;
        node->prevPtr = NULL;
        
        if (temp == NULL){      //If the temporary linked list is empty, insert the node at the beginning
            node->nextPtr = temp;
            temp = node;
        }
        else{       //else, find the position to insert the node in the sorted temporary linked list
            Ticket *prev = NULL;
            Ticket *curr = temp;
            while (curr != NULL && strcmp(curr->visitor.Name, node->visitor.Name) > 0){
                prev = curr;
                curr = curr->nextPtr;
            }
            node->nextPtr = curr;       //Inserts the node at the correct position
            
            if (prev == NULL)
                temp = node;
            else
                prev->nextPtr = node;
        }
        current = current->nextPtr;     //Moves to the next node in the original linked list
    }
    
    current = temp;     //Printing the sorted data from the temporary linked list
    int count = 0;
    printf("___________________________________________________\n");
    while (current != NULL){
        ++count;
        current = current->nextPtr;     //Move to the next node in the temporary linked list
    }
    printf("Total Visitors:%d\n",count);
    current = temp;
    print_reverse(current);
    printf("___________________________________________________\n");
}

void print_reverse(Ticket* start)
{   
    if (start == NULL) {     //Check if the list is empty
        return;
    }
    print_reverse(start->nextPtr);      //Print the rest of the list first, recursively
    printf("%s %s %s %s\n",start->visitor.SrNo,start->visitor.ticketNo,start->visitor.Name,start->visitor.ID);
}

void FreeList(Ticket *start){           //function to free memory
    Ticket *freeobj = start;
    Ticket *holdobj = NULL;
    while(freeobj != NULL){
        holdobj = freeobj->nextPtr;     //frees memory that was allocated
        free(freeobj);
        freeobj = holdobj;
    }
    printf("\nFree memory...\n");
}

Ticket *printNameDuplicate(Ticket *startPtr, char*input){       //function to print duplicates when duplicates are found
    Ticket *ticketRef = startPtr;
    while(ticketRef != NULL){
        char tempName[NAME_LENGTH]="\0";
        strncpy(tempName,ticketRef->visitor.Name,strlen(ticketRef->visitor.Name));
        spaceReplace(tempName,strlen(tempName));
        stringuppercase(tempName);
        char printedInfo[100]="\0";
        if(strncmp(input,tempName, strlen(tempName)) == 0){     //prints duplicates
            sprintf(printedInfo,"%s %s %s %s\n",ticketRef->visitor.SrNo,ticketRef->visitor.ticketNo,ticketRef->visitor.Name,ticketRef->visitor.ID);
            printf("%s",printedInfo);
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"%s",printedInfo);
            fclose(ptrToOutput);
        }
        ticketRef = ticketRef->nextPtr;
    }
    return startPtr;
}

Ticket *printIDDuplicate(Ticket *startPtr, char*input){         //function to print duplicates when duplicates are found
    Ticket *ticketRef = startPtr;
    while(ticketRef != NULL){
        char tempID[ID_LENGTH]="\0";
        strncpy(tempID,ticketRef->visitor.ID,strlen(ticketRef->visitor.ID));
        spaceReplace(tempID,strlen(tempID));
        stringuppercase(tempID);
        char printedInfo[100]="\0";
        if(strncmp(input,tempID, strlen(tempID)) == 0){     //prints duplicate
            sprintf(printedInfo,"%s %s %s %s\n",ticketRef->visitor.SrNo,ticketRef->visitor.ticketNo,ticketRef->visitor.Name,ticketRef->visitor.ID);
            printf("%s",printedInfo);
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"%s",printedInfo);
            fclose(ptrToOutput);
        }
        ticketRef = ticketRef->nextPtr;
    }
    return startPtr;
}


Ticket *searchListbyName(Ticket *startPtr){             //function to search list
    printf("Enter Name to be Searched: ");
    char input[NAME_LENGTH];
    fgets(input, NAME_LENGTH, stdin);
    spaceReplace(input,NAME_LENGTH);    //make name same format as the input name
    stringuppercase(input);
    trim_nl(input,TICKETNO_LENGTH);
    Ticket *ticketRef = startPtr;
    Ticket *ticketToDelete = NULL;
    bool isPrinted = false;
    while(ticketRef != NULL){
        char tempName[NAME_LENGTH]="\0";
        strncpy(tempName,ticketRef->visitor.Name,strlen(ticketRef->visitor.Name));
        spaceReplace(tempName,strlen(tempName));
        stringuppercase(tempName);
        char printedInfo[100];
        if((strncmp(input,tempName, strlen(tempName)) == 0) && (isPrinted == false) && (strlen(input) == strlen(tempName))){
            printf("\nThe following record of the visitor is found:\n");
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"The following record of the visitor is found:\n");
            fclose(ptrToOutput);
            isPrinted = true;
        }
        if((strncmp(input,tempName, strlen(tempName)) == 0) && (strlen(input) == strlen(tempName))){
            sprintf(printedInfo,"%s %s %s %s\n",ticketRef->visitor.SrNo,ticketRef->visitor.ticketNo,ticketRef->visitor.Name,ticketRef->visitor.ID);
            printf("%s",printedInfo);
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"%s",printedInfo);
            fclose(ptrToOutput);
        }
        ticketRef = ticketRef->nextPtr;
    }
    if (isPrinted == false){
        printf("Name not found.\n");
    }
    isPrinted = false;
    return startPtr;
}

Ticket *searchListbyID(Ticket *startPtr){       //function to search list
    printf("Enter ID to be Searched: ");
    char input[ID_LENGTH];
    fgets(input, ID_LENGTH, stdin);
    stringuppercase(input);
    trim_nl(input,ID_LENGTH);
    Ticket *ticketRef = startPtr;
    Ticket *ticketToDelete = NULL;
    bool isPrinted = false;
    while(ticketRef != NULL){
        char tempID[ID_LENGTH]="\0";
        strncpy(tempID,ticketRef->visitor.ID,strlen(ticketRef->visitor.ID));
        spaceReplace(tempID,strlen(tempID));
        stringuppercase(tempID);
        char printedInfo[100];
        if((strncmp(input,tempID, strlen(tempID)) == 0) && (isPrinted == false) && (strlen(input)==strlen(tempID))){
            printf("\nThe following record of the visitor is found:\n");
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"The following record of the visitor is found:\n");
            fclose(ptrToOutput);
            isPrinted = true;
        }
        if((strncmp(input,tempID, strlen(tempID)) == 0) && (strlen(input)==strlen(tempID))){
            sprintf(printedInfo,"%s %s %s %s\n",ticketRef->visitor.SrNo,ticketRef->visitor.ticketNo,ticketRef->visitor.Name,ticketRef->visitor.ID);
            printf("%s",printedInfo);
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"%s",printedInfo);
            fclose(ptrToOutput);
        }
        ticketRef = ticketRef->nextPtr;
    }
    if (isPrinted == false){
        printf("ID not found.\n");
    }
    isPrinted = false;
    return startPtr;
}

Ticket *searchListbyTicketNo(Ticket *startPtr){     //function to search list
    printf("Enter Ticket No. to be Searched: ");
    char input[TICKETNO_LENGTH];
    fgets(input, TICKETNO_LENGTH, stdin);
    spaceReplace(input,TICKETNO_LENGTH);   
    trim_nl(input,TICKETNO_LENGTH);
    Ticket *ticketRef = startPtr;
    Ticket *ticketToDelete = NULL;
    bool isPrinted = false;
    while(ticketRef != NULL){
        char tempticketno[TICKETNO_LENGTH]="\0";
        strncpy(tempticketno,ticketRef->visitor.ticketNo,strlen(ticketRef->visitor.ticketNo));
        spaceReplace(tempticketno,strlen(tempticketno));
        stringuppercase(tempticketno);
        char printedInfo[100];
        if((strncmp(input,tempticketno, strlen(tempticketno)) == 0) && (isPrinted == false) && (strlen(input) == strlen(tempticketno))){
            printf("\nThe following record of the visitor is found:\n");
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"The following record of the visitor is found:\n");
            fclose(ptrToOutput);
            isPrinted = true;
        }
        if((strncmp(input,tempticketno, strlen(tempticketno)) == 0) && (strlen(input) == strlen(tempticketno))){
            sprintf(printedInfo,"%s %s %s %s\n",ticketRef->visitor.SrNo,ticketRef->visitor.ticketNo,ticketRef->visitor.Name,ticketRef->visitor.ID);
            printf("%s",printedInfo);
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"%s",printedInfo);
            fclose(ptrToOutput);
        }
        ticketRef = ticketRef->nextPtr;
    }
    if (isPrinted == false){
        printf("Ticket No. not found.\n");
    }
    isPrinted = false;
    return startPtr;
}

Ticket *deleteFromListbyName(Ticket *startPtr){     //function to delete a node from list by name
    printf("Enter Name to Delete: ");
    char input[NAME_LENGTH];
    fgets(input, NAME_LENGTH, stdin);
    spaceReplace(input,NAME_LENGTH);
    stringuppercase(input);
    trim_nl(input,NAME_LENGTH);
    Ticket *ticketRef = startPtr;
    Ticket *ticketToDelete = NULL;
    int repeatCounter = -1;

    while(ticketRef != NULL){                  //traverse the list to check if id inputed is in the list, and the amount of times it appeared
        char tempName[NAME_LENGTH]="\0";
        strncpy(tempName,ticketRef->visitor.Name,strlen(ticketRef->visitor.Name));
        spaceReplace(tempName,strlen(tempName));
        stringuppercase(tempName);
        if((strncmp(input,tempName, strlen(tempName)) == 0) && (strlen(input) == strlen(tempName))){
            ticketToDelete = ticketRef;
            repeatCounter = repeatCounter+1;
        }
        ticketRef = ticketRef->nextPtr;
    }
    if((repeatCounter == 0)){
        printf("The visitor: %s information is deleted.\n",ticketToDelete->visitor.Name);
        ptrToOutput = fopen("output.txt","a");
        fprintf(ptrToOutput,"The visitor: %s information is deleted.\n",ticketToDelete->visitor.Name);
        fclose(ptrToOutput);
        if((startPtr != NULL) && (ticketToDelete == startPtr)){         //in case if the deleted is at the first node
            if(ticketToDelete->nextPtr != NULL){                        //if list have more than one data, the next node's previous node becomes NULL
                ticketToDelete->nextPtr->prevPtr = NULL;
                startPtr = ticketToDelete->nextPtr;
            } else{
                startPtr = NULL;                                        //if list only has one element, clears the list
            }
        } else{     //if not the first node, move pointers of previous node's next node to the next node of the deleted node and next node's previous node to the deleted node's previous node
            if(ticketToDelete != NULL){
                if(ticketToDelete->prevPtr != NULL){
                    ticketToDelete->prevPtr->nextPtr = ticketToDelete->nextPtr;
                }
                if(ticketToDelete->nextPtr != NULL){
                    ticketToDelete->nextPtr->prevPtr = ticketToDelete->prevPtr;
                }
            }
        }
    }
    if(repeatCounter == -1)
        printf("Name not found!\n");
    if(ticketToDelete != NULL){
        ticketToDelete = NULL;
        free(ticketToDelete);
    }
    if(repeatCounter >=1){
        printf("The following visitors have same names, Please enter the ID to be deleted:\n");
        printNameDuplicate(startPtr, input);
        startPtr = deleteFromListbyID(startPtr);
    }
    return startPtr;
}

Ticket *deleteFromListbyID(Ticket *startPtr){           //function to delete a node from list by id
    printf("Enter ID to Delete: ");
    char input[ID_LENGTH];
    fgets(input, ID_LENGTH, stdin);
    stringuppercase(input);
    trim_nl(input,ID_LENGTH);
    Ticket *ticketRef = startPtr;
    Ticket *ticketToDelete = NULL;
    int repeatCounter = -1;

    while(ticketRef != NULL){           //traverse the list to check if id inputed is in the list, and the amount of times it appeared
        char tempID[ID_LENGTH]="\0";
        strncpy(tempID,ticketRef->visitor.ID,strlen(ticketRef->visitor.ID));
        stringuppercase(tempID);
        if((strncmp(input,tempID, strlen(tempID)) == 0) && (strlen(input)==strlen(tempID))){
            ticketToDelete = ticketRef;
            repeatCounter = repeatCounter + 1;
        }
        ticketRef = ticketRef->nextPtr;
    }
    if(repeatCounter == 0){
        printf("The visitor with ID: %s, information is deleted.\n",ticketToDelete->visitor.ID);
        ptrToOutput = fopen("output.txt","a");
        fprintf(ptrToOutput,"The visitor with ID: %s, information is deleted.\n",ticketToDelete->visitor.ID);
        fclose(ptrToOutput);
        if(startPtr != NULL && ticketToDelete == startPtr){     //in case if the deleted is at the first node
            if(ticketToDelete->nextPtr != NULL){                //if list have more than one data, the next node's previous node becomes NULL
            ticketToDelete->nextPtr->prevPtr = NULL;
            startPtr = ticketToDelete->nextPtr;
            } else{
            startPtr = NULL;                                    //if list only has one element, clears the list
            }
        } else{     //if not the first node, move pointers of previous node's next node to the next node of the deleted node and next node's previous node to the deleted node's previous node
            if((ticketToDelete != NULL)){
                if(ticketToDelete->prevPtr != NULL){
                    ticketToDelete->prevPtr->nextPtr = ticketToDelete->nextPtr;
                }
                if(ticketToDelete->nextPtr != NULL){
                    ticketToDelete->nextPtr->prevPtr = ticketToDelete->prevPtr;
                }
            }
        }
    }
    if(repeatCounter == -1)
        printf("ID not found!\n");

    if(ticketToDelete != NULL){
        ticketToDelete = NULL;
        free(ticketToDelete);
    }
    if(repeatCounter >=1){
        printf("The following visitors have same ID, Please enter the ticket.no to be deleted:\n");
        printIDDuplicate(startPtr, input);
        startPtr = deleteFromListbyTicketNo(startPtr);
    }
    return startPtr;
}

Ticket *deleteFromListbyTicketNo(Ticket *startPtr){         //function to delete a node from list by ticket no.
    printf("Enter Ticket.No to Delete: ");
    char input[TICKETNO_LENGTH];
    fgets(input, TICKETNO_LENGTH, stdin);
    trim_nl(input,TICKETNO_LENGTH);
    Ticket *ticketRef = startPtr;
    Ticket *ticketToDelete = NULL;

    while(ticketRef != NULL){               //traverse the list to check if ticket no. inputed is in the list
        char tempticketno[TICKETNO_LENGTH]="\0";
        strncpy(tempticketno,ticketRef->visitor.ticketNo,strlen(ticketRef->visitor.ticketNo));
        stringuppercase(tempticketno);
        if((strncmp(input,tempticketno, strlen(tempticketno)) == 0) && (strlen(input) == strlen(tempticketno))){
            ticketToDelete = ticketRef;
            printf("The visitor with Ticket No.: %s, information is deleted.\n",ticketToDelete->visitor.ticketNo);
            ptrToOutput = fopen("output.txt","a");
            fprintf(ptrToOutput,"The visitor with Ticket No.: %s, information is deleted.\n",ticketToDelete->visitor.ticketNo);
            fclose(ptrToOutput);
            break;
        }
        ticketRef = ticketRef->nextPtr;
    }
    if((startPtr != NULL) && (ticketToDelete == startPtr)){     //in case if the deleted is at the first node
        if(ticketToDelete->nextPtr != NULL){                    //if list have more than one data, the next node's previous node becomes NULL
            ticketToDelete->nextPtr->prevPtr = NULL;
            startPtr = ticketToDelete->nextPtr;
        } else{                                                 //if list only has one element, clears the list
            startPtr = NULL;
        }
    } else{         //if not the first node, move pointers of previous node's next node to the next node of the deleted node and next node's previous node to the deleted node's previous node
        if(ticketToDelete != NULL){
            if(ticketToDelete->prevPtr != NULL){
                ticketToDelete->prevPtr->nextPtr = ticketToDelete->nextPtr;
            }
            if(ticketToDelete->nextPtr != NULL){
                ticketToDelete->nextPtr->prevPtr = ticketToDelete->prevPtr;
            }
        }
    }
    if(ticketToDelete == NULL){
        printf("Error: Ticket No. (%s) is not found in database.\n",input);
    }
    else if(ticketToDelete != NULL){
        ticketToDelete = NULL;
        free(ticketToDelete);
    }
    return startPtr;
}

void printToTxtFile(Ticket *start){         //function to write to Tickets.txt
    FILE *pFile;
    pFile = fopen("Tickets.txt","w");
    Ticket *countTicket = start;
    Ticket *indexTicket = start;
    int count = 0;

    while(countTicket != NULL){
        count++;
        countTicket = countTicket->nextPtr;
    }
    int visitorsAmt = count;
    if(pFile != NULL){
        fprintf(pFile,"Total Visitors: %d\n",visitorsAmt);
        for(int i = 0; i<count; i++)
        {
            fprintf(pFile,"%s %s %s %s\n",indexTicket->visitor.SrNo,indexTicket->visitor.ticketNo,indexTicket->visitor.Name,indexTicket->visitor.ID);
            indexTicket = indexTicket->nextPtr;
        }
        fclose(pFile);
        pFile = NULL;
    } else {
        printf("CREATING FILE...\n");
    }
}

Ticket *ReadListIn(Ticket *start){              //function to read the Tickets.txt file to load info to the program
    FILE *pFile;
    pFile = fopen("Tickets.txt","r");
    if(pFile != NULL){
        rewind(pFile);
        FreeList(start);
        start = NULL;
        char buffer1[200];
        char buffer2[200];
        char numEntries[5];
        fscanf(pFile,"%s %s %s\n",buffer1,buffer2,numEntries);
        int count = atoi(numEntries);

        printf("%d entries in Tickets.txt\n",count);
        int loop = 0;
        for(loop = 0; loop<count; loop++){
            start = ReadNextFromFile(start, pFile);
        } 
    } else {
        printf("\n\nThere are 0 visitors, 40 tickets left\n");
    }
    return start;
}

Ticket *ReadNextFromFile(Ticket *start, FILE *pFile){       //reads list node by node
    size_t returnValue;
    char name_copy[100] = "\0";
    char id_copy[100] = "\0";
    char serialNumber_copy[100] = "\0";
    char ticketNumber_copy[100] = "\0";
    if(start == NULL){          //create the first ticket of linked-list, if ticket exists in Tickets.txt
        start = malloc(sizeof(Ticket));
        fscanf(pFile,"%s %s %s %s\n",serialNumber_copy,ticketNumber_copy,name_copy,id_copy);
        visitData data = {
            .Name = name_copy,
            .ID = id_copy,
            .SrNo = serialNumber_copy,
            .ticketNo = ticketNumber_copy
        };
        start->visitor.ID = malloc(strlen(data.ID)+1);
        start->visitor.Name = malloc(strlen(data.Name)+1);
        start->visitor.ticketNo = malloc(strlen(data.ticketNo)+1);
        start->visitor.SrNo = malloc(strlen(data.SrNo)+1);
        strcpy(start->visitor.ID,data.ID);
        strcpy(start->visitor.Name,data.Name);
        strcpy(start->visitor.ticketNo,data.ticketNo);
        strcpy(start->visitor.SrNo,data.SrNo);
        start->nextPtr = NULL;
        start->prevPtr = NULL;
    } else {
        Ticket *indexTicket = start;
        Ticket *newTicket = malloc(sizeof(Ticket));
        while(indexTicket->nextPtr != NULL) {
            indexTicket = indexTicket->nextPtr;
        }       //traverse to linked-list's final element
        fscanf(pFile,"%s %s %s %s\n",serialNumber_copy,ticketNumber_copy,name_copy,id_copy);
        visitData data = {
            .Name = name_copy,
            .ID = id_copy,
            .SrNo = serialNumber_copy,
            .ticketNo = ticketNumber_copy
        };
        newTicket->visitor.ID = malloc(strlen(data.ID)+1);
        newTicket->visitor.Name = malloc(strlen(data.Name)+1);
        newTicket->visitor.ticketNo = malloc(strlen(data.ticketNo)+1);
        newTicket->visitor.SrNo = malloc(strlen(data.SrNo)+1);
        strcpy(newTicket->visitor.ID,data.ID);
        strcpy(newTicket->visitor.Name,data.Name);
        strcpy(newTicket->visitor.ticketNo,data.ticketNo);
        strcpy(newTicket->visitor.SrNo,data.SrNo);
        indexTicket->nextPtr = newTicket;
        newTicket->nextPtr = NULL;
        newTicket->prevPtr = indexTicket;
    }
    return start;
}

void printTicketsSold(){        //function to print tickets sold
    char str1[100]="\0";
    char str2[100]="\0";
    char totaltickets[100]="\0";
    FILE *ptrFILE;
    ptrFILE = fopen("Tickets.txt","r");
    if(ptrFILE != NULL){
        fscanf(ptrFILE,"%s %s %s\n",str1,str2,totaltickets);
        int ticketsold = atoi(totaltickets);
        puts("");
        printf("%d ticket(s) have been sold, there are %d ticket(s) left.\n",ticketsold,40-ticketsold);
    }else{
        printf("0 tickets have been sold, there are 40 tickets left.\n");
    }
    fclose(ptrFILE);
}