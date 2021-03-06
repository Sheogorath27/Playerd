//
//


#include <mach/mach.h>
#include <mach/mach_error.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <IOBluetooth/IOBluetoothUserLib.h>
#include <IOBluetooth/IOBluetoothUtilities.h>
#include <objc/runtime.h>
#include <objc/message.h>

id dev;

void Perror(int exp)
{
	IOReturn  err = exp;
	if ( err != kIOReturnSuccess )
	 	fprintf( stderr, "%s(%x)\n", mach_error_string(err), err );
    //exit(EX_OSERR)    
}

void Psdp(CFArrayRef *sdparr)
{
    IOBluetoothSDPServiceRecordRef *sdpobj;
    CFIndex count;
    int i;
    count = CFArrayGetCount(*(sdparr));
    fprintf(stdout, "Found %ld services", count);
    for (i = 0; i < count; i++)
    {
        sdpobj = (IOBluetoothSDPServiceRecordRef* )CFArrayGetValueAtIndex( *sdparr, i);
        //will add according to requirement in header.
    }
}

typedef struct _sdpdel{
    Class isa;
}sdpdel;
Class sdpdelclass;

void sdpdel_sdpQueryComplete(sdpdel *self, SEL _cmd, id dev, IOReturn status)
{
    CFArrayRef *sdparr;
    if ( status != kIOReturnSuccess)
    {
        Perror(status);
    }
    fprintf(stdout,"Inquiry completed" );
    sdparr = ((CFArrayRef* (*) (id, SEL)) objc_msgSend)( dev, sel_getUid("services"));
    Psdp(sdparr);
    CFRunLoopStop(CFRunLoopGetCurrent());
}

__attribute__((constructor))
void initsdpdel()
{
    sdpdelclass = objc_allocateClassPair(objc_getClass("NSObject"), "sdpDelegate", 0);
    class_addMethod(sdpdelclass, sel_getUid("sdpQueryComplete:status:"), (IMP) sdpdel_sdpQueryComplete, "v@:@i");
    objc_registerClassPair(sdpdelclass);
}

void browse_sdp(const char *addstr)
{
    BluetoothDeviceAddress btadd;
    CFStringRef addcfstr;
    IOReturn err;
    id sdpdelobj = objc_msgSend( objc_msgSend((id)objc_getClass("sdpDelegate"), sel_getUid("alloc")), sel_getUid("init"));
    //add method to convert Char * to CFString
    addcfstr = CFSTR("");
    IOBluetoothCFStringToDeviceAddress(addcfstr, &btadd);
    
    dev = objc_msgSend((id) objc_getClass("IOBluetoothDevice"), sel_getUid("deviceWithAddress:"), &btadd);
    err = ((IOReturn (*) (id, SEL, id)) objc_msgSend)( dev, sel_getUid("performSDPQuery:"), sdpdelobj);
    if (err != kIOReturnSuccess)
    {
        Perror(err);
        exit(EX_OSERR);
    }

    CFRunLoopRun();
}

int main(int argc, char const *argv[])
{
    //test
    browse_sdp("");
    return 0;
}
