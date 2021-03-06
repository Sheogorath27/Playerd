#import "SdpDelegate.h"
#include <mach/mach.h>
#include <mach/mach_error.h>


void Perror(int exp)
{
	IOReturn  err = exp;
	if ( err != kIOReturnSuccess )
	 	fprintf( stderr, "%s(%x)\n", mach_error_string(err), err );
    //exit(EX_OSERR)
}

void Psdp(CFArrayRef sdparr)
{
    IOBluetoothSDPServiceRecordRef *sdpobj;
    CFIndex count;
    int i;
    count = CFArrayGetCount((sdparr));
    fprintf(stdout, "Found %ld services", count);
    for (i = 0; i < count; i++)
    {
        sdpobj = (IOBluetoothSDPServiceRecordRef* )CFArrayGetValueAtIndex( sdparr, i);
        //will add according to requirement in header.
    }
}

@implementation SdpDelegate

- (void)sdpQueryComplete:(IOBluetoothDevice *)device
                  status:(IOReturn)status;
{
    NSArray *sdparr;
    CFArrayRef cfsdparr;
    if ( status != kIOReturnSuccess)
    {
        Perror(status);
    }
    fprintf(stdout,"Inquiry completed" );
    sdparr = [device services];
    cfsdparr = (__bridge CFArrayRef)sdparr;
    Psdp(cfsdparr);
    CFRunLoopStop(CFRunLoopGetCurrent());
}

@end

int main()
{
    BluetoothDeviceAddress btadd;
    CFStringRef addcfstr;
    IOReturn err;
    SdpDelegate *sdpdel = [[SdpDelegate alloc] init];
    addcfstr = CFSTR("80:50:1B:3D:8E:1B");
    IOBluetoothCFStringToDeviceAddress(addcfstr, &btadd);
    IOBluetoothDevice *dev = [IOBluetoothDevice deviceWithAddress: &btadd];
    err = [dev performSDPQuery: sdpdel];
    
    if (err != kIOReturnSuccess)
    {
        Perror(err);
        //exit(EX_OSERR);
    }
    
    CFRunLoopRun();
}
