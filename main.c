#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
/*

  OGUZHAN BIRK 21893952
  BIL332-02
  HW-2 BUDDY SYSTEM

  I hereby affirmed my solution

*/
struct Node {
	char nameOfProcess;// process name
	int space; // process space
	int internalFr; // internal fragmention
	struct Node* next;
};

typedef struct Node Node;
void requestSpace(Node*, char, int); // request space
void releaseSpace(Node**, char); // release space
void displayBuddy(Node*); // print the list
int controlNode(Node*, char);
int main()
{
	Node* buddy = (Node*)malloc(sizeof(Node));
	int memory_operation;
	char process_name;
	int req_space, name_control;

	buddy->space = 1024; // Initialization
	buddy->nameOfProcess = 'X';
	buddy->internalFr = -1;
	buddy->next = NULL;


	printf("**************** MEMORY **************\n");
	printf("Block-1: free - 1024M\n");
	printf("**************** MEMORY **************\n");


	while (1) {

		printf("> Enter 0(EXIT),  1(request),  2(release): ");
		scanf("%d", &memory_operation);


		switch (memory_operation) {

		case 1:
			printf("Enter process name and requested space(M): ");
			scanf(" %c%d", &process_name, &req_space);
			printf("*****************MEMORY***************\n");
			requestSpace(buddy, process_name, req_space);
			displayBuddy(buddy);
			printf("**************************************\n");
			break;

		case 2:
			printf("Enter process name: ");
			scanf(" %c", &process_name);
			name_control = controlNode(buddy, process_name);
			if (name_control == 1) {
				printf("*****************MEMORY***************\n");
				releaseSpace(&buddy, process_name);
				displayBuddy(buddy);
				printf("**************************************\n");
			}
			else {
				printf("Node has not been found!!!\n");//Error Condition
			}

			break;

		default:
			printf("!!!  THE END  !!!\n");
			return 0;

		}

	}

	return 0;


}
int controlNode(Node* buddy, char pName) {
	while (buddy != NULL) { // Print the list

		if (buddy->nameOfProcess == pName)
			return 1;
		buddy = buddy->next;
	}

	return 0;

}

void requestSpace(Node* buddy, char pName, int req_space) {

	Node* new_node = NULL;
	Node* temp = buddy;
	int half = 0;

	if (buddy == NULL) {
		printf("the given buddy node cannot be NULL!!!");
		return;
	}



	while (temp) {

		half = (temp->space) / 2;

		if ((req_space > half) && (req_space < temp->space) && (temp->nameOfProcess == 'X')) { // buddy system condition

			temp->nameOfProcess = pName;
			temp->internalFr = (temp->space) - req_space;
			return;
		}

		else if (req_space == temp->space && temp->nameOfProcess == 'X') {

			temp->nameOfProcess = pName;
			temp->internalFr = 0;
			return;
		}

		else {

			new_node = (Node*)malloc(sizeof(Node));

			/*if (new_node == NULL)
				printf("Failed to add process. Out of memory");*/

			new_node->space = (temp->space) / 2;
			new_node->nameOfProcess = 'X';
			new_node->internalFr = -1; // new node is created

			if (temp->nameOfProcess == 'X') {
				if (req_space == half) { // buddy control

					new_node->next = temp->next;
					temp->space = (temp->space) / 2;
					temp->nameOfProcess = pName;
					temp->internalFr = 0;
					temp->next = new_node;
					return;
				}


				if (req_space < half) { // if requested space is less than half space of node

					new_node->next = temp->next;
					temp->space = (temp->space) / 2;
					temp->next = new_node;
					continue;
				}


			}



		}

		temp = temp->next;

	}

}
void releaseSpace(Node** buddy, char pName) {

	Node* temp = *buddy, * previousNode = *buddy;
	int sum = 0;


	if (temp != NULL && temp->nameOfProcess == pName) { // If node is in the head

		if (temp->next->space != temp->space) {
			(*buddy)->nameOfProcess = 'X';
			return;
		}

		while (temp->next->space == temp->space) {
			if (temp->next->nameOfProcess == 'X') {
				*buddy = temp->next;
				sum = ((*buddy)->space) * 2;
				(*buddy)->space = sum;
				(*buddy)->nameOfProcess = 'X';
				free(temp);
				temp = *buddy;
			}
			else {
				(*buddy)->nameOfProcess = 'X';
				return;
			}

			if (temp->next == NULL) {
				break;
			}

		}

		return;
	}// If the name of first node which will be released equals pName(process name) 

	//*********************************************************************************
	while (1) {// If the released node is not in head 

		while (temp != NULL && temp->nameOfProcess != pName) {

			previousNode = temp;
			temp = temp->next;
		}

		if (temp == NULL) {
			return;
		} // If process name was not found in buddy list


		// node is founded
		break;
	}

	previousNode = temp;
	if (temp->nameOfProcess == pName && temp != NULL) {  // If the released node is not in head 

		if (temp->next == NULL) { // If the release node in end
			temp->nameOfProcess = 'X';
			temp->internalFr = -1;
			return;
		}


		if (temp->next->space != temp->space) {
			temp->nameOfProcess = 'X';
			return;
		}

		while (previousNode->next->space == previousNode->space) { // buddy system release operation
			if (previousNode->next->nameOfProcess == 'X') {
				temp = previousNode->next;
				sum = (temp->space) * 2;
				temp->space = sum;
				temp->nameOfProcess = 'X';
				free(previousNode);
				previousNode = temp;
			}
			else {
				temp->nameOfProcess = 'X';
				return;
			}

			if (previousNode->next == NULL) {
				break;
			}

		}

		(*buddy)->next = temp;
		return;

	}



}

void displayBuddy(Node* buddy) {

	int n = 1;

	while (buddy != NULL) { // Print the list
		if (buddy->internalFr == -1) { // design configurations
			if (buddy->nameOfProcess == 'X')
				printf("Block-%d:  free\t- %dM\n", n, buddy->space);
			else
				printf("Block-%d:  %c\t- %dM\n", n, buddy->nameOfProcess, buddy->space);
		}

		else
			printf("Block-%d:  %c\t- %dM\t(Internal Frag = %dM)\n", n, buddy->nameOfProcess, buddy->space, buddy->internalFr);
		n++;
		buddy = buddy->next;
	}
}