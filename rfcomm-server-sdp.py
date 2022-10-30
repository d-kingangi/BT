from bluetooth import *
port = get_available_port( RFCOMM )
server_sock=BluetoothSocket( RFCOMM )
server_sock.bind(("",port))
server_sock.listen(1)
advertise_service( server_sock, "Bluetooth Serial Port",
service_classes = [ SERIAL_PORT_CLASS ],
profiles = [ SERIAL_PORT_PROFILE ] )
client_sock, client_info = server_sock.accept()
print "Accepted connection from ", client_info
data = client_sock.recv(1024)
print "received [%s]" % data
client_sock.close()
server_sock.close()