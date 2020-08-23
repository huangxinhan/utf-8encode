#include "const.h"
#include "sequitur.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */
int byteCounter = 0;

void initialize_byte(){
    byteCounter = 0; //resets the byte counter
}

//This method translates the value into UTF8 encoded bytes and then prints it out via FILE OUT
void utf8Encode(int value, FILE *out){
    //depending on the value of the character, we will either do 1, 2, 3, or 4 byte UTF8 encodings with this function
    //if the value is 0 - 127, then this is a single byte encoding
    //if the value is 128-2047, then this is a two-byte encoding
    //if the value is 2048-65535, then this is a 3-byte encoding
    //if the value is 65536-1114111, then it is a 4-byte encoding
    if (value >= 0 && value < 128){
        //One byte UTF8 encoding no shifting is necessary
        debug("one byte encoding, %d", value);
        fputc(value, out);
        byteCounter++;
    }
    else if (value >= 128 && value < 2048){
        //TWo byte UTF8 encoding
        //int finalValue = 0;
        int upperShifter = 192; // This is 11000000
        int upperMask = 1984; //This is binary 11111000000, takes the upper 5 bits
        int upperBits = (value & upperMask);
        upperBits = upperBits >> 6; //first shift the upper bits right by 6
        upperBits = (upperBits | upperShifter); //ori to combine with uppershifter
        fputc(upperBits, out);
        byteCounter++;

        int lowerShifter = 128; //This is 10000000
        int lowerMask = 63; //This is 111111, takes the lower 6 bits
        int lowerBits = (value & lowerMask);
        lowerBits = (lowerBits | lowerShifter);
        fputc(lowerBits, out);
        byteCounter++;

        //upperBits = upperBits << 8; //now shift upper bits by 8 to the left
        //finalValue = (upperBits | lowerBits); //combine upper bits and lower bits with ori to get the final value
        //debug("Two byte encoding final value: %d", finalValue);
        //fputc(finalValue, out);
    }
    else if (value >= 2048 && value < 65536){
        //Three byte UTF8 encoding
        //int finalValue = 0;
        int firstShifter = 224; //This is 1110 0000
        int firstMask = 61440; //This is 1111 0000 0000 0000, takes upper 4 bits
        int firstBits = (value & firstMask); //This gets the top 4 bits
        firstBits = firstBits >> 12; //shift the firstBits to the right by 12
        firstBits = (firstBits | firstShifter); //ori to combine with firstShifter
        fputc(firstBits, out);
        byteCounter++;

        int secondShifter = 128; //This is 10000000
        int secondMask = 4032; // This is 111111 000000 to take the middle 6 bytes
        int secondBits = (value & secondMask); //This gets the middle bits
        secondBits = secondBits >> 6; //shift the second bits right by 6
        secondBits = (secondBits | secondShifter); //ori to combine with secondShifter
        fputc(secondBits, out);
        byteCounter++;

        int thirdShifter = 128;
        int thirdMask = 63;
        int thirdBits = (value & thirdMask);
        thirdBits = (thirdBits | thirdShifter);
        fputc(thirdBits, out);
        byteCounter++;


        //firstBits = firstBits << 16;
        //secondBits = secondBits << 8;
        //finalValue = (firstBits | secondBits | thirdBits);
        //debug("Three byte encoding final value: %d", finalValue);
        //fputc(finalValue, out);
    }
    else if (value >= 65536 && value < 1114115){
        //Four byte UTF8 encoding
        //int finalValue = 0;
        int firstShifter = 240; //This is 1111 0000
        int firstMask = 1835008; //THis takes the upper 3 bits
        int firstBits = (value & firstMask);
        firstBits = firstBits >> 18;
        firstBits = (firstBits | firstShifter);
        fputc(firstBits, out);
        byteCounter++;

        int secondShifter = 128;
        int secondMask = 258048; //takes the middle 6 bits
        int secondBits = (value & secondMask);
        secondBits = secondBits >> 12;
        secondBits = (secondBits | secondShifter);
        fputc(secondBits, out);
        byteCounter++;

        int thirdShifter = 128;
        int thirdMask = 4032; //This takes the middle 6 bytes
        int thirdBits = (value & thirdMask);
        thirdBits = thirdBits >> 6;
        thirdBits = (thirdBits | thirdShifter);
        fputc(thirdBits, out);
        byteCounter++;

        int fourthShifter = 128;
        int fourthMask = 63;
        int fourthBits = (value & fourthMask);
        fourthBits = (fourthBits | fourthShifter);
        fputc(fourthBits, out);
        byteCounter++;

        //firstBits = firstBits << 24;
        //secondBits = secondBits << 16;
        //thirdBits = thirdBits << 8;
        //finalValue = (firstBits | secondBits | thirdBits | fourthBits);
        //debug("Four byte encoding final value: %d", finalValue);
        //fputc(finalValue, out);
    }


}

//This method recursiely traverses the linked list and prints out the UTF8 encodings
void traverseLinkedList(SYMBOL *rule, FILE *out){
    //FIrst we must define several different cases regarding the rule head
    SYMBOL *ptr = rule; //first we define a pointer that points to the rule itself
    if (ptr -> nextr == main_rule){
        //IF the pointer's next rule is the main rule, either the main rule is the ONLY rule
        //Or we run into a situation where the rule's next rule is main, so we can end the loop there, no recursion
        if (ptr -> next -> rule == ptr -> next){
            //IF the pointer's next rule is the next rule, aka we reached the end
            utf8Encode(ptr -> value, out);
        }
        while (ptr->next->rule != ptr -> next){
            //While the next rule is NOT the main rule

            utf8Encode(ptr -> value, out); //Then we encode it
            ptr = ptr -> next; //advance the pointer
            if (ptr->next->rule == ptr->next){
                //we need to print out the EOB marker
                utf8Encode(ptr -> value, out);
                fputc(132, out);
                byteCounter++;
                //fflush(out); //Then flush the out and break out of the loop
                break; //break out when we reached the end
            }
        }
    }
    else{
        //if the pointer's next rule is NOT the main rule, then we need to recursively call this function
        if (ptr -> next -> rule == ptr -> next){
            utf8Encode(ptr -> value, out);
            fputc(133, out);
            byteCounter++;
        }
        while (ptr->next->rule != ptr->next){

            utf8Encode(ptr -> value, out);
            ptr = ptr -> next;
            //Now if we reach the end, we recursively call this function
            if (ptr->next->rule == ptr->next){
                utf8Encode(ptr -> value, out);
                fputc(133, out); //print out a rule delimiter at the end of this ruleset
                byteCounter++;
                traverseLinkedList(ptr->next->nextr, out); //Then we head to the next main rule
            }
        }
    }

}





/**
 * Main compression function.
 * Reads a sequence of bytes from a specified input stream, segments the
 * input data into blocks of a specified maximum number of bytes,
 * uses the Sequitur algorithm to compress each block of input to a list
 * of rules, and outputs the resulting compressed data transmission to a
 * specified output stream in the format detailed in the header files and
 * assignment handout.  The output stream is flushed once the transmission
 * is complete.
 *
 * The maximum number of bytes of uncompressed data represented by each
 * block of the compressed transmission is limited to the specified value
 * "bsize".  Each compressed block except for the last one represents exactly
 * "bsize" bytes of uncompressed data and the last compressed block represents
 * at most "bsize" bytes.
 *
 * @param in  The stream from which input is to be read.
 * @param out  The stream to which the block is to be written.
 * @param bsize  The maximum number of bytes read per block.
 * @return  The number of bytes written, in case of success,
 * otherwise EOF.
 */
int compress(FILE *in, FILE *out, int bsize) {
    // To be implemented.
    //start off by outputting a SOT block onto the output stream
    SYMBOL *newPtr; //this pointer always points torwards the newly created SYMBOL
    SYMBOL *ptr;

    initialize_byte();//Initialize the byte counter first

    int sizeCounter = 0; //This size counter keeps track of the b size
    int value = 129; //This is the 0x82 SOT block
    //int mainRule = 256;
    fputc(value, out); //First output the SOT block onto the
    byteCounter++;
    init_symbols();
    init_rules();
    init_digram_hash();
    //First reinit those 3, then create the main rule SYMBOL
    ptr = new_rule(next_nonterminal_value); //Then set the main rule as the value of the FIRST_NONTERMINAL
    add_rule(ptr);
    //*(rule_map + next_nonterminal_value) = ptr;

    do{

        if (sizeCounter == bsize){
            //if the size counter reaches b size, then we must end the block and start a new block
            //Now start outputting everything to UTF-8 format first...
            //mainRule = 256; //reinstantiate the main rule value
            fputc(131, out); //print out the SOB marker
            byteCounter++;
            traverseLinkedList(main_rule, out); //traverse the linked list starting from the main head rule
            init_symbols();
            init_rules();
            init_digram_hash();
            ptr = new_rule(next_nonterminal_value);
            add_rule(ptr);
            //*(rule_map + next_nonterminal_value) = ptr;

        }

        int valuein = fgetc(in); //gets the next byte of value
        if (valuein == -1){
            //if EOF is reached, expand out the block of rule we have, then return it.
            fputc(131, out);
            byteCounter++;
            traverseLinkedList(main_rule, out);
            fputc(130, out); // output the EOT marker
            byteCounter++;
            fflush(out);
            break;
        }

        //printf("%x",ptr->value);
        newPtr = new_symbol(valuein, main_rule);
        //printf("head's previous %d\n", main_rule -> prev -> value);
        insert_after(main_rule -> prev, newPtr); //use insert_after() to insert the newly created symbol from the byte after ptr
        //ptr = newPtr; //advance the old pointer
        check_digram(newPtr->prev); //Then we use check_digram on the second to last symbol
        sizeCounter++;

    }while(1);

    return byteCounter;
}

void recursiveOutput(SYMBOL *rule, FILE *out);

void mainruleExpansion(SYMBOL *rule, FILE *out){
    SYMBOL *ptr = rule; //set a pointer to the main rule
    debug("pointer's value in expansion is %d", ptr -> value);
    while (ptr -> next -> rule != ptr -> next){
        //while the pointer's next rule is not the same rule, aka not the main rule
        ptr = ptr -> next; //advance to the next symbol body
        debug("ptr's next value is %d", ptr->value);
        if (ptr -> value < FIRST_NONTERMINAL){
            debug("less than first non terminal");
            fputc((ptr->value), out); //then print out that byte of data
            byteCounter++;

        }
        else{
            //IF it is not a terminal value, but a non-terminal, recursively call itself
            debug("non-terminal symbol encountered");
            SYMBOL *terminal_symbol = *(rule_map + (ptr -> value));
            debug("terminal symbol has value %d", terminal_symbol -> value);
            mainruleExpansion(terminal_symbol, out);
        }
    }
}


void recursiveOutput(SYMBOL *rule, FILE *out){

    if (((rule->value) < FIRST_NONTERMINAL)){
        fputc((rule->value), out);
        if ((rule->next)->rule != rule->next){
            //printf("terminal %d ", rule->value);
            recursiveOutput((rule->next), out);
            return;
        }
        else{
            //printf("aw it ended ");
            return;
        }
    }
    else{
        //It is a non-terminal
        SYMBOL *terminal_Symbol = *(rule_map + (rule->value));
        //printf("non-terminal %d ", rule->value);
        recursiveOutput((terminal_Symbol->next), out);
        return;
    }
}

void printList(SYMBOL *rule){
    //SYMBOL *ptr = rule;
    while ((rule->nextr) != main_rule){
        printf("%x RULE HEADS", rule->value);
        SYMBOL *ptr = rule;
        while(ptr->next != rule){
           printf("%x rule bodies", ptr->value);
            ptr = ptr->next;
        }
        rule = rule->nextr;
    }

    for (int i = 0; i < MAX_SYMBOLS; i++){
        if (*(rule_map + i)){
            printf("%p\n", *(rule_map + i));
        }
    }
}


/**
 * Main decompression function.
 * Reads a compressed data transmission from an input stream, expands it,
 * and and writes the resulting decompressed data to an output stream.
 * The output stream is flushed once writing is complete.
 *
 * @param in  The stream from which the compressed block is to be read.
 * @param out  The stream to which the uncompressed data is to be written.
 * @return  The number of bytes written, in case of success, otherwise EOF.
 */
SYMBOL *add_symbol(int value, SYMBOL *s, SYMBOL *rule);
int get_utf8(FILE *in);
int decompress(FILE *in, FILE *out) {

    initialize_byte(); //First reset the byte counter to 0
    // To be implemented.
    SYMBOL *currentSymbol = NULL; //pointer that points to the current symbol
    //SYMBOL *currentRule = NULL; //pointer that points to the current rule
    SYMBOL *currentHead = NULL; //this pointer points to the head of a rule

    int value = get_utf8(in);
    if (value == 129){
        //If the value is indeed 129, which SHOULD be the first value
        init_symbols();
        init_rules();
    }
    else{
        return EOF; //return an error if not the case at all
    }

    do{
    int value = get_utf8(in);

    if (value == -1){

        break; //EOF break out of the loop
    }
    /*else if (value == 129){
        //If the value is a 0x81 SOT marker
        init_symbols();
        init_rules();
    }*/

    else if (value == 130){
        //IF the value is 0x82 EOT marker
        fflush(out);
    }

    else if (value == 131){
        debug("SOB marker detected");
        //if the value is 0x83 SOB marker
        init_rules(); //call init rules to destroy all previous rules?
        int nextValue = get_utf8(in);
        SYMBOL *rule = new_rule(nextValue); //create a new rule
        add_rule(rule); //and add the rule to itself fist
        *(rule_map + nextValue) = rule; //Add that value to the rulemap
        debug("adding value %d to rulemap", nextValue);
        //currentRule = rule;
        currentSymbol = rule;
        currentHead = rule;
    }

    else if (value == 132){
        //if the value is the EOB marker 0x84
        //continue; // go to the next and break
        //we will need to start using recursion to print out the rules

        //recursiveOutput(main_rule -> next, out); //recursively output the main rule and out
        //printList(main_rule);
        debug("end of block reached, expanding rule %d", main_rule->value);
        mainruleExpansion(main_rule, out);
    }

    else if (value == 133){
        //if the value is the rule delimiter or 0x85
        int nextValue = get_utf8(in);
        SYMBOL *rule = new_rule(nextValue);
        add_rule(rule);
        *(rule_map + nextValue) = rule;
        //currentRule = rule;
        currentSymbol = rule;
        currentHead = rule;
        debug("adding value %d to rulemap", nextValue);
    }

    else{
        //For any other values we append and build the rules recursively

         currentSymbol = add_symbol(value, currentSymbol, currentHead); //we will add a rule after the current symbol
    }

    }
    while(1);

    if (byteCounter == 0){
        return EOF; //if there is no rule body, and just a  rule, return EOF
    }

    return byteCounter;
}

int get_utf8(FILE *in){
    char headCheckerOneByte = 0; //This checks whether or not the binr begins with a 0
    char headCheckerTwoByte = 192; //This checks whther or not the binary begins with 110
    char headCheckerThreeByte = 224; //This checks whether or not the binary begins with 1110
    char headCheckerFourByte = 240; //This checks whether or not the binary begins with 11110

    char bitwiseShifterThree = 7; //THis is the binary 0000 0111 and it can be used to andi to retrieve the last 3 digits
    char bitwiseShifterFour = 15; //This is the binary 0000 1111 and it can be used to andi to retrieve the last 4 digits
    char bitwiseShifterFive = 31; //This is the binary 0001 1111 and it can be used to andi to retrieve the last 5 digits
    char bitwiseShifterSix = 63;
    char bitwiseShifterSeven = 127; //This is the binary 0111 1111 and it can be used to andi to retrieve the last 7 digits


        int c = fgetc(in);
        debug ("getting utf8 %x", c);

        //printf("%x\n", c);
        if(c == -1){
            //printf("%d EOF marker detected", c);
            return c;
            //fclose(in);
        }

        int ruleDelimiterChecker = 133; //THis char will only check for 85s
        int SOTmakerChecker = 129; //This char will check for the SOT marker, which is 0x81
        int SOBmarkerChecker = 131; //THis char will check for 131, which is the start of rule marker
        int EOTmarkerChecker = 130; //This char will check for the EOT marker, which is 0x82
        int EOBmarkerChecker = 132; //This char will check for the EOB marker, which is 0x84
        if(c == ruleDelimiterChecker){
            //start a new rule
            //printf("%x This is a rule delimiter! \n", c);
            return c;
        }

        if (c == SOTmakerChecker){
            //printf("%x this is the SOT marker \n",c );
            return c;
        }

        if (c == SOBmarkerChecker){
            //printf("%x This is the SOR marker \n",c);
            //Start creating a new rule
            return c;
        }

        if (c == EOTmarkerChecker){
            //printf("%x THIS IS THE EOT MARKER CHECKER! \n",c);
            return c;
        }

        if (c == EOBmarkerChecker){
            //printf("%x THIS IS THE EOB MARKER CHECKER \n",c);
            return c;
        }


        char twoByteNumberChecker = 224; //this will check whether or not the byte begins with the binary 110
        char FourByteNumberChecker = 240; //this will be the bitwise 11110000 to check whether the byte begins with binary 1111
        char oneByteNumberChecker = 128; //This will be 1000 0000 and checks whether the byte is empty
        char comparator1 = (c & twoByteNumberChecker); //andi the byte with 1110 0000 to get first 3 most significant bits
        char comparator2 = (c & FourByteNumberChecker);
        char comparator3 = (c & oneByteNumberChecker);
        if (comparator2 == headCheckerFourByte){
            //if the comparator shows that the byte begins with 1111, this is a four byte encoding
            int d = fgetc(in);

            int e = fgetc(in);

            int f = fgetc(in);

            int firstValue = (c & bitwiseShifterThree); //retrieves the last 3 digits of the first value
            int secondValue = (d & bitwiseShifterSix); //retrieves the last 6 digits of the second value
            int thirdValue = (e & bitwiseShifterSix); //retrieves the last 6 digits of the third value
            int fourthValue = (f & bitwiseShifterSix); //retrieves the last 6 digits of the fourth value
            int combinedValue;
            firstValue = firstValue << 18; //shift over the first value to the left by 18
            secondValue = secondValue << 12; //shift over the second value to the left by 12
            thirdValue = thirdValue <<  6;
            combinedValue = (firstValue | secondValue | thirdValue | fourthValue); //the combinedValue is the ori of all the values
            //printf("%x -> 4-byte encoding \n", combinedValue);
            return combinedValue;

        }
        else if (comparator1 == headCheckerThreeByte){
            //if the result contains 3 byte utf-8 encodings
            int d = fgetc(in);
            int e = fgetc(in);
            int firstValue = (c & bitwiseShifterFour); //retrieve the last 4 digits
            int secondValue = (d & bitwiseShifterSix); //retrieve the last 6 digits
            int thirdValue = (e & bitwiseShifterSix); //retrieves the last 6 digits
            int combinedValue;
            firstValue = firstValue << 12; //Shift over by 12
            secondValue = secondValue << 6; //shift over by 6
            combinedValue = (firstValue | secondValue | thirdValue);
            //printf("%x -> 3-byte encoding \n", combinedValue);
            return combinedValue;

        }

        else if (comparator1 == headCheckerTwoByte) {
            //If the bitwise andi of 1110 0000 and the byte gives 1100 0000, this means that it is a large number.
            //Having a large number mean that the UTF-8 code will need to involve the next number.
            int d = fgetc(in); //get the next byte right now
            int firstValue = (c & bitwiseShifterFive); //retrieves the last 5 digits
            int secondValue = (d & bitwiseShifterSix); //retrieves the last 6 digits of the second value
            int combinedValue; //This will be the value that holds both first and second value into one symbol value
            firstValue = firstValue << 6; //shift the first value to the left by 6
            combinedValue = (firstValue | secondValue); //THe combined value will be the ori of first and second value
            //printf("%x -> 2-byte encoding \n", combinedValue);
            return combinedValue;



        }

        else if (comparator3 == headCheckerOneByte){
            //The result contains a 1 byte utf-8 encoding if such is that case
            //If the bitwise andi of 1000 0000 and the byte gives 0000 0000, this means that this is a one byte UTF-8 encoding
            int combinedValue = (c & bitwiseShifterSeven);

            //printf("%x -> 1-byte encoding \n", combinedValue);
            return combinedValue;

        }

        else{
            //If nothing else fits the criteria, then that means we return EOF because the file format contains an error

            //printf("error!! \n");
            debug("error");
            return EOF;
        }

    return EOF;
}

int get_utf8Test(FILE *in){
    //int returnValue = 0; //This is the initial return value for this function
    int headCheckerOneByte = 0; //This checks whether or not the binr begins with a 0
    int headCheckerTwoByte = 192; //This checks whther or not the binary begins with 110
    int headCheckerThreeByte = 224; //This checks whether or not the binary begins with 1110
    int headCheckerFourByte = 240; //This checks whether or not the binary begins with 11110

    int bitwiseShifterThree = 7; //THis is the binary 0000 0111 and it can be used to andi to retrieve the last 3 digits
    int bitwiseShifterFour = 15; //This is the binary 0000 1111 and it can be used to andi to retrieve the last 4 digits
    int bitwiseShifterFive = 31; //This is the binary 0001 1111 and it can be used to andi to retrieve the last 5 digits
    int bitwiseShifterSix = 63;
    int bitwiseShifterSeven = 127; //This is the binary 0111 1111 and it can be used to andi to retrieve the last 7 digits

    do{
        int c = fgetc(in);
        printf("%x\n", c);
        if(c == -1){
            printf("reached the end");
            printf("%d ", c);
            break;
            //fclose(in);
        }

        int ruleDelimiterChecker = 133; //THis char will only check for 85s
        int SOTmakerChecker = 129; //This char will check for the SOT marker, which is 0x81
        int SOFmarkerChecker = 131; //THis char will check for 131, which is the start of rule marker
        int EOTmarkerChecker = 130; //This char will check for the EOT marker, which is 0x82
        int EOBmarkerChecker = 132; //This char will check for the EOB marker, which is 0x84
        if(c == ruleDelimiterChecker){
            //start a new rule
            printf("%x This is a rule delimiter! \n", c);
            continue;
        }

        if (c == SOTmakerChecker){
            printf("%x this is the SOT marker \n",c );
            continue;
            //c = fgetc(in); //goes to the next character
        }

        if (c == SOFmarkerChecker){
            printf("%x This is the SOR marker \n",c);
            //Start creating a new rule
            continue;
        }

        if (c == EOTmarkerChecker){
            printf("%x THIS IS THE EOT MARKER CHECKER! \n",c);
            continue;
        }

        if (c == EOBmarkerChecker){
            printf("%x THIS IS THE EOB MARKER CHECKER \n",c);
            continue;
        }


        int twoByteNumberChecker = 224; //this will check whether or not the byte begins with the binary 110
        int FourByteNumberChecker = 240; //this will be the bitwise 11110000 to check whether the byte begins with binary 1111
        int oneByteNumberChecker = 128; //This will be 1000 0000 and checks whether the byte is empty
        int bytechecker = 192; //This will be 1100 0000 to check whether or not the next getfs start with 10

        int comparator1 = (c & twoByteNumberChecker); //andi the byte with 1110 0000 to get first 3 most significant bits
        int comparator2 = (c & FourByteNumberChecker);
        int comparator3 = (c & oneByteNumberChecker);
        if (comparator2 == headCheckerFourByte){
            //if the comparator shows that the byte begins with 1111, this is a four byte encoding
            int d = fgetc(in);
            int dChecker = (d & bytechecker); //
            if(dChecker != oneByteNumberChecker){
                return EOF;
            }
            int e = fgetc(in);
            int eChecker = (e & bytechecker);
            if (eChecker != oneByteNumberChecker){
                return EOF;
            }
            int f = fgetc(in);
            int fChecker = (f & bytechecker);
            if (fChecker != oneByteNumberChecker){
                return EOF;
            }
            int firstValue = (c & bitwiseShifterThree); //retrieves the last 3 digits of the first value
            int secondValue = (d & bitwiseShifterSix); //retrieves the last 6 digits of the second value
            int thirdValue = (e & bitwiseShifterSix); //retrieves the last 6 digits of the third value
            int fourthValue = (f & bitwiseShifterSix); //retrieves the last 6 digits of the fourth value
            int combinedValue;
            firstValue = firstValue << 18; //shift over the first value to the left by 18
            secondValue = secondValue << 12; //shift over the second value to the left by 12
            thirdValue = thirdValue <<  6;
            combinedValue = (firstValue | secondValue | thirdValue | fourthValue); //the combinedValue is the ori of all the values
            printf("%x -> code1 \n", combinedValue);

        }

        else if (comparator1 == headCheckerThreeByte){
            //if the result contains 3 byte utf-8 encodings
            int d = fgetc(in);
            int dChecker = (d & bytechecker);
            if (dChecker != oneByteNumberChecker){
                return EOF;
            }
            int e = fgetc(in);
            int eChecker = (e & bytechecker);
            if (eChecker != oneByteNumberChecker){
                return EOF;
            }
            int firstValue = (c & bitwiseShifterFour); //retrieve the last 4 digits
            int secondValue = (d & bitwiseShifterSix); //retrieve the last 6 digits
            int thirdValue = (e & bitwiseShifterSix); //retrieves the last 6 digits
            int combinedValue;
            firstValue = firstValue << 12; //Shift over by 12
            secondValue = secondValue << 6; //shift over by 6
            combinedValue = (firstValue | secondValue | thirdValue);
            printf("%x -> code3 \n", combinedValue);
        }

        else if (comparator1 == headCheckerTwoByte) {
            //If the bitwise andi of 1110 0000 and the byte gives 1100 0000, this means that it is a large number.
            //Having a large number mean that the UTF-8 code will need to involve the next number.
            int d = fgetc(in); //get the next byte right now
            int dChecker = (d & bytechecker);
            if (dChecker != oneByteNumberChecker){
                return EOF;
            }
            int firstValue = (c & bitwiseShifterFive); //retrieves the last 5 digits
            int secondValue = (d & bitwiseShifterSix); //retrieves the last 6 digits of the second value
            int combinedValue; //This will be the value that holds both first and second value into one symbol value
            firstValue = firstValue << 6; //shift the first value to the left by 6
            combinedValue = (firstValue | secondValue); //THe combined value will be the ori of first and second value
            printf("%x -> code2 \n", combinedValue);

        }

        else if (comparator3 == headCheckerOneByte){
            //The result contains a 1 byte utf-8 encoding if such is that case
            //If the bitwise andi of 1000 0000 and the byte gives 0000 0000, this means that this is a one byte UTF-8 encoding
            int combinedValue = (c & bitwiseShifterSeven);

            printf("%x -> code4 \n", combinedValue);

        }

        else{
            //If nothing else fits the criteria, then that means we return EOF because the file format contains an error

            printf("error!! \n");
            return EOF;
        }


    }while(1);

    return EOF;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int compare_string(char *first, char *second);
int to_int(char *string);
int power(int base, int exponent);

int validargs(int argc, char **argv)
{
    // To be implemented.
    // If the string argument starts with a \0
    //int i;
    //for (i = 0; i < argc; i++){
        //printf("argv[%d] = %s\n", i, argv[i]);
    //}

    //If the argument starts with bin, then we append the pointer
    if(compare_string(*argv, "bin/sequitur") == 0 && argc > 1){
    argv++;
    }

    else{
        return -1;
    }

    //case that the flag is -h, then exit success
    if (compare_string(*argv, "-h") == 0){
        global_options = 1;
        return 0;
    }
    else if (compare_string(*argv, "-c") == 0 && argc == 3){
        return -1;
    }

    //if the first flag is -c
    else if (compare_string(*argv, "-c") == 0 && argc > 3){
        //printf("actually here");
        global_options = 2;
        argv++;
        if(compare_string(*argv, "-h") == 0){
            //if -h is after -d, then exit failure
            global_options = 0;
            return -1;
        }

        else if(compare_string(*argv, "-d") == 0){
            //cannot have -d after h, this returns an error
            global_options = 0;
            return -1;
        }

        else if(compare_string(*argv, "-b") == 0 && argc == 4){
            argv++; //if -b, then we need to specify the block size
            if(compare_string(*argv, "\0") == 0){
                global_options = 0;
                return -1; //-b must be followed by a number
            }


            int temp = to_int(*argv);
            if (temp > 1024 || temp < 0){
                global_options = 0;
                return -1; //BLOCK_SIZE NOT IN RANGE
            }
            else{
                int masker = temp;
                masker = (masker << 16); //setting masker to 16 most significant bit
                global_options = (global_options | masker); //global_options ori masker
                return 0;
            }

        }

        else{
            return -1;
        }


    }

    else if (compare_string(*argv, "-c") == 0 && argc == 2){
        global_options = 2;
        return 0;
    }

    else if (compare_string(*argv, "-c") == 0 && argc != 2){
        argv++;
        global_options = 2;
        return 0;
    }

    //if the first flag is -d
    else if (compare_string(*argv, "-d") == 0 && argc == 2){
        global_options = 4;
        return 0;
    }



    else if (compare_string(*argv, "-d") == 0 && argc != 2){
        argv++; //increment argv
        if (compare_string(*argv, "-h") == 0){
            //if -h is after -d, then exit failure
            global_options = 0;
            return -1;
        }

        else if (compare_string(*argv, "-b") == 0){
            //if -b is after -d, then this is certainly incorrect
            global_options = 0;
            return -1;
        }

        else if (compare_string(*argv, "-c") == 0){
            //cannot have -c after -d, this returns an error
            global_options = 0;
            return -1;
        }

        else{
            global_options = 0;
            return -1;
        }

    }

    //if -b came before any optional parameter, aka second arg, it is incorrect
    else if (compare_string(*argv, "-b") == 0){
        return -1;
    }

    //case that no flags are provided
    else if (compare_string(*argv, "\0") == 0){
        return -1;
    }

    else{
        //All other cases
        return -1;
    }

    return 0;
}


int compare_string(char *first, char *second){
    if (*first != *second){
        //If the first characters are not the same
        return -1;
    }

    else{

        while(*first == *second){

            first++;
            second++; //increment the pointers to the next character of the string
            if(*first != *second){
                return -1;
            }
            else if (*first == '\0' || *second == '\0'){
                break;
            }

        }

        if (*first == '\0' && *second == '\0'){
            return 0; //IF they reach the end of the null pointer at the same time, they are equal
        }
        else{
            return -1; //else they are not
        }
    }

}

int to_int(char *string){
    int i;
    int temp;
    int temp2;
    int result = 0;
    int counter = 0; //counts the number of characters in the string
    while(*string != '\0'){
        counter++;
        string++;

    }
    for (i = 0; i < counter; i++){
        string--; //reset the string
    }


    for (i = counter; i > 0; i--){
        temp = *string - 48;
        temp2 = power(10, i - 1) * temp;
        result += temp2;
        string++;
    }
    return result;
}

int power(int base, int exponent){
    int result = 1;
    while(exponent != 0){
        result *= base;
        exponent--;
    }
    return result;
}


