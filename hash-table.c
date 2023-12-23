#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANDOPERATOR 0
#define OROPERATOR 1
#define MAPSTATEEMPTY 0
#define MAPSTATEFILLED 1
#define MAPSTATEDELETED 2

typedef struct keys {
	char value[40];
	struct keys* next;
}KEYS;

typedef struct input{
	KEYS* keysList;
	char link[40];
	int numOfKeys;
	struct input* next;
}INPUT;

typedef struct hashElement{
	char link[40];
	struct hashElement* next;
}HASHELEMENT;

typedef struct hashMap{
	int state;
	HASHELEMENT* headOfElementList;
	char keyword[40];
}HASHMAP;

void printLink(INPUT* p){
	KEYS* listP;
	
	listP = p->keysList;
	
	while(listP != NULL){
		printf("KeysList->value: %s\n", listP->value);
		listP = listP->next;
	}
}

KEYS* createKeysListElement(char str[40]){
	KEYS* ptr;
	
	ptr = (KEYS*)malloc(sizeof(KEYS));
		
	strcpy(ptr->value,str);
	ptr->next = NULL;
	
	return ptr;
}

KEYS* insertIntoKeysList(KEYS* head, char temp[40]){
	KEYS* p;
	KEYS* ptr;

	if(head == NULL){
		head = createKeysListElement(temp);
		return head;
	}
	
	ptr = head;
	
	while(ptr->next != NULL)
		ptr = ptr->next;
	
	p = createKeysListElement(temp);
	
	p->next = NULL;
	ptr->next = p;
	
	return head;
}

INPUT* insertIntoInputList(INPUT* head, INPUT *p){
	INPUT* ptr;

	if(head == NULL){
		head = p;
		head->next = NULL;
		return head;
	}
	
	ptr = head;
	
	while(ptr->next != NULL)
		ptr = ptr->next;
	
	ptr->next = p;
	p->next = NULL;
	
	return head;
}

int countLineNumberOfDoc(){
	char nameOfFile[] = "Sample.txt";
	FILE *fp;
	char c;
	int lineNumber;
	
	lineNumber = 0;
	fp = fopen(nameOfFile,"r");
	
	for (c = getc(fp); c != EOF; c = getc(fp)){
    	if (c == '\r') 
        	lineNumber++;
	}
	fclose(fp);
	
	return lineNumber;
}


INPUT* getDataFromFile(){
	char nameOfFile[] = "Sample.txt";
	char temp[40];
	int i=0;
	FILE* fp = fopen(nameOfFile, "r");	
	INPUT* head;
	INPUT* p;
	int numOfLine = countLineNumberOfDoc();
	
	head = NULL;
	while(numOfLine>0){
		fscanf(fp, "%1023s", temp);
		p = (INPUT*)malloc(sizeof(INPUT));
		if(strncmp("https",temp,5) == 0){
			i=0;
			p->numOfKeys = 0;
			strcpy(p->link,temp);
			p->link[strlen(p->link)] = '\0';
		}
		
		else{
			p->keysList = NULL;	
			p->keysList = insertIntoKeysList(p->keysList, temp);
			(p->numOfKeys)++;
			i++;
		}
		head = insertIntoInputList(head,p);
		numOfLine--;
	}
		
	fclose(fp);
	return head;		
}

int isPrime(int number){
	int i = number-1;
	while(i>=2 && number%i != 0)
		i--;
	
	if(i < 2)
	    return 1;

	return 0;
}

int calculateHashTableSize(int aproxSize){
	int upperPointer;
	
	upperPointer = aproxSize;
	
	while(!isPrime(upperPointer))
		upperPointer++;
		
	return upperPointer;
}

void printInput(INPUT* head){
	INPUT* p;
	
	p = head;
	
	if(p == NULL)
		return;
		
	while(p != NULL){
		printLink(p);
		p = p->next;
	}
	
}

void printKeysList(KEYS* head){
	KEYS* p;
	p = head;
	
	if(head == NULL)
		return;
		
	while(p != NULL){
		printf("%s\n", p->value);
		p = p->next;
	}
}

int countUniqueKeys(INPUT* inputListHead, KEYS** keysHead){
	INPUT* inputPtr;
	KEYS* keysPtr;
	KEYS* inputKeysPtr;
	int counter = 0;
	
	if(inputListHead == NULL || inputListHead->keysList == NULL)
		return 0;
				
	*keysHead = createKeysListElement(inputListHead->keysList->value);
	inputPtr = inputListHead;
	
	while(inputPtr != NULL){
		inputKeysPtr = inputPtr->keysList;
		while(inputKeysPtr != NULL){
			keysPtr = *keysHead;
			while(keysPtr->next != NULL && strcmp(keysPtr->value, inputKeysPtr->value) != 0)
				keysPtr = keysPtr->next;
						
			if(strcmp(keysPtr->value, inputKeysPtr->value) != 0){
				keysPtr->next = createKeysListElement(inputKeysPtr->value);
				counter++;
			}
			inputKeysPtr = inputKeysPtr->next;
		}
		inputPtr = inputPtr->next;
	}
	return --counter;	
}

unsigned long long int stringHash(char str[40]){
	int i,length,tmp,k;
	unsigned long long int hashKey = 0;
	
	k = 31;
	
	length = strlen(str);	
	
	for(i=0; i<length; i++){
		tmp = str[i]-'A'+1;
		hashKey = hashKey * k +tmp;
	}	

	return hashKey;
}

int calculateHashIndex(unsigned long long int strHashKey, int i, int tableSize){
	int hashKey = strHashKey + i;

	if(tableSize < 1)
	    return 0;

	return (hashKey % tableSize);
}

HASHELEMENT* createHashElement(char link[40]){
	HASHELEMENT* ptr;

	ptr = (HASHELEMENT*)malloc(sizeof(HASHELEMENT));
	
	strcpy(ptr->link, link);
	ptr->next = NULL;

	return ptr;
}

HASHELEMENT* insertIntoHashElementList(HASHELEMENT* head, char link[40]){
	HASHELEMENT* ptr;
	
	if(head == NULL){
		head = createHashElement(link);
		return head;
	}
	
	ptr = head;
	
	while(ptr->next != NULL)
		ptr = ptr->next;
		
	ptr->next = createHashElement(link);
	
	return head;
}

int compareStrings(char* str1, char* str2){
    int i, len;
    
    len = 0;
	if(str1 != NULL)
		len = strlen(str1);
    
    if(str1 == NULL || str2 == NULL)
        return 0;
        
    while(i<strlen(str1) && str1[i] == str2[i])
        i++;
        
    if(i >= strlen(str1))
        return 0;
        
    return 1;
}

int isKeysListContains(KEYS* keysListHead, char value[40]){
	KEYS* ptr;
	char ptrValue[40] = "";
	
	printKeysList(keysListHead);
		
		
	if(keysListHead == NULL)
		return 0;
		
	
	ptr = keysListHead;
		
	while(ptr != NULL && ptr->value != NULL && compareStrings(ptr->value, value) != 0)
		ptr = ptr->next;	
	
	if(ptr == NULL || ptr->value == NULL)
		return 0;
		
		
	else if(compareStrings(ptr->value, value) == 0)
		return 1;
		
	return 0;
}

void createHashTable(HASHMAP** hashArray, KEYS* headOfUniqueKeys, INPUT* inputListHead, int m){
	INPUT* inputListPtr;
	KEYS* keysListPtr;
	int i,hashIndex;

	i = 0;
	while(keysListPtr != NULL){
		inputListPtr = inputListHead;
		while(inputListPtr != NULL){
			if(isKeysListContains(inputListPtr->keysList,keysListPtr->value)){
				hashIndex = calculateHashIndex(stringHash(keysListPtr->value),i,m);
				i++;
				
				while((*hashArray[hashIndex]).state != MAPSTATEEMPTY){
					hashIndex = calculateHashIndex(stringHash(keysListPtr->value),i,m);
					i++;
				}
				
				if((*hashArray[hashIndex]).state == MAPSTATEEMPTY){
					insertIntoHashElementList((*hashArray[hashIndex]).headOfElementList, inputListPtr->link);
					(*hashArray[hashIndex]).state = MAPSTATEFILLED;
				}
			}
			inputListPtr = inputListPtr->next;
		}
		keysListPtr = keysListPtr->next;
	}
}

int parseInput(char searchInput[100], char fWord[100], char conj[100], char sWord[100]){
	char delim[2] = " ";
	char* token;
	
	token=strtok(searchInput,delim);
	strcpy(fWord, token);
	
	token=strtok(NULL,delim);
	strcpy(conj, token);
	
	token=strtok(NULL,delim);
	strcpy(sWord, token);
	
	printf("%s\n", fWord);
	
	if(strcmp(conj, "ve") == 0)
		return ANDOPERATOR;
		
	return OROPERATOR;
}

HASHMAP search(HASHMAP* maps, char word[100], int m){
	int i=0,hashIndex=0;

	hashIndex = calculateHashIndex(stringHash(word),i++,m);
	
	
	while(maps[hashIndex].state != MAPSTATEEMPTY && strcmp(maps[hashIndex].keyword, word) != 0)
		hashIndex = calculateHashIndex(stringHash(word),i++,m);
	
	return maps[hashIndex];
}

void printLinksList(HASHELEMENT* head){
	HASHELEMENT* p;
	
	p = head;
	
	while(p!=NULL){
		printf("%s\t", p->link);
		p = p->next;
	}
	printf("\n");
}

int main(void){
	int menuSelection = 5;
	HASHMAP searchResult;
	INPUT* inputListHead;
	KEYS* headOfUniqueKeys;
	HASHMAP* maps;
	int numOfUniqueElements, loadFactor, m,i;
	char firstWord[100], conj[100], secWord[100], searchText[100];
	
	inputListHead = getDataFromFile();
	numOfUniqueElements = countUniqueKeys(inputListHead,&headOfUniqueKeys);
	
	while(menuSelection != 3){	
		printf("Menu\n");
		printf("Yapmak Istediginiz Islemi Secin\n");
		printf("1- Hash Table Olustur\n");
		printf("2- Arama Yap\n");
		printf("3- Cikis\n");
		printf("Seciminiz: ");
		scanf("%d", &menuSelection);
		
		if(menuSelection == 1){
			printf("Load Factor Giriniz(%%): ");
			scanf("%d", &loadFactor);	
			
			m = calculateHashTableSize((numOfUniqueElements*100)/loadFactor);
			maps = (HASHMAP*)malloc(m*sizeof(HASHMAP));
			
			for(i=0; i<m; i++){
				maps[i].state = MAPSTATEEMPTY;
			}
			
			createHashTable(&maps,headOfUniqueKeys, inputListHead, m);
		}
		
		else if(menuSelection == 2){
			printf("Aranacak Kelimeyi Giriniz");
			scanf("%s", searchText);
			searchResult = search(maps,searchText,m);
			if(searchResult.state == MAPSTATEEMPTY)
				printf("Aranan Anahtar Kelime Bulunamadi\n");
				
			else {
				printLinksList(searchResult.headOfElementList);
			}
		}
		
		else if(menuSelection != 3)
			printf("Hatali Giris Yaptiniz.\n");
	}
	
	return 0;
}
