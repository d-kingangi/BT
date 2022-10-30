from bluetooth import *
from select import *
class MyDiscoverer(DeviceDiscoverer):
def pre_inquiry(self):
self.done = False
def device_discovered(self, address, device_class, name):
print "%s - %s" % (address, name)
def inquiry_complete(self):
self.done = True
d = MyDiscoverer()
d.find_devices(lookup_names = True)
while True:
can_read, can_write, has_exc = select( [d], [], [] )
if d in can_read:
d.process_event()
if d.done: break