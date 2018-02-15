#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_SIZE 14 //max size a name can be ( 14 + null terminator)
#define QUIT "quit\n"
#define ADD "add\n"
#define DELETE "delete\n"
#define PRINT "print\n"

struct Node {
	char *name;
	struct Node *leftParent; //father
	struct Node *rightParent; //mother
	struct Node *child;//upper level node
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
	char temp[NAME_SIZE];
	fgets(temp, NAME_SIZE, stdin);
	char *pos;
	if ((pos = strchr(temp, '\n')) != NULL) *pos = '\0'; // remove newline that fgets captures
	root->name = realloc(root->name, sizeof(char) * strlen(temp));
	strcpy(root->name, temp);
	consoleLoop();
	// TODO: free here?
}

void consoleLoop() {
	while(1) {
		printf("Please specify whether to add or delete an entry, or print the tree\n");
		char command[NAME_SIZE];
		fgets(command, NAME_SIZE, stdin);
		
		if (strcmp(command, QUIT) == 0) quit(root);
		else if (strcmp(command, ADD) == 0) add();
		else if (strcmp(command, DELETE) == 0) deletePrompt();
		else if (strcmp(command, PRINT) == 0) print(root, 0);
		else printf("invalid command\n");
	}
}

void deletePrompt() {
	printf("Please specify the name to delete\n");
	char name[NAME_SIZE];
	fgets(name, NAME_SIZE, stdin);
	char *pos;
	if ((pos = strchr(name, '\n')) != NULL) *pos = '\0'; // remove newline
	struct Node *searchNode = search(root, name);
	if (!searchNode) {
		printf("name not found, the node you are trying to delete does not exist!\n");
		return;
	}
	if ((searchNode->child)->leftParent == searchNode) {
		(searchNode->child)->leftParent = NULL;
	} else {
		(searchNode->child)->rightParent = NULL;
	}
	delete(searchNode);
	return;
}

void delete(struct Node *n) {
	if (!n) return;
	delete(n->leftParent);
	free(n->name);
	delete(n->rightParent);
// 	printf("about to free: %s", n->name);
	free(n);
	n = NULL;
}

void quit() {
	delete(root);
	printf("Program Terminating\n");
	exit(0);
}

void add() {
	printf("Please specify a relation to add\n");
	char *input= malloc(sizeof(char) * 100);
	// I lost 2 hours of my life figuring out that you have to maintain a ref to input because fgets does something stupid
	char * const toFree = input;
        char *name2 = NULL;
	fgets(input, 100, stdin);
        char *pos;
        if ((pos = strchr(input, '\n')) != NULL) *pos = '\0'; // remove newline
	if (strlen(input) < 6 && strncmp(input, "father", 6) != 0 && strncmp(input, "mother", 6) != 0) {
		printf("invalid relationship, try again\n");
		goto cleanup;
	}
	char *relation = strsep(&input, "(");
	char *temp = strsep(&input, "(");
	char *name1 = strsep(&temp, ",");
	char *temp2 = strsep(&temp, ",");
	name2 = malloc(strlen(temp2 - 1) * sizeof(char)); // minus 1 because we don't need space for the ")"
	strncpy(name2, temp2, strlen(temp2) - 1);
	struct Node *searchNode = search(root, name2);
	if (!searchNode) {
		printf("name not found, child does not exist!\n");
		goto cleanup;
	}
	if (strncmp(relation, "father", 6) == 0) {
		if (searchNode->leftParent) {
			printf("relationship already exists, father already exists!\n");
			goto cleanup;
		} else {
			struct Node *parent = malloc(sizeof(struct Node));
                        parent->name = realloc(parent->name, sizeof(char) * strlen(name1));
			strcpy(parent->name, name1);
			searchNode->leftParent = parent;
			parent->child = searchNode;
			goto cleanup;
		}
	} else { //has to be mother
                if (searchNode->rightParent) {
                        printf("relationship already exists, mother already exists!\n");
                        goto cleanup;
                } else {
                        struct Node *parent = malloc(sizeof(struct Node));
			parent->name = realloc(parent->name, sizeof(char) * strlen(name1));			
                        strcpy(parent->name, name1);
                        searchNode->rightParent = parent;
			parent->child = searchNode;
                        goto cleanup;
                }

	} 

	cleanup:
		free(toFree);
		free(name2);
//		input = NULL;
//		name2 = NULL;
		return;

}

void print(struct Node *n, int depth) {
        if (!n) return;
        int temp = depth;
        while (temp != 0) {
                printf("\t");
                temp -= 1;
        }

	printf("%s", n->name);
	printf("\n");

	print(n->leftParent, depth + 1);
	print(n->rightParent, depth + 1);
}

struct Node* search(struct Node *n, char *name) {
	if (!n) return NULL;
	if (strcmp(n->name, name) == 0) return n;
	else {
		if (n->leftParent) {
			struct Node* tmp = search(n->leftParent, name);
			if (tmp) {
				return tmp;
			}
		}
		if (n->rightParent) {
			return search(n->rightParent, name);
		}
		return NULL;
	}
}
