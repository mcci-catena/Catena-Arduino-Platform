/*

Module:  Catena_FramStorage.h

Function:
        class McciCatena::cFramStorage

Copyright notice:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	March 2017

*/

#ifndef _CATENA_FRAMSTORAGE_H_		/* prevent multiple includes */
#define _CATENA_FRAMSTORAGE_H_

#include <stdint.h>

#ifndef _MCCIADK_GUID_H_
# include "mcciadk_guid.h"
#endif

namespace McciCatena {

class cFramStorage
        {
public:
        // the standard objects= tags.
        enum StandardKeys : uint8_t
                {
                kHeader = 0,
                kSysEUI = 1,
                kPlatformGuid = 2,
                kDevEUI = 3,
                kAppEUI = 4,
                kDevAddr = 5,
                kJoin = 6,
                kLoRaClass = 7,
                kNwkSKey = 8,
                kAppSKey = 9,
                kFCntDown = 10,
                kFCntUp = 11,
                kNetID = 12,
                kAppKey = 13,
                kBootCount = 14,
                kOperatingFlags = 15,
                kBme680Cal = 16,
                kAppConf = 17,
                kLmicSessionState = 18,
                kUplinkInterval = 19,
                kDataLogHeader = 20,
                kDataLogBuffer = 21,
                // when you add something, also update McciCatena::cFramStorage::vItemDefs[]!
                kMAX
                };

        /* client should beware of MaxAppConfSize using the kAppConf key*/
        const static unsigned int MaxAppConfSize = 64;

        class StandardItem
                {
        private:
                static uint32_t constexpr makeValue(
                        uint8_t a_uKey,
                        uint16_t a_uSize,
                        bool a_fNumber,
                        bool a_fReplicated
                        )
                        {
                        return ((!a_fReplicated) << 25) | (a_fNumber << 24) | (a_uKey << 16) | a_uSize;
                        }
        public:
                StandardItem() : uValue(0) {};
                StandardItem(uint8_t a_uKey, uint16_t a_uSize, bool a_fNumber, bool a_fReplicated = true)
                        : uValue(makeValue(a_uKey, a_uSize, a_fNumber, a_fReplicated)) {};
                uint16_t getSize() const
                        {
                        return this->uValue & 0xFFFFu;
                        }
                StandardKeys getKey() const
                        {
                        return StandardKeys((this->uValue >> 16) & 0xFF);
                        }
                bool isNumber() const
                        {
                        return this->uValue & (1 << 24);
                        }
                bool isReplicated() const
                        {
                        return !(this->uValue & (1 << 25));
                        }
                const MCCIADK_GUID_WIRE *getGuid() const
                        {
                        return &skFramGuid;
                        }
        private:
                uint32_t uValue;
                };

        typedef uint32_t Offset;
        enum : Offset { kInvalidOffset = ~UINT32_C(0) };

        static const StandardItem vItemDefs[kMAX];

        class Object;           // forward reference.
        struct DataLogHeader_t; // forward reference
        struct DataLogBuffer_t; // forward referencce

        static constexpr uint16_t kDataLogBufferSize = 2048;

        enum : uint32_t
                {
                SIZE_MASK = 0xFFFFu,
                NONSTD_MASK = 0x80000000u,
                DATASIZE_MASK = 0x7FFF0000u,
                };

        struct ObjectRaw
                {
                // the size of the object, and the size of the data payload:
                uint32_t uSizeKey;

                // the GUID for this object.  The first object is always the
                // "header GUID"
                MCCIADK_GUID_WIRE	Guid;

                // a discriminator, allowing for 256 different subvalues
                uint8_t		uKey;

                // a trick used for 30 years for storing data reliably: three values
                // that (in this case) select one location or the other. Written to
                // the other value sequentially, and when read, if v1 == v2, we
                // believe it, otherwise we think that either v1 or v2 was corrupted
                // during write, and instead use v3.

                // the discriminator images.
                uint8_t		uVer[3];

                 bool isStandard() const
                        {
                        return (this->uSizeKey & NONSTD_MASK) == 0;
                        }

                Object *getStandardObject()
                        {
                        if (this->isStandard())
                                return (Object *)this;
                        else
                                return nullptr;
                        }
               };

        static MCCIADK_GUID_WIRE const skFramGuid;
        static constexpr size_t kObjectQuantum = sizeof(uint32_t);
        static constexpr size_t kObjectStandardClicks =
                                (sizeof(ObjectRaw) + kObjectQuantum - 1) /
                                        kObjectQuantum;
        static constexpr size_t kObjectStandardSize = kObjectQuantum * kObjectStandardClicks;

        static uint32_t constexpr getField(uint32_t v, uint32_t mask)
                {
                return (v & mask) / (mask & (~mask + 1u));
                }
        static uint32_t constexpr setField(
                uint32_t oldv, uint32_t fv, uint32_t mask
                )
                {
                return (oldv & ~mask) +
                       (fv * (mask & (~mask + 1u)));
                }

        // get the maximum value for a field
        static uint32_t constexpr getMaxValue(uint32_t mask)
                {
                return getField(mask, mask);
                }

        static uint32_t constexpr uSizeKey_GetSize(uint32_t uSizeKey)
                {
                return (uSizeKey & SIZE_MASK) * kObjectQuantum;
                }

        static size_t constexpr getClicks(size_t nBytes)
                {
                return (nBytes + kObjectQuantum - 1) / kObjectQuantum;
                }
        static constexpr uint32_t uSizeKey_SetClicks(
                        uint32_t uSizeKey, uint32_t v
                        )
                {
                return (1 <= v && v <= SIZE_MASK)
                        ? (uSizeKey & ~SIZE_MASK) | v
                        : 0;
                }
        static uint32_t constexpr uSizeKey_SetSize(
                uint32_t uSizeKey,
                size_t sizeInBytes
                )
                {
                return uSizeKey_SetClicks(
                        uSizeKey, getClicks(sizeInBytes)
                        );
                }
        // set the data key size, clamping at max value.
        static uint32_t constexpr uSizeKey_SetDataSize(
                size_t oldSizeKey,
                size_t nBytes
                )
                {
                return setField(
                        oldSizeKey,
                        nBytes <= getMaxValue(DATASIZE_MASK) ? nBytes
                                                             : getMaxValue(DATASIZE_MASK),
                        DATASIZE_MASK
                        );
                }
        // get the index to the data field given a replicant index
        static uint32_t constexpr dataOffset(
                size_t dataSize,
                bool dataReplicant
                )
                {
                return kObjectStandardSize + (dataReplicant ? getClicks(dataSize) * kObjectQuantum
                                                            : 0);
                }
        };


struct cFramStorage::Object : public cFramStorage::ObjectRaw
        {
public:

        uint16_t getObjectSize(void) const
                {
                return uSizeKey_GetSize(this->uSizeKey);
                }
        uint16_t nextObjectOffset(void) const
                {
                uint16_t size = uSizeKey_GetSize(this->uSizeKey);
                if (size == 0)
                        size = kObjectQuantum;
                return size;
                }
        uint16_t getObjectClicks(void) const
                {
                return this->uSizeKey & SIZE_MASK;
                }
        bool setObjectSize(uint32_t sizeInBytes)
                {
                uint32_t const uSizeKey = uSizeKey_SetSize(
                                                this->uSizeKey,
                                                sizeInBytes
                                                );
                if (uSizeKey != 0)
                        {
                        this->uSizeKey = uSizeKey;
                        return true;
                        }
                else
                        {
                        return false;
                        }
                }
        uint16_t hasValidSize() const
                {
                // standard object... check the data size
                return this->validDataSize(this->getDataSize());
                }
        uint16_t getDataSize() const
                {
                return (uint16_t) getField(this->uSizeKey, DATASIZE_MASK);
                }

        // compare this object's guid to another
        bool matchesGuid(const MCCIADK_GUID_WIRE &guid) const;

        int getKey() const
                {
                return this->uKey;
                }
        uint16_t static neededClicks(size_t nBytes, bool fReplicated)
                {
                size_t nClicks;

                nClicks = getClicks(nBytes);
                if (fReplicated)
                        nClicks *= 2;

                if (nClicks > SIZE_MASK - kObjectStandardClicks)
                        return 0;
                else
                        return (uint16_t) nClicks + kObjectStandardClicks;
                }

        bool validDataSize(size_t nBytes) const
                {
                uint16_t const n = neededClicks(nBytes, false);
                return (n <= this->getObjectClicks());
                }

        bool isReplicated() const
                {
                const uint16_t dataSize = this->getDataSize();
                const uint16_t dataClicks = (uint16_t) getClicks(dataSize);

                return (2 * dataClicks + kObjectStandardClicks <=
                                this->getObjectClicks());
                }
        unsigned nReplicants() const
                {
                const uint16_t dataSize = this->getDataSize();
                const uint16_t dataClicks = (uint16_t) getClicks(dataSize);

                if (dataClicks == 0)
                        return 0;

                return (this->getObjectClicks() - kObjectStandardClicks) /
                                dataClicks;
                }

        // offset to the i-th repliant from the base of this object.
        // zero means error.
        uint32_t offsetOfReplicant(unsigned i) const
                {
                const uint16_t dataSize = this->getDataSize();
                const uint16_t dataClicks = (uint16_t) getClicks(dataSize);

                return kObjectStandardSize + i * dataClicks * kObjectQuantum;
                }


        // get current discriminator; use voting algorigthm
        uint8_t getCurrent() const
                {
                uint8_t const v1 = this->uVer[0];

                if (v1 == this->uVer[1])
                        return v1;
                else
                        return this->uVer[2];
                }

        // set discriminator (only in local image, of cousre)
        void setCurrent(uint8_t v)
                {
                this->uVer[0] = v;
                this->uVer[1] = v;
                this->uVer[2] = v;
                }

        // get the pointer to the vector of discriminators
        uint8_t *getDiscriminatorBuffer()
                {
                return this->uVer;
                }

        // get the size of the vector of discriminators
        size_t constexpr getDiscriminatorBufferSize()
                {
                return sizeof(this->uVer);
                }

        // get the offset of the vector of discriminators
        static size_t constexpr offsetOfDiscriminator()
                {
                return offsetof(ObjectRaw, uVer);
                }

        // return the size of the object taken as a buffer
        static constexpr size_t getBufferSize()
                {
                return kObjectStandardSize;
                }

        uint8_t *getBuffer()
                {
                return (uint8_t *) &this->uSizeKey;
                }

        // methods that are not in-line

        // initialize a standard object image.
        bool initialize(
                const MCCIADK_GUID_WIRE &Guid,
                uint8_t uKey,
                size_t valueSizeInBytes,
                bool fReplicated
                );

        bool initialize(StandardItem);

        // check whether the object is formally valid
        bool isValid(void) const;
        };

///
/// @brief the header for the FRAM-based data log
///
/// @details
///     We support a limited data logging function, for storing a FIFO queue of fixed-size
///     records in a buffer in FRAM. This is implememnted with two FRAM objects. A single
///     DataLogBuffer_t is allocated storing the data elements; pointers are retained using
///     a separate cFramStorage::DataLogHeader_t object.
///
///     Unlike the buffer, the header is maintained using normal cFramStorage techniques;
///     it is therefore updated reliably. It contains the classic ring-buffer insert and
///     remove pointers. It also needs to remember the size of the buffer, so it knows
///     how to handle wrap-around. So that we can handle software updates sanely, it keeps
///     an idea of the size of the objects recorded and of the data format; if care is taken
///     to update the data format when things change, software should be able to recognize
///     a log buffer that it doesn't understand and take appropriate action.
///
///     The names of methods and type for the Data Logging Header are modeled on the names
///     used in the C++ standard header <dequeue>. We don't implement all the operations,
///     from <dequeue>, but since a ring buffer is fundamentally a dequeue,
///     it seems sane to follow that naming convention.
///
///     Bear in mind, however, that the methods of the DataLogHeader_t do not move data
///     to the FRAM; there is a wrapper driver that takes care of this. The code here
///     manipulates the indices but another layer needs to carefully stage the writing
///     of the ring buffer headers, and stage the writing of the header vs the data
///     at the appropriate moments.
///

struct cFramStorage::DataLogHeader_t
        {
public:
        using size_type = uint16_t;

        enum class OverflowPolicy_t : uint8_t
                {
                kDropOldest = 0,
                kDropNew = 1,
                };

        ///
        /// @brief check whether the structure read from FRAM is consistent with current software
        /// @param version      [in] the version tag for the data log buffer used by the current software.
        /// @param buffersize   [in] the size of the log buffer desired by the current software.
        /// @param itemsize     [in] the size of each item in the buffer
        /// @return \c true if things look ok, \c false if things look out of sync.
        ///
        /// \c version only needs to change if the data format changes but the size of an item doesn't change.
        /// \c buffersize is normally read from the buffer object; the software's idea of the size should
        /// only be used when creating the buffer object.
        ///
        bool queryVersionMatch(uint8_t version, uint16_t buffersize, uint16_t itemsize) const
                {
                return version == this->m_version &&
                       buffersize == this->m_buffersize &&
                       itemsize == this->m_itemsize
                       ;
                }

        /// @brief calculate the maximum capacity of the buffer
        /// @return number of elements that will fit.
        ///
        /// @details If itemsize is zero, the result will be zero.
        ///
        /// @note The name is based on the similar function in <dequeue>.
        /// \c this->max_size() is also the largest permitted index value
        /// in the buffer. It's one less than the allocation size of the
        /// buffer (in units of itemsize)
        constexpr size_type max_size() const
                {
                return (this->m_itemsize == 0)                  ? 0 :
                       (this->m_buffersize < this->m_itemsize)  ? 0 :
                                                                  (this->m_buffersize / this->m_itemsize - 1);
                }

        /// @brief check whether the ring-buffer control structure is self-consistent.
        /// @return true if things look reasonable.
        bool queryConsistent() const
                {
                // basic checks
                if (this->m_itemsize == 0)
                        return false;

                uint16_t const nItems = this->m_buffersize / this->m_itemsize;
                if (nItems == 0)
                        return false;

                if (this->m_insert >= nItems || this->m_remove >= nItems)
                        return false;

                return true;
                }

        /// @brief empty the ring buffer header
        ///
        /// @details
        ///     The reset pointer is advanced to the head.
        ///
        /// @note The name is based on the similar function in <dequeue>.
        void clear()
                {
                if (this->m_insert < this->max_size())
                        this->m_remove = this->m_insert;
                else
                        this->m_remove = this->m_insert = 0;
                }

        /// @brief return the number of elements currently in the buffer
        /// @return the count, in 0 .. max_size().
        constexpr size_type size() const
                {
                if (this->m_remove <= this->m_insert)
                        return this->m_insert - this->m_remove;
                else
                        return this->m_insert + this->max_size() + 1 - this->m_remove;
                }

        bool push_back(size_type &insertIndex)
                {
                insertIndex = this->m_insert;

                return this->push_back();
                }

        bool push_back(void)
                {
                if (this->size() == this->max_size())
                        {
                        ++this->m_nDropped;
                        return false;
                        }

                if (this->m_insert == this->max_size())
                        this->m_insert = 0;
                else
                        ++this->m_insert;
                return true;
                }

        bool pop_front(size_type &removeIndex)
                {
                removeIndex = this->m_remove;
                return this->pop_front();
                }

        bool pop_front()
                {
                if (this->size() == 0)
                        return false;
                else
                        {
                        if (this->m_remove == this->max_size())
                                this->m_remove = 0;
                        else
                                ++this->m_remove;

                        return true;
                        }
                }

        constexpr size_type indexToOffset(size_type bufferIndex) const
                {
                return bufferIndex * this->m_itemsize;
                }

private:
        size_type       m_insert;
        size_type       m_remove;
        size_type       m_buffersize;
        size_type       m_itemsize;
        uint16_t        m_nDropped;
        uint8_t         m_version;
        OverflowPolicy_t m_overflowPolicy;
        };

struct cFramStorage::DataLogBuffer_t
        {
        uint8_t         m_buffer[kDataLogBufferSize];
        };

}; // namespace McciCatena

/**** end of Catena_FramStorage.h ****/
#endif /* _CATENA_FRAMSTORAGE_H_ */
