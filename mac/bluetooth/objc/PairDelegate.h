#import <Foundation/Foundation.h>
#import <IOBluetooth/objc/IOBluetoothDevicePair.h>

@interface PairDelegate : NSObject {}

- (void) devicePairingStarted:(id)sender;

- (void) devicePairingConnecting:(id)sender;

- (void) devicePairingPINCodeRequest:(id)sender;

- (void) devicePairingUserConfirmationRequest:(id)sender
                                 numericValue:(BluetoothNumericValue)numericValue;

- (void) devicePairingUserPasskeyNotification:(id)sender
                                      passkey:(BluetoothPasskey)passkey;

- (void) devicePairingFinished:(id)sender
                         error:(IOReturn)error;

- (void) deviceSimplePairingComplete:(id)sender
                              status:(BluetoothHCIEventStatus)status;

@end
