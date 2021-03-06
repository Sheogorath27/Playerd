#include <bluetoothWinLib.h>
//#include <Stringapiset.h>

int interFindRemoteDevice(plibDiscoveryList **list)
{
	int err;
	err = winFindRemoteDevice(list);	
	WSACleanup();
	return err;
}

int interSdpQuery(plibBtDevice *dev, plibUuid uuid)
{
	int err;
	if (winInitSocket() == errSocket)
	{
		WSACleanup();
		return errSocket;
	}
	switch(uuid)
	{
		case PUBLIC_BROWSE : err = winSdpQuery(dev, &PublicBrowseGroupServiceClass_UUID);
		    break;

		case HID           : err = winSdpQuery(dev, &HumanInterfaceDeviceServiceClass_UUID);
		    break;

		case A2DP          : err = winSdpQuery(dev, &AdvancedAudioDistributionServiceClass_UUID);
		     break;

		case HSP           : err = winSdpQuery(dev, &HeadsetServiceClass_UUID);
		     break;

		case  HFP          : err = winSdpQuery(dev, &HandsfreeServiceClass_UUID);
		      break;

		default            : err = errInvalidUuid;
		        break;
	}
	WSACleanup;
	return err;
}

int interEnableBluetoothAdapter(int state)
{
	int err;
	if (winInitSocket() == errSocket)
	{
		WSACleanup();
		return errSocket;
	}
	if (state)
		err = winEnableAdapter();
	else
		err = winDisableAdapter();
	WSACleanup();
	return err;	
}

int interEnableDiscovery(int state)
{
	int err;
	if (winInitSocket() == errSocket)
	{
		WSACleanup();
		return errSocket;
	}
	if (state)
		err = winEnableDiscoverable();
	else
		err = winDisableDiscoverable();
	WSACleanup();
	return err;
}