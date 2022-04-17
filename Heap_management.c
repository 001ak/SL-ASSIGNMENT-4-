//AMAN KUMAR
//ENROLLMENT NO.-BT20CSE009
//SL ASSIGNMENT 4-PROBLEM 1,PART II

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE_NAME 10
typedef enum{FAILURE, SUCCESS} pass_status;
typedef struct free_Node_tag
{
    int address;
	int size;
    struct free_Node_tag* next;
    struct free_Node_tag* prev;
} free_Node;

typedef struct var_Node_tag
{
    char name[SIZE_NAME];
    int address;
	int size;
    struct var_Node_tag* next;
    struct var_Node_tag* prev;
} var_Node;

//Declaration of structure Heap
typedef struct Heap_tag
{
    int size;
    free_Node* freelist;
    var_Node* varlist;
} Heap;

//////Function defintions////////////
free_Node* make_free_Node(int address, int size);    //Creates free_Node
var_Node* make_var_Node(char* name, int address, int size);  //Creates var_Node
void start_heap(Heap *hptr, int size);   //Initialize heap 

void insert_sorted_free(free_Node **headptr, free_Node *nptr);   //Inserts free_Node into freelist in a sorted manner
void insert_sorted_var(var_Node **headptr, var_Node *nptr);   //Inserts var_Node into varlist in a sorted manner
pass_status delete_free_Node(free_Node** lpptr, free_Node* p);   //Deletes Node "p" from freelist
pass_status delete_var_Node(var_Node** lpptr, var_Node* p);  //Deletes Node "p" from varlist

pass_status memory_alloc(Heap *hptr, char* blockName, int varSize); //Allocates memory of size "varSize" in the heap and alots that to variable name "blockName"
pass_status free_memory(Heap *hptr, char *blockName);   //Frees memory of space pointed by the variable name "blockName" 
void print_free_List(free_Node *free_ptr);   //Prints the freelist
void print_var_List(var_Node *var_lptr); //Prints the varlist
void printHeap(Heap *hptr); //Prints all the heap blocks (allocated and free)
void freeHeap(Heap * hptr); //Frees both freelist and varlist completely


//Function to create free_Node.
//returns pointer to free_Node.
free_Node* make_free_Node(int address, int size)
{
    free_Node* nptr = (free_Node*)malloc(sizeof(free_Node));

    nptr->size = size;
    nptr->address = address;
    nptr->next = NULL;
    nptr->prev = NULL;

    return nptr;
}

//Function to create var_Node.
//returns pointer to var_Node.
var_Node* make_var_Node(char* name, int address, int size)
{
    var_Node* nptr = (var_Node*)malloc(sizeof(var_Node));

    strcpy(nptr->name, name);
    nptr->size = size;
    nptr->address = address;
    nptr->next = NULL;
    nptr->prev = NULL;

    return nptr;
}

//Function to initialize a Heap with number of bytes = size
void start_heap(Heap *hptr, int size)
{
    hptr->size = size;
    hptr->freelist = make_free_Node(0, hptr->size);
    hptr->varlist = NULL;
}

//Function to insert "nptr" into the free list, where headptr is dereferenced to get the head of the free list.
void insert_sorted_free(free_Node **headptr, free_Node *nptr)
{
    free_Node *ptr, *prev_node, *head;
	head = *headptr;
	nptr->next = nptr->prev = NULL;

	if(head == NULL)	//Empty list case
	{
	    head = nptr;
	}
	else
	{
	    ptr = head;
	    while(ptr->next != NULL && ptr->address < nptr->address)	//Skip all nodes having address lesser than nptr->address
	    {
	        ptr = ptr->next;
	    }
	    
	    if(ptr->next == NULL && ptr->address < nptr->address)  	//If inserted node address is more than address of all nodes in the list
	    {
	        ptr->next = nptr;
	        nptr->prev = ptr;
	    }
	    else
	    {
	        if(ptr == head) 	//If inserted node address is less than address of all nodes in the list
	        {
			    head = nptr;
	        }    
	        prev_node = ptr->prev;

			//Joining all links
	        if(prev_node != NULL)
	        {
	            prev_node->next = nptr;
	        }
	        nptr->prev = prev_node;
	        nptr->next = ptr;
	        ptr->prev = nptr;
	    }
	}
	*headptr = head;
}

//Function to insert "nptr" into the variable list, where headptr is dereferenced to get the head of the variable list.
void insert_sorted_var(var_Node **headptr, var_Node *nptr)
{
    var_Node *ptr, *prev_node, *head;
	head = *headptr;
	nptr->next = nptr->prev = NULL;

	if(head == NULL)	//Empty list case
	{
	    head = nptr;
	}
	else
	{
	    ptr = head;
	    while(ptr->next != NULL && ptr->address < nptr->address)	//Skip all nodes having address lesser than nptr->address
	    {
	        ptr = ptr->next;
	    }
	    
	    if(ptr->next == NULL && ptr->address < nptr->address)  	//If inserted node address is more than address of all nodes in the list
	    {
	        ptr->next = nptr;
	        nptr->prev = ptr;
	    }
	    else
	    {
	        if(ptr == head) 	//If inserted node address is less than address of all nodes in the list
	        {
			    head = nptr;
	        }    
	        prev_node = ptr->prev;

			//Joining all links
	        if(prev_node != NULL)
	        {
	            prev_node->next = nptr;
	        }
	        nptr->prev = prev_node;
	        nptr->next = ptr;
	        ptr->prev = nptr;
	    }
	}
	*headptr = head;
}

//Function to delete a node "p" which is present in the free list, where lpptr is dereferenced to get the head of the free list.
//return SUCCESS or FAILURE
pass_status delete_free_Node(free_Node** lpptr, free_Node* p)
{
    pass_status ret_val = SUCCESS;
    free_Node *prev_node, *next_node;
    
    if(p == NULL)
    {
        ret_val = FAILURE;
    }
    else
    {
        prev_node = p->prev;
        next_node = p->next;

        if(prev_node != NULL)   //If we are not deleting the first node
        {
            prev_node->next = next_node;
        }
        else    //If we are deleting the first node
        {
            *lpptr = next_node;
        }

        if(next_node != NULL)   //If we are not deleting the last node
        {
            next_node->prev = prev_node;
        }

        free(p);
    }

    return ret_val;
}

//Function to delete a node "p" which is present in the variable list, where lpptr is dereferenced to get the head of the variable list.
//return SUCCESS or FAILURE
pass_status delete_var_Node(var_Node** lpptr, var_Node* p)
{
    pass_status ret_val = SUCCESS;
    var_Node *prev_node, *next_node;
    
    if(p == NULL)
    {
        ret_val = FAILURE;
    }
    else
    {
        prev_node = p->prev;
        next_node = p->next;

        if(prev_node != NULL)   //If we are not deleting the first node
        {
            prev_node->next = next_node;
        }
        else    //If we are deleting the first node
        {
            *lpptr = next_node;
        }

        if(next_node != NULL)   //If we are not deleting the last node
        {
            next_node->prev = prev_node;
        }

        free(p);
    }

    return ret_val;
}

//Function to allocate memory according to first fit strategy.
//return SUCCESS or FAILURE
pass_status memory_alloc(Heap *hptr, char* blockName, int varSize)
{
    pass_status sc = SUCCESS;
    free_Node *free_lptr, *fptr;
    var_Node *var_lptr, *vptr;

    free_lptr = hptr->freelist;
    fptr = free_lptr;
    var_lptr = hptr->varlist;
    vptr = var_lptr;
    

    int found = 0;

    while(fptr != NULL && !found)	//Search for the first fit block in the free list.
    {
        if(fptr->size >= varSize)
        {
            found = 1;
        }
        else
        {
            fptr = fptr->next;
        }
    }

    if(!found || varSize == 0)	// If not found, failed to allocate memory
    {
        sc = FAILURE;
    }
    else	// If found
	{
		var_Node *var_nptr = make_var_Node(blockName, fptr->address, varSize);		// make a new var_Node of same size and address, and given name
		insert_sorted_var(&var_lptr, var_nptr);	// and sorted insert into the varlist

		fptr->address = fptr->address + varSize;	//Updating the free block after partition
		fptr->size = fptr->size - varSize;

		if(fptr->size == 0)		//If size becomes 0, delete from free list
		{
			delete_free_Node(&free_lptr, fptr);
		}
	}

    hptr->freelist = free_lptr;
    hptr->varlist = var_lptr;
    return sc;
}

//Function to free memory
//return SUCCESS or FAILURE
pass_status free_memory(Heap *hptr, char *blockName)
{
	pass_status sc = SUCCESS;
	free_Node *free_lptr, *fptr, *free_prev_node;
    var_Node *var_lptr, *vptr;
	int found = 0;

    free_lptr = hptr->freelist;
    fptr = free_lptr;
    var_lptr = hptr->varlist;
    vptr = var_lptr;

	while(vptr != NULL && !found)	//Search the varlist for the var_Node of given name
	{
		if(strcmp(blockName, vptr->name) == 0)
		{
			found = 1;
		}
		else
		{
			vptr = vptr->next;
		}
	}

	if(!found)	//If not found, failed to free memory
	{
		sc = FAILURE;
	}
	else	//If found
	{
		free_Node *free_nptr = make_free_Node(vptr->address, vptr->size);	// make a new free_Node of same size and address.
		insert_sorted_free(&free_lptr, free_nptr);	//and sorted insert into free list
		delete_var_Node(&var_lptr, vptr);	//delete the var_Node from varlist

		//Merging free nodes
		free_Node *free_next_node, *free_prev_node;
		free_next_node = free_nptr->next;
		free_prev_node = free_nptr->prev;

		if(free_next_node != NULL)
		{
			if(free_nptr->address + free_nptr->size == free_next_node->address)	//If next free_Node is adjacent to the newly inserted free_Node
			{
				free_nptr->size = free_nptr->size + free_next_node->size;
				delete_free_Node(&free_lptr, free_next_node);
			}
		}
		if(free_prev_node != NULL)
		{
			if(free_prev_node->address + free_prev_node->size == free_nptr->address)	//If previous free_Node is adjacent to the newly inserted free_Node
			{
				free_prev_node->size = free_prev_node->size + free_nptr->size;
				delete_free_Node(&free_lptr, free_nptr);
			}
		}
	}

	hptr->freelist = free_lptr;
    hptr->varlist = var_lptr;
	return sc;
}

//Function to print the free list in address sorted manner.
void print_free_List(free_Node *free_ptr)
{
	free_Node *ptr = free_ptr;
	printf("Free list display:\n");
	if(free_ptr == NULL)
	{
		printf("No free space\n");
	}
	else
	{
		printf("Start_Address\tEnd_Address\tSize\n");
		while(ptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\n", ptr->address, ptr->address + ptr->size - 1, ptr->size);
			ptr = ptr->next;
		}
	}
	printf("\n");
}

//Function to print the variable list in address sorted manner.
void print_var_List(var_Node *var_lptr)
{
	var_Node *ptr = var_lptr;
	printf("Variable list display:\n");
	if(var_lptr == NULL)
	{
		printf("No allocated space\n");
	}
	else
	{
		printf("Start_Address\tEnd_Address\tSize\tVariable_name\n");
		while(ptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\t%s\n", ptr->address, ptr->address + ptr->size - 1, ptr->size, ptr->name);
			ptr = ptr->next;
		}
	}
	printf("\n");
}

//Function to print the heap blocks (Both allocated and free) in address sorted manner.
void printHeap(Heap *hptr)
{
	var_Node *vptr = hptr->varlist;
	free_Node *fptr = hptr->freelist;
	printf("Total heap space = %d bytes\n", hptr->size);
	printf("Heap memory display:\n");
	printf("Start_Address\tEnd_Address\tSize\tAllocation_Status\n");
	while(vptr != NULL && fptr != NULL)
	{
		if(vptr->address < fptr->address)
		{
			printf("%d\t\t%d\t\t%d\tAllocated\n", vptr->address, vptr->address + vptr->size - 1 ,vptr->size);
			vptr = vptr->next;
		}
		else
		{
			printf("%d\t\t%d\t\t%d\tFree\n", fptr->address, fptr->address + fptr->size - 1 ,fptr->size);
			fptr = fptr->next;
		}
	}

	if(vptr != NULL)
	{
		while(vptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\tAllocated\n", vptr->address, vptr->address + vptr->size - 1 ,vptr->size);
			vptr = vptr->next;
		}
	}
	else
	{
		while(fptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\tFree\n", fptr->address, fptr->address + fptr->size - 1 ,fptr->size);
			fptr = fptr->next;
		}
	}
	printf("\n");
}

//Function to free both freelist and varlist completely
void freeHeap(Heap * hptr)
{
	free_Node *free_lptr = hptr->freelist;
	free_Node *fptr = free_lptr;
	var_Node *var_lptr = hptr->varlist;
	var_Node *vptr = var_lptr;

	while(free_lptr != NULL)
	{
		fptr = free_lptr;
		free_lptr = free_lptr->next;
		free(fptr);
	}

	while(var_lptr != NULL)
	{
		vptr = var_lptr;
		var_lptr = var_lptr->next;
		free(vptr);
	}

	hptr->freelist = NULL;
	hptr->varlist = NULL;
}


int main()
{
	int choice;
    Heap a_heap;
    start_heap(&a_heap, 2500);	//heap's max size is taken initially 2500
	//SO,if size is greater than 2500,no memory allocated
	printf("\n--Memory allocation using First Fit strategy with memory of 2500 units available.--\n");
	char blockName[SIZE_NAME];
	int size;
	pass_status ps;
	choice=3;
	printf("\n					HEAP MANAGEMENT SCHEME					\n");
	printf("				   	Using First Fit Strategy					\n");
	printf("					Enter your Choices Accordingly:					\n");
	while(choice != 0)
	{
		printf("1.	Allocate Memory:\n");
		printf("2.	Deallocate Memory:\n");
		printf("3.	Print Heap and Lists:\n");
		printf("0.	Exit Terminal!\n");
		scanf(" %d",&choice);
		switch(choice) 
		{
			case 1:		//Memory allocation with first fit strategy
			{
				printf("Enter a Block name:\n");
				scanf("%s", blockName);
				printf("Enter size:\n");
				scanf("%d", &size);
				ps = memory_alloc(&a_heap, blockName, size);
				if(ps == SUCCESS)
					printf("Successfully Allocated.\n");
				else
					printf("Allocation Unsuccessful.\n");
				break;
			}
			case 2:		//Memory free
			{
				printf("Enter a Block name:\n");
				scanf("%s", blockName);
				ps = free_memory(&a_heap, blockName);
				if(ps == SUCCESS)
					printf("Successfully Deallocated.\n");
				else
					printf("Deallocation Unsuccessful.\n");
				break;
			}
			case 3:		//Print Heap, free list and variable list
			{
				printHeap(&a_heap);
				print_free_List(a_heap.freelist);
				print_var_List(a_heap.varlist);
				break;
			}
			case 0:		//Exit terminal
			{
				printf("Terminal exited\n");
				break;
			}
			default:
			{
				printf("Wrong option entered\n");
				break;
			}
		}
		
	}
	printf("Thank You!!\n");
	freeHeap(&a_heap);
    return 0;
}