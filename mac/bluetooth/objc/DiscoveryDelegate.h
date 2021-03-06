#import <Foundation/Foundation.h>
#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>
#import <IOBluetooth/objc/IOBluetoothDevice.h>

@interface DiscoveryDelegate : NSObject {
    
}

-(void) deviceInquiryComplete: (IOBluetoothDeviceInquiry *) sender
                        error: (IOReturn) error
                      aborted: (BOOL) aborted;

-(void) deviceInquiryDeviceFound: (IOBluetoothDeviceInquiry *) sender
                          device: (IOBluetoothDevice *) device;

-(void) deviceInquiryStarted: (IOBluetoothDeviceInquiry *) sender;

@end