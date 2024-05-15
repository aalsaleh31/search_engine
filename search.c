#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "search.h"
#include <string.h>
#include <math.h>
#include <glob.h>

int main(void){
    printf("How many buckets?:\n");
    int num_of_buckets;
    scanf("%d", &num_of_buckets);
    if(num_of_buckets<=0){
        printf("ERROR you have to have at least 1 bucket please restart the program\n");
        return 0;
    }
    struct hashmap * hm = hm_create(num_of_buckets);//number of buckets 

    int number_of_documents = 0;
    glob_t mything;
    char *pathofdocuments = {"p5docs/D*.txt"};
    glob(pathofdocuments, 0, NULL, &mything);
    number_of_documents = mything.gl_pathc;
    globfree(&mything);

    int i;
    double score[number_of_documents+1];

    
    //glob now works
    training(hm, number_of_documents);
    
    //1a. done
    //1b am i suppouse to use the hashmap hash function and remove the word in that case
    
    
    while(1){
        char *input = NULL;
        input = read_query();
        if(strcmp(input, "X") == 0){
            hm_destroy(hm);
            // printf("we are exiting\n");
            return 1;
        }
        // i = 1;
        // if (i == 1){
        //     hm_destroy(hm);
        //     return 1;
        // }
        rank(input, score, number_of_documents, hm);


        //the ranking this is the last thing in the project works perfectly
        printranking(score, number_of_documents);
        for(i = 0; i<=number_of_documents;i++){//am i suppoused to reset the score or not
            score[i] = 0;
        }
    }
    //i need white spaces with this its allowed 
    
    // char* piece = strtok(current_word, " ");
    // printf("%s\n", piece);
    // piece = strtok(NULL, " ");
    // printf("%s\n", piece);
    
    // i need to reset it now
    
   
   
    hm_destroy(hm);//destroying the hashmap
    
    // int x = whydoesntitwork();
    // printf("%d\n", x);
    return 0;
}
// int whydoesntitwork(){
//     return 1;
// }
void rank(char * input, double score[], int number_of_documents, struct hashmap * hm){
    double df = 0;
    int i, j;
    char D[2];
    char number[20];
    // int testing = 0;
    // printf("before the curr \"%s\"\n", input);
    char* current_word = strtok(input, " ");
    // printf("%safter strtok\n",input)
    while(current_word != NULL){
        for (i = 1; i<=number_of_documents;i++){
            double idf = 0;
            // printf("%s\n", current_word); 
            df = 0;
            for (j = 1; j<=number_of_documents;j++){//this is for the regular hashmap
                
                D[0] = 0;
                memset(D, 0, 20);

                number[0] = 0;
                memset(number, 0, 20);
                D[0] = 'D';//for opening documents fo rthe number of dcouments
                sprintf(number, "%d", j);
                strcat(D,number);//making the string the word

                // printf("%s how many times %d\n",D, hm_get(hm, current_word, D));
                if(hm_get(hm, current_word, D) > 0){//if one was found
                    df++;
                    continue;
                }
            }   
        // printf("check df ======%d\n",df);
            
            D[0] = 0;
            memset(D, 0, 20);
            D[0] = 'D';//for opening documents fo rthe number of dcouments

            number[0] = 0;
            memset(number, 0, 20);

            sprintf(number, "%d", i);
            strcat(D,number);//making the string the word
            // printf("document testing dont worry %s\n", D);
            int tf = hm_get(hm, current_word, D);
            if (tf == -1){//this is becasuei want to see if there isnt a tf
                tf = 0;
            }
            if (df == 0){//if df == 0 then it causes floating point exception (core dumped)
                // printf("word:\"%s\" not found skip\n",current_word);
                break;
            }
            else {
                double value = number_of_documents/(df);
                idf = log(value);// log doesnt work
                //if (idf == 0){
                //     removeword();
                // }
            }
            
            
            // printf("tf = %d idf =%f\n",tf,idf);

            score[i] += tf * idf;//*idf
            // printf("%s:\t %f\n",D, score[i]);

            memset(D, 0, 20);
            memset(number, 0, 20);

            
        }

        current_word = strtok(NULL, " ");//go to next word
    }
    // printf("testing done %d\n", testing);
}
void printranking(double score[],int number_of_documents){//rank done works SUII
    int i;
    int j;
    FILE *fp1;
    fp1 = fopen("search_scores.txt", "a+");
    
    int numbers [number_of_documents];//this is to get the current file in the document
    for (int i = 1; i<= number_of_documents; i++){
        numbers[i] = i;
    }
    double temp;//temperory for the scores (double)
    int numberstemp;//temp for the numbers
	for (i = 1; i<=number_of_documents;i++){
        for (j = i+1; j<=number_of_documents;j++){//bubble sort in descending order(start with the biggerst)
            if (score[i] < score[j]){
                temp = score[i];
                score[i] = score[j];
                score[j] = temp;

                numberstemp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = numberstemp;
            }
            
        }
    }
    for (i = 1; i<= number_of_documents; i++){
        printf("D%d.txt\n", numbers[i]);
        fprintf(fp1, "D%d: %f\n",numbers[i], score[i]);
    }
    fprintf(fp1, "\n\n");

    fclose(fp1);
    
}
char* read_query(void){//need void or error
    //works suiiiiii
    char input [1024];
    printf("Enter search string or X to exit\n");
    scanf(" %[^\n]%*c", input);//get any input
    // scanf(" %1024[0-9a-zA-Z ]", input);//length of 1024 because base
    //we need the white spaces for strtok and the space before the % is becasue of implementing it in a while loop
    char *this;// need to return a string to a pointer
    this = input;
    return this;
}

void training(struct hashmap * hm, int number_of_documents){
    int i, j;
    char D[3];//to get D1 D2 D3 D4 and so on
    char path [20]= {"p5docs/D"};
    char end [20]= {".txt"};
    char number[20];//to get the current number in i of a forloop
    char words[20];
    int df = 0;

    struct hashmap * removed;
    removed = hm_create(20);


    char numberj[20];
    char Din[3];
    for (i=1;i<=number_of_documents;i++){
        memset(path, 0, 20);//restarts the path and the D
        memset(D, 0, 20);
        D[0] = 'D';//for opening documents fo rthe number of dcouments
        char path [20]= {"p5docs/D"};
        sprintf(number, "%d", i);

        strcat(path,number);//get the i and copy it into the path
        strcat(path,end);//copy the .txt to the path to open file

        strcat(D,number);//copy i into D so D1 D2 .... Dn
        FILE * document = fopen(path,"r");

        while (fscanf(document, "%s", words) == 1){//software engineering fscanf
            if(hm_get(removed, words, D)> 0){
                continue;
            }
            int found = hm_get(hm, words, D);//if it is not found its = -1
            if(found == -1)//weird that i dont need to specufy path here questionable 
                    found = 0;
            hash_table_insert(hm, words, D, found + 1);//put takes 4 parameters(hm, word, document_id, num_occurrences)
            df = 0;
            if (i==number_of_documents){//this is pointless i just did it to make the code look neater because this would only work when we read all the files
                for (j = 1; j<=number_of_documents;j++){//this is for the regular hashmap

                    numberj[0] = 0;
                    memset(numberj, 0, 20);
        
                    memset(Din, 0, 20);
                    Din[0] = 'D';//for opening documents fo rthe number of dcouments

                    sprintf(numberj, "%d", j);
                    strcat(Din,numberj);//making the string the word
                    int debug = hm_get(hm, words, Din);
                    // printf("%s how many times %d\n",D, hm_get(hm, current_word, D));
                    if(debug > 0){//if one was found
                        df++;
                        continue;
                    }
                }
                if(df == number_of_documents){
                    stop_word(hm, number_of_documents, words, removed);
                    // printf("please remember to remove me \"%s\"\n", words);//debugginf purposes
                    // printf("their new get() %d\n", hm_get(hm, words, "D3"));
                }
            }
            
        }
        
        fclose(document);
        
        // D[2] = '\0'; 
    }
    hm_destroy(removed);
}
void stop_word(struct hashmap * hm, int number_of_documents, char * current_word, struct hashmap * removed){
    
    int i;
    char D[3];//to get D1 D2 D3 D4 and so on
    char number[20];
    // printf("is this the proble");
    for(i=1;i<=number_of_documents;i++){
        
        D[0] = 0;
        memset(D, 0, 3);
        D[0] = 'D';//for opening documents fo rthe number of dcouments

        number[0] = 0;
        memset(number, 0, 20);

        sprintf(number, "%d", i);
        strcat(D,number);//making the string the word
        int condition = hm_get(hm, current_word, D);
        while(condition != -1){//problem in here remove not removing for D2 and
            hm_remove(hm,current_word, D);
            hash_table_insert(removed, current_word, "D3", 1);
            
            condition = hm_get(hm, current_word, D);
        }

    }

    
}