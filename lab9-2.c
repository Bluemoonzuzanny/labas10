#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Fill out this structure
struct HashType
{
    struct RecordType** records;
    int* count;
    int size;
};

// Compute the hash function
int hash(int x)
{
    int hashSz = 10;  // assuming a hash table size of 10
    return x % hashSz;
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
    int i;

    for (i=0;i<hashSz;++i)
    {
        if (pHashArray->count[i] > 0)  // Check if the index is occupied
        {
            printf("index %d -> ", i);
            struct RecordType* current = pHashArray->records[i];
            for (int j = 0; j < pHashArray->count[i]; j++)
            {
                printf("%d, %c, %d -> ", current->id, current->name, current->order);
                current++;
            }
            printf("NULL\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    struct HashType hashTable;
    hashTable.size = 10;  // Size of hash table
    hashTable.records = (struct RecordType**)malloc(sizeof(struct RecordType*) * hashTable.size);
    hashTable.count = (int*)calloc(hashTable.size, sizeof(int));

    // Populate the hash table
    for (int i = 0; i < recordSz; i++)
    {
        int index = hash(pRecords[i].id);
        int count = hashTable.count[index];
        if (count == 0)
        {
            hashTable.records[index] = (struct RecordType*)malloc(sizeof(struct RecordType));
        }
        else
        {
            hashTable.records[index] = (struct RecordType*)realloc(hashTable.records[index], sizeof(struct RecordType) * (count + 1));
        }
        hashTable.records[index][count] = pRecords[i];
        hashTable.count[index]++;
    }

    // Display the records in the hash table
    displayRecordsInHash(&hashTable, hashTable.size);
}
