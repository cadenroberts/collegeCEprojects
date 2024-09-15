from pox.core import core
import pox.openflow.libopenflow_01 as of
log = core.getLogger()
class Final (object):
  def __init__ (self, connection):
    self.connection = connection
    connection.addListeners(self)
  def do_final (self, packet, packet_in, port_on_switch, switch_id):
    packetICMP = packet.find("icmp")
    packetIPV4 = packet.find("ipv4")
    if(packetIPV4 is not None):
      if(switch_id == 1): # f1s1 connects h101 h102
        if(port_on_switch == 8):
          if(packetIPV4.dstip == "128.114.1.102"):
            self.allow(packet, packet_in, 9)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 9):
          if(packetIPV4.dstip == "128.114.1.101"):
            self.allow(packet, packet_in, 8)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 3):
          if(packetIPV4.dstip == "128.114.1.101"):
            self.allow(packet, packet_in, 8)
          elif(packetIPV4.dstip == "128.114.1.102"):
            self.allow(packet, packet_in, 9)
          else:
            self.block(packet, packet_in)
        else:
          self.block(packet, packet_in)
      elif(switch_id == 2): # f1s2 connects h103 and h104
        if(port_on_switch == 8):
          if(packetIPV4.dstip == "128.114.1.104"):
            self.allow(packet, packet_in, 9)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 9):
          if(packetIPV4.dstip == "128.114.1.103"):
            self.allow(packet, packet_in, 8)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 3):
          if(packetIPV4.dstip == "128.114.1.103"):
            self.allow(packet, packet_in, 8)
          elif(packetIPV4.dstip == "128.114.1.104"):
            self.allow(packet, packet_in, 9)
          else:
            self.block(packet, packet_in)
        else:
          self.block(packet, packet_in)
      elif(switch_id == 3): # f2s1 connects h201 and h202
        if(port_on_switch == 8):
          if(packetIPV4.dstip == "128.114.2.202"):
            self.allow(packet, packet_in, 9)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 9):
          if(packetIPV4.dstip == "128.114.2.201"):
            self.allow(packet, packet_in, 8)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 3):
          if(packetIPV4.dstip == "128.114.2.201"):
            self.allow(packet, packet_in, 8)
          elif(packetIPV4.dstip == "128.114.2.202"):
            self.allow(packet, packet_in, 9)
          else:
            self.block(packet, packet_in)
        else:
          self.block(packet, packet_in)
      elif(switch_id == 4): # f2s2 connects h203 and h204
        if(port_on_switch == 8):
          if(packetIPV4.dstip == "128.114.2.204"):
            self.allow(packet, packet_in, 9)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 9):
          if(packetIPV4.dstip == "128.114.2.203"):
            self.allow(packet, packet_in, 8)
          else:
            self.allow(packet, packet_in, 3)
        elif(port_on_switch == 3):
          if(packetIPV4.dstip == "128.114.2.203"):
            self.allow(packet, packet_in, 8)
          elif(packetIPV4.dstip == "128.114.2.204"):
            self.allow(packet, packet_in, 9)
          else:
            self.block(packet, packet_in)
        else:
          self.block(packet, packet_in)
      elif(switch_id == 5): # core switch connected f1s1 f1s2 f2s1 f2s2 dataCenterSwitch h_trust h_untrust
        if(packetICMP is not None):
          if(port_on_switch == 7 and (packetIPV4.dstip == "128.114.1.101" or packetIPV4.dstip == "128.114.1.102" or packetIPV4.dstip == "128.114.1.103" or packetIPV4.dstip == "128.114.1.104" or packetIPV4.dstip == "128.114.2.201" or packetIPV4.dstip == "128.114.2.202" or packetIPV4.dstip == "128.114.2.203" or packetIPV4.dstip == "128.114.2.204" or packetIPV4.dstip == "128.114.3.178")):
            self.block(packet, packet_in)
          elif(port_on_switch == 5 and (packetIPV4.dstip == "128.114.2.201" or packetIPV4.dstip == "128.114.2.202" or packetIPV4.dstip == "128.114.2.203" or packetIPV4.dstip == "128.114.2.204" or packetIPV4.dstip == "128.114.3.178")):
            self.block(packet, packet_in)
          elif((port_on_switch == 1 or port_on_switch == 2) and (packetIPV4.dstip == "128.114.2.201" or packetIPV4.dstip == "128.114.2.202" or packetIPV4.dstip == "128.114.2.203" or packetIPV4.dstip == "128.114.2.204")):
            self.block(packet, packet_in)
          elif((port_on_switch == 3 or port_on_switch == 4) and (packetIPV4.dstip == "128.114.1.101" or packetIPV4.dstip == "128.114.1.102" or packetIPV4.dstip == "128.114.1.103" or packetIPV4.dstip == "128.114.1.104")):
            self.block(packet, packet_in)
          else:
            if(port_on_switch == 1):
              if(packetIPV4.dstip == "128.114.1.103" or packetIPV4.dstip == "128.114.1.104"):
                self.allow(packet, packet_in, 2)
              elif(packetIPV4.dstip == "192.47.38.109"):
                self.allow(packet, packet_in, 5)
              elif(packetIPV4.dstip == "128.114.3.178"):
                self.allow(packet, packet_in, 6)
              else:
                self.block(packet, packet_in)
            elif(port_on_switch == 2):
              if(packetIPV4.dstip == "128.114.1.101" or packetIPV4.dstip == "128.114.1.102"):
                self.allow(packet, packet_in, 1)
              elif(packetIPV4.dstip == "192.47.38.109"):
                self.allow(packet, packet_in, 5)
              elif(packetIPV4.dstip == "128.114.3.178"):
                self.allow(packet, packet_in, 6)
              else:
                self.block(packet, packet_in)
            elif(port_on_switch == 3):
              if(packetIPV4.dstip == "128.114.2.203" or packetIPV4.dstip == "128.114.2.204"):
                self.allow(packet, packet_in, 4)
              elif(packetIPV4.dstip == "128.114.3.178"):
                self.allow(packet, packet_in, 6)
              else:
                self.block(packet, packet_in)
            elif(port_on_switch == 4):
              if(packetIPV4.dstip == "128.114.2.201" or packetIPV4.dstip == "128.114.2.202"):
                self.allow(packet, packet_in, 3)
              elif(packetIPV4.dstip == "128.114.3.178"):
                self.allow(packet, packet_in, 6)
              else:
                self.block(packet, packet_in)
            elif(port_on_switch == 5):
              if(packetIPV4.dstip == "128.114.1.101" or packetIPV4.dstip == "128.114.1.102"):
                self.allow(packet, packet_in, 1)
              elif(packetIPV4.dstip == "128.114.1.103" or packetIPV4.dstip == "128.114.1.104"):
                self.allow(packet, packet_in, 2)
              elif(packetIPV4.dstip == "108.35.24.113"):
                self.allow(packet, packet_in, 7)
              else:
                self.block(packet, packet_in)
            elif(port_on_switch == 6):
              if(packetIPV4.dstip == "128.114.1.101" or packetIPV4.dstip == "128.114.1.102"):
                self.allow(packet, packet_in, 1)
              elif(packetIPV4.dstip == "128.114.1.103" or packetIPV4.dstip == "128.114.1.104"):
                self.allow(packet, packet_in, 2)
              elif(packetIPV4.dstip == "128.114.2.201" or packetIPV4.dstip == "128.114.2.202"):
                self.allow(packet, packet_in, 3)
              elif(packetIPV4.dstip == "128.114.2.203" or packetIPV4.dstip == "128.114.2.204"):
                self.allow(packet, packet_in, 4)
              else:
                self.block(packet, packet_in)
            elif(port_on_switch == 7):
              if(packetIPV4.dstip == "192.47.38.109"):
                self.allow(packet, packet_in, 5)
              else:
                self.block(packet, packet_in)
            else:
              self.block(packet, packet_in)
        elif(packetIPV4.srcip == "192.47.38.109" and packetIPV4.dstip == "128.114.3.178"):
          self.block(packet, packet_in)
        elif(packetIPV4.srcip == "108.35.24.113" and packetIPV4.dstip == "128.114.3.178"):
          self.block(packet, packet_in)
        else:
          self.allow(packet, packet_in, of.OFPP_FLOOD)
      elif(switch_id == 6): # data center switch connects h_server
        if(port_on_switch == 8):
          self.allow(packet, packet_in, 2)
        elif(port_on_switch == 2):
          self.allow(packet, packet_in, 8)
        else:
          self.block(packet, packet_in)
    else:
      self.allow(packet, packet_in, of.OFPP_FLOOD)
  def _handle_PacketIn (self, event):
    packet = event.parsed # This is the parsed packet data.
    if not packet.parsed:
      log.warning("Ignoring incomplete packet")
      return
    packet_in = event.ofp # The actual ofp_packet_in message.
    self.do_final(packet, packet_in, event.port, event.dpid)
  def allow(self, packet, packet_in, output_port):
    msg = of.ofp_flow_mod()
    msg.match = of.ofp_match.from_packet(packet)
    msg.idle_timeout = 30
    msg.hard_timeout = 30
    msg.buffer_id = packet_in.buffer_id
    msg.actions.append(of.ofp_action_output(port = output_port))
    msg.data = packet_in
    self.connection.send(msg)
  def block(self, packet, packet_in):
    msg = of.ofp_flow_mod()
    msg.match = of.ofp_match.from_packet(packet)
    msg.idle_timeout = 30
    msg.hard_timeout = 30
    msg.buffer_id = packet_in.buffer_id
    self.connection.send(msg)
def launch ():
  def start_switch (event):
    log.debug("Controlling %s" % (event.connection,))
    Final(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)
