/*

Module:	Arduino_LoRaWAN.h

Function:
	Host-test stand-in for arduino-lorawan: only what
	Catena_FramStorage.cpp needs (the size of the session state).

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	October 2026

*/

#pragma once

#include <cstdint>

class Arduino_LoRaWAN
	{
public:
	struct SessionState
		{
		uint8_t	data[216];
		};
	};
