#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_SIZE 12 //max size a name can be ( 12 + null terminator)
#define QUIT "quit"
#define ADD "add"
#define DELETE "delete"
#define PRINT "print"

struct Node {
	char name[NAME_SIZE];
	struct Node *leftParent; //father
	struct Node *rightParent; //mother
};

void consoleLoop();
void add();
void print(struct Node *n, int depth);
void quit();
void delete(struct Node *n);
void deletePrompt();
struct Node* search(struct Node *n, char *name);


static struct Node *root = NULL;

int main() {
	printf("Please enter your name: ");
	root = malloc(sizeof(struct Node));
	fgets(root->name, NAME_SIZE, stdin);
	char *pos;
	if ((pos = strchr(root->name, '\n')) != NULL) *pos = '\0'; // remove newline that fgets captures
	consoleLoop();
}

void consoleLoop() {
	while(1) {
		printf("Please specify whether to add or delete an entry, or print the tree: ");
		char command[NAME_SIZE];
		fgets(command, NAME_SIZE, stdin);
		
		if (strncmp(command, QUIT, 4) == 0) quit(root);
		else if (strncmp(command, ADD, 3) == 0) add();
		else if (strncmp(command, DELETE, 6) == 0) deletePrompt();
		else if (strncmp(command, PRINT, 5) == 0) print(root, 0);
		else printf("Unrecognized command, try again\n");
	}
}

void deletePrompt() {
	printf("Please specify the name to delete: ");
	char name[NAME_SIZE];
	fgets(name, NAME_SIZE, stdin);
	char *pos;
	if ((pos = strchr(name, '\n')) != NULL) *pos = '\0'; // remove newline
	struct Node *searchNode = search(root, name);
	if (!searchNode) {
		printf("Error, the node you are trying to delete does not exist!\n");
		return;
	}
	delete(searchNode);
}

void delete(struct Node *n) {
	if (!n) return;
	delete(n->leftParent);
	delete(n->rightParent);
	free(n);
}

void quit() {
	delete(root);
	printf("Program Terminating");
	exit(0);
}

void add() {
	printf("Please specify a relation to add: ");
	char *input = malloc(sizeof(char) * 100);
        char *name2 = NULL;
	fgets(input, 100, stdin);
	if (strncmp(input, "father", 6) != 0 && strncmp(input, "mother", 6) != 0) {
		printf("Unrecognized relation, try again\n");
		goto cleanup;
	}
	char *relation = strsep(&input, "(");
	char *temp = strsep(&input, "(");
	char *name1 = strsep(&temp, ",");
	char *temp2 = strsep(&temp, ",");
	name2 = malloc(strlen(temp2 - 1) * sizeof(char));
	strncpy(name2, temp2, strlen(temp2) - 2);
	printf("Relation: %s\n", relation);
	printf("Name1: %s\n", name1);
	printf("Name2: %s\n", name2);
	struct Node *searchNode = search(root, name2);
	if (!searchNode) {
		printf("Error, child does not exist!\n");
		goto cleanup;
	}
	if (strncmp(relation, "father", 6) == 0) {
		if (searchNode->leftParent) {
			printf("Error, father already exists!\n");
			goto cleanup;
		} else {
			struct Node *parent = malloc(sizeof(struct Node));
			strcpy(parent->name, name1);
			searchNode->leftParent = parent;
			goto cleanup;
		}
	} else { //has to be mother
                if (searchNode->rightParent) {
                        printf("Error, mother already exists!\n");
                        goto cleanup;
                } else {
                        struct Node *parent = malloc(sizeof(struct Node));
                        strcpy(parent->name, name1);
                        searchNode->rightParent = parent;
                        goto cleanup;
                }

	} 

	cleanup:
		free(input);
		free(name2);
		return;

}

//TODO: the printing is still kind of ugly
void print(struct Node *n, int depth) {
        if (!n) return;
        int temp = depth;
        while (temp != 0) {
                printf("\t");
                temp -= 1;
        }

	printf("%s", n->name);

	print(n->leftParent, depth + 1);
	printf("\n");
	print(n->rightParent, depth + 1);
}

struct Node* search(struct Node *n, char *name) {
	if (!n) return NULL;
	if (strncmp(n->name, name, strlen(name)) == 0) return n;
	else {
		if (n->leftParent) return search(n->leftParent, name);
		if (n->rightParent)return search(n->rightParent, name);		
	}
}
