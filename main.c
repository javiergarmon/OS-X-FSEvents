
/* Compile with con gcc -framework CoreServices main.c -o main */

#include <stdlib.h>
#include <CoreServices/CoreServices.h>

int eventModified = kFSEventStreamEventFlagItemFinderInfoMod |
                    kFSEventStreamEventFlagItemModified |
                    kFSEventStreamEventFlagItemInodeMetaMod |
                    kFSEventStreamEventFlagItemChangeOwner |
                    kFSEventStreamEventFlagItemXattrMod;

int eventRenamed = kFSEventStreamEventFlagItemCreated |
                   kFSEventStreamEventFlagItemRemoved |
                   kFSEventStreamEventFlagItemRenamed;

int eventSystem = kFSEventStreamEventFlagUserDropped |
                  kFSEventStreamEventFlagKernelDropped |
                  kFSEventStreamEventFlagEventIdsWrapped |
                  kFSEventStreamEventFlagHistoryDone |
                  kFSEventStreamEventFlagMount |
                  kFSEventStreamEventFlagUnmount |
                  kFSEventStreamEventFlagRootChanged;

void myCallbackFunction(

    ConstFSEventStreamRef streamRef,
    void *clientCallBackInfo,
    size_t numEvents,
    void *eventPaths,
    const FSEventStreamEventFlags eventFlags[],
    const FSEventStreamEventId eventIds[]

){
    
    int i;
    char **paths = eventPaths;
    
    for( i=0; i < numEvents; i++ ){
        
        printf( "Changed %s\n", paths[i] );
        printf( "  Modified : %d\n", !!( eventFlags[i] & eventModified ) );
        printf( "  Renamed  : %d\n", !!( eventFlags[i] & eventRenamed  ) );
        printf( "  System   : %d\n", !!( eventFlags[i] & eventSystem   ) );

    }

}

int main( int argc, char* argv[] ){

    /* Define variables and create a CFArray object containing CFString objects containing paths to watch. */
    CFStringRef mypath      = CFStringCreateWithCString( NULL, argv[ 1 ], kCFStringEncodingUTF8);
    CFArrayRef pathsToWatch = CFArrayCreate( NULL, ( const void ** ) &mypath, 1, NULL );
    void *callbackInfo      = NULL; // could put stream-specific data here. FSEventStreamRef stream;*/
    CFAbsoluteTime latency  = 3.0; /* Latency in seconds */
    FSEventStreamRef stream;

    /* Create the stream, passing in a callback */
    stream = FSEventStreamCreate(

        NULL,
        &myCallbackFunction,
        callbackInfo,
        pathsToWatch,
        kFSEventStreamEventIdSinceNow, /* Or a previous event ID */
        latency,
        kFSEventStreamCreateFlagFileEvents /* Flags explained in reference: https://developer.apple.com/library/mac/documentation/Darwin/Reference/FSEvents_Ref/Reference/reference.html */

    );

    /* Create the stream before calling this. */
    FSEventStreamScheduleWithRunLoop( stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode );

    FSEventStreamStart( stream );

    CFRunLoopRun();

    return 0;

}
