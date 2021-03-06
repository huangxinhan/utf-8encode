#include "const.h"
#include "sequitur.h"

/*
 * Digram hash table.
 *
 * Maps pairs of symbol values to first symbol of digram.
 * Uses open addressing with linear probing.
 * See, e.g. https://en.wikipedia.org/wiki/Open_addressing
 */

/**
 * Clear the digram hash table.
 */
void init_digram_hash(void) {
    // To be implemented.
    //sets all of digram table to null
    int i;
    for (i = 0; i < MAX_DIGRAMS; i++){
    *(digram_table + i) = NULL;
    }



}

/**
 * Look up a digram in the hash table.
 *
 * @param v1  The symbol value of the first symbol of the digram.
 * @param v2  The symbol value of the second symbol of the digram.
 * @return  A pointer to a matching digram (i.e. one having the same two
 * symbol values) in the hash table, if there is one, otherwise NULL.
 */
SYMBOL *digram_get(int v1, int v2) {
    // To be implemented.

    SYMBOL *ptr = NULL; //first initialize a pointer that we will be using to traverse the array
    int index = DIGRAM_HASH(v1, v2); //first get the initial index from the digram hash function
    //Then starting from that index, we look to check the digram
    ptr = *(digram_table + index); //set the pointer to the digram_table address
    if (ptr == NULL || ptr == TOMBSTONE){
        return NULL;
    }

    else if (ptr->value == v1){
        if(ptr -> next -> value == v2){ //if statement together then it would cause a null pointer
            return ptr; //return the pointer if the value is equal to v1 and the next value is equal to v2
        }
    }

    while (ptr->value != v1 && ptr->next->value != v2){
        index = (index + 1) % MAX_DIGRAMS;
        ptr = *(digram_table + index);
        if (ptr == NULL || ptr == TOMBSTONE){
            //IF the ptr is null
            return NULL; //if nothing is found, return null
        }

        else if (ptr->value == v1){
            if (ptr->next->value == v2){
                return ptr; //return the pointer if the value is equal to v1 and the next value is equal to v2
            }
        }
    }
    return NULL;
}

/**
 * Delete a specified digram from the hash table.
 *
 * @param digram  The digram to be deleted.
 * @return 0 if the digram was found and deleted, -1 if the digram did
 * not exist in the table.
 *
 * Note that deletion in an open-addressed hash table requires that a
 * special "tombstone" value be left as a replacement for the value being
 * deleted.  Tombstones are treated as vacant for the purposes of insertion,
 * but as filled for the purpose of lookups.
 *
 * Note also that this function will only delete the specific digram that is
 * passed as the argument, not some other matching digram that happens
 * to be in the table.  The reason for this is that if we were to delete
 * some other digram, then somebody would have to be responsible for
 * recycling the symbols contained in it, and we do not have the information
 * at this point that would allow us to be able to decide whether it makes
 * sense to do it here.
 */
int digram_delete(SYMBOL *digram) {
    // To be implemented.
    if (digram == NULL){
        return -1;
    }

    SYMBOL *ptr = NULL; //initialize a symbol pointer first
    int v1 = digram -> value;
    int v2 = digram -> next -> value;
    //FIrst get the two values associated with the digram
    int index = DIGRAM_HASH(v1, v2);
    ptr = *(digram_table + index);
    if (ptr == NULL){
        return -1;
    }

    else if(ptr == digram){
        *(digram_table + index) = TOMBSTONE;
        return 0;
    }

    while (ptr != digram){
        index = (index + 1) % MAX_DIGRAMS;
        ptr = *(digram_table + index);

        if (ptr == digram){
            *(digram_table + index) = TOMBSTONE; //set tombstone to digram + index, to delete from hash table
            return 0; //return 0 if found and deleted
        }

        else if (ptr == NULL){
            //IF the ptr is null
            return -1; //if nothing is found, return -1
        }
    }
    return -1;
}

/**
 * Attempt to insert a digram into the hash table.
 *
 * @param digram  The digram to be inserted.
 * @return  0 in case the digram did not previously exist in the table and
 * insertion was successful, 1 if a matching digram already existed in the
 * table and no change was made, and -1 in case of an error, such as the hash
 * table being full or the given digram not being well-formed.
 */
int digram_put(SYMBOL *digram) {
    if (digram == NULL){
        return -1;
    }
    //First check if the matching digram already exists in the hash table
    SYMBOL *ptr = NULL; //first initialize a SYMBOL
    if (digram -> next == NULL){
        //if digram's next is NULL, making the digram not well-formed;
        return -1;
    }
    int v1 = digram -> value;
    int v2 = digram -> next -> value;

    int index = DIGRAM_HASH(v1,v2); //get the index from the DIGRAM_HASH
    ptr = *(digram_table + index);
    if(ptr == NULL || ptr == TOMBSTONE){
        *(digram_table + index) = digram;
        return 0;
    }
    else if (ptr == digram){
        return 1;
    }
    while(ptr != NULL || ptr != TOMBSTONE){
        index = (index + 1) % MAX_DIGRAMS;
        ptr = *(digram_table + index);
        if (ptr == NULL || ptr == TOMBSTONE){
            //if the ptr is null or a  tombstone, then we insert it
            *(digram_table + index) = digram;
            return 0;
        }
        else if (ptr == digram){
            //If we find that the ptr is equal to the digram first, then it already exists
            return 1;
        }
    }

    return -1;
}
