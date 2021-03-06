#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

int main(int argc, char **argv)
{
    uuid_t service_uuid;
    int err;
    bdaddr_t target;
    sdp_list_t *response_list = NULL, *search_list, *attrid_list, *list = 0, *proto = 0;
    sdp_session_t *session = 0;
    char *buf;

    if (argc < 2)
    {
        printf("Usage: sdp <Bluetooth address> \n");
        return 1;
    }
    str2ba( argv[1], &target );

    session = sdp_connect( BDADDR_ANY, &target, SDP_RETRY_IF_BUSY );

    sdp_uuid16_create( &service_uuid, PUBLIC_BROWSE_GROUP );  
    search_list = sdp_list_append( NULL, &service_uuid );

    uint32_t range = 0x0000ffff;
    attrid_list = sdp_list_append( NULL, &range );

    err = sdp_service_search_attr_req( session, search_list, SDP_ATTR_REQ_RANGE, attrid_list, &response_list);

    sdp_list_t *r = response_list;


    for (; r; r = r->next ) 
    {
        sdp_record_t *rec = (sdp_record_t*) r->data;
        
        sdp_record_print(rec);
        printf("Service RecHandle: 0x%x\n", rec->handle);

        //to do : Add functions for service class id, protocol, port,profile
        printf("------------------\n\n");
        sdp_record_free( rec );
    }

    sdp_list_free(response_list, 0);
    sdp_list_free(search_list, 0);
    sdp_list_free(attrid_list, 0);
    sdp_close(session);
}