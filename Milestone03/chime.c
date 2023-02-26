#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

char g_bKeepLooping = 1;
#define MAX_THREADS 5

struct ChimeThreadInfo
{
    int        nIndex;
    float      fChimeInterval;
    char       bIsValid;
    pthread_t  ThreadID;
};

struct ChimeThreadInfo  TheThreads[MAX_THREADS];

void * ThreadChime (void * pData)
{
    struct ChimeThreadInfo  * pThreadInfo;
    /* Which chime are we? */
    pThreadInfo = (struct ChimeThreadInfo *) pData;
    while(g_bKeepLooping)
    {
        sleep(pThreadInfo->fChimeInterval);
        printf("Ding - Chime %d with an interval of %f s!\n", pThreadInfo->nIndex, 
pThreadInfo->fChimeInterval);
    }
    return NULL;
}

#define BUFFER_SIZE 1024

int main (int argc, char *argv[])
{
    char szBuffer[BUFFER_SIZE];
    // Set Variables to store inputs
    int chimeNumber;
    float chimeInterval;
    int parseResult;
    
    /* Set all of the thread information to be invalid (none allocated) */
    for(int j=0; j<MAX_THREADS; j++)
    {
        TheThreads[j].bIsValid = 0;
    }
    while(1)
    {
        /* Prompt and flush to stdout */
        printf("CHIME>");
        fflush(stdout);
        /* Wait for user input via fgets */
        fgets(szBuffer, BUFFER_SIZE, stdin);
        /* If the command is quit - join any active threads and finish up gracefully */
        if (strcmp(szBuffer, "exit\n") == 0)
        {
            return 0;
        }
        /* If the command is chime, the second argument is the chime number (integer) 
           and the third number is the new interval (floating point). If necessary, start 
           the thread as needed */
        else if (strncmp(szBuffer, "chime", 5) == 0)
        {
            parseResult = sscanf(szBuffer, "chime %d %f", &chimeNumber, &chimeInterval);
            if (parseResult == 2)
            {
                if (chimeNumber >= 0 && chimeNumber < MAX_THREADS)
                {
                    if (TheThreads[chimeNumber].bIsValid == 1)
                    {
                        /* The thread is already running, adjust its sleep interval */
                        TheThreads[chimeNumber].fChimeInterval = chimeInterval;
                        printf("Adjusting chime %d to have an interval of %f s\n", 
                               chimeNumber, chimeInterval);
                    }
                    else
                    {
                        /* The thread is not running, start a new thread */
                        TheThreads[chimeNumber].nIndex = chimeNumber;
                        TheThreads[chimeNumber].fChimeInterval = chimeInterval;
                        TheThreads[chimeNumber].bIsValid = 1;
                        pthread_create(&TheThreads[chimeNumber].ThreadID, NULL, ThreadChime, 
                                        &TheThreads[chimeNumber]);
                        printf("Starting thread %ld for chime %d, interval of %f s\n", 
                               TheThreads[chimeNumber].ThreadID, chimeNumber, chimeInterval);
                    }
                }
                else
                {
                    printf("Cannot adjust chime %d, out of range\n", chimeNumber);
                }
            }
            else
            {
                printf("Invalid input format for chime command\n");
            }
        }
        else
        {
            printf("Unknown command: %s", szBuffer);
        }
    }
}