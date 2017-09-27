#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -net nic,model=rtl8139,vlan=2,macaddr=12:34:56:AB:CD:EE -net socket,vlan=2,connect=localhost:1234