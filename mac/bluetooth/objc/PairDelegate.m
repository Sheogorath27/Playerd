#import "PairDelegate.h"
#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/IOBluetoothUtilities.h>

void Perror(int exp)
{
	IOReturn  err = exp;
	if ( err != kIOReturnSuccess )
	 	fprintf( stderr, "%s(%x)\n", mach_error_string(err), err );
    //exit(EX_OSERR)
}

@implementation PairDelegate

- (void) devicePairingStarted:(id)sender;
{
    fprintf( stdout, "Pairing started.\n");
}

- (void) devicePairingConnecting:(id)sender;
{
    fprintf( stdout, "Connecting to remote device.\n");
}

- (void) devicePairingPINCodeRequest:(id)sender;
{
    BluetoothPINCode pin ;
    ByteCount pinsize;
    //testing
    //will add method with function pointer to implemented for UI call.
    pin.data[1] = '1';
    pin.data[2] = '2';
    pinsize = sizeof(pin);
    [sender replyPINCode: pinsize PINCode: &pin];
}

- (void) devicePairingUserConfirmationRequest:(id)sender
                                 numericValue:(BluetoothNumericValue)numericValue;
{
    BOOL reply;
    fprintf(stdout, "Numerical Value : %u\nEnter Confirmation(YES or NO) : ", numericValue);
    fscanf(stdin, "%s",&reply);
    [sender replyUserConfirmation: reply];
}

- (void) devicePairingUserPasskeyNotification:(id)sender
                                      passkey:(BluetoothPasskey)passkey;
{
    fprintf(stdout, "PassKey : %u", passkey);
}

- (void) devicePairingFinished:(id)sender
                         error:(IOReturn)error;
{
    if (error != kIOReturnSuccess)
    {
        Perror(error);
        //exit(EX_OSERR);
    }
    fprintf( stdout, "Pairing to remote device complete.\n");
    CFRunLoopStop(CFRunLoopGetCurrent());
}

- (void) deviceSimplePairingComplete:(id)sender
                              status:(BluetoothHCIEventStatus)status;
{
    if (status != kBluetoothHCIEventSimplePairingComplete)
    {
        Perror(status);
        //exit(EX_OSERR);
    }
    fprintf( stdout, "Simple Secure Pairing to remote device complete.\n");
    CFRunLoopStop(CFRunLoopGetCurrent());
}

@end

//test
int main()
{
    BluetoothDeviceAddress btadd;
    CFStringRef addcfstr;
    IOReturn err;
    PairDelegate *pdel = [[PairDelegate alloc] init];
    addcfstr = CFSTR("80:50:1B:3D:8E:1B");
    IOBluetoothCFStringToDeviceAddress(addcfstr, &btadd);
    IOBluetoothDevice *dev = [IOBluetoothDevice deviceWithAddress: &btadd];
    IOBluetoothDevicePair *devpair = [[IOBluetoothDevicePair alloc] init];
    [devpair setDevice: dev];
    [devpair setDelegate: pdel];
    err = [devpair start];
    CFRunLoopRun();
}