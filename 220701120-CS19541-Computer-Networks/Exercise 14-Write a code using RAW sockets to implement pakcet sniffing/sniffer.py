from scapy.all import sniff,conf
from scapy.layers.inet import IP, TCP, UDP, ICMP
# print(dir(IP), dir(TCP), dir(UDP), dir(ICMP),sep="\n\n")
for iface in conf.ifaces:
    print(conf.ifaces[iface].name)

def packet_callback(packet):
    if IP in packet:
        # print(packet)
        ip_layer = packet[IP]
        protocol = ip_layer.proto
        src_ip = ip_layer.src
        dst_ip = ip_layer.dst

        # Determine the protocol
        protocol_name = ""
        if protocol == 1:
            protocol_name = "ICMP"
        elif protocol == 6:
            protocol_name = "TCP"
        elif protocol == 17:
            protocol_name = "UDP"
        else:
            protocol_name = "Unknown Protocol"

        # Print packet details
        if src_ip=="192.168.137.220":
            print(f"Protocol: {protocol_name}")
            print(f"Source IP: {src_ip}")
            print(f"Destination IP: {dst_ip}")
            print("-" * 50)

    # Capture packets on the default network interface
sniff(prn=packet_callback, iface="Wi-Fi",filter="ip", store=0)
