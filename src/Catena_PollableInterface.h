/* Catena_PollableInterface.h	Sat Mar 18 2017 14:48:29 tmm */

/*

Module:  Catena_PollableInterface.h

Function:
	McciCatena::cPollableInterface

Version:
	V0.5.0	Sat Mar 18 2017 14:48:29 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sat Mar 18 2017 14:48:29  tmm
	Module created.

*/

#ifndef _CATENA_POLLABLEINTERFACE_H_		/* prevent multiple includes */
#define _CATENA_POLLABLEINTERFACE_H_

#pragma once

namespace McciCatena {

class cPollableInterface
	{
public:
	// empty virtual detructor
	virtual ~cPollableInterface() {};

	// override this, please:
	virtual void poll(void) = 0;
	};

class cPollingEngine; // forward reference

class cPollableObject : public cPollableInterface
	{
public:
        cPollableObject() {};

	// neither copyable nor movable
	cPollableObject(const cPollableObject&) = delete;
	cPollableObject& operator=(const cPollableObject&) = delete;
	cPollableObject(const cPollableObject&&) = delete;
	cPollableObject& operator=(const cPollableObject&&) = delete;

private:
        // forward link in polling list
	cPollableObject *m_pNext = nullptr;
        // reverse link in polling list
	cPollableObject *m_pLast = nullptr;

        // polling engine owns this list so can manipulate.
	friend class cPollingEngine;
	};

class cPollingEngine : public cPollableInterface
	{
public:
	cPollingEngine() {};
	virtual ~cPollingEngine() {};

	// neither copyable nor movable
	cPollingEngine(const cPollingEngine&) = delete;
	cPollingEngine& operator=(const cPollingEngine&) = delete;
	cPollingEngine(const cPollingEngine&&) = delete;
	cPollingEngine& operator=(const cPollingEngine&&) = delete;

        // prepare for operation.
	void begin() {};

        // poll this list represented by this engine.
	virtual void poll()
		{
		auto const pHead = this->m_pHead;
		if (pHead != nullptr)
			{
			auto pThis = pHead;
			do
				{
				pThis->poll();
				pThis = pThis->m_pNext;
				} while (pThis != pHead);
			}
		}

        // register an object to be polled in this list.
        void registerObject(cPollableObject *pObject)
                {
                auto const pHead = this->m_pHead;
                if (pHead == nullptr)
                        {
                        pObject->m_pNext = pObject;
                        pObject->m_pLast = pObject;
                        this->m_pHead = pObject;
                        }
                else
                        {
                        auto const pLast = pHead->m_pLast;
                        pObject->m_pNext = pHead;
                        pObject->m_pLast = pLast;
                        pLast->m_pNext = pObject;
                        pHead->m_pLast = pObject;
                        }
                }

private:
	cPollableObject *m_pHead = nullptr;
	};


}; // namespace McciCatena

/**** end of Catena_PollableInterface.h ****/
#endif /* _CATENA_POLLABLEINTERFACE_H_ */
