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
            g_bKeepLooping = 0; // set flag to exit chime threads
            for(int j=0; j<MAX_THREADS; j++)
            {
                if (TheThreads[j].bIsValid == 1)
                {
                    printf("Joining Chime %d (Thread %ld)\n", TheThreads[j].nIndex,TheThreads[j].ThreadID);
                    pthread_join(TheThreads[j].ThreadID, NULL);
                    printf("Join Complete for Chime %d!\n", TheThreads[j].nIndex);
                }
            }
            printf("Exit chime program ...\n");
            return 0;
        }
        /* If the command is chime, the second argument is the chime number (integer) 
           and the third number is the new interval (floating point). If necessary, start 
           the thread as needed */
        else if (strncmp(szBuffer, "chime", 5) == 0)
        {
            // Use sscanf to read in inputs and store them in the correct variables for future use
            // Sscanf returns the number of fields assigned, which is used for error checking
            parseResult = sscanf(szBuffer, "chime %d %f", &chimeNumber, &chimeInterval);
            if (parseResult == 2) //executes if two fields were assigned
            {
                // executes if the chime number is in the correct range (i.e 0 to 5)
                if (chimeNumber >= 0 && chimeNumber < MAX_THREADS)
                {
                    //Executes if the thread is already running, and adjusts its sleep interval
                    if (TheThreads[chimeNumber].bIsValid == 1)
                    {
                        // Update the chime interval and display message
                        TheThreads[chimeNumber].fChimeInterval = chimeInterval;
                        printf("Adjusting chime %d to have an interval of %f s\n", 
                               chimeNumber, chimeInterval);
                    }
                    else //Executes if the thread is not running, and starts a new thread
                    {
                        // Set the Chime fields to the user input
                        TheThreads[chimeNumber].nIndex = chimeNumber;
                        TheThreads[chimeNumber].fChimeInterval = chimeInterval;
                        TheThreads[chimeNumber].bIsValid = 1;
                        // Create the thread and display message
                        pthread_create(&TheThreads[chimeNumber].ThreadID, NULL, ThreadChime, 
                                        &TheThreads[chimeNumber]);
                        printf("Starting thread %ld for chime %d, interval of %f s\n", 
                               TheThreads[chimeNumber].ThreadID, chimeNumber, chimeInterval);
                    }
                }
                else // Error message if the chime number is not in the correct interval
                {
                    printf("Cannot adjust chime %d, out of range\n", chimeNumber);
                }
            }
            else // Error message if the fields were incorrectly provided
            {
                printf("Invalid input format for chime command\n");
            }
        }
        else // Error message if the command is not chime or exit
        {
            printf("Unknown command: %s", szBuffer);
        }
    }
}