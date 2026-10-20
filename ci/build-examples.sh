#!/bin/bash

##############################################################################
#
# Module: build-examples.sh
#
# Function:
#	Compile this library's examples for the supported boards, with
#	arduino-cli.
#
# Copyright notice:
#	See accompanying LICENSE file.
#
# Author:
#	Terry Moore, MCCI Corporation	October 2026
#
# Usage:
#	ci/build-examples.sh -a {samd|stm32} -L {libraries} [-b board | -R] [-n]
#
#	-a	the architecture: samd (MCCI Catena SAMD boards) or stm32
#		(MCCI Catena STM32 boards).
#	-L	a directory holding the libraries this one needs
#		(library.properties "depends": arduino-lmic, arduino-lorawan,
#		Catena-mcciadk, MCCI_FRAM_I2C, MCCI-Catena-nPM1300, and, for
#		samd, RTCZero), each in its own subdirectory. This library is
#		taken from the directory above ci/.
#	-b	build only this board (for example mcci_catena_4610), and
#		no region builds.
#	-R	build only the region builds.
#	-n	print the commands without running them.
#
#	Set ARDUINO_CLI to run arduino-cli another way, for example with
#	--config-file. The MCCI board packages must be installed already
#	(mcci-catena-ci setup.sh does this in CI).
#
#	The build list follows the old .travis.yml: catena_hello on every
#	board, catena_hello_lora on every STM32 board, catena_usercommand
#	and catena_fsm on one board per architecture, and catena_hello_lora
#	in every region on one board per architecture. Boards the installed
#	board package doesn't have are skipped, with a message.
#
##############################################################################

set -e

PNAME="$(basename "$0")"
LIBDIR="$(cd "$(dirname "$0")/.." && pwd)"
ARDUINO_CLI="${ARDUINO_CLI:-arduino-cli}"
# the command and any arguments, as words.
read -r -a ACLI <<< "$ARDUINO_CLI"

OPTARCH=
OPTLIBS=
OPTBOARD=
OPTDRYRUN=0
OPTREGIONS=0

function _fatal {
	echo "$PNAME: $*" 1>&2
	exit 1
}

while getopts a:L:b:nR OPT ; do
	case "$OPT" in
	a)	OPTARCH="$OPTARG" ;;
	L)	OPTLIBS="$OPTARG" ;;
	b)	OPTBOARD="$OPTARG" ;;
	n)	OPTDRYRUN=1 ;;
	R)	OPTREGIONS=1 ;;
	*)	_fatal "usage: -a {samd|stm32} -L {libraries} [-b board | -R] [-n]" ;;
	esac
done

[[ -n "$OPTARCH" ]] || _fatal "-a not given"
[[ -d "$OPTLIBS" ]] || _fatal "-L not given, or not a directory: $OPTLIBS"

SAMD_BOARDS=(mcci_catena_4410 mcci_catena_4420 mcci_catena_4450 mcci_catena_4460 mcci_catena_4470)
STM32_BOARDS=(mcci_catena_4551 mcci_catena_4610 mcci_catena_4611 mcci_catena_4612 mcci_catena_4617
	mcci_catena_4618 mcci_catena_4630 mcci_catena_4801 mcci_catena_4802
	mcci_model_4916 mcci_model_4917 mcci_model_4931 mcci_model_4933
	mcci_catena_5220 mcci_catena_5230)
REGIONS=(us915 eu868 au915 as923 as923jp kr920 in866)

case "$OPTARCH" in
samd)	VENDOR_ARCH=mcci:samd ; BOARDS=("${SAMD_BOARDS[@]}") ; EXTRA_BOARD=mcci_catena_4470 ; REGION_BOARD=mcci_catena_4450 ; STM32=0 ;;
stm32)	VENDOR_ARCH=mcci:stm32 ; BOARDS=("${STM32_BOARDS[@]}") ; EXTRA_BOARD=mcci_catena_4610 ; REGION_BOARD=mcci_catena_4610 ; STM32=1 ;;
*)	_fatal "unknown architecture: $OPTARCH" ;;
esac

# the boards the installed board package has.
INSTALLED="$("${ACLI[@]}" board listall "$VENDOR_ARCH" 2>/dev/null | awk '{print $NF}')"

declare -i NBUILT=0 NFAILED=0 NSKIPPED=0
FAILED=()

function _has_board {
	grep -qx "$VENDOR_ARCH:$1" <<< "$INSTALLED"
}

# _build board region example
function _build {
	local board="$1" region="$2" example="$3"
	local fqbn="$VENDOR_ARCH:$board:lorawan_region=$region"
	local cmd=("${ACLI[@]}" compile --fqbn "$fqbn" --library "$LIBDIR" --libraries "$OPTLIBS" "$LIBDIR/examples/$example")

	if ! _has_board "$board" ; then
		echo "== skip $board ($region) $example: not in the installed $VENDOR_ARCH board package"
		NSKIPPED+=1
		return 0
	fi
	echo "== $board ($region) $example"
	if [[ $OPTDRYRUN -ne 0 ]]; then
		echo "${cmd[@]}"
		return 0
	fi
	if "${cmd[@]}" > /tmp/"$PNAME".$$.log 2>&1 ; then
		NBUILT+=1
		grep -E "^Sketch uses" /tmp/"$PNAME".$$.log || true
	else
		NFAILED+=1
		FAILED+=("$board $region $example")
		tail -40 /tmp/"$PNAME".$$.log
	fi
	rm -f /tmp/"$PNAME".$$.log
}

for board in "${BOARDS[@]}" ; do
	[[ $OPTREGIONS -eq 0 ]] || break
	[[ -z "$OPTBOARD" || "$OPTBOARD" == "$board" ]] || continue
	_build "$board" us915 catena_hello
	if [[ $STM32 -ne 0 ]]; then
		_build "$board" us915 catena_hello_lora
	fi
	if [[ "$board" == "$EXTRA_BOARD" ]]; then
		_build "$board" us915 catena_usercommand
		_build "$board" us915 catena_fsm
	fi
done

if [[ -z "$OPTBOARD" ]]; then
	for region in "${REGIONS[@]}" ; do
		_build "$REGION_BOARD" "$region" catena_hello_lora
	done
fi

echo "$PNAME: $NBUILT built, $NFAILED failed, $NSKIPPED skipped"
for f in "${FAILED[@]}" ; do
	echo "$PNAME: failed: $f"
done
[[ $NFAILED -eq 0 ]]
