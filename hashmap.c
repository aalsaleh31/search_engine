#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct hashmap* hm_create(int num_buckets){

    if(num_buckets <= 0){//if number of buckets less than or = 0 then remove
        return NULL;  
    }
    struct hashmap * hm = calloc(sizeof(struct hashmap), 1);//make sure that it doesnt return null
    if (hm == NULL){//lauren(TA) told me to check if it actually callocs when i allocate4 memory.
        return NULL;
    }
    hm->num_elements = 0;//set empty elementss
    hm->num_buckets = num_buckets;//set the number of buckets
    hm->map = calloc(hm->num_buckets, sizeof(struct llnode *));//need to allocate and check if it allocated
    if (hm->map == NULL){
        return NULL;
    }
    int i;//declared before for loop "C things"
    for(i = 0; i < num_buckets; i++){
        hm->map[i] = NULL;
    }
    return hm;
    
}

int hm_get(struct hashmap* hm, char* word, char* document_id){
    if(hm == NULL){//error checking
        return -1;
    }
    if ((word == NULL)) {//error checking if word is empty
        return -1;
    }
    if (document_id == NULL)//error checking if the file is not found
        return -1;

    int hashnum = hash_code(hm,word);
    if(hashnum == -1){//error checking
        return -1;
    }
    
    if (hm->map[hashnum] == NULL) {//checking if empty return -1
        return -1;//if there is nothing in that area
    }
    struct llnode * node = hm->map[hashnum];
    while(node != NULL){//while the node is not empty and
        if(strcmp(document_id, node->document_id) == 0){
            if(strcmp(word, node->word) == 0){
                return node->num_occurrences;//if found return how many time found for example homework in D3 = 2
            }
            
        }
        node = node->next;//go to the next node until i reach it or if its not found return -1
    }
    return -1;//return -1 because not found
    
}

void hash_table_insert(struct hashmap* hm, char* word, char* document_id, int num_occurrences){
    if (hm == NULL)//error checking for first parameter
        return;
    if(strcmp(word, "") == 0 || word == NULL)//error checking for second parameter
        return;
    
    if(strcmp(document_id, "") == 0 || document_id == NULL)//error checking for third parameter
        return;

    
    int storething = hash_code(hm,word); //get number to know where to store
    struct llnode * node = hm->map[storething];//go to that place to store the value
    while(node != NULL){//go until the end to see if found
        if(strcmp(document_id, node->document_id) == 0){//basiccaly a get the same thing to check if its there
            if(strcmp(word, node->word) == 0){//its basicallly compare
                node->num_occurrences = num_occurrences;
                return;
            }
        }
        node = node->next;//go to the next node
    }
   
    node = hm->map[storething];//if theres no instance of that word create it
    if(node == NULL){//if the node is null without this i get seg faults
        struct llnode * nnode = calloc(sizeof(struct llnode),1); //create a new node
        if(nnode == NULL){//if it doesnt alloc properly
            return;
        }
        nnode->word = strdup(word);//i give it the word
        nnode->document_id = strdup(document_id);//document file
        nnode->num_occurrences = num_occurrences;//number of occurences
        nnode->next = NULL;//set the next to NULL
        hm->map[storething] = nnode;
        node = hm->map[storething];
        hm->num_elements++;//increment number of elements
    }
    else{
        while(node->next != NULL){//go until the last bucket
            node = node->next;
        }
        struct llnode * nnode = calloc(sizeof(struct llnode),1);//new node again do the same thing as above but at the last
        if(nnode == NULL){//if it doesnt alloc properly
            return;
        }
        node->next = nnode;//make the final thing the new node
        node = node->next;//go to it
        node->next = NULL;//set the new final thing to null
        node->word = strdup(word);//assignt the things i need for llnode in hashman.h
        node->document_id = strdup(document_id);
        node->num_occurrences = num_occurrences;
        
        hm->num_elements++;//increase the total number of elements in the hashmaap
    }
}
void hm_remove(struct hashmap* hm, char* word, char* document_id){
    if (hm == NULL)//error checking
        return;
    if(strcmp(word, "") == 0 || word == NULL)//error checking
        return;
    
    if(strcmp(document_id, "") == 0 || document_id == NULL)//error checking
        return;

    int hashing_num = hash_code(hm,word);//find where im supposed to look
    struct llnode * node = hm->map[hashing_num];//go to that bucket so i can search node by node until i reach it
    struct llnode * temp;//temp to remove and free the node 
    int m = 1;//to check if its the first place
    while(node != NULL){//if its not null
        if(m==1){//if its at the head then set the head to Nothing and go to the next example suppouse we remove 4      4->5->3->null we just do 5->3->null
            if(strcmp(document_id, node->document_id) == 0){
                if(strcmp(word, node->word) == 0){
                    hm->map[hashing_num] = node->next;
                
                    free(node->document_id);//removing 
                    free(node->word);
                    free(node);
                    return;//end while loop
                }
                
            }
        }
        m++;
        if(strcmp(document_id, node->next->document_id) == 0){
            if(strcmp(word, node->next->word) == 0){
                temp = node->next;//for example remove the num in between like 4->3->5 in this case it just removes 3 and it becomes 4->5 
                node->next = temp->next;//doing as explained in the line above
                free(temp->document_id);//removing 
                free(temp->word);
                free(temp);
                return;//end while loop
            }
        }
        node = node->next;//go to the next node until we reach that specific node that we want to remove
    }
}

void hm_destroy(struct hashmap* hm){
    int i;
    for (i = 0; i < hm->num_buckets; i++) {
        struct llnode * node = hm->map[i];//free the nodes of i
        while (node != NULL) {
            struct llnode *temp = node->next;
            free(node->word);//freeing becasue of the valgrind memory leaks
            free(node->document_id);
            free(node);
            node = temp;
        }
    }
    free(hm->map);//valgrind lost 200 bytes without it
    hm->num_elements = 0;//elements = 0 because its destroyed
    free(hm);//free the hashmap for valgrind

}

int hash_code(struct hashmap* hm, char* word){//copied of the software engineering project 1
    double k = 0;
    char * c;
    for(c = word; *c ; c++){
        k += (*c);
    }
    return (unsigned int)(((unsigned long) k) % hm->num_buckets);
}
