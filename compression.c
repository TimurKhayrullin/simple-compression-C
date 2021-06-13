//Timur Khayrullin
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// Declaration of my own "list" structure. It contains a pointer to
//   a linked list and a size.
struct List{

    int size;
    struct Node* list;
};

// Declaration of node for linked list structure
struct Node{
    char* data;
    struct Node* next;
};

// given a pointer to a char array s (a string) and an integer n,
//    mutates the array that s points to to represent an integer
//    as a string.
void intToStr(char* s, int n) {
    
    //declare and initialize a string and an index
    char str[8];
    for (int j = 0; j < 8; j++) str[j] = '\0';
    int i = -1;

    //special case for n = 0, just return "0"
    if(n == 0)
    {
        str[0] = '0';
        strcpy(s, str);
        return;
    }

    //using i, counts the amount of digits in the integer
    int copy = n;
    while(copy > 0) {
        copy /= 10;
        i++;
    }

    // counting down from the initial value of i, fills
    //   the string with the coresponding digits in char form
    while(n > 0) {
        char digit = n % 10 + 48;
        str[i] = digit;
        i--;
        n /= 10;
    }

    // mutates the string that s points to
    strcpy(s, str);
}

// reverses a linked list lst, returns the head of the new 
//    reversed list
struct Node *reverse(struct Node *lst){

    struct Node *before = NULL;
    struct Node *after = NULL;

    for (struct Node *cur = lst; cur;cur = after){
        
        after = cur->next;

        cur->next = before;

        before = cur;

    }

    return before;

}

// a function that uses ungetc to check if the next
//    character in the input stream is EOF.
//    if the next char is EOF, returns EOF.
int peekChar(){
    int c = getchar();
    return c==EOF? EOF: ungetc(c, stdin);
}


// constructor function for my List structure. 
//  it allocates space on the heap for the list itself,
//  and initializes the linked list inside, along with setting.
//  size to zero.
//  returns a pointer to the new structure
struct List* makeList(){

    struct List* l = malloc(sizeof(struct List));

    l->size = 0;

    l->list = NULL;

    return l;
}


// given a pointer to a string "v", and a pointer to a linked
//   list "next", adds a new anode to the given linked list. returns a pointer to the new node
struct Node *cons(char *v, struct Node *next){

    struct Node *res = malloc(sizeof(struct Node));

    res->data = v;

    res->next = next;

    return res;   
}

// given a list structure l and a pointer to a string v, 
//    adds a copy of v to l and increments l's size.
void add(struct List* l, char* v){

    char *newStr = malloc(sizeof(char)*(strlen(v)+1)); // allocates enough space for the string and a null pointer
    strcpy(newStr, v); // copies the string into the allocated heap space

    struct Node* n = cons(newStr, l->list);
    l->list = n;
    l->size++;

}

//  given a pointer to a list l, and a string pointer w, 
//    searches l's linked list for the string if found, returns the index at which that
//    string lies in the linked list (starts at 0)
//    if string is not found, returns -1.
int search(struct List* l,char* w){

    int i = l->size-1;
    
    
    for(struct Node* temp = l->list; temp; temp = temp->next){

        if(!strcmp(temp->data, w)){
            return i;
        }

        i--;

    }

    return -1;
}


// given a linked list pointer n, frees all data associated with it and
//   all the nodes it points to, imperatively 
void freeNode(struct Node* n){

    for(struct Node *cur=n; cur; ){
        struct Node *tmp = cur;
        cur=cur->next;
        free(tmp->data); // frees string
        free(tmp); // frees anode itself
    }
}

// given a list structure l, frees its associated linked list
//  and the list structure itself.
void freeList(struct List* l){

    freeNode(l->list);

    free(l);

}

// prints the entire contents of a given list structure "out"'s
//   linked list. then, it frees all the data associated with the list
void printListLiteral(struct List* out){

    struct Node* rev = reverse(out->list); // the linked list needs to be reversed to 
    // print in the correct order

    for(struct Node *tmp=rev;tmp;tmp=tmp->next){
            printf("%s", tmp->data);
    }

    freeNode(rev); // frees the linked list

    free(out); // frees the list structure itself
}

int main(){

    //initializes 2 lists, one for tracking words and one for output
    struct List* l = makeList();
    struct List* out = makeList();

    //these variables are used to track the current word being built,
    // and the results of searching for a word in a list's linked list
    char cur[81];
    for (int j = 0; j < 81; j++) cur[j] = '\0';
    int res;
    int i = 0;

    // string pointer used for int to string conversion
    char* str = malloc(sizeof(char)*8);

    while(peekChar() != EOF){

        char c = getchar();

        // if c is a newline char
        if(c == '\n'){
            
            if(cur[0] != '\0'){ //   checks if the current word being built is empty.
                res = search(l, cur); // if it isn't, searches for it in l's linked list.

                if(res==-1){  // if it is not found in the seach, puts it in the output list
                    add(l, cur);
                    add(out, cur);
                    add(out, "\n");
                }else{
                    intToStr(str,res); // if it is found in the search, replaces it in the output with its index.
                    add(out, str);
                    add(out, "\n");
                }

                cur[0] = '\0'; // reset the current word
                i = 0;
            } else{
                add(out, "\n"); // if the word is empty, adds the space/newline to the output and moves on
            }    
        }else if(isspace(c)){ // similar logic is used if c is a space.

            if(cur[0] != '\0'){
                res = search(l, cur);

                if(res==-1){
                    add(l, cur);
                    add(out, cur);
                    add(out, " ");
                }else{
                    intToStr(str, res);
                    add(out, str);
                    add(out, " ");
                }

                cur[0] = '\0';
                i = 0;
            } else{
                add(out, " ");
            }            

        } else { // if c is not a newline or space, adds c to the current word being built
            // i tracks the end of the word
            cur[i] = c;
            cur[i+1] = '\0'; 
            i++;
        }

        
    }

    //after EOF has been reached, we check whether there was a word right before
    //  if there was, we parse it as above.
    if(cur[0] != '\0'){
        res = search(l, cur);

        if(res!=-1){
            intToStr(str, res);
            add(out, str);
        }else{
            add(out, cur);
        }
    }

    for (int j = 0; j < 8; j++) cur[j] = '\0';

    //prints output
    printListLiteral(out);
    //frees word bank and string pointer we used for integer conversion
    free(str);
    freeList(l);
}