#!/usr/bin/python
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
class MyTopology(Topo):
  def __init__(self):
    Topo.__init__(self)
    switch = self.addSwitch('s1')
    host1 = self.addHost('h1')
    host2 = self.addHost('h2')
    host3 = self.addHost('h3')
    host4 = self.addHost('h4')
    self.addLink(host1, switch)
    self.addLink(host2, switch)
    self.addLink(host3, switch)
    self.addLink(host4, switch)
if __name__ == '__main__':
  topo = MyTopology()
  net = Mininet( topo=topo )
  net.start()
  CLI(net)
  net.stop()
