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
#include "btest.h"

id dvcpair;

void Perror(int exp)
{
	IOReturn  err = exp;
	if ( err != kIOReturnSuccess )
	 	fprintf( stderr, "%s(%x)\n", mach_error_string(err), err );
    //exit(EX_OSERR)
}

typedef struct _pairdel{
    Class isa;
}pairdel;
Class pairdelclass;

void pairdel_pairstarted(pairdel *self, SEL _cmd, id sender)
{
    fprintf( stdout, "Pairing started.\n");
}

void pairdel_connecting(pairdel *self, SEL _cmd, id sender)
{
    fprintf( stdout, "Connecting to remote device.\n");
}

void pairdel_pincoderequest(pairdel *self, SEL _cmd, id sender)
{
    BluetoothPINCode pin ;
    ByteCount pinsize;
    //testing
    //will add method with function pointer to implemented for UI call.
    pin.data[1] = '1';
    pin.data[2] = '2';
    pinsize = sizeof(pin);
    ((void (*)(id, SEL, ByteCount, BluetoothPINCode*))objc_msgSend)(sender, sel_getUid("replyPINCode:PINCode:"), pinsize, &pin);
}

void pairdel_userconfirmation(pairdel *self, SEL _cmd, id sender, BluetoothNumericValue numvalue)
{
    BOOL reply;
    fprintf(stdout, "Numerical Value : %u\nEnter Confirmation(YES or NO) : ", numvalue);
    fscanf(stdin, "%s",&reply);
    ((void (*) (id, SEL, BOOL))objc_msgSend)(sender, sel_getUid("replyUserConfirmation:"), reply);
}

void pairdel_userpasskey(pairdel *self, SEL _cmd, id sender, BluetoothPasskey passkey)
{
    fprintf(stdout, "PassKey : %u", passkey);
}

void pairdel_pairingcomplete(pairdel *self, SEL _cmd, id sender, IOReturn err)
{
    if (err != kIOReturnSuccess)
    {
        Perror(err);
        exit(EX_OSERR);
    }
    fprintf( stdout, "Pairing to remote device complete.\n");
    CFRunLoopStop(CFRunLoopGetCurrent());
}

void pairdel_simplepairingcomplete(pairdel *self, SEL _cmd, id sender, BluetoothHCIEventStatus status)
{
    if (status != kBluetoothHCIEventSimplePairingComplete)
    {
        Perror(status);
        exit(EX_OSERR);
    }
    fprintf( stdout, "Simple Secure Pairing to remote device complete.\n");
    CFRunLoopStop(CFRunLoopGetCurrent());
}

__attribute__((constructor))
void initpairdel()
{
    pairdelclass = objc_allocateClassPair(objc_getClass("NSObject"), "pairDelegate", 0);
    class_addMethod(pairdelclass, sel_getUid("devicePairingStarted:"), (IMP) pairdel_pairstarted, "v@:@");
    class_addMethod(pairdelclass, sel_getUid("devicePairingConnecting:"), (IMP) pairdel_connecting, "v@:@");
    class_addMethod(pairdelclass, sel_getUid("devicePairingPINCodeRequest:"), (IMP) pairdel_pincoderequest, "v@:@");
    class_addMethod(pairdelclass, sel_getUid("devicePairingUserConfirmationRequest:numericValue"), (IMP) pairdel_userconfirmation, "v@:@I");
    class_addMethod(pairdelclass, sel_getUid("devicePairingUserPasskeyNotification:passkey"), (IMP) pairdel_userpasskey, "v@:@I");
    class_addMethod(pairdelclass, sel_getUid("devicePairingFinished:error:"), (IMP) pairdel_pairingcomplete, "v@:@");
    class_addMethod(pairdelclass, sel_getUid("deviceSimplePairingComplete:status:"), (IMP) pairdel_simplepairingcomplete, "v@:@");
    objc_registerClassPair(pairdelclass);
}

void device_pair(const char *addstr)
{
    id dev;
    BluetoothDeviceAddress btadd;
    CFStringRef addcfstr;
    IOReturn err;
    id pairdelobj = objc_msgSend( objc_msgSend((id)objc_getClass("pairDelegate"), sel_getUid("alloc")), sel_getUid("init"));
    addcfstr = CFSTR("");
    IOBluetoothCFStringToDeviceAddress(addcfstr, &btadd);
    
    dev = objc_msgSend((id) objc_getClass("IOBluetoothDevice"), sel_getUid("deviceWithAddress:"), &btadd);
    dvcpair = objc_msgSend( objc_msgSend((id)objc_getClass("IOBluetoothDevicePair"), sel_getUid("alloc")), sel_getUid("init"));
    ((void (*) (id, SEL, id))objc_msgSend)(dvcpair, sel_getUid("setDevice:"), dev);
    ((void (*) (id, SEL, id))objc_msgSend)(dvcpair, sel_getUid("setDelegate:"), pairdelobj);
    err = ((IOReturn (*) (id, SEL))objc_msgSend)(dvcpair, sel_getUid("start"));
    CFRunLoopRun();
}

int main(int argc, char const *argv[])
{
    //test
    device_pair("");
    return 0;
}