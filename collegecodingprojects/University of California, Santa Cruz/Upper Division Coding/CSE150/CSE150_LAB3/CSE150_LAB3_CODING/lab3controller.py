from pox.core import core
import pox.openflow.libopenflow_01 as of
log = core.getLogger()
class Firewall (object):
  def __init__ (self, connection):
    self.connection = connection
    connection.addListeners(self)
    self.install_rules()
  def install_rules(self):
    msg = of.ofp_flow_mod()
    msg.priority = 10
    msg.match.dl_type = 0x0806
    msg.actions.append(of.ofp_action_output(port=of.OFPP_FLOOD))
    self.connection.send(msg)
    msg = of.ofp_flow_mod()
    msg.priority = 9 
    msg.match.dl_type = 0x0800
    msg.match.nw_proto = 6
    msg.actions.append(of.ofp_action_output(port=of.OFPP_FLOOD))
    self.connection.send(msg)
    msg = of.ofp_flow_mod()
    msg.priority = 1
    msg.match.dl_type = 0x0800
    self.connection.send(msg)
  def do_firewall(self, packet, packet_in):
    if packet.type == packet.ARP_TYPE:
        log.debug("Allowing and flooding ARP packet")
        self.flood_packet(packet_in)
    elif packet.type == packet.IP_TYPE:
        ip_packet = packet.payload
        if ip_packet.protocol == 6:
            log.debug("Allowing and flooding TCP packet")
            self.flood_packet(packet_in)
        else:
            log.debug("Dropping non-TCP IPv4 packet")
    else:
        log.debug("Dropping packet of type %s" % packet.type)
  def flood_packet(self, packet_in):
    msg = of.ofp_packet_out()
    msg.data = packet_in
    action = of.ofp_action_output(port=of.OFPP_FLOOD)
    msg.actions.append(action)
    self.connection.send(msg)
  def _handle_PacketIn (self, event):
    packet = event.parsed
    if not packet.parsed:
      log.warning("Ignoring incomplete packet")
      return
    packet_in = event.ofp
    self.do_firewall(packet, packet_in)
def launch ():
  def start_switch (event):
    log.debug("Controlling %s" % (event.connection,))
    Firewall(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)