/* Program to generate text based on the context provided by input prompts.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2023, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2023.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [Ai Ling Chiam 1384412]
  Dated:     [5 October 2023]
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define NOSFMT "Number of statements: %d\n"                 // no. of statements
#define NOCFMT "Number of characters: %d\n"                 // no. of chars
#define NPSFMT "Number of states: %d\n"                     // no. of states
#define TFQFMT "Total frequency: %d\n"                      // total frequency

#define CRTRNC '\r'                             // carriage return character

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t;   // a state in an automaton
typedef struct node  node_t;    // a node in a linked list

struct node {                   // a node in a linked list of transitions has
    char*           str;        // ... a transition string
    state_t*        state;      // ... the state reached via the string, and
    node_t*         next;       // ... a link to the next node in the list.
};

typedef struct {                // a linked list consists of
    node_t*         head;       // ... a pointer to the first node and
    node_t*         foot;       // ... a pointer to the last node in the list.
} list_t;

struct state {                  // a state in an automaton is characterized by
    unsigned int    id;         // ... an identifier,
    unsigned int    freq;       // ... frequency of traversal,
    int             visited;    // ... visited status flag, and
    list_t*         outputs;    // ... a list of output states.
};

typedef struct {                // an automaton consists of
    state_t*        ini;        // ... the initial state, and
    unsigned int    nid;        // ... the identifier of the next new state.
} automaton_t;

typedef struct {                // a structure that consists of 
    char            ch;         // the character that has the highest frequency,
    int             freq;       // the frequency.
} nextchar_t;
/* USEFUL FUNCTIONS ----------------------------------------------------------*/
char mygetchar(void);             // getchar() that skips carriage returns
automaton_t *make_new_automaton(void); // Creating a root for the automaton
state_t *make_new_state(void); // Creating a new state 
list_t *make_new_list(void); // Creating a new linked list
char *make_new_string(void); // Creating a new string
node_t *make_new_node(void); // Creating a new node
node_t *recursive_search(node_t *node, char c); // Checks if c in linked list
list_t *insert_at_foot(list_t *list, char c, int id); // Inserts a new node into foot of linked list
nextchar_t *make_newchar(void); // Creating a nextchar structure
char cmp_char(char c1, char c2); // Comparing 2 characters and returns the higher ASCII value char

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    int c, char_num = 0, line_num = 0, state_num = 1, stage = 0;
    int curr_freq, end_string = 0, output_num = 0;
    int prev_c = -1; // Initialize the prev_c
    char curr_c;
    printf(SDELIM, stage);
    
    // Initialize the root of the tree
    automaton_t *new_automaton;
    new_automaton = make_new_automaton(); 
    
    // Initializing a current_list
    list_t *curr_list;
    curr_list = make_new_list();
    
    // Initializing a current_node
    node_t *curr_node;
    curr_node = make_new_node();
    
    // Initializing a next_char structure
    nextchar_t *nchar;
    nchar = make_newchar();
    
    // Reading the input, Making the automaton and printing values
    curr_list = new_automaton->ini->outputs;
    while ((c = mygetchar()) != EOF) {
        if (stage == 0) {
            if (c == '\n') {
                curr_node->state->freq = 0;
                curr_list = new_automaton->ini->outputs; // Return back to the start
                if (prev_c != '\n') {
                    new_automaton->ini->freq++;
                }
            } else {
                char_num++;
                curr_node = curr_list->head;
                curr_node = recursive_search(curr_node, c);
                if (curr_node == NULL) {
                    curr_list = insert_at_foot(curr_list, c, state_num); // Updates the curr_list by inserting a new node
                    if (curr_list == new_automaton->ini->outputs) {
                        new_automaton->nid = state_num; // Updates to id of the next new state
                    }
                    state_num++;
                    curr_node = curr_list->foot;
                    curr_list = curr_node->state->outputs;
                } else {
                    curr_node->state->freq++;
                    curr_list = curr_node->state->outputs;
                }
            }
            if (prev_c == '\n') {
                line_num++;
                if (c == '\n') {
                    printf(NOSFMT, line_num);
                    printf(NOCFMT, char_num);
                    printf(NPSFMT, state_num);
                    curr_list = new_automaton->ini->outputs;
                    stage++;
                    printf(SDELIM, stage);
                    continue;
                }
            }
        }
        if (stage == 1) {
            if (end_string == 1) {
                if (c == '\n') {
                    for (int i = 0; i < 3;i++) {
                        if (output_num < 37) {
                            printf(".");
                            output_num++;
                            if (i == 2) {
                                printf("\n");
                            }
                        } else {
                            printf("\n");
                            break;
                        }
                    }
                    output_num = 0;
                    end_string = 0;
                } else {
                    if (output_num < 36) {
                        printf("%c", c);
                        output_num++;
                    } 
                }
                curr_list = new_automaton->ini->outputs;
                continue;
            } else {
                curr_node = curr_list->head;
                if (prev_c == '\n' && c == '\n') {
                    stage++;
                    printf(SDELIM, stage);
                }
                if (c == '\n') {
                    printf("...");
                    while (curr_node->state->freq != 0) {
                        while (curr_node != NULL) {
                            curr_c = curr_node->str[0];
                            curr_freq = curr_node->state->freq;
                            // Comparing frequencies
                            if (curr_freq > nchar->freq) {
                                nchar-> freq = curr_freq;
                                nchar->ch = curr_c;
                            }
                            else if (curr_freq == nchar->freq) {
                                nchar-> ch = cmp_char(curr_c, nchar->ch);
                            }
                            curr_node = curr_node->next; // going to the next sibling node
                        }
                        curr_node = curr_list->head; // put it back to the start of curr_list
                        curr_node = recursive_search(curr_node, nchar->ch);
                        printf("%c", nchar->ch);
                        curr_list = curr_node->state->outputs;
                        curr_node = curr_list->head;
                        // Resets the nchar structure
                        nchar->ch = '\0';
                        nchar->freq = 0;
                    }
                    printf("%c\n", curr_node->str[0]);
                    output_num = 0; // Resetting output_num to 0
                    curr_list = new_automaton->ini->outputs; // Returns back to the start 
                } else {
                    // Reads the tree
                    curr_node = recursive_search(curr_node, c);
                    if (curr_node != NULL) {
                        if (output_num > 36) {
                            end_string = 1;
                        } else {
                            printf("%c", c); 
                            output_num++;
                            if (curr_node->state->freq == 0) {
                                end_string = 1;
                            } else {
                                curr_list = curr_node->state->outputs;
                            }
                        }
                    } else {
                        printf("%c", c);
                        end_string = 1;
                    }
                }
            }
        }
        if (stage == 2) {
            printf(THEEND);
            return EXIT_SUCCESS;
        }
        prev_c = c;

    }
}
/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
char mygetchar() {
    char c;
    while ((c=getchar())==CRTRNC);
    return c;
}

automaton_t 
*make_new_automaton(void) {
    automaton_t *new_automaton;
    new_automaton = (automaton_t*)malloc(sizeof(*new_automaton));
    assert(new_automaton!=NULL);
    new_automaton->ini = make_new_state();
    new_automaton->ini->id = 0;
    new_automaton->ini->freq = 0;
    new_automaton->ini->visited = 0;
    new_automaton->nid = 0;
    return new_automaton;
}

state_t
*make_new_state(void) {
    state_t *new_state;
    new_state = (state_t*)malloc(sizeof(*new_state));
    assert(new_state!=NULL);
    new_state->id = 0;
    new_state->freq = 1;
    new_state->visited = 0;
    new_state->outputs = make_new_list();
    return new_state;
}

list_t
*make_new_list(void) {
    list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

char
*make_new_string(void) {
	char *str;
	str = (char*)malloc(sizeof(*str));
	assert(str!=NULL);
	return str;
}

node_t
*make_new_node(void) {
    node_t *new;
    new = (node_t*)malloc(sizeof(*new));
    new->str = make_new_string();
    new->state = make_new_state();
    new->next = NULL; 
    return new;
}

node_t
*recursive_search(node_t *node, char c) {
    if (node == NULL) {
        return NULL;
    }
    
    else if (node->str[0] == c) {
        return node;
    }
    return recursive_search(node->next, c);
}

list_t
*insert_at_foot(list_t *list, char c, int state) {
    node_t *new;
    new = (node_t*)malloc(sizeof(*new));
    assert(list!=NULL && new!=NULL);
    new->str = make_new_string();
    new->str[0] = c;
    new->state = make_new_state();
    new->state->id = state;
    new->next = NULL;
    if (list->foot == NULL) {
        list->head = list->foot = new;
    } else {
        list->foot->next = new;
        list->foot = new;
    }
    return list;
}

nextchar_t
*make_newchar(void) {
    nextchar_t *new;
    new = (nextchar_t*)malloc(sizeof(*new));
    assert(new!=NULL);
    new->ch = '\0';
    new->freq = 0;
    return new;
}

char 
cmp_char(char c1, char c2) {
    if (c1 > c2) {
        return c1;
    } else {
        return c2;
    }
}

// algorithms are fun!!
/* THE END -------------------------------------------------------------------*/
