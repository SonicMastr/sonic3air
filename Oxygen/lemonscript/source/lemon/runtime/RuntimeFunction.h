/*
*	Part of the Oxygen Engine / Sonic 3 A.I.R. software distribution.
*	Copyright (C) 2017-2021 by Eukaryot
*
*	Published under the GNU GPLv3 open source software license, see license.txt
*	or https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#pragma once

#include "lemon/runtime/RuntimeOpcode.h"


namespace lemon
{
	class Program;
	class ScriptFunction;

	struct RuntimeOpcodeBuffer
	{
	public:
		~RuntimeOpcodeBuffer();

		inline bool empty() const				{ return mSize == 0; }
		inline size_t size() const				{ return mSize; }
		inline const uint8* getStart() const	{ return mBuffer; }
		inline const uint8* getEnd() const		{ return mBuffer + mSize; }

		inline const uint8& operator[](size_t offset) const  { return mBuffer[offset]; }

		inline const std::vector<RuntimeOpcode*>& getOpcodePointers() const  { return mOpcodePointers; }

		void reserveForOpcodes(size_t numOpcodes);
		RuntimeOpcode& addOpcode(size_t parameterSize);

	public:
		std::vector<RuntimeOpcode*> mOpcodePointers;	// Direct pointers to runtime opcodes

	private:
		uint8* mBuffer = nullptr;
		size_t mSize = 0;		// In bytes
		size_t mReserved = 0;	// In bytes
	};


	class API_EXPORT RuntimeFunction
	{
	public:
		void build(const Runtime& runtime);

		const uint8* getFirstRuntimeOpcode() const	{ return mRuntimeOpcodeBuffer.getStart(); }

		size_t translateFromRuntimeProgramCounter(const uint8* runtimeProgramCounter) const;
		const uint8* translateToRuntimeProgramCounter(size_t originalProgramCounter) const;

	private:
		void createRuntimeOpcode(RuntimeOpcodeBuffer& buffer, const Opcode* opcodes, int numOpcodesAvailable, int& outNumOpcodesConsumed, const Runtime& runtime);

	public:
		const ScriptFunction* mFunction = nullptr;
		RuntimeOpcodeBuffer mRuntimeOpcodeBuffer;
		std::vector<size_t> mProgramCounterByOpcodeIndex;	// Program counter (= byte index inside "mRuntimeOpcodeData") where runtime opcode for given original opcode index starts
	};

}
