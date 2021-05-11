/*

Module:  Catena4420.h

Function:
	class Catena4420 (Aduino header file)

Copyright notice:
	See LICENSE file accompanying this project.

Author:
	Terry Moore, MCCI Corporation   December 2016

*/

#ifndef _CATENA4420_H_		/* prevent multiple includes */
#define _CATENA4420_H_

#ifndef _CATENAFEATHERM0_H_
# include "CatenaFeatherM0.h"
#endif

namespace McciCatena {

class Catena4420 : public CatenaFeatherM0
	{
public:
	/*
	|| Methods
	*/
	Catena4420();

	/*
	|| LoRaWAN binding
	*/
	class LoRaWAN /* forward */;

	// Sigfox binding
	class Sigfox /* forward */;

	virtual const char *CatenaName() const override { return "Catena 4420"; };

protected:
	using Super = CatenaFeatherM0;
        virtual const Arduino_LoRaWAN::ProvisioningTable *
        	GetLoRaWANkeys(void) const override;

	virtual void getCpuIdPlatformTable(
		const CPUID_PLATFORM_MAP * &vCpuIdToPlatform,
		size_t &nvCpuIdToPlatform
		) override;

private:
        static const Arduino_LoRaWAN::ProvisioningTable gk_LoRaWAN_Keys
                /* __attribute__((__weak__)) */;

	static const CPUID_PLATFORM_MAP vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	};

/*
|| The LoRaWAN class for the Catena4420. For now, we assume The Things
|| Network.
*/
class Catena4420::LoRaWAN : public CatenaFeatherM0::LoRaWAN
	{
public:
        using Super = CatenaFeatherM0::LoRaWAN;

	/*
	|| the constructor.
	*/
	LoRaWAN() {};

protected:
	/*
	|| we use the CatenaFeatherM0 defaults
	*/

private:
	};

/*
|| The Sigfox class for the Catena4420.
*/
class Catena4420::Sigfox : public CatenaFeatherM0::Sigfox
	{
public:
        using Super = CatenaFeatherM0::Sigfox;

	/*
	|| the constructor.
	*/
	Sigfox() {};

protected:
	/*
	|| we use the CatenaFeatherM0 defaults
	*/

private:
	};

} /* namespace McciCatena */


/**** end of Catena4420.h ****/
#endif /* _CATENA4420_H_ */
