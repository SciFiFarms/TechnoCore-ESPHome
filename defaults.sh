#!/bin/env bash

#TODO: Anything that gets sent to STDOUT from this file gets included in the 
#      final compose file, which usually breaks it. 

# compose.sh defaults $service_name to the service's directory name. It can be overridden here. 
# service_name=

# Leave blank to disable this service by default.
set_service_flag $service_name

if [[ "$LIVE_MOUNT_ESPHOME_ENABLED" != "" ]]; then
    # START HERE: I think these might need
    export ESPHOME_VOLUME=${TECHNOCORE_ROOT}/hals/
else
    export ESPHOME_VOLUME=esphome
fi
#set_service_flag $service_name yes

# Sets the application prefix depending on what $INGRESS_TYPE is set to. 
# Results in one of the following paths: 
# https://some.domain/prefix/
# https://prefix.some.domain/
# prefix=$service_name

set_optional_service home-assistant
set_optional_service vernemq
set_optional_service vault
set_optional_service syncthing

#generate_mount dev shell-migrations /usr/share/dogfish/shell-migr
#      - ${esphome_live_mount:-./empty/:/opt/.dummy}
