#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int		id;
    char	name;
    int		order; 
};

// Node for the linked list in each hash table bucket
struct HashType
{
    struct RecordType* data;
    struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
    return x % 23; 
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    printf("\nHash Table:\n");
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].next != NULL) {
            printf("index %d -> ", i);
            struct HashType *current = pHashArray[i].next;
            while (current != NULL) {
                struct RecordType record = *current->data;
                printf("%d, %c, %d", record.id, record.name, record.order);
                if (current->next != NULL) {
                    printf(" -> ");
                }
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);

    // Your hash implementation
    
    int hashTableSize = 23; 
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * hashTableSize);
    if (hashTable == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    for (int i = 0; i < hashTableSize; ++i) {
        hashTable[i].next = NULL;
    }

    
    for (int i = 0; i < recordSz; ++i) {
        struct RecordType record = pRecords[i];
        int index = hash(record.id);

        
        struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            exit(-1);
        }
        newNode->data = &pRecords[i];
        newNode->next = NULL;

        
        if (hashTable[index].next == NULL) {
            
            hashTable[index].next = newNode;
        } else {
            
            struct HashType* current = hashTable[index].next;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    
    displayRecordsInHash(hashTable, hashTableSize);
    free(pRecords);
    for (int i = 0; i < hashTableSize; ++i) {
        struct HashType* current = hashTable[i].next;
        while (current != NULL) {
            struct HashType* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable);

    return 0;
}
