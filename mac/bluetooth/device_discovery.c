//
//

#include <mach/mach.h>
#include <mach/mach_error.h>
#include <CoreFoundation/CFRunLoop.h>
#include <stdio.h>
#include <IOBluetooth/IOBluetoothUserLib.h>
#include <IOBluetooth/IOBluetoothUtilities.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include "btest.h"

void Perror(int exp)
{
	IOReturn  err = exp;
	if ( err != kIOReturnSuccess )
	 	fprintf( stderr, "%s(%x)\n", mach_error_string(err), err );
    //exit(EX_OSERR)

}

typedef struct _bdidel{
    Class isa;
}bdidel;
Class bdidelclass;

void bdidel_deviceInquiryComplete(bdidel *self, SEL _cmd, id bdi, IOReturn error, BOOL aborted )
{
    if ( error != kIOReturnSuccess && !aborted )
    {
        Perror(error);
    }
    fprintf(stdout,"Inquiry completed" );
    CFRunLoopStop(CFRunLoopGetCurrent());
}

void bdidel_deviceInquiryDeviceFound(bdidel *self, SEL _cmd, id bdi, id dev)
{
    CFStringRef add;
    BluetoothDeviceAddress *btadd;
    btadd = ((BluetoothDeviceAddress* (*)(id, SEL))objc_msgSend)(dev, sel_getUid("getAddress"));
    add = IOBluetoothCFStringFromDeviceAddress(btadd);
    //fprintf(stdout, "Device Found: %s",add);
}

 __attribute__((constructor))
void initbdidel()
{
    bdidelclass = objc_allocateClassPair(objc_getClass("NSObject"), "BdiDelegate", 0);
    class_addMethod(bdidelclass, sel_getUid("deviceInquiryComplete:"), (IMP) bdidel_deviceInquiryComplete, "v@:@ii");
    class_addMethod(bdidelclass, sel_getUid("deviceInquiryDeviceFound:"), (IMP) bdidel_deviceInquiryDeviceFound, "v@:@@");
    objc_registerClassPair(bdidelclass);
}

void device_discovery()
{
    id bdi = objc_msgSend(objc_msgSend((id)objc_getClass("IOBluetoothDeviceInquiry"), sel_getUid("alloc")), sel_getUid("init"));
    id bdidelobj = objc_msgSend( objc_msgSend((id)objc_getClass("BdiDelegate"), sel_getUid("alloc")), sel_getUid("init"));
    ((void (*)(id, SEL, id))objc_msgSend)(bdi, sel_getUid("setDelegate:"), bdidelobj);
    ((void (*)(id, SEL))objc_msgSend)(bdi, sel_getUid("start"));
    CFRunLoopRun();
}

int main(int argc, const char * argv[])
{
	//test
    device_discovery();
    return 0;
}

