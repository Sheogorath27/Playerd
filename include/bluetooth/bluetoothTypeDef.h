#ifndef BLUETOOTH_TYPEDEF_H_
#define BLUETOOTH_TYPEDEF_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

//Possible error codes.
#define BluetoothSuccess            0;
#define errUnknown                 -1;
#define errMethodNotSupported      -2;
#define errAdapterNotFound          1;
#define errDeviceNotFound           2;
#define errConnectionTimeout        3;
#define errConnectionFailed         4;
#define errPairingFailed            5;
#define errWrongPassKey             6;
#define errSdpQueryFailed           7;
#define errDeviceDiscoveryFailed    8;
#define errNoServiceFound           9;
#define errAdpaterRefusedChange    10;
#define errAlreadyEnabele          11;
#define errAlreadyDiscoverable     12;
#define errAlreadyDisabele         13;
#define errAlreadyNonDiscoverable  14;
#define errSocket                  15;
#define errInvalidUuid             16;

typedef enum _plibUuid
{
	SDP,
	RFCOMM,
	L2CAP,
	PUBLIC_BROWSE,
	HID,
	A2DP,
	HSP,
	HFP,
}plibUuid;

//structures.
typedef struct _plibProfile
{
	plibUuid        protocol;
	uint16_t        channel;
}plibProfile;

typedef struct _plibService
{
	char            name[51];
	char            description[51];
	plibUuid        service;
	int             numOfData;
	plibProfile     *data;
}plibService;

typedef struct _plibSdpList
{
	plibService         data; 
	struct plibSdpList  *next;
}plibSdpList;

typedef struct _plibBtDevice
{
	int             sdp;
	char            btAddress[19];
	char            name[30];
	plibSdpList     *sdplist; 
}plibBtDevice;

typedef struct _plibDiscoveryList
{
	plibBtDevice                dev;
	struct plibDiscoveryList    *next;
}plibDiscoveryList;

#ifdef __cplusplus
}
#endif

#endif //BLUETOOTH_TYPEDEF_H_