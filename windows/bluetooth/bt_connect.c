
#include <stdio.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <Bthsdpdef.h>
#include <BluetoothAPIs.h>
#include <windows.h>
#include <bluetoothTypeDef.h>

BTH_ADDR device_addr;
void Perror()
{
    LPVOID lpMsgBuf;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, (LPSTR) &lpMsgBuf, 0, NULL );
    fprintf( stderr, "%s\n", lpMsgBuf );
    free(lpMsgBuf); 
}

void PdvcInfo(BLUETOOTH_DEVICE_INFO *pbtdi)
{
    fprintf(stdout, "Name: %S\n", pbtdi->szName );
}

plibDiscoveryList* winGoToEnd(plibDiscoveryList **list)
{
    plibDiscoveryList *end = *list;
    while(end->next != NULL)
        end = end->next;
    return end;
}

void winCreateNode(BLUETOOTH_DEVICE_INFO *pbtdi, plibDiscoveryList **list)
{
    SOCKADDR_BTH dev;
    char buf[50] = { 0 };
    int buf_len = sizeof(buf);
    plibDiscoveryList *tmp1;
    int i;
    plibDiscoveryList *tmp = (plibDiscoveryList*) malloc(sizeof(plibDiscoveryList));

    dev.addressFamily = AF_BTH;
    dev.btAddr = pbtdi->Address.ullLong;
    if(WSAAddressToString( (LPSOCKADDR)&dev, (DWORD) sizeof(dev), NULL, buf, &buf_len ) != 0)
        Perror();
    for ( i = 0; i < 18; ++i)
        temp->btAddress[i] = buf[i+1];
    temp->dev.btAddress[18] = '\0';
    temp->dev.sdp = 0;
    
    if(WideCharToMultiByte(CP_UTF8, 0, (pbtdi->szName), 29, temp->dev.name, sizeof(temp->dev.name), NULL, NULL ) == 0)
        Perror();
    temp->next = NULL;

    if (*list == NULL)
        *list = temp;
    else
    {
        tmp1 = winGoToEnd(plibDiscoveryList **list);
        temp1->next = temp;
    }
}

void Psdp(WSAQUERYSET *qs)
{   
    int c = qs->lpcsaBuffer->iProtocol;
    printf( "Name : %s\n", qs->lpszServiceInstanceName );
    printf( "Description : %s\n", qs->lpszComment );
    SOCKADDR_BTH *dvbt = qs->lpcsaBuffer->RemoteAddr.lpSockaddr;
    switch( c )
    {
        case BTHPROTO_RFCOMM: fprintf( stdout, "RFCOMM on " );
                              fprintf(stdout, "%d\n", dvbt->port );
                              break;

        case BTHPROTO_L2CAP: fprintf( stdout, "L2CAP on " );
                             fprintf(stdout, "%d\n", dvbt->port );
                             break;

        default: //fprintf(stdout, "%d on",c );
                 break;
    }    
}

plibSdpList* winGoToEndSdp(plibSdpList *list)
{
    plibSdpList *end;
    while(end->next != NULL)
        end = end->next;
    return end;
}

void winCreateSdpNode(WSAQUERYSET *qs, plibBtDevice *dev)
{
    //GUID guid;

    int count = qs->dwNumberOfCsAddrs;
    int i = 0;
    int c;
    plibSdpList *temp, *temp1;
    temp = (plibSdpList*) malloc(sizeof(plibSdpList));

    //this function was meant for searching specific service, so doesn't 
    //return uuid even on Public Browse Service

    strncpy(temp->data.name, qs->lpszServiceInstanceName, 50);
    strncpy(temp->data.description, qs->lpszComment, 50);
    temp->data.name[50] = '\0';
    temp->data.description[50] = '\0';
    temp->data.numOfData = count;
    temp->data.data = (plibProfile*) malloc(count*sizeof(plibProfile));
    temp->next = NULL;

    // printf( "Name : %s\n", qs->lpszServiceInstanceName );
    // printf( "Description : %s\n", qs->lpszComment );
    // if (qs->lpServiceClassId == NULL)
    // {
    //     printf("not returned\n");
    // }
    // else if (IsEqualGUID(&sid, &PublicBrowseGroupServiceClass_UUID))
    // {
    //     printf("return same\n");
    // }
    //printf("%llx\n", PublicBrowseGroupServiceClass_UUID);

    while(count)
    {
        //guid = *(qs->lpServiceClassId);

        c = qs->lpcsaBuffer[i].iProtocol;
        SOCKADDR_BTH *dvbt = qs->lpcsaBuffer[i].RemoteAddr.lpSockaddr;
        
        //guid = dvbt->serviceClassId;
        // if (IsEqualGUID(&guid, &RFCOMM_PROTOCOL_UUID))
        // {
        //     printf("return same\n");
        // }
        
        //guid = RFCOMM_PROTOCOL_UUID;
        // printf("Guid = {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        
        switch( c )
        {
            case BTHPROTO_RFCOMM: //fprintf( stdout, "RFCOMM on " );
                                  //fprintf(stdout, "%d\n", dvbt->port );
                                  temp->data.data[i].protocol = RFCOMM;
                                  temp->data.data[i].channel = dvbt->port;
                                  break;

            case BTHPROTO_L2CAP: //fprintf( stdout, "L2CAP on " );
                                 //fprintf(stdout, "%d\n", dvbt->port );
                                 temp->data.data[i].protocol = L2CAP;
                                 temp->data.data[i].channel = dvbt->port;
                                 break;

            default: //fprintf(stdout, "%d on",c );
                     break;
        }

        //switch()
        i++;
        count--;
        //printf("\n----------------\n");
    }
    if (dev->sdplist == NULL)
        dev->sdplist = temp;
    else
    {
        temp1 = winGoToEndSdp(dev->sdplist);
        temp1->next = temp;
    }
}

int winInitSocket()
{
    WSADATA wsadata;
    WORD wVersionRequested = MAKEWORD( 2, 0 );
    if (WSAStartup( wVersionRequested, &wsadata ) != NO_ERROR )
    {
        Perror();
        WSACleanup();
        return errSocket;
    }
    fprintf(stdout, "Done ....\n" );
    return BluetoothSuccess;
}

//If bluetooth radio is turned off from main control pannel then it
//resists all changes.
int winEnableAdapter()
{
    HANDLE hRadio = NULL;
    if ( !BluetoothIsConnectable( hRadio ) )
    {
        if ( !BluetoothEnableIncomingConnections( hRadio, 1 ) )
        {
            fprintf( stderr, "No radio accepted change\n" );
            return errAdpaterRefusedChange;
        }
        //fprintf( stdout, "Bluetooth is now connectable\n" );
        return BluetoothSuccess;
    }
    //fprintf( stdout, "Bluetooth radio already connectable\n" );
    return errAlreadyEnabele;
}

int winDisableDiscoverable()
{
    HANDLE hRadio = NULL;
    if ( BluetoothIsDiscoverable( hRadio ) )
    {
        if ( !BluetoothEnableDiscovery( hRadio, 1 ) )
        {
            fprintf( stderr, "No radio accepted change\n" );
            return errAdpaterRefusedChange;
        }
        fprintf( stdout, "Bluetooth is now non-discoverable\n" );
        return BluetoothSuccess;
    }
    fprintf( stdout, "Bluetooth radio already non-discoverable\n" );
    return errAlreadyNonDiscoverable;
}

int winDisableAdapter()
{
    HANDLE hRadio = NULL;
    if ( BluetoothIsDiscoverable( hRadio ) )
    {
        //fprintf( stdout, "Make non-discoverable\n" );
        if ( winDisableDiscoverable() == errAdpaterRefusedChange )
            return errAdpaterRefusedChange;            
    }
    if ( BluetoothIsConnectable( hRadio ) )
    {
        if ( !BluetoothEnableIncomingConnections( hRadio, 0 ) )
        {
            fprintf( stderr, "No radio accepted change\n" );
            return errAdpaterRefusedChange;
        }
        //fprintf( stdout, "Bluetooth is now non-connectable\n" );
        return BluetoothSuccess;
    }
    fprintf( stdout, "Bluetooth radio already non-connectable\n" );
    return errAlreadyDisabele;
}

int winEnableDiscoverable()
{
    HANDLE hRadio = NULL;
    if ( !BluetoothIsConnectable( hRadio ) ) 
        if ( winEnableAdapter == errAdpaterRefusedChange )
            return errAdpaterRefusedChange;

    if ( !BluetoothIsDiscoverable( hRadio ) )
    {
        if ( !BluetoothEnableDiscovery( hRadio, 1 ) )
        {
            fprintf( stderr, "No radio accepted change\n" );
            return errAdpaterRefusedChange;
        }
        fprintf( stdout, "Bluetooth is now discoverable\n" );
        return BluetoothSuccess;
    }
    fprintf( stdout, "Bluetooth radio already discoverable\n" );
    return errAlreadyDiscoverable;
}

void connect_rfcomm( BTH_ADDR dvaddr,int dvprt )
{
    SOCKET sock;
    SOCKADDR_BTH btdev = { 0 };
    int btdev_len = sizeof( btdev );
    btdev.addressFamily = AF_BTH;
    btdev.btAddr = dvaddr;
    btdev.port = dvprt;
    WCHAR dvkey[BLUETOOTH_MAX_PASSKEY_SIZE];
    DWORD dverror;


    sock = socket( AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM );
    if ( sock == SOCKET_ERROR )
    {
        Perror();
        ExitProcess( 2 );
    }

    //A dedicated application for key code can be later registered.
    fprintf( stdout, "Enter Key : " );
    fscanf( stdin, "%S", &dvkey );
    dverror = BluetoothAuthenticateDevice( NULL, NULL, pbtdev, &dvkey, BLUETOOTH_MAX_PASSKEY_SIZE );
    if ( dverror != ERROR_SUCCESS && dverror != ERROR_NO_MORE_ITEMS )
    Perror();

    if ( SOCKET_ERROR == connect( sock, (LPSOCKADDR) &btdev, btdev_len ) )
    {
        Perror();
        ExitProcess( 2 );
    }
    
    send( sock, "Connected!!", 11, 0 );
    
    closesocket( sock );
}

int winSdpQuery( plibBtDevice *dev, LPGUID uuid )
{
    int dvprt = 0;
    HANDLE h;
    WSAQUERYSET *qs;
    DWORD flags = 0;
    DWORD qs_len;
    int done;
    int error;

    qs_len = sizeof( WSAQUERYSET );
    qs = ( WSAQUERYSET * ) malloc( qs_len );
    ZeroMemory( qs, qs_len );
    qs->dwSize = qs_len;
    qs->lpServiceClassId = uuid;
    qs->dwNameSpace = NS_BTH;
    qs->dwNumberOfCsAddrs = 0;
    qs->lpszContext = (LPSTR) dev->btAddress;
    flags = LUP_FLUSHCACHE | LUP_RETURN_ADDR;

    if ( SOCKET_ERROR == WSALookupServiceBegin( qs, flags, &h ) )
    {
        Perror();
        return errSocket;
    }

    done = 0;
    while( !done )
    {
        if ( SOCKET_ERROR == WSALookupServiceNext( h, flags, &qs_len, qs ) )
        {
            error = WSAGetLastError();
            if ( error == WSAEFAULT )
                {
                    free(qs);
                    qs = ( WSAQUERYSET * ) malloc( qs_len );
                }
             else if ( error == WSA_E_NO_MORE )
             done = 1;
             else
             {
                Perror();
                return errSdpQueryFailed;
             }     
        }
        else
        winCreateSdpNode(qs, dev);
    }

    free( qs );
    dev->sdp = 1;
    WSALookupServiceEnd(h);
    return BluetoothSuccess;
}

int winFindRemoteDevcie(plibDiscoveryList **list)
{
    BLUETOOTH_DEVICE_SEARCH_PARAMS pbtsp = { 0, 1, 1, 1, 1, 1, 10, NULL};
    BLUETOOTH_DEVICE_INFO pbtdi;
    HANDLE hfind;
    pbtsp.dwSize = sizeof( BLUETOOTH_DEVICE_SEARCH_PARAMS );
    pbtdi.dwSize = sizeof( BLUETOOTH_DEVICE_INFO );
    hfind = BluetoothFindFirstDevice( &pbtsp, &pbtdi );
    if ( hfind == NULL )
    {
        Perror();
        BluetoothFindDeviceClose( hfind );
        return errDeviceDiscoveryFailed;
    }
    winCreateNode( &pbtdi, list );

    while( BluetoothFindNextDevice( hfind, &pbtdi ) )
    {
        winCreateNode( &pbtdi, list );
    }
    BluetoothFindDeviceClose( hfind );
    return BluetoothSuccess;
}


// int main(int argc, char const *argv[])
// {
//     //testing
//     init_win_socket();
//     make_discoverable();
//     //search_device();
//     browse_sdp(argv[1]);
//     //fprintf( stdout, "%llx\n",pbtdev->Address.ullLong );
//     //connect_rfcomm( pbtdev->Address.ullLong, 2);
//     stop_connectable();
//     WSACleanup();
//     return 0;
// }