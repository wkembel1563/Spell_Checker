#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spell.h"

/*  Suggestions
- When you work with 2D arrays, be careful. Either manage the memory yourself, or
work with local 2D arrays. Note C99 allows parameters as array sizes as long as 
they are declared before the array in the parameter list. See: 
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/

Worst case time complexity to compute the edit distance from T test words
 to D dictionary words where all words have length MAX_LEN:
- Each edit distance call = O(MAX_LEN^2) 
- Computing for D words = O(D * MAX_LEN^2) 
- Computing for D words, T different times = O(T * D * MAX_LEN^2) 
Student answer:  Theta(T*D*MAX_LEN^2)

Worst case to do an unsuccessful binary search in a dictionary with D words, when 
all dictionary words and the searched word have length MAX_LEN 
- Worst case strcmp = O(MAX_LEN)
- Worst case bin search = O(lgD) for D words 
- 1 strmp per loop iteration of search 
Student answer:  Theta(MAX_LEN * lgD)
*/
/* You can write helper functions here */

// IS PUNCTUATION
// Returns true for !., or ? 
int isPunctuation(char c){

	int result = 0; 

	if(c == '!' || c == '.' || c == ',' || c == '?') result = 1;

	return result; 

}


// BINARY SEARCH 
// O(MAX_LEN*logN) due to strcmp 
int binarySearch(int length, int width, char array[length][width], char key[width], int left, int right, int printOn){

	// place key in convertable array 
	char sort_key[width]; 
	strcpy(sort_key, key); 

	// convert to lowercase
	int i;
	for(i = 0; sort_key[i] != '\0'; ++i) if(sort_key[i] < 91 && sort_key[i] > 64) sort_key[i] += 32; 

	if(printOn) printf("Binary search for:  %s\n", key); 

	int found = 0; 
	int words_compared = 0; 
	int m = 0, comp = 0; 
	while(left <= right){
		
		m = (left + right)/2; 
		if(printOn) printf("dict[%d] = %s\n", m, array[m]); 
		comp = strcmp(array[m], sort_key); 
		words_compared++; 
		if(comp == 0){			// found 	
			found = 1; 
			break; 
		}
		else if(comp > 0) right = m - 1; 
		else 		  left = m + 1;

		if(left > right && printOn) printf("Not found\n\n"); 
	}

	// result notification
	if(found == 0){
		printf("---> |%s| (words compared when searching: %d)\n", key, words_compared); 
	}
	else{
		
		printf("---> |%s| (words compared when searching: %d)\n\t- OK\n\n\n", key, words_compared); 
	}

	return found; 
} 

// COMPARE
// Restatement of strcmp for use as parameter of qsort 
int compare(const void * str1, const void * str2){ return strcmp(str1, str2); }

// FIND MIN 
// finds minimum of three ints
int findMin(int x, int y, int z){

	int min = 0; 

	if(x <= y && x <= z) min = x; 	// x is smaller than both or they are all equal 
	else if(y < z) min = y; 	// x must be bigger than z or y, and they can't be equal
					// therefore the min is z or y
	else min = z; 			// min is not y, so must be z	

	return min; 
}

// O(MAX_LEN * MAX_LEN) = O(MAX_LEN^2) 
int editDistanceCalc(int ** table, int width, int height){

	// distance calcs start at (1,1)
	int i, j, left, corner, top; 
	int match = 0; 
	for(i = 1; i < height; i++){
		for(j = 1; j < width; j++){
			
			// check if the chars are the same
			if(table[0][j] == table[i][0]) match = 1; 

			// 4 cases: (1,1) calc, first row, first col, everything else
			if(i == 1 && j == 1){	// (1,1) calculation

				// set corner calc
				if(match) corner = 0; 
				else corner = 1; 
				// set left, top calcs
				left = i+1; 
				top = j+1; 

				// calculate edit distance 
				table[i][j] = findMin(corner, left, top); 
				

			}
			else if(i == 1){	// first row calculations

				// set corner calc
				if(match) corner = (j - 1); 
				else corner = j; 
				// set left, top calcs
				left = table[i][j-1] + 1; 
				top = j + 1; 

				// calculate edit distance 
				table[i][j] = findMin(corner, left, top); 
				
			}
			else if(j == 1){	// first col calculation 

				// set corner calc
				if(match) corner = (i - 1); 
				else corner = i; 
				// set left, top calcs
				left = i + 1; 
				top = table[i-1][j] + 1; 

				// calculate edit distance 
				table[i][j] = findMin(corner, left, top); 
				
			}
			else{			// normal cell calculation 

				// set corner calc
				if(match) corner = table[i-1][j-1]; 
				else corner = table[i-1][j-1] + 1; 
				// set left, top calcs
				left = table[i][j-1] + 1; 
				top = table[i-1][j] + 1; 

				// calculate edit distance 
				table[i][j] = findMin(corner, left, top); 
				
			}

			// reset match flags
			match = 0; 
		}
	}

	return table[height-1][width-1]; 
} 

/*
TABLE FORMAT 
 0    1    2   3
0   |  c|  a|  t|
 ----------------
1  c|
 ----------------
2  a|
 ----------------
3  v|
 ----------------
4  e|
 ----------------
*/ 

// O(MAX_LEN) 
int ** editDTable(char * string1, char * string2){

        // get dimensions of table
        int str1_size = strlen(string1); 
        int str2_size = strlen(string2); 

        // initialize table
        // string 1 in first col, string 2 in first row 

	// Height = size of string 1 + 1
	int ** table = (int**) malloc((str1_size + 1) * sizeof(int*)); 
	// Width = size of string 2 + 1
	int i; 
	for(i = 0; i <= str1_size; ++i) table[i] = (int*) malloc((str2_size + 1) * sizeof(int));  // O(MAX_LEN) 

        // 0,0 element is an empty space 
        table[0][0] = ' '; 

        // fill out first row with string 2
        for(i = 1; i <= str2_size; i++) table[0][i] = string2[i - 1]; 	//O(MAX_LEN)

        // file out first col with string 1
        for(i = 1; i <= str1_size; i++) table[i][0] = string1[i - 1];   //O(MAX_LEN)

        return table; 
        
}

/*
   |   |  c|  a|  t|
-------------------- (4 spaces per section of ~)
   |  0|  1|  2|  3|
--------------------
  c|  1|
--------------------
  a|
--------------------
  v|
--------------------
  e|
--------------------

Actual table in memory 
 0    1    2   3
0   |  c|  a|  t|
 ----------------
1  c|
 ----------------
2  a|
 ----------------
3  v|
 ----------------
4  e|
 ----------------
**Initial calc values are not stored, we can just use loop counter values
i = row, j = col
D(1,j) (first row of real calcs) = corner(= i - 1 = 0), 	top = j(variable), 	left = i(static)
D(i,1) (first col of real calcs) = corner(= j - 1 = 0), 	top = j(static), 	left = i(variable)
*/ 

void printT(int ** table, int w, int h){

	// blank will be stored as a single space

	// print row by row 
	int i, j, k;
	for(i = 0; i < h; i++){
		for(j = 0; j < w; j++){

			// if first row, print 2 spaces and the word
			if(i == 0){ 
	
				if(j == 0 || j == 1) printf("   |"); 
				else{
					printf("%3c|", table[i][j-1]); 	
				}

			}

			// if second row, print space, value of i, and then all values of j
			else if(i == 1){
		
				// print space
				if(j == 0){ printf("   |"); }
				// print trivial first row edit calc 
				else{ printf("%3d|", j - 1); }

			}
			// otherwise, print first char of vert word (skip space), value of i, 
			// then each int-calc int the cell
			else{

				// print char 
				if(j == 0){ printf("%3c|", table[i - 1][j]); }
				// print trivial first col edit calc 
				else if(j == 1) { printf("%3d|", i - 1); }
				// print calculation
				else{ printf("%3d|", table[i - 1][j - 1]); }

			}

		}
		printf("\n"); 
		for(k = 0; k < w; k++) printf("----"); 
		printf("\n"); 
	}

	printf("\n"); 

}
/*
Parameters:
first_string - pointer to the first string (displayed vertical in the table)
second_string - pointer to the second string (displayed horizontal in the table)
print_table - If 1, the table with the calculations for the edit distance will be printed.
              If 0 (or any value other than 1)it will not print the table
(See sample run)
Return:  the value of the edit distance (e.g. 3)
*/

// O(MAX_LEN^2) 
int edit_distance(char * first_string, char * second_string, int print_table){

	// create table for strings - store each char as an int, all other cells are for calculations 
	int ** table = editDTable(first_string, second_string); 	// O(MAX_LEN) 
	int width = strlen(second_string) + 1; 
	int height = strlen(first_string) + 1; 

	// compute distance with table
	int distance = editDistanceCalc(table, width, height);		// O(MAX_LEN^2) 

	// print table if verbose mode is 1
	if(print_table){

		// print table
        	// extra space is required for formatting 
		printT(table, width + 1, height + 1); 			// O(MAX_LEN^2) 

	}

	// delete table from heap 
	int i;
	for(i = 0; i < height; i++) free(table[i]); 	
	free(table); 


	return distance; 
}
	  

/*
Parameters:
testname - string containing the name of the file with the paragraph to spell check, includes .txt e.g. "text1.txt" 
dictname - name of file with dictionary words. Includes .txt, e.g. "dsmall.txt"
printOn - If 1 it will print EXTRA debugging/tracing information (in addition to what it prints when 0):
			 dictionary as read from the file	
			 dictionary AFTER sorting in lexicographical order
			 for every word searched in the dictionary using BINARY SEARCH shows all the "probe" words and their indices indices
			 See sample run.
	      If 0 (or anything other than 1) does not print the above information, but still prints the number of probes.
		     See sample run.
*/


/*
	FIX SPELL_CHECK.c and TEST
*/ 
void init_files(FILE ** fp_text, FILE ** fp_out, FILE ** fp_dic, char * testname, char * dictname){

	// Open text file 
	//FILE *fp_text; 
	if((*fp_text = fopen(testname, "r")) == NULL){
		printf("Could not open text file\n");
		exit(1); 
	} 

	// Open corrected output file 
	char corrected_output[101] = "out_";
	strcat(corrected_output, testname); 
	//FILE *fp_out; 
	if((*fp_out= fopen(corrected_output, "w")) == NULL){
		printf("Could not open output file\n");
		exit(1); 
	} 
	printf("Corrected output filename: %s\n", corrected_output); 

	// Open dictionary 
	//FILE *fp_dic; 
	if((*fp_dic = fopen(dictname, "r")) == NULL){
		printf("Could not open dictionary\n");
		exit(1); 
	} 

} 


// prints all punctuation then space to file, or just a space 
void printPunct(char * punct, FILE * fp_out){

	// is punct emtpy? 
	if(punct[0] == '\0') fprintf(fp_out, "%c", ' '); // just print space if emtpy 
	else{

		// print all punctuation and clear contents 
		for(int i = 0; punct[i] != '\0'; ++i){
			fprintf(fp_out, "%c", punct[i]); 
			punct[i] = '\0'; 
		}

		// print space at end 
		fprintf(fp_out, "%c", ' '); 

	}

} 

void spell_check(char * testname, char * dictname, int printOn){

	// initialize text file, output file, dictionary
	FILE *fp_text, *fp_out, *fp_dic; 
	init_files(&fp_text, &fp_out, &fp_dic, testname, dictname); 


 	// Read dictionary - Load into table for searching 
	printf("\nLoading the dictionary file: %s\n\n", dictname); 
	int dict_size; 
	fscanf(fp_dic, "%d", &dict_size); 			// get size of dictionary 
	char dict[dict_size][101]; 				// load words
	int i;
	for(i = 0; i < dict_size; ++i) fscanf(fp_dic, " %s", dict[i]); 

	printf("Dictionary has size: %d\n", dict_size); 	// start of UI

	// Print original dict if in verbose mode
	if(printOn){
		printf(" Original Dictionary:\n"); 
		for(i = 0; i < dict_size; ++i) printf("%d. %s\n", i, dict[i]); 
	}
	printf("\n");
	
	// Sort dictionary with qsort 
	qsort(dict, dict_size, 101*sizeof(char), compare); 	// 

	// Print sorted dict if in verbose mode
	if(printOn){
		printf(" Sorted Dictionary (alphabetical order) :\n"); 
		for(i = 0; i < dict_size; ++i) printf("%d. %s\n", i, dict[i]); 
	}
	printf("\n");

	// Compare text file against dictionary 
	int edit_distances[dict_size]; 
	char word[101]; 
	char punct[101]; int p = 0; memset(punct, '\0', 101); 
	int found = 0; 
	while(!feof(fp_text)){
	
		p = 0; 

		fscanf(fp_text, " %s ", word); 		
	
		// clean word of punctuation and store it 
		// .!?,
		for(i = 0; word[i] != '\0'; ++i){
			if(isPunctuation(word[i])){
				punct[p++] = word[i]; 	
				word[i] = '\0'; 
			}
		}

		// use binary search to find word in dictionary 
		// O(MAX_LEN * logD) 
		found = binarySearch(dict_size, 101, dict, word, 0, dict_size - 1, printOn); 
		
		// special UI when word not found 
		int min_dist = 120; 
		int computations = 0;		// tracks num of edit dist computations  
		int distance = 0; 
		if(!found){

			// compute edit distance for each word in dict
			for(i = 0; i < dict_size; ++i){
				distance = edit_distance(dict[i], word, 0); 
				computations++; 
				edit_distances[i] = distance;			// store distance for ith item 
				
				if(distance < min_dist) min_dist = distance;	// update min distance 
			} 	

			// output menu 
			printf("-1 - type correction\n");
			printf(" 0 - leave word as is (do not fix spelling)\n");
			printf("     Minimum distance: %d  (computed edit distances: %d)\n", min_dist, computations);
			printf("     Words that give minimum distance:\n"); 

			int count = 0; 
			for(i = 0; i < dict_size; ++i){		// print items with min distance 
				if(edit_distances[i] <= min_dist){
					printf("%2d - %s\n", ++count, dict[i]); 
					edit_distances[i] = -1; // mark ith index as a min distance word
				}
			}
			
			// get user choice and execute
			int choice; 
			char input[101]; 
			printf("Enter your choice (from the above options): "); 
			scanf("%d", &choice); 
		
			if(choice == -1){					// change word
	
				// take new spelling  
				printf("Enter correct word: "); 
				scanf("%s", input); 

				// write word to output file 
				fprintf(fp_out, "%s", input); 

				// output punctuation if needed or a space
				printPunct(punct, fp_out); 

			}
			else if(choice == 0){
				fprintf(fp_out, "%s", word); 			// do nothing to word
				printPunct(punct, fp_out); 
			}
			else if(choice > 0){					// replace word

				count = 0; 
				for(i = 0; i < dict_size; ++i){			// get index of new word 

					if(edit_distances[i] == -1){
						count++; 
						if(count == choice) break; 	// break with i = chosen word's index 
					}	
				}

				if(i != dict_size){				// replace with new word 
					fprintf(fp_out, "%s", dict[i]); 
					printPunct(punct, fp_out); 		// print punctuation or space
				}

				else{
					printf("Invalid input...defaulting to 0 - no change\n"); // if i is not a valid index, word is not found 
					fprintf(fp_out, "%s", word); 
					printPunct(punct, fp_out); 
				}

			}
			else{
				printf("Invalid choice...defaulting to 0 - no change\n"); 
				fprintf(fp_out, "%s", word); 	
				printPunct(punct, fp_out); 
			}	
			printf("\n"); 

		}
		else{
			
			// word was found, print to output
			fprintf(fp_out, "%s", word); 
			printPunct(punct, fp_out); 
	
		}
		
	}

	// no newline in file 
	// output file: out_<filename>.txt

	// Close files 
	fclose(fp_out); 
	fclose(fp_dic); 	
	fclose(fp_text); 
	

}

