#import <Foundation/Foundation.h>
#import <IOBluetooth/objc/IOBluetoothDevice.h>

@interface SdpDelegate : NSObject {}

- (void)sdpQueryComplete:(IOBluetoothDevice *)device
                  status:(IOReturn)status;

@end