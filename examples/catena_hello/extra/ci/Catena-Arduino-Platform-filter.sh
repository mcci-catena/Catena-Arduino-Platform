#!/bin/bash

##############################################################################
#
# Module: catena_hello.ino/extra/ci/Catena-Arduino-Platform-filter.sh
#
# Function:
#	This script must be sourced; it sets variables used by other
#	scripts in this directory.
#
# Usage:
#	source ci/Catena-Arduino-Platform-filter.sh
#
# Copyright and License:
#	See accompanying LICENSE.md file
#
# Author:
#	RamaSubbu, MCCI	August 2021
#
##############################################################################

#### use the common code.
# shellcheck source=../../../../ci/Catena-Arduino-Platform-filter-common.sh
source "$(dirname "$MCCI_CI_FILTER_NAME")"/../../../../ci/Catena-Arduino-Platform-filter-common.sh

#### end of file ####
