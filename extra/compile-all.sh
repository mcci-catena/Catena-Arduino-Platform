#!/bin/bash

# use arduino-cli (must be in path) to build some number of targets

# halt on errors.
set -e

# setup PNAME
PNAME="$(basename "$0" .sh)"
readonly PNAME

# other globals
typeset -i OPTDEBUG=0
typeset -i OPTVERBOSE=0

# defaults
OPTFQBN_DEFAULT=mcci:stm32:mcci_catena_4610
readonly OPTFQBN_DEFAULT

OPTBUILD_DEFAULT="${TMP-/tmp}/$PNAME"
readonly OPTBUILD_DEFAULT

typeset -a OPTFQBN
typeset -a OPTSKETCHES
typeset -a OPTARDUINO_CLI_FLAGS

##############################################################################
# verbose output
##############################################################################

function _verbose {
	if [ "$OPTVERBOSE" -ne 0 ]; then
		echo "$PNAME:" "$@" 1>&2
	fi
}

##############################################################################
# debug output
##############################################################################

function _debug {
	if [ "$OPTDEBUG" -ne 0 ]; then
		echo "$@" 1>&2
	fi
}

##############################################################################
# error output
##############################################################################

#### _error: define a function that will echo an error message to STDERR.
#### using "$@" ensures proper handling of quoting.
function _error {
	echo "$@" 1>&2
}

#### _fatal: print an error message and then exit the script.
function _fatal {
	_error "$PNAME:" "$@" ; exit 1
}

##############################################################################
# help
##############################################################################

function _help {
    less <<.
${PNAME} runs one or more test builds using the arduino-cli tool.

Usage:
    ${PNAME} --[options] sketch1 sketch2 ...

Options:
    --clean does a clean prior to building.

    --verbose causes more info to be displayed.

    --fqbn={a b c ...} selects one or more fully-qualfied board names.
        The default is ${OPTFQBN_DEFAULT}. As usual, add BSP-specific
        options separated by commas.

    --debug turns on more debug output.

    --help prints this message.

    --warnings=X specifies the warning level for the build. If not
        specified or empty, no correspondign argument is supplied
        to arduino-cli.

    --output={path} selects the top level path for the build. Subdirs are
        used on a sketch/fqbn basis. Default is ${OPTBUILD_DEFAULT}.
.
}


function _check_arduino_cli {
        which arduino-cli >& /dev/null || return 1
        return 0
}

function _scanargs {
    OPTDEBUG=0
    OPTVERBOSE=0
    OPTFQBN=()
    OPTBUILD="$OPTBUILD_DEFAULT"
    OPTARDUINO_CLI_FLAGS=()
    OPTSKETCHES=()

    local IOPT=0
    typeset -i IOPT

    for opt in "$@"; do
        case "$opt" in
        "--clean" )
            OPTARDUINO_CLI_FLAGS+=( "--clean" )
            ;;
        "--verbose" )
            OPTVERBOSE=$((OPTVERBOSE + 1))
            if [[ $OPTVERBOSE -gt 1 ]]; then
                OPTARDUINO_CLI_FLAGS+=( "-v")
            fi
            ;;
        "--help" )
            _help
            exit 0
            ;;
        "--debug" )
            OPTDEBUG=1
            ;;
        "--fqbn="* )
            # shellcheck disable=SC2206
            OPTFQBN+=( ${opt#--fqbn=} )
            ;;
        "--output="* )
            OPTBUILD="${opt#--output=}"
            ;;
        "--warnings="* )
            OPTARDUINO_CLI_FLAGS+=( "--warnings" "${opt#--warnings=}" )
            ;;
        "--" )
            IOPT=$((IOPT + 1))
            break
            ;;
        "-"* )
            _fatal "Unrecognized option:" "$opt"
            ;;
        * )
            break
            ;;
        esac
        IOPT=$((IOPT + 1))
    done

    shift $(( IOPT ))
    OPTSKETCHES=( "$@" )

    if [[ "${#OPTFQBN[@]}" -eq 0 ]]; then
        _verbose "Using default FQBN: $OPTFQBN_DEFAULT"
        OPTFQBN=( "$OPTFQBN_DEFAULT" )
    fi

    _debug "OPTBUILD:" "$OPTBUILD"
    _debug "OPTFQBN:" "${OPTFQBN[@]}"
    _debug "OPTSKETCHES:" "${OPTSKETCHES[@]}"
    _debug "OPTARDUINO_CLI_FLAGS:" "${OPTARDUINO_CLI_FLAGS[@]}"

    if [[ "${#OPTSKETCHES[@]}" -eq 0 ]]; then
        _fatal "No sketches specified"
    fi
}

# compile. first arg is the script, second is FQBN, third is top of build tree, fourth: misc args
function _compile {
    local SKETCH="$1"
    local FQBN="$2"
    local FQBN_DIR="$(echo "$FQBN" | tr ':/\|&,' -)"
    local BUILDTREE="$3"
    shift 3

    _verbose "BUILD:" "$SKETCH" "$FQBN"

    _verbose arduino-cli compile -b "$FQBN" --build-path "$BUILDTREE/$FQBN_DIR/$(basename "$SKETCH" .ino)" "$@" "$SKETCH"
    arduino-cli compile -b "$FQBN" --build-path "$BUILDTREE/$FQBN_DIR/$(basename "$SKETCH" .ino)" "$@" "$SKETCH" || return 1
    return 0
}

# scan args, then do the work
function _main {
    _check_arduino_cli || _fatal "could not find arduino-cli on path"
    _scanargs "$@"
    local ERRORS
    typeset -a ERRORS
    ERRORS=()

    for ISKETCH in "${OPTSKETCHES[@]}"; do
        for IFQBN in "${OPTFQBN[@]}"; do
            if ! _compile "$ISKETCH" "$IFQBN" "$OPTBUILD" "${OPTARDUINO_CLI_FLAGS[@]}"; then
                ERRORS+=( "Failed: $ISKETCH $IFQBN" )
            fi
        done
    done

    if [[ "${#ERRORS[@]}" -ne 0 ]]; then
        printf "Errors:\n"
        printf "%s\n" "${ERRORS[@]}"
    else
        _verbose "No errors!"
    fi
}

_main "$@"
