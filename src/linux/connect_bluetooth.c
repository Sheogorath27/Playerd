/*
 *  playerd
 *  Copyright (C) 2014,2015 Copyleft Games Group
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program; if not, see http://www.gnu.org/licenses
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <bluetoothTypeDef.h>
#include <syslog.h>
#include <errno.h>

int oslinuxSocket;

void linuxPerror(char *str)
{
    openlog("Playerd", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
    syslog(LOG_INFO, "Program started by User : %d", getuid() );
    syslog(LOG_ERR, "%s" ,str);
    syslog(LOG_ERR, Error code "%d : %s" ,errno, strerror(errno));
    closelog();
}

int linuxFindRemoteDevice(inquiry_info **devlist, int *num_resp, int *sock)
{
    //bluetooth discovery
    //inquiry_info *ii = NULL;
    int max_rsp;//, num_rsp
    int dev_id, len, flags;//, sock
    //int i;
    //char addr[19] = { 0 };
    //char name[30] = { 0 };

    dev_id = hci_get_route(NULL);
    *sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        linuxPerror(" Error while opening socket");
        return errAdapterNotFound;
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    *devlist = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    *num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, devlist, flags);
    if( *num_rsp < 0 )
    {
        linuxPerror("hci inquiry failed");
        return errDeviceDiscoveryFailed;
    }
    return BluetoothSuccess;
}


int init_bluetooth(char *btAddress) {
    struct sockaddr_rc addr={0};
    int status;
    /*-----------------------------------------------------------
    * SOCK_STREAM
    * Provides sequenced, reliable, two-way, connection-based
    * byte streams. An out-of-band data transmission
    * mechanism may be supported.
    *----------------------------------------------------------*/
    // Allocate a socket
    oslinuxSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    // Set what type and who to connect to
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(btAddress, &addr.rc_bdaddr);
    // Pairing
    // Dirty hackish solution since can't think of (or find) a better way to do the pairing that does
    // not involve a few hundred lines of code, yet.
    char command[60];
    const char *f = "bluez-simple-agent hci0 ";
    strcpy( command, f);
    strcpy( command+strlen(f), btAddress);
    system(command);

    // Connect
    // Not really useful (or needed) yet. But it will be needed once we have some stuff ready.
    // Also, it should work, but tends to work only when the stars align and the device decides to feel friendly. 
    // The magic of Bluetooth.
    status = connect(oslinuxSocket, (struct sockaddr *)&addr, sizeof(addr) );
    if (status < 0) 
    {
        perror(" \e[1;31m Error connecting Bluetooth\e[0m");
        return status;
    }
    if (status == 0)
    {
        //do something while connected.
        connected_bluetooth(btAddress);
    }
    return 0;
}

int start_bluetooth(void) 
{
    char btAddress[18];

    // search for devices
    find_bluetooth();

    printf("\e[1;34m Please input the address of the device you would like to connect to:\n (It should be something similar to 00:16:53:01:2C:84)\n\e[0m");
    scanf("%s", btAddress);

    // initiate bluetooth connection
    if (init_bluetooth(btAddress) < 0) 
    {
        close(oslinuxSocket);
        unpair_bluetooth(btAddress);
        return 1;
    }
    printf("bluetooth connected to %s \n", btAddress);
    close(oslinuxSocket);
    printf("bluetooth disconnected from %s \n", btAddress);
    return 0;
}

int unpair_bluetooth(char *btAddress)
{
    char confirmation;
    printf("Are you sure that you want to unpair device %s ? [Y/N] \n", btAddress);
    scanf(" %c", &confirmation);
    if (tolower(confirmation) == 'y')
    {
        // Dirty lazy solution again.
        printf("Unpairing now... \n");
        char command[60];
        const char *f = "bluez-test-device remove ";
        strcpy( command, f);
        strcpy( command+strlen(f), btAddress);
        system(command);
        return 0;
    }
    else
    {
        printf("Unpairing aborted. \n");
      g return 0;
    }
    return 0;
}

int connected_bluetooth(char *btAddress)
{
    // Do something while connected. For now just be annoying and ask to be unpaired.
    while (1)
    {
        sleep(10);
        unpair_bluetooth(btAddress);
    }
    return 0;
}