/**
 * @file fifo.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-14
 * @version 2022-02-08
 * 
 * @warning This file is for internal use only. The content is subject to change at any time.
 * @internal Macros to handle embedded FIFOs via circular buffers. Define a macro with
 * the argument list as handle for your convenience when using this.
 * @remarks The handle needs to be given such an argument list: buf, size, head, tail
 * @remarks This file requires C++11 or newer.
 * @remarks Here head is the next write position, whereas tail marks the next read position.
 * This approach limits the usable buffer capacity to (size - 1) but ensures proper states
 * in concurrent situations as write and read functions only access disjoint variables.
 */
#ifndef __SCDINTERNAL_FIFO_H__
#define __SCDINTERNAL_FIFO_H__

#include <stdint.h>
#include <string.h>


/**
 * Helper function which acts like static_cast<> but suppresses the
 * "type qualifiers ignored on function return type" warning.
 * 
 * @param[in] val - value to cast
 * @return casted value
 * @tparam R - desired result type
 * @tparam T - input type
 */
template <typename R, typename T>
__attribute__((always_inline)) static inline R _FIFO_CAST(const T val) {
	return static_cast<R>(val);
}


/* helper macro to expand the passed argument list */
#define _FIFOX_CALL(fn, ...)      fn(__VA_ARGS__)
/* initializes the index variables head and tail */
#define _FIFOX_INIT(handle)       _FIFOX_CALL(_FIFO_INIT, handle)
/* resets the index variables to clear the buffer */
#define _FIFOX_CLEAR(handle)      _FIFOX_CALL(_FIFO_CLEAR, handle)
/* adds a new element to the end of the circular buffer or returns false if the buffer was full */
#define _FIFOX_PUSH(handle, val)  _FIFOX_CALL(_FIFO_PUSH, handle, val)
/* infinitely tries to add a new element to the end of the circular buffer; an interrupt handle may free up space in the mean time */
#define _FIFOX_WPUSH(handle, val) _FIFOX_CALL(_FIFO_WPUSH, handle, val)
/* returns the first element of the circular buffer without removing it or -1 if the buffer was empty */
#define _FIFOX_PEEK(handle)       _FIFOX_CALL(_FIFO_PEEK, handle)
/* returns the first element of the circular buffer and removes it or -1 if the buffer was empty */
#define _FIFOX_POP(handle)        _FIFOX_CALL(_FIFO_POP, handle)
/* returns true if the buffer is empty, else false */
#define _FIFOX_EMPTY(handle)      _FIFOX_CALL(_FIFO_EMPTY, handle)
/* returns true if the buffer is full, else false */
#define _FIFOX_FULL(handle)       _FIFOX_CALL(_FIFO_FULL, handle)
/* returns to maximum capacity of the buffer */
#define _FIFOX_CAPACITY(handle)   _FIFOX_CALL(_FIFO_CAPACITY, handle)
/* returns the number of available free elements */
#define _FIFOX_AVAILABLE(handle)  _FIFOX_CALL(_FIFO_AVAILABLE, handle)
/* returns the number of used elements */
#define _FIFOX_SIZE(handle)       _FIFOX_CALL(_FIFO_SIZE, handle)
/* returns the number of elements available and accessible as block */
#define _FIFOX_BSIZE(handle)      _FIFOX_CALL(_FIFO_BSIZE, handle)
/* returns the real buffer index calculated from the given offset */
#define _FIFOX_INDEX(handle, idx) _FIFOX_CALL(_FIFO_INDEX, handle, idx)


#define _FIFO_INIT(buf, size, head, tail) do { \
		(head) = 0; \
		(tail) = 0; \
	} while ( false )

#define _FIFO_CLEAR(buf, size, head, tail) _FIFO_INIT((buf), (size), (head), (tail))

#define _FIFO_PUSH(buf, size, head, tail, val) [&]() -> bool { \
		const __typeof__((head)) __fifo_next = _FIFO_CAST<__typeof__((head))>(((head) + 1) % (size)); \
		if (__fifo_next == (tail)) return false; \
		(buf)[(head)] = val; \
		(head) = __fifo_next; \
		return true; \
	}()

#define _FIFO_WPUSH(buf, size, head, tail, val) do { \
		const __typeof__((head)) __fifo_next = _FIFO_CAST<__typeof__((head))>(((head) + 1) % (size)); \
		while (__fifo_next == (tail)) __WFI(); \
		(buf)[(head)] = val; \
		(head) = __fifo_next; \
	} while ( false )

#define _FIFO_PEEK(buf, size, head, tail) [&]() -> int { \
		if ((head) == (tail)) return -1; \
		return (buf)[(tail)]; \
	}()

#define _FIFO_POP(buf, size, head, tail) [&]() -> int { \
		if ((head) == (tail)) return -1; \
		const int __fifo_res = (buf)[(tail)]; \
		tail = _FIFO_CAST<__typeof__((tail))>(((tail) + 1) % (size)); \
		return __fifo_res; \
	}()

#define _FIFO_EMPTY(buf, size, head, tail) \
	((head) == (tail))

#define _FIFO_FULL(buf, size, head, tail) \
	((((head) + 1) % (size)) == (tail))

#define _FIFO_CAPACITY(buf, size, head, tail) ((size) - 1)

#define _FIFO_AVAILABLE(buf, size, head, tail) \
	(((head) >= (tail)) ? \
		size_t((size) + (tail) - (head) - 1) : \
		size_t((tail) - (head) - 1))

#define _FIFO_SIZE(buf, size, head, tail) \
	(((head) >= (tail)) ? \
		size_t((head) - (tail)) : \
		size_t((size) + (head) - (tail)))

#define _FIFO_BSIZE(buf, size, head, tail) \
	size_t(((head) < (tail)) ? (size) - (tail) : (head) - (tail))

#define _FIFO_INDEX(buf, size, head, tail, idx) \
	_FIFO_CAST<__typeof__((tail))>(((tail) + (idx)) % (size))


/** Same as std::conditional. */
template <bool B, typename T, typename F>  struct _FifoConditional { typedef T type; };
template <typename T, typename F> struct _FifoConditional<false, T, F> { typedef F type; };


/** Gives the smallest unsigned integer type which can hold the given value. */
template <uint32_t TSize>
struct _FifoIndexTypeFor {
	typedef typename _FifoConditional<
		(TSize > uint32_t(0xFFFF)),
		uint32_t,
		typename _FifoConditional<
			(TSize > uint32_t(0xFF)),
			uint16_t,
			uint8_t
		>::type
	>::type type;
};


/**
 * Implements a FIFO of the given size. The FIFO is made up of uint8_t elements.
 * No data is overwritten. The corresponding functions will fail if the maximum
 * capacity was reached.
 * 
 * @tparam TSize - FIFO size in bytes
 * @remarks This class is optimized for speed and little RAM usage in contrast to
 * other implementations that minimize flash usage.
 * @remarks Designed for single producer, single consumer.
 */
template <uint32_t TSize>
struct _FifoClass {
	/** Index type. */
	typedef typename _FifoIndexTypeFor<TSize>::type SizeType;
#define _FIFO_HANDLE this->buffer, Size, this->head, this->tail
	enum {
		/** FIFO size in memory. */
		Size = TSize,
		/** Half of the FIFO size in memory clamped at lower boundary to 1. */
		HalfSize = ((TSize / 2) > 0) ? (TSize / 2) : 1,
		/** FIFO capacity, e.i. the number of bytes the FIFO can hold at maximum. */
		Capacity = _FIFOX_CAPACITY(_FIFO_HANDLE)
	};
	volatile SizeType head; /**< Next write position within buffer. */
	volatile SizeType tail; /**< Next read position within buffer. */
	uint8_t buffer[TSize]; /**< Circular buffer of the FIFO. */
	
	/** Constructor. */
	explicit _FifoClass():
		head(0),
		tail(0)
	{}
	
	/**
	 * Returns whether the FIFO is empty.
	 * 
	 * @return true if empty, else false
	 */
	bool empty() const {
		return _FIFOX_EMPTY(_FIFO_HANDLE);
	}
	
	/**
	 * Returns whether the FIFO is full.
	 * 
	 * @return true if full, else false
	 */
	bool full() const {
		return _FIFOX_FULL(_FIFO_HANDLE);
	}
	
	/**
	 * Clears the FIFO by resetting the index variables.
	 */
	void clear() {
		_FIFOX_CLEAR(_FIFO_HANDLE);
	}
	
	/**
	 * Returns the calculated buffer index of the given
	 * element position.
	 * 
	 * @param[in] pos - position, whereas 0 is always the first readable element
	 * @return corresponding index in buffer
	 */
	uint32_t index(const uint32_t pos) const {
		return _FIFOX_INDEX(_FIFO_HANDLE, pos);
	}
	
	/**
	 * Adds a new element to the FIFO.
	 * 
	 * @param[in] val - value to add
	 * @return true on success, else false
	 */
	bool push(const uint8_t val) {
		return _FIFOX_PUSH(_FIFO_HANDLE, val);
	}
	
	/**
	 * Blocking adds a new element to the FIFO.
	 * 
	 * @param[in] val - value to add
	 */
	void blockingPush(const uint8_t val) {
		_FIFOX_WPUSH(_FIFO_HANDLE, val);
	}
	
	/**
	 * Returns the next element from the FIFO
	 * and removes it from there.
	 * 
	 * @return value if available, else -1
	 */
	int pop() {
		return _FIFOX_POP(_FIFO_HANDLE);
	}
	
	/**
	 * Returns the next element from the FIFO.
	 * The value remains in the FIFO.
	 * 
	 * @return value if available, else -1
	 */
	int peek() {
		return _FIFOX_PEEK(_FIFO_HANDLE);
	}
	
	/**
	 * Writes a block of data to the FIFO.
	 * 
	 * @param[in] buf - input data buffer
	 * @param[in] len - length of the data
	 * @return number of bytes written
	 */
	uint32_t write(const uint8_t * buf, const uint32_t len) {
		uint32_t res = 0;
#ifdef STM32CUBEDUINO_SMALL_FLASH
		for (; res < len && this->push(*buf); buf++, res++);
#else
		const SizeType curTail = this->tail;
		SizeType curHead = this->head;
		SizeType nextHead;
		/* copy until buffer end */
		if (curTail <= curHead) {
			const SizeType maxSize = SizeType((curTail == 0) ? Size - 1 : Size);
			const SizeType endHead = SizeType(maxSize > (curHead + len) ? curHead + len : maxSize);
			const SizeType cpyLen = SizeType(endHead - curHead);
			memcpy(this->buffer + curHead, buf, cpyLen);
			buf += cpyLen;
			res = uint32_t(res + cpyLen);
			curHead = SizeType(curHead + cpyLen) % Size;
			this->head = curHead;
			nextHead = SizeType((curHead + 1) % Size);
			if (res >= len || nextHead == curTail) return res;
		} else {
			nextHead = SizeType((curHead + 1) % Size);
		}
		/* copy until full */
		const SizeType endNextHead = SizeType(((nextHead + len - res) >= curTail) ? curTail : nextHead + len - res);
		const SizeType cpyLen = SizeType(endNextHead - nextHead);
		memcpy(this->buffer + curHead, buf, cpyLen);
		this->head = SizeType(curHead + cpyLen);
		res = uint32_t(res + cpyLen);
#endif
		return res;
	}
	
	/**
	 * Blocking writes a block of data to the FIFO.
	 * 
	 * @param[in] buf - input data buffer
	 * @param[in] len - length of the data
	 */
	void blockingWrite(const uint8_t * buf, const uint32_t len) {
		for (uint32_t i = 0; i < len; buf++, i++) this->blockingPush(*buf);
	}
	
	/**
	 * Writes the given amount dummy data to the FIFO.
	 * 
	 * @param[in] len - bytes to write
	 * @return bytes written
	 */
	uint32_t dummyWrite(const uint32_t len) {
		const uint32_t available = this->availableForWrite();
		const uint32_t skipping = (len <= available) ? len : available;
		this->head = static_cast<SizeType>((this->head + skipping) % TSize);
		return skipping;
	}
	
	/**
	 * Reads a block of data from the FIFO
	 * and removes the data from it.
	 * 
	 * @param[out] buf - output data buffer 
	 * @param[in] len - length of the data buffer
	 * @return number of bytes copied
	 */
	uint32_t read(uint8_t * buf, const uint32_t len) {
		uint32_t res = 0;
		for (int val; res < len && (val = this->pop()) >= 0; buf++, res++) {
			*buf = uint8_t(val);
		}
		return res;
	}
	
	/**
	 * Discards the given number of bytes stored in the FIFO.
	 * 
	 * @param[in] len - number of bytes to skip
	 * @return number of bytes skipped
	 */
	uint32_t skip(const uint32_t len) {
		const uint32_t available = this->availableForRead();
		const uint32_t skipping = (len <= available) ? len : available;
		this->tail = _FIFOX_INDEX(_FIFO_HANDLE, skipping);
		return skipping;
	}
	
	/**
	 * Number of bytes available for instant reading.
	 * 
	 * @return number of bytes that can be instantly read
	 */
	uint32_t availableForRead() const {
		return uint32_t(_FIFOX_SIZE(_FIFO_HANDLE));
	}
	
	/**
	 * Number of bytes available for instant block read.
	 * This is convenient for function that do not support circular buffers
	 * but only regular buffers. Use tail as start position within buffer.
	 * 
	 * @return number of bytes that can be instantly read as block
	 */
	uint32_t availableForBlockRead() const {
		return uint32_t(_FIFOX_BSIZE(_FIFO_HANDLE));
	}
	
	/**
	 * Number of bytes available for instant writing.
	 * 
	 * @return number of bytes that can be instantly written
	 */
	uint32_t availableForWrite() const {
		return uint32_t(_FIFOX_AVAILABLE(_FIFO_HANDLE));
	}
#undef _FIFO_HANDLE
};


/**
 * Implements a FIFO of any size. The FIFO is made up of uint8_t elements.
 * No data is overwritten. The corresponding functions will fail if the maximum
 * capacity was reached. This class provides the base functions but no
 * storage container.
 * 
 * @remarks This class is optimized for flash size.
 * @remarks Designed for single producer, single consumer.
 */
struct _DynFifoBase {
	/** Index type. */
	typedef uint32_t SizeType;
#define _FIFO_HANDLE reinterpret_cast<uint8_t *>(this + 1), this->size, this->head, this->tail
#define _CONST_FIFO_HANDLE reinterpret_cast<const uint8_t *>(this + 1), this->size, this->head, this->tail
	const SizeType size; /**< Size of the FIFO. */
	volatile SizeType head; /**< Next write position within buffer. */
	volatile SizeType tail; /**< Next read position within buffer. */
	/**< Circular buffer of the FIFO is located here and exists in the derived class. */
	
	/**
	 * Constructor
	 * 
	 * @param[in] elements - number of elements in the FIFO
	 */
	explicit _DynFifoBase(const SizeType elements):
		size(elements),
		head(0),
		tail(0)
	{}
	
	/**
	 * Returns whether the FIFO is empty.
	 * 
	 * @return true if empty, else false
	 */
	bool empty() const {
		return _FIFOX_EMPTY(_CONST_FIFO_HANDLE);
	}
	
	/**
	 * Returns whether the FIFO is full.
	 * 
	 * @return true if full, else false
	 */
	bool full() const {
		return _FIFOX_FULL(_CONST_FIFO_HANDLE);
	}
	
	/**
	 * Clears the FIFO by resetting the index variables.
	 */
	void clear() {
		_FIFOX_CLEAR(_FIFO_HANDLE);
	}
	
	/**
	 * Returns the calculated buffer index of the given
	 * element position.
	 * 
	 * @param[in] pos - position, whereas 0 is always the first readable element
	 * @return corresponding index in buffer
	 */
	uint32_t index(const uint32_t pos) const {
		return _FIFOX_INDEX(_CONST_FIFO_HANDLE, pos);
	}
	
	/**
	 * Adds a new element to the FIFO.
	 * 
	 * @param[in] val - value to add
	 * @return true on success, else false
	 */
	bool push(const uint8_t val) {
		return _FIFOX_PUSH(_FIFO_HANDLE, val);
	}
	
	/**
	 * Blocking adds a new element to the FIFO.
	 * 
	 * @param[in] val - value to add
	 */
	void blockingPush(const uint8_t val) {
		_FIFOX_WPUSH(_FIFO_HANDLE, val);
	}
	
	/**
	 * Returns the next element from the FIFO
	 * and removes it from there.
	 * 
	 * @return value if available, else -1
	 */
	int pop() {
		return _FIFOX_POP(_FIFO_HANDLE);
	}
	
	/**
	 * Returns the next element from the FIFO.
	 * The value remains in the FIFO.
	 * 
	 * @return value if available, else -1
	 */
	int peek() {
		return _FIFOX_PEEK(_FIFO_HANDLE);
	}
	
	/**
	 * Writes a block of data to the FIFO.
	 * 
	 * @param[in] buf - input data buffer
	 * @param[in] len - length of the data
	 * @return number of bytes written
	 */
	uint32_t write(const uint8_t * buf, const uint32_t len) {
		uint32_t res = 0;
		for (; res < len && this->push(*buf); buf++, res++);
		return res;
	}
	
	/**
	 * Blocking writes a block of data to the FIFO.
	 * 
	 * @param[in] buf - input data buffer
	 * @param[in] len - length of the data
	 */
	void blockingWrite(const uint8_t * buf, const uint32_t len) {
		for (uint32_t i = 0; i < len; buf++, i++) this->blockingPush(*buf);
	}
	
	/**
	 * Writes the given amount dummy data to the FIFO.
	 * 
	 * @param[in] len - bytes to write
	 * @return bytes written
	 */
	uint32_t dummyWrite(const uint32_t len) {
		const uint32_t available = this->availableForWrite();
		const uint32_t skipping = (len <= available) ? len : available;
		this->head = static_cast<SizeType>((this->head + skipping) % this->size);
		return skipping;
	}
	
	/**
	 * Reads a block of data from the FIFO
	 * and removes the data from it.
	 * 
	 * @param[out] buf - output data buffer 
	 * @param[in] len - length of the data buffer
	 * @return number of bytes copied
	 */
	uint32_t read(uint8_t * buf, const uint32_t len) {
		uint32_t res = 0;
		for (int val; res < len && (val = this->pop()) >= 0; buf++, res++) {
			*buf = uint8_t(val);
		}
		return res;
	}
	
	/**
	 * Discards the given number of bytes stored in the FIFO.
	 * 
	 * @param[in] len - number of bytes to skip
	 * @return number of bytes skipped
	 */
	uint32_t skip(const uint32_t len) {
		const uint32_t available = this->availableForRead();
		const uint32_t skipping = (len <= available) ? len : available;
		this->tail = _FIFOX_INDEX(_FIFO_HANDLE, skipping);
		return skipping;
	}
	
	/**
	 * Number of bytes available for instant reading.
	 * 
	 * @return number of bytes that can be instantly read
	 */
	uint32_t availableForRead() const {
		return uint32_t(_FIFOX_SIZE(_CONST_FIFO_HANDLE));
	}
	
	/**
	 * Number of bytes available for instant block read.
	 * This is convenient for function that do not support circular buffers
	 * but only regular buffers. Use tail as start position within buffer.
	 * 
	 * @return number of bytes that can be instantly read as block
	 */
	uint32_t availableForBlockRead() const {
		return uint32_t(_FIFOX_BSIZE(_CONST_FIFO_HANDLE));
	}
	
	/**
	 * Number of bytes available for instant writing.
	 * 
	 * @return number of bytes that can be instantly written
	 */
	uint32_t availableForWrite() const {
		return uint32_t(_FIFOX_AVAILABLE(_CONST_FIFO_HANDLE));
	}
#undef _FIFO_HANDLE
#undef _CONST_FIFO_HANDLE
};

/**
 * Implements a FIFO of any size. The FIFO is made up of uint8_t elements.
 * No data is overwritten. The corresponding functions will fail if the maximum
 * capacity was reached. This class extends the base functions with a storage
 * container of the given size.
 * 
 * @remarks This class is optimized for flash size.
 * @remarks Designed for single producer, single consumer.
 */
template <uint32_t TSize>
struct _DynFifoClass : public _DynFifoBase {
	/** Index type. */
	typedef _DynFifoBase::SizeType SizeType;
	
	uint8_t buffer[TSize];
	
	explicit _DynFifoClass():
		_DynFifoBase(TSize)
	{}
	
	using _DynFifoBase::empty;
	using _DynFifoBase::full;
	using _DynFifoBase::clear;
	using _DynFifoBase::index;
	using _DynFifoBase::push;
	using _DynFifoBase::blockingPush;
	using _DynFifoBase::pop;
	using _DynFifoBase::peek;
	using _DynFifoBase::write;
	using _DynFifoBase::blockingWrite;
	using _DynFifoBase::dummyWrite;
	using _DynFifoBase::read;
	using _DynFifoBase::skip;
	using _DynFifoBase::availableForRead;
	using _DynFifoBase::availableForBlockRead;
	using _DynFifoBase::availableForWrite;
};


/**
 * Implements a FIFO of the given size. The FIFO is made up of blocks with uint8_t elements.
 * No data is overwritten. The corresponding functions will fail if the maximum capacity was
 * reached.
 * 
 * @tparam TSize - total FIFO size in bytes
 * @tparam BSize - FIFO block size in bytes
 * @remarks This class is optimized for speed and little RAM usage in contrast to
 * other implementations that minimize flash usage.
 * @remarks Designed for single producer, single consumer.
 */
template <uint32_t TSize, uint32_t BSize>
struct _BlockFifoClass {
	/** Index type. */
	typedef typename _FifoIndexTypeFor<TSize / BSize>::type IndexType;
	/** Block size type. */
	typedef typename _FifoIndexTypeFor<BSize>::type SizeType;
	enum {
		/** total FIFO size in memory. */
		TotalSize = TSize,
		/** Half of the total FIFO size in memory. */
		HalfTotalSize = TSize / 2,
		/** FIFO block size */
		BlockSize = BSize,
		/** Number of blocks. */
		Count = TSize / BSize,
		/** Half of the total block count. */
		HalfCount = Count / 2
	};
#define _FIFO_HANDLE this->size, Count, this->head, this->tail
	volatile IndexType head; /**< Next write block within block. Written after isFull and size. */
	volatile IndexType tail; /**< Next read block within block. */
	SizeType size[Count]; /**< Block lengths. */
	uint8_t block[Count][BlockSize]; /**< Circular buffer of the FIFO. */
	
	/** Constructor. */
	explicit _BlockFifoClass() {
		static_assert((Count * BlockSize) == TotalSize, "Total size needs to be an integer multiple of block size.");
		static_assert(Count >= 2, "At least 2 blocks are required.");
		this->clear();
	}
	
	/**
	 * Returns whether the FIFO is empty in terms of available blocks.
	 * 
	 * @return true if empty, else false
	 */
	bool empty() const {
		return _FIFOX_EMPTY(_FIFO_HANDLE);
	}
	
	/**
	 * Returns whether the FIFO is empty in terms of available blocks
	 * and still uncommitted block.
	 * 
	 * @return true if empty, else false
	 */
	bool totallyEmpty() const {
		const IndexType curHead = this->head;
		if (curHead != this->tail) return false; /* blocks available */
		return this->size[curHead] == 0; /* uncommitted data available? */
	}
	
	/**
	 * Returns whether the FIFO is full in terms of available blocks.
	 * 
	 * @return true if full, else false
	 */
	bool full() const {
		return _FIFOX_FULL(_FIFO_HANDLE);
	}
	
	/**
	 * Clears the FIFO by resetting the index and block length variables.
	 */
	void clear() {
		_FIFOX_CLEAR(_FIFO_HANDLE);
		for (IndexType i = 0; i < Count; i++) this->size[i] = 0;
	}
	
	/**
	 * Returns the calculated block index
	 * of the given position.
	 * 
	 * @param[in] pos - position, whereas 0 is always the first readable element
	 * @return corresponding block index
	 */
	uint32_t index(const uint32_t pos) const {
		return _FIFOX_INDEX(_FIFO_HANDLE, pos);
	}
	
	/**
	 * Adds a new element to the FIFO.
	 * 
	 * @param[in] val - value to add
	 * @return true on success, else false
	 */
	bool push(const uint8_t val) {
		const IndexType curHead = this->head;
		const IndexType nextHead = IndexType((curHead + 1) % Count);
		const SizeType bSize = this->size[curHead];
		const SizeType bSizeNext = SizeType(bSize + 1);
		if (nextHead == this->tail && bSizeNext >= BlockSize) return false; /* full */
		this->block[curHead][bSize] = val;
		this->size[curHead] = bSizeNext;
		if (bSizeNext >= BlockSize) {
			this->head = nextHead;
		}
		return true;
	}
	
	/**
	 * Blocking adds a new element to the FIFO.
	 * 
	 * @param[in] val - value to add
	 */
	void blockingPush(const uint8_t val) {
		const IndexType curHead = this->head;
		const IndexType nextHead = IndexType((curHead + 1) % Count);
		const SizeType cbSize = this->size[curHead];
		if (nextHead == this->tail && (cbSize + 1) >= BlockSize) {
			/* wait until write space is available */
			while (nextHead == this->tail) __WFI();
		}
		const SizeType bSize = this->size[curHead];
		const SizeType bSizeNext = SizeType(bSize + 1);
		this->block[curHead][bSize] = val;
		this->size[curHead] = bSizeNext;
		if (bSizeNext >= BlockSize) {
			this->head = nextHead;
		}
	}
	
	/**
	 * Removes a block from the FIFO.
	 * 
	 * @return true on success, else false
	 */
	bool pop() {
		const IndexType curTail = this->tail;
		if (this->head == curTail) return false; /* empty */
		const IndexType nextTail = IndexType((curTail + 1) % Count);
		this->size[curTail] = 0;
		this->tail = nextTail;
		return true;
	}
	
	/**
	 * Returns the next block from the FIFO.
	 * The block remains in the FIFO.
	 * 
	 * @param[out] outLen - sets the length of the block on success
	 * @return block pointer on success, else NULL
	 */
	const uint8_t * peek(uint32_t & outLen) const {
		const IndexType curTail = this->tail;
		if (this->head == curTail) return NULL; /* empty */
		outLen = uint32_t(this->size[curTail]);
		return this->block[curTail];
	}
	
	/**
	 * Writes some data to the FIFO.
	 * 
	 * @param[in] buf - input data buffer
	 * @param[in] len - length of the data
	 * @return number of bytes written
	 */
	uint32_t write(const uint8_t * buf, const uint32_t len) {
		uint32_t res = 0;
		for (; res < len && this->push(*buf); buf++, res++);
		return res;
	}
	
	/**
	 * Blocking writes some data to the FIFO.
	 * 
	 * @param[in] buf - input data buffer
	 * @param[in] len - length of the data
	 */
	void blockingWrite(const uint8_t * buf, const uint32_t len) {
		for (uint32_t i = 0; i < len; buf++, i++) this->blockingPush(*buf);
	}
	
	/**
	 * Marks the current block complete and moves write
	 * pointers to the next block.
	 * 
	 * @return true on success, else false
	 */
	bool commitBlock() {
		const IndexType curHead = this->head;
		const IndexType nextHead = IndexType((curHead + 1) % Count);
		if (nextHead == this->tail) return false; /* no free block available */
		if (this->size[curHead] == 0) return false; /* empty block */
		this->head = nextHead;
		return true;
	}
	
	/**
	 * Reads some data from the FIFO
	 * and removes the data from it.
	 * 
	 * @param[out] buf - output data buffer 
	 * @param[in] len - length of the data buffer
	 * @return number of bytes copied
	 * @remarks The passed buffer should be able to hold at least one block.
	 */
	uint32_t read(uint8_t * buf, const uint32_t len) {
		uint32_t res = 0;
		uint32_t blockSize;
		const uint8_t * blockPtr;
		while ((blockPtr = this->peek(blockSize)) != NULL && (res + blockSize) < len) {
			for (uint32_t i = 0; i < blockSize; i++, buf++, blockPtr++) {
				*buf = *blockPtr;
			}
			res += blockSize;
			this->pop();
		}
		return res;
	}
	
	/**
	 * Number of bytes available for instant reading.
	 * 
	 * @return number of bytes that can be instantly read
	 */
	uint32_t availableForRead() const {
		const IndexType curHead = this->head;
		uint32_t res = 0;
		for (IndexType i = this->tail; i != curHead; i = IndexType((i + 1) % Count)) {
			res += this->size[i];
		}
		return res;
	}
	
	/**
	 * Number of bytes available for instant writing.
	 * 
	 * @return number of bytes that can be instantly written
	 */
	uint32_t availableForWrite() const {
		const IndexType curTail = this->tail;
		const IndexType curHead = this->head;
		const SizeType curSize = this->size[curHead];
		const IndexType availableBlocks = (curHead >= curTail) ? IndexType(Count + curTail - curHead) : IndexType(curTail - curHead);
		return uint32_t((availableBlocks * BlockSize) - curSize - 1);
	}
#undef _FIFO_HANDLE
};


/* unit tests:
-------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#define __WFI()
#include "fifo.h"

#define BUF_SIZE 8

enum State { ADD, DEL, CLR, STATES };

State rndState() {
	return State(rand() % STATES);
}

#define assert(exp) \
	if (!(exp)) { \
		fprintf(stderr, "%s:%i: failed assertion: %s\n", __FILE__, int(__LINE__), #exp); \
		fprintf(stderr, "exp: %u\n", unsigned(exp)); \
		fprintf(stderr, "head: %u\n", unsigned(head)); \
		fprintf(stderr, "tail: %u\n", unsigned(tail)); \
		fprintf(stderr, "size: %u\n", unsigned(size)); \
		exit(1); \
	}

int main() {
	size_t head, tail;
	char buf[BUF_SIZE];
	#define HANDLE buf, BUF_SIZE, head, tail
	size_t size = 0;
	_FIFOX_INIT(HANDLE);
	assert(_FIFOX_EMPTY(HANDLE));
	assert(_FIFOX_SIZE(HANDLE) == 0);
	assert(_FIFOX_AVAILABLE(HANDLE) == (BUF_SIZE - 1));
	assert(_FIFOX_CAPACITY(HANDLE) == (BUF_SIZE - 1));
	assert(_FIFOX_BSIZE(HANDLE) == 0);
	for (size_t i = 0; i < 1000000; i++) {
		switch (rndState()) {
		case ADD:
			if ((size + 1) < BUF_SIZE) {
				_FIFOX_PUSH(HANDLE, char(head));
				size++;
			}
			break;
		case DEL:
			if (size > 0) {
				const char expVal = char(tail);
				const char peekVal = char(_FIFOX_PEEK(HANDLE));
				const char popVal = char(_FIFOX_POP(HANDLE));
				assert(peekVal == popVal);
				assert(expVal == popVal);
				size--;
			} else {
				assert(_FIFOX_PEEK(HANDLE) == -1);
				assert(_FIFOX_POP(HANDLE) == -1);
			}
			break;
		case CLR:
			_FIFOX_CLEAR(HANDLE);
			size = 0;
			break;
		case STATES:
			break;
		}
		assert(_FIFOX_SIZE(HANDLE) == size);
		assert(_FIFOX_AVAILABLE(HANDLE) == size_t(BUF_SIZE - size - 1));
		assert(_FIFOX_INDEX(HANDLE, size) == head);
		assert(_FIFOX_BSIZE(HANDLE) <= _FIFOX_SIZE(HANDLE));
		const size_t blockEnd = _FIFOX_INDEX(HANDLE, _FIFOX_BSIZE(HANDLE));
		assert(blockEnd == 0 || blockEnd == head);
		if (size == 0) {
			assert(_FIFOX_EMPTY(HANDLE));
			assert(_FIFOX_SIZE(HANDLE) == 0);
			assert(_FIFOX_AVAILABLE(HANDLE) == (BUF_SIZE - 1));
		} else {
			assert(!_FIFOX_EMPTY(HANDLE));
		}
		if ((size + 1) >= BUF_SIZE) {
			assert(_FIFOX_FULL(HANDLE));
			assert(_FIFOX_SIZE(HANDLE) == (BUF_SIZE - 1));
			assert(_FIFOX_AVAILABLE(HANDLE) == 0);
		} else {
			assert(!_FIFOX_FULL(HANDLE));
		}
	}
	return 0;
}
-------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#define __WFI()
#include "fifo.h"

#define TOTAL_SIZE 8
#define BLOCK_SIZE 4
#define BLOCK_COUNT ((TOTAL_SIZE) / (BLOCK_SIZE))

typedef _BlockFifoClass<TOTAL_SIZE, BLOCK_SIZE> Fifo;

enum State { ADD, COM, DEL, CLR, STATES };

State rndState() {
	return State(rand() % STATES);
}

#define assert(exp) \
	if (!(exp)) { \
		fprintf(stderr, "%s:%i: failed assertion: %s\n", __FILE__, int(__LINE__), #exp); \
		fprintf(stderr, "exp: %u\n", unsigned(exp)); \
		fprintf(stderr, "size: %u\n", unsigned(size)); \
		fprintf(stderr, "blockSize: %u\n", unsigned(blockSize)); \
		fprintf(stderr, "blocks: %u\n", unsigned(blocks)); \
		fprintf(stderr, "head: %u\n", unsigned(fifo.head)); \
		fprintf(stderr, "tail: %u\n", unsigned(fifo.tail)); \
		fprintf(stderr, "sizes: ["); \
		for (size_t _n = 0; _n < BLOCK_COUNT; _n++) { \
			if (_n != 0) fprintf(stderr, ", "); \
			fprintf(stderr, "%u", unsigned(fifo.size[_n])); \
		} \
		fprintf(stderr, "]\n"); \
		fprintf(stderr, "data: ["); \
		for (size_t _n = 0; _n < BLOCK_COUNT; _n++) { \
			if (_n != 0) fprintf(stderr, ", "); \
			fprintf(stderr, "["); \
			for (size_t _m = 0; _m < BLOCK_SIZE; _m++) { \
				if (_m != 0) fprintf(stderr, ", "); \
				fprintf(stderr, "%u", unsigned(fifo.block[_n][_m])); \
			} \
			fprintf(stderr, "]"); \
		} \
		fprintf(stderr, "]\n"); \
		exit(1); \
	}

int main() {
	Fifo fifo;
	size_t size = 0, blockSize = 0, blocks = 0;
	assert(fifo.empty());
	assert(fifo.availableForRead() == 0);
	assert((fifo.availableForWrite() + 1) == TOTAL_SIZE);
	assert(fifo.TotalSize == TOTAL_SIZE);
	assert(fifo.BlockSize == BLOCK_SIZE);
	assert(fifo.Count == BLOCK_COUNT);
	for (size_t i = 0; i < 1000000; i++) {
		switch (rndState()) {
		case ADD:
			if ((size + 1) < TOTAL_SIZE) {
				const bool expRes = (blocks + 1) < BLOCK_COUNT || (blockSize + 1) < BLOCK_SIZE;
				const bool res = fifo.push(char(blockSize));
				assert(res == expRes);
				if ( res ) {
					size++;
					blockSize++;
					if (blockSize >= BLOCK_SIZE) {
						blockSize = 0;
						blocks++;
					}
				}
			}
			break;
		case COM:
			{
				const bool expRes = (blocks + 1) < BLOCK_COUNT && blockSize > 0;
				const bool res = fifo.commitBlock();
				assert(res == expRes);
				if ( res ) {
					blockSize = 0;
					blocks++;
				}
			}
			break;
		case DEL:
			{
				uint32_t aBlockSize;
				const uint8_t * blockPtr = fifo.peek(aBlockSize);
				if (blocks > 0) {
					assert(blockPtr != NULL);
					assert(aBlockSize > 0);
					assert(aBlockSize == fifo.size[fifo.tail]);
					for (uint32_t k = 0; k < aBlockSize; k++) {
						assert(k == blockPtr[k]);
					}
					fifo.pop();
					size = size_t(size - aBlockSize);
					blocks--;
				} else {
					assert(blockPtr == NULL);
					assert(aBlockSize == 0);
				}
			}
			break;
		case CLR:
			fifo.clear();
			size = 0;
			blockSize = 0;
			blocks = 0;
			break;
		case STATES:
			break;
		}
		if (size == 0) {
			assert(fifo.empty());
			assert(fifo.totallyEmpty());
			assert( ! fifo.full() );
			assert(fifo.availableForRead() == 0);
			assert((fifo.availableForWrite() + 1) == TOTAL_SIZE);
		} else if ((size + 1) == TOTAL_SIZE) {
			assert( ! fifo.empty() );
			assert( ! fifo.totallyEmpty() );
			assert(fifo.full());
			assert((fifo.availableForRead() + blockSize + 1) == TOTAL_SIZE);
			assert(fifo.availableForWrite() == 0);
		} else {
			if (blocks < 1) {
				assert(fifo.empty());
			} else {
				assert( ! fifo.empty() );
			}
			assert( ! fifo.totallyEmpty() );
			if ((blocks + 1) == BLOCK_COUNT) {
				assert(fifo.full());
			} else {
				assert( ! fifo.full() );
			}
			assert(fifo.availableForRead() == (size - blockSize));
			assert(fifo.availableForWrite() == (((BLOCK_COUNT - blocks) * BLOCK_SIZE) - blockSize - 1));
		}
	}
	return 0;
}
*/

#endif /* __SCDINTERNAL_FIFO_H__ */
