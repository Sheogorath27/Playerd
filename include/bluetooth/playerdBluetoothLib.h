#ifndef PLAYERD_BLUETOOTH_LIB_H_
#define PLAYERD_BLUETOOTH_LIB_H_

#include <bluetoothTypeDef.h>

#ifdef __cplusplus
extern "C"{
#endif

/*
	Check "bluetoothTypeDef.h" for common defined error codes. 
*/

int enableBluetoothAdapter(int state);

int enableDeviceDiscovery(int state);
/*
-------- Specification for both enable functions: ----------
	Argument:
        int state : 0 to disable and 1 to enable.
	Return:
        int error
*/

int findRemoteDevice(plibDiscoveryList **list);
/*
-------- Specification: -----------
	Argument:
        plibDiscoveryList **list : pass pointer to null pointer.
	Return:
        plibDiscoveryList **list : allocated list of devices containing 
                                  plibBtDevice structure with initialized 
                                  name and btAddress, call freeDiscoveryList
                                  to free memory.
        int error	
*/

int freeDiscoveryList(plibDiscoveryList *list);
/*
-------- Specification: -----------
	Return:
        int error
*/

int getDeviceName(plibBtDevice *dev);
/*
-------- Specification: -----------
	Argument:
        plibBtDevice *dev : pointer plibBtDevice structure initialized with
                            btAddress.
	Return:
        char[20] name : first 29 characters of user friendly name of device.
        int error		
*/

int performSdpQuery(plibBtDevice *dev, plibUuid uuid);
/*
-------- Specification: -----------
	Argument:
        plibBtDevice *dev : pointer to plibBtDevice structure initialized with
                            valid btAdress and sdplist pointing to null.
        plibUuid uuid : select from enumerated uuid, only these are supported.
                        Use "PUBLIC_BROWSE" for complete list.
	Return:
        plibSdpList *sdplist : list of service record, call freeSdpList to
                               free memory.
        int error
*/

int freeSdpList(plibSdpList *sdplist);
/*
-------- Specification: -----------
	Return:
		int error
*/

//planning to register callback function for entering code.
int startPairing(plibBtDevice *dev);
/*
-------- Specification: -----------
	Argument:
	Return:
*/

//planning to use null function pointer, which points to send function
//after successfull connection.	
int connectRfcomm(plibBtDevice *dev, uint16_t socket);
/*
-------- Specification: -----------
	Argument:
	Return:
*/

int connectL2cap(plibBtDevice *dev, uint16_t channel);
/*
-------- Specification: -----------
	Argument:
	Return:
*/

#ifdef __cplusplus
}
#endif

#endif //PLAYERD_BLUETOOTH_LIB_H_