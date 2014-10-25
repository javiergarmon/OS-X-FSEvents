
/* Compile with con gcc -framework CoreServices -lstdc++ main.cpp -o main */

#include <iostream>
#include <CoreServices/CoreServices.h>

using namespace std;

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
    char **paths = (char **)eventPaths;
    
    for( i=0; i < numEvents; i++ ){
        
        cout << "Changed " << endl << paths[i];
        cout << "  Modified : " << !!( eventFlags[i] & eventModified ) << endl;
        cout << "  Renamed  : " << !!( eventFlags[i] & eventRenamed  ) << endl;
        cout << "  System   : " << !!( eventFlags[i] & eventSystem   ) << endl;

    }

}

int main( int argc, char* argv[] ){

    /* Define variables and create a CFArray object containing CFString objects containing paths to watch. */
    CFStringRef mypath              = CFStringCreateWithCString( NULL, argv[ 1 ], kCFStringEncodingUTF8);
    CFArrayRef pathsToWatch         = CFArrayCreate( NULL, ( const void ** ) &mypath, 1, NULL );
    CFAbsoluteTime latency          = 3.0; /* Latency in seconds */
    FSEventStreamRef stream;

    /* Create the stream, passing in a callback */
    stream = FSEventStreamCreate(

        NULL,
        &myCallbackFunction,
        NULL, // could put stream-specific data here. FSEventStreamRef stream;*/
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
