#ifndef SEARCH_H 
#define SEARCH_H

void training(struct hashmap * hm, int number_of_documents);
char * read_query(void);
void rank(char * input, double score[], int number_of_documents, struct hashmap * hm);
void stop_word(struct hashmap * hm, int number_of_documents, char * input, struct hashmap * removed);
void printranking(double score[],int number_of_documents);

#endif