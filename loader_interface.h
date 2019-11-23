/*
 * loader_interface.h
 *
 *  Created on: Nov 23, 2019
 *      Author: nidal
 *
 * This header file includes the interface for the binary loader which defines the main classes & their attributes.
 *	For the sake of simplicity , we are only interested in basic specs for binary loading.
 */

#ifndef LOADER_INTERFACE_H_
#define LOADER_INTERFACE_H_

#include <stdint.h>
#include <string>
#include <vector>

class Binary ;
class Section ;
class Symbol ;

/*
 * The symbol class contains all the specs about the symbols needed to be loaded from the binary.
 * */
class Symbol
{
public :
	enum SymbolType		//Right now we are only interested in loading function symbols.
	{
		SYM_TYPE_UNKOWN = 0,
		SYM_TYPE_FUNCION = 1
		// More symbol types to be add in the future.
	};

	SymbolType type ;
	std::string name ;
	uint64_t addr ;	//Start address.

	// Class constructor with the default values using member initializer list.
	Symbol () : type(SYM_TYPE_UNKOWN),name(),addr(0) {}
};


/* The Section class contains all the specs about the scrions of the binary like : .text ;
 * */
class Section
{
public :
	enum SectionType
	{
		SEC_TYPE_NONE,	//Assigned 0 by default.
		SEC_TYPE_DATA,	//Assigned 1 by default.
		SEC_TYPE_CODE	//etc...
	};

	Binary 	*binary;//pointer to the binary containing the section.
	std::string name;
	SectionType type;
	uint64_t vma;	//Starting virtual memory address.
	uint64_t size;	//size in bytes.
	uint8_t *bytes;	//Raw bytes in the section.

	Section () : binary(NULL), name(), type(SEC_TYPE_NONE), vma(0), size(0), bytes(NULL){}

	// To determine if a given address parameter is contained in the section, return true if contained.
	bool contains (uint64_t addr) { if((addr >= vma) && (addr-vma < size)) return true ; return false;}


};


/*
 * The Main class of the binary, the blueprint for all executables in which all attributes reside.
 * */
class Binary
{
public:
	enum BinaryType
	{
		BIN_TYPE_AUTO = 0,
		BIN_TYPE,ELF = 1,
		BIN_TYPE_PE = 2
	};

	enum ArchType
	{
		ARCH_NONE = 0,
		ARCH_X86 =1		//only supported arch right now, more archs to be added.
	};

	std::string name;
	BinaryType bin_type;
	std::string str_bin_type;
	ArchType arch_type;
	std::string str_arch_type;
	unsigned bits;	//bit width, PS :unsigned ->  shorthand for unsigned int;
	uint64_t entry;	//Entry point address.
	std::vector<Section> sections;
	std::vector<Symbol> symbols;

	Binary () : bin_type(BIN_TYPE_AUTO), arch_type(ARCH_NONE), bits(0), entry(0){}

	// Function used to retrieve the text section which contains the code instructions;
	Section * get_text_section()
	{
		for (unsigned int i = 0 ; i <this->sections.size(); i++)
			{
			if ((this->sections[i]).name ==".text") return &(this->sections[i]) ;
			return NULL;
			}
	}
};

int load_binary(std::string &fname , Binary *bin, Binary::BinaryType type);
void unload_binary (Binary *bin);

#endif /* LOADER_INTERFACE_H_ */
