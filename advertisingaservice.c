#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
sdp_session_t *register_service()
{
uint32_t svc_uuid_int[] = { 0, 0, 0, 0xABCD };
uint8_t rfcomm_port = 11;
const char *service_name = "Roto-Rooter Data Router";
const char *service_dsc = "An experimental plumbing router";
const char *service_prov = "Roto-Rooter";
uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid, svc_class_uuid;
sdp_list_t *l2cap_list = 0,
*rfcomm_list = 0,
*root_list = 0,
*proto_list = 0,
*access_proto_list = 0,
*svc_class_list = 0,
*profile_list = 0;
sdp_data_t *channel = 0;
sdp_profile_desc_t profile;
sdp_record_t record = { 0 };
sdp_session_t *session = 0;
// PART ONE
// set the general service ID
sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
sdp_set_service_id( &record, svc_uuid );
// set the service class
sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
svc_class_list = sdp_list_append(0, &svc_class_uuid);
sdp_set_service_classes(&record, svc_class_list);
// set the Bluetooth profile information
sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
profile.version = 0x0100;
profile_list = sdp_list_append(0, &profile);
sdp_set_profile_descs(&record, profile_list);
// make the service record publicly browsable
sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
root_list = sdp_list_append(0, &root_uuid);
sdp_set_browse_groups( &record, root_list );
// set l2cap information
sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
l2cap_list = sdp_list_append( 0, &l2cap_uuid );
proto_list = sdp_list_append( 0, l2cap_list );
// register the RFCOMM channel for RFCOMM sockets
sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );
sdp_list_append( rfcomm_list, channel );
sdp_list_append( proto_list, rfcomm_list );
access_proto_list = sdp_list_append( 0, proto_list );
sdp_set_access_protos( &record, access_proto_list );
// set the name, provider, and description
sdp_set_info_attr(&record, service_name, service_prov, service_dsc);
// PART TWO
// connect to the local SDP server, register the service record, and
// disconnect
session = sdp_connect( BDADDR_ANY, BDADDR_LOCAL, 0 );
sdp_record_register(session, &record, 0);
// cleanup
sdp_data_free( channel );
sdp_list_free( l2cap_list, 0 );
sdp_list_free( rfcomm_list, 0 );
sdp_list_free( root_list, 0 );
sdp_list_free( access_proto_list, 0 );
return session;
}
int main()
{
sdp_session_t* session = register_service();
// The rest of the program here
sdp_close( session );
return 0;
}
