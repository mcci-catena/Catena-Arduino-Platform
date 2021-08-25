#!/bin/bash

##############################################################################
#
# Module: Catena-Arduino-Platform-filter-common.sh
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
#	Ramasubbu, MCCI	August 2021
#
##############################################################################

#### Capture the file path ####
MCCI_THISFILE="$0"

#### mandatory function: do the filtering
function _Catena-Arduino-Platform_filter {
	declare -r CMD="$1"
	shift
	case "$CMD" in

	# return 0 (success) if should process this sketch.
	"process")
		case "$MCCI_CI_ARCH:$(basename "$1")" in
			# we need to skip this sketch until the SAMD
			# bsp is updated; the Time library uses prog_read_ptr()
			# which is broken in v2.3.0
			"samd:catena_fsm.ino" | \
			"samd:catena_hello.ino" | \
			"samd:catena_hello_lora.ino" | \
			"samd:catena_usercommand.ino")
				return 1
				;;
			# some of the feather sketches fail on non-Feathers
			"stm32:catena_fsm.ino" | \
			"stm32:catena_hello.ino" | \
			"stm32:catena_hello_lora.ino" | \
			"stm32:catena_usercommand.ino")
				return 1
				;;
			*)
				return 0
				;;
		esac
		;;
	esac
}
