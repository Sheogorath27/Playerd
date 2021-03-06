#import "DiscoveryDelegate.h"

@implementation DiscoveryDelegate

-(void) deviceInquiryStarted:(IOBluetoothDeviceInquiry *)sender
{
    printf("Started");
}

-(void) deviceInquiryComplete: (IOBluetoothDeviceInquiry *) sender
                        error: (IOReturn) error
                      aborted: (BOOL) aborted;
{
	printf("Inquiry complete\n");
	CFRunLoopStop(CFRunLoopGetCurrent());
}

-(void) deviceInquiryDeviceFound: (IOBluetoothDeviceInquiry *) sender
                          device: (IOBluetoothDevice *) device;
{
	printf("discovered %s\n", [ device.addressString cString]);
}

@end

//test
int main(int argc, char const *argv[])
{
	//NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	DiscoveryDelegate *d = [[DiscoveryDelegate alloc] init];
    
	IOBluetoothDeviceInquiry *bdi = [[IOBluetoothDeviceInquiry alloc] init];
	[bdi setDelegate: d];
	[bdi start];
    
	CFRunLoopRun();
    
	//[bdi release];
	//[d release];
	//[pool release];
	return 0;
}