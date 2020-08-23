#include "const.h"
#include "sequitur.h"

/*
 * Symbol management.
 *
 * The functions here manage a statically allocated array of SYMBOL structures,
 * together with a stack of "recycled" symbols.
 */

/*
 * Initialization of this global variable that could not be performed in the header file.
 */
int next_nonterminal_value = FIRST_NONTERMINAL;

/*
*Init a pointer that points to the HEAD of the recycled list, it will be null at first
*ALso initialize a recycleCount variable to count the number of recycled pointers
*/
struct symbol *recyclePointer = NULL;
int recycleCount = 0; //initialize recycle count to 0 at first

/**
 * Initialize the symbols module.
 * Frees all symbols, setting num_symbols to 0, and resets next_nonterminal_value
 * to FIRST_NONTERMINAL;
 */
void init_symbols(void) {
    // To be implemented.
    //Free all symbols
    num_symbols = 0;
    next_nonterminal_value = FIRST_NONTERMINAL;
    recyclePointer = NULL;
    recycleCount = 0;
}

/**
 * Get a new symbol.
 *
 * @param value  The value to be used for the symbol.  Whether the symbol is a terminal
 * symbol or a non-terminal symbol is determined by its value: terminal symbols have
 * "small" values (i.e. < FIRST_NONTERMINAL), and nonterminal symbols have "large" values
 * (i.e. >= FIRST_NONTERMINAL).
 * @param rule  For a terminal symbol, this parameter should be NULL.  For a nonterminal
 * symbol, this parameter can be used to specify a rule having that nonterminal at its head.
 * In that case, the reference count of the rule is increased by one and a pointer to the rule
 * is stored in the symbol.  This parameter can also be NULL for a nonterminal symbol if the
 * associated rule is not currently known and will be assigned later.
 * @return  A pointer to the new symbol, whose value and rule fields have been initialized
 * according to the parameters passed, and with other fields zeroed.  If the symbol storage
 * is exhausted and a new symbol cannot be created, then a message is printed to stderr and
 * abort() is called.
 *
 * When this function is called, if there are any recycled symbols, then one of those is removed
 * from the recycling list and used to satisfy the request.
 * Otherwise, if there currently are no recycled symbols, then a new symbol is allocated from
 * the main symbol_storage array and the num_symbols variable is incremented to record the
 * allocation.
 */
SYMBOL *new_symbol(int value, SYMBOL *rule) {
    // To be implemented.
    // num_symbols should have the array index that keeps track of the pointer
    struct symbol *ptr = NULL;
    //ptr = symbol_storage;
    int i; //looper variable

    if (value < FIRST_NONTERMINAL){
        /*struct symbol newSYMBOL = {value, 0, NULL, NULL, NULL, NULL, NULL};
        *(symbol_storage + num_symbols) = newSYMBOL;
        num_symbols++;*/
        if (recycleCount == 0){
            ptr = symbol_storage;
            for (i = 0; i < num_symbols; i++){
                ptr++;
            }

            ptr -> value = value;
            ptr -> refcnt = 0;
            ptr -> rule = NULL;
            ptr -> next = NULL;
            ptr -> prev = NULL;
            ptr -> nextr = NULL;
            ptr -> prevr = NULL;
            num_symbols++;
                debug("new terminal symbol value: %d at <%lu>", value, SYMBOL_INDEX(ptr));
        }
        else if (recycleCount != 0){
            ptr = recyclePointer; // if the recycle count is 0, then ptr points to the recyclePointer
            recyclePointer = recyclePointer->prev; //set the new recycle pointer to the previous pointer
            recyclePointer -> next = NULL; //then set the next of that to null
            ptr -> value = value;
            ptr -> refcnt = 0;
            ptr -> rule = NULL;
            ptr -> next = NULL;
            ptr -> prev = NULL;
            ptr -> nextr = NULL;
            ptr -> prevr = NULL;
            recycleCount--;
                debug("new terminal symbol value: %d from a recycled pointer at <%lu>", value, SYMBOL_INDEX(ptr));
        }
    }

    else if (value >= FIRST_NONTERMINAL){
        if(rule != NULL && recycleCount == 0){
            //Then it is a large value
            rule -> refcnt++;
            ptr = symbol_storage;
            for (i = 0; i < num_symbols; i++){
                ptr++;
            }

            ptr -> value = value;
            ptr -> refcnt = 0;
            ptr -> rule = rule;
            ptr -> next = NULL;
            ptr -> prev = NULL;
            ptr -> nextr = NULL;
            ptr -> prevr = NULL;
            num_symbols++;
                debug("new non-terminal symbol value: %d at <%lu>", value, SYMBOL_INDEX(ptr));
        }

        else if (rule != NULL && recycleCount != 0){
            rule -> refcnt++;
            ptr = recyclePointer; // if the recycle count is 0, then ptr points to the recyclePointer
            recyclePointer = recyclePointer->prev; //set the new recycle pointer to the previous pointer
            recyclePointer -> next = NULL; //then set the next of that to null
            ptr -> value = value;
            ptr -> refcnt = 0;
            ptr -> rule = rule;
            ptr -> next = NULL;
            ptr -> prev = NULL;
            ptr -> nextr = NULL;
            ptr -> prevr = NULL;
            recycleCount--;
                debug("new non-terminal symbol value: %d from a recycled pointer at <%lu>", value, SYMBOL_INDEX(ptr));
        }

        else if (rule == NULL && recycleCount == 0){
            //We just push the symbol into the array as is
            ptr = symbol_storage;
            for (i = 0; i < num_symbols; i++){
                ptr++;
            }

            ptr -> value = value;
            ptr -> refcnt = 0;
            ptr -> rule = NULL;
            ptr -> next = NULL;
            ptr -> prev = NULL;
            ptr -> nextr = NULL;
            ptr -> prevr = NULL;
            num_symbols++;
                debug("new terminal symbol value: %d at <%lu>", value, SYMBOL_INDEX(ptr));

        }
        else if (rule == NULL && recycleCount != 0){
            ptr = recyclePointer; // if the recycle count is 0, then ptr points to the recyclePointer
            recyclePointer = recyclePointer->prev; //set the new recycle pointer to the previous pointer
            recyclePointer -> next = NULL; //then set the next of that to null
            ptr -> value = value;
            ptr -> refcnt = 0;
            ptr -> rule = NULL;
            ptr -> next = NULL;
            ptr -> prev = NULL;
            ptr -> nextr = NULL;
            ptr -> prevr = NULL;
            recycleCount--;
                debug("new terminal symbol value: %d from a recycled pointer at <%lu>", value, SYMBOL_INDEX(ptr));
        }

    }
    return ptr; //return the pointer the array
}

/**
 * Recycle a symbol that is no longer being used.
 *
 * @param s  The symbol to be recycled.  The caller must not use this symbol any more
 * once it has been recycled.
 *
 * Symbols being recycled are added to the recycled_symbols list, where they will
 * be made available for re-allocation by a subsequent call to new_symbol.
 * The recycled_symbols list is managed as a LIFO list (i.e. a stack), using the
 * next field of the SYMBOL structure to chain together the entries.
 */
void recycle_symbol(SYMBOL *s) {
    // To be implemented.
    // Basically a recycled symbol is a linked list of symbols, and there is a pointer to the HEAD of the symbol
    debug("recycling symbol with value %d at <%lu>", s->value, SYMBOL_INDEX(s));
    if (recycleCount != 0){
        recyclePointer->next = s; //set the pointer's next to the symbol to be recycled
        s->prev = recyclePointer;
        recyclePointer = s;
        recycleCount++;
    }
    else if (recycleCount == 0){
        recyclePointer = s; //if the head is null, then the recyclePointer will now point to s instead
        s->prev = s;
        s->next = s; //set prev and next to itself
        recycleCount++; //increment the recycle count
    }
}

/*
* This function will be used to add a symbol to the body of a rule
*
* @param s The symbol to which a new symbol will be added to
* @param value  The value to be added to the symbol
*/
SYMBOL *add_symbol(int value, SYMBOL *s, SYMBOL *rule){
    struct symbol *ptr = NULL; //set up a pointer to a symbol
    ptr = new_symbol(value, NULL); //set the pointer to a fresh instance of a symbol with no rule to itself
    s -> next = ptr;
    ptr -> prev = s;
    //printf("current traveling in line 203 symbol.c");

    rule -> prev = ptr;
    ptr -> next = rule;

    // symbol *traverseListPointer = NULL; //a pointer to traverse the linked list to find the head
    //traverseListPointer = s; //set that pointer equal to s

    //if(traverseListPointer -> rule == traverseListPointer){//if the pointer's rule points to itself, or it is head
        //printf("current traveling in line 208 symbol.c");
        //traverseListPointer -> prev = ptr; //set the head's prev to the new pointer to complete a circular linked list
        //ptr -> next = traverseListPointer; //set the new symbol's next to the head node to complete a circular linked list
        //return ptr;
    //}

    //if not equal, then we use a while loop to find the head
    /*else{
        while (traverseListPointer->rule == NULL){ //while the pointer's rule is not itself, aka not head
            traverseListPointer = traverseListPointer -> prev; //we set the pointer to the previous node
        printf("current traveling in line 216 symbol.c");
            if (traverseListPointer -> rule != NULL){
                //if the rule is equal to itself, and is therefore a head
                traverseListPointer -> prev = ptr;
                ptr -> next = traverseListPointer;
                printf("destination found");
                return ptr;
            }
        }
    }*/

    return ptr;
}
