### Qos ###

<<<<<<< HEAD
This is a Linux bash script that will set up tc to limit the outgoing bandwidth for connections to the Bitcoin network. It limits outbound TCP traffic with a source or destination port of 59682, but not if the destination IP is within a LAN (defined as 192.168.x.x).

This means one can have an always-on prcycoind instance running, and another local prcycoind/prcycoin-qt instance which connects to this node and receives blocks from it.
=======
This is a Linux bash script that will set up tc to limit the outgoing bandwidth for connections to the Bitcoin network. It limits outbound TCP traffic with a source or destination port of 51472, but not if the destination IP is within a LAN (defined as 192.168.x.x).

This means one can have an always-on pivxd instance running, and another local pivxd/pivx-qt instance which connects to this node and receives blocks from it.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
