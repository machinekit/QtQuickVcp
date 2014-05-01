#!/bin/bash
export DEBUG=5
. /home/linuxcnc/machinekit/scripts/rip-environment
# kill any current sessions
realtime stop
cd /home/linuxcnc/motorctrl
halrun -I motorctrl.hal
exit 0
