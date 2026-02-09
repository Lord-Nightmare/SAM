#include <stdio.h>
#include <string.h>
#include "reciter.h"
#include "ReciterTabs.h"
#include "debug.h"

unsigned char Ar, Xr, Yr;
extern int debug;

static unsigned char inputtemp[256];   // secure copy of input tab36096

void Code37055(unsigned char mem59)
{
	Xr = mem59;
	Xr--;
	Ar = inputtemp[Xr];
	Yr = Ar;
	Ar = tab36376[Yr];
	return;
}

void Code37066(unsigned char mem58)
{
	Xr = mem58;
	Xr++;
	Ar = inputtemp[Xr];
	Yr = Ar;
	Ar = tab36376[Yr];
}

unsigned char GetRuleByte(unsigned short mem62, unsigned char Yr)
{
	unsigned int address = mem62;
	
	if (mem62 >= 37541) 
	{
		address -= 37541;
		return rules2[address+Yr];
	}
	address -= 32000;
	return rules[address+Yr];
}

int TextToPhonemes(char *input) // Code36484
{
	//unsigned char *tab39445 = &mem[39445];   //input and output
	//unsigned char mem29;
	unsigned char mem56;      //output position for phonemes
	unsigned char mem57;
	unsigned char mem58;
	unsigned char mem59;
	unsigned char mem60;
	unsigned char mem61;
	unsigned short mem62;     // memory position of current rule

	unsigned char mem64;      // position of '=' or current character
	unsigned char mem65;     // position of ')'
	unsigned char mem66;     // position of '('
	unsigned char mem36653;

	inputtemp[0] = ' ';

	// secure copy of input
	// because input will be overwritten by phonemes
	Xr = 1;
	Yr = 0;
	do
	{
		//pos36499:
		Ar = input[Yr] & 0x7f; // mask input to low ASCII range
		// if we're a lowercase character, make us uppercase; this is equivalent to `if ( Ar >= 0x60 ) Ar -= 0x20;`
		if ( Ar >= 0x70 ) Ar = Ar & 0x5f;
		else if ( Ar >= 0x60 ) Ar = Ar & 0x4f;
		inputtemp[Xr] = Ar;
		Xr++;
		Yr++;
	} while (Yr != 255);


	Xr = 255;
	inputtemp[Xr] = 27;
	mem61 = 255;


pos36550:
	Ar = 255;
	mem56 = 255;


pos36554:
	while(1)
	{
		mem61++;
		Xr = mem61;
		Ar = inputtemp[Xr];
		mem64 = Ar;
		if (Ar == '[')
		{
			mem56++;
			Xr = mem56;
			Ar = 155;
			input[Xr] = 155;
			//goto pos36542;
			//			Code39771(); 	//Code39777();
			return 1;
		}

		//pos36579:
		if (Ar != '.') break;
		Xr++;
		Yr = inputtemp[Xr];
		Ar = tab36376[Yr] & 1;
		if(Ar != 0) break;
		mem56++;
		Xr = mem56;
		Ar = '.';
		input[Xr] = '.';
	} //while


	//pos36607:
	Ar = mem64;
	Yr = Ar;
	Ar = tab36376[Ar];
	mem57 = Ar;
	if((Ar&2) != 0)
	{
		mem62 = 37541;
		goto pos36700;
	}

	//pos36630:
	Ar = mem57;
	if(Ar != 0) goto pos36677;
	Ar = ' ';
	inputtemp[Xr] = ' ';
	mem56++;
	Xr = mem56;
	if (Xr > 120) goto pos36654;
	input[Xr] = Ar;
	goto pos36554;

	// -----

	//36653 is unknown. Contains position

pos36654:
	input[Xr] = 155;
	Ar = mem61;
	mem36653 = Ar;
	//	mem29 = Ar; // not used
	//	Code36538(); das ist eigentlich
	return 1;
	//Code39771();
	//go on if there is more input ???
	mem61 = mem36653;
	goto pos36550;

pos36677:
	Ar = mem57 & 128;
	if(Ar == 0)
	{
		//36683: BRK
		return 0;
	}

	// go to the right rules for this character.
	Xr = mem64 - 'A';
	mem62 = tab37489[Xr] | (tab37515[Xr]<<8);

	// -------------------------------------
	// go to next rule
	// -------------------------------------

pos36700:

	// find next rule
	Yr = 0;
	do
	{
		mem62 += 1;
		Ar = GetRuleByte(mem62, Yr);
	} while ((Ar & 128) == 0);
	Yr++;

	//pos36720:
	// find '('
	while(1)
	{
		Ar = GetRuleByte(mem62, Yr);
		if (Ar == '(') break;
		Yr++;
	}
	mem66 = Yr;

	//pos36732:
	// find ')'
	do
	{
		Yr++;
		Ar = GetRuleByte(mem62, Yr);
	} while(Ar != ')');
	mem65 = Yr;

	//pos36741:
	// find '='
	do
	{
		Yr++;
		Ar = GetRuleByte(mem62, Yr);
		Ar = Ar & 127;
	} while (Ar != '=');
	mem64 = Yr;

	Xr = mem61;
	mem60 = Xr;

	// compare the string within the bracket
	Yr = mem66;
	Yr++;
	//pos36759:
	while(1)
	{
		mem57 = inputtemp[Xr];
		Ar = GetRuleByte(mem62, Yr);
		if (Ar != mem57) goto pos36700;
		Yr++;
		if(Yr == mem65) break;
		Xr++;
		mem60 = Xr;
	}

// the string in the bracket is correct

//pos36787:
	Ar = mem61;
	mem59 = mem61;

pos36791:
	while(1)
	{
		mem66--;
		Yr = mem66;
		Ar = GetRuleByte(mem62, Yr);
		mem57 = Ar;
		//36800: BPL 36805
		if ((Ar & 128) != 0) goto pos37180;
		Xr = Ar & 127;
		Ar = tab36376[Xr] & 128;
		if (Ar == 0) break;
		Xr = mem59-1;
		Ar = inputtemp[Xr];
		if (Ar != mem57) goto pos36700;
		mem59 = Xr;
	}

//pos36833:
	Ar = mem57;
	if (Ar == ' ') goto pos36895;
	if (Ar == '#') goto pos36910;
	if (Ar == '.') goto pos36920;
	if (Ar == '&') goto pos36935;
	if (Ar == '@') goto pos36967;
	if (Ar == '^') goto pos37004;
	if (Ar == '+') goto pos37019;
	if (Ar == ':') goto pos37040;
	//	Code42041();    //Error
	//36894: BRK
	return 0;

	// --------------

pos36895:
	Code37055(mem59);
	Ar = Ar & 128;
	if(Ar != 0) goto pos36700;
pos36905:
	mem59 = Xr;
	goto pos36791;

	// --------------

pos36910:
	Code37055(mem59);
	Ar = Ar & 64;
	if(Ar != 0) goto pos36905;
	goto pos36700;

	// --------------


pos36920:
	Code37055(mem59);
	Ar = Ar & 8;
	if(Ar == 0) goto pos36700;
pos36930:
	mem59 = Xr;
	goto pos36791;

	// --------------

pos36935:
	Code37055(mem59);
	Ar = Ar & 16;
	if(Ar != 0) goto pos36930;
	Ar = inputtemp[Xr];
	if (Ar != 'H') goto pos36700;
	Xr--;
	Ar = inputtemp[Xr];
	if ((Ar == 'C') || (Ar == 'S')) goto pos36930;
	goto pos36700;

	// --------------

pos36967:
	Code37055(mem59);
	Ar = Ar & 4;
	if(Ar != 0) goto pos36930;
	Ar = inputtemp[Xr];
	if (Ar != 'H') goto pos36700;
	if ((Ar != 'T') && (Ar != 'C') && (Ar != 'S')) goto pos36700;
	mem59 = Xr;
	goto pos36791;

	// --------------


pos37004:
	Code37055(mem59);
	Ar = Ar & 32;
	if(Ar == 0) goto pos36700;

pos37014:
	mem59 = Xr;
	goto pos36791;

	// --------------

pos37019:
	Xr = mem59;
	Xr--;
	Ar = inputtemp[Xr];
	if ((Ar == 'E') || (Ar == 'I') || (Ar == 'Y')) goto pos37014;
	goto pos36700;
	// --------------

pos37040:
	Code37055(mem59);
	Ar = Ar & 32;
	if(Ar == 0) goto pos36791;
	mem59 = Xr;
	goto pos37040;

//---------------------------------------


pos37077:
	Xr = mem58+1;
	Ar = inputtemp[Xr];
	if (Ar != 'E') goto pos37157;
	Xr++;
	Yr = inputtemp[Xr];
	Xr--;
	Ar = tab36376[Yr] & 128;
	if(Ar == 0) goto pos37108;
	Xr++;
	Ar = inputtemp[Xr];
	if (Ar != 'R') goto pos37113;
pos37108:
	mem58 = Xr;
	goto pos37184;
pos37113:
	if ((Ar == 'S') || (Ar == 'D')) goto pos37108;
	if (Ar != 'L') goto pos37135;
	Xr++;
	Ar = inputtemp[Xr];
	if (Ar != 'Y') goto pos36700;
	goto pos37108;
	
pos37135:
	if (Ar != 'F') goto pos36700;
	Xr++;
	Ar = inputtemp[Xr];
	if (Ar != 'U') goto pos36700;
	Xr++;
	Ar = inputtemp[Xr];
	if (Ar == 'L') goto pos37108;
	goto pos36700;

pos37157:
	if (Ar != 'I') goto pos36700;
	Xr++;
	Ar = inputtemp[Xr];
	if (Ar != 'N') goto pos36700;
	Xr++;
	Ar = inputtemp[Xr];
	if (Ar == 'G') goto pos37108;
	//pos37177:
	goto pos36700;

	// -----------------------------------------

pos37180:

	Ar = mem60;
	mem58 = Ar;

pos37184:
	Yr = mem65 + 1;

	//37187: CPY 64
	//	if(? != 0) goto pos37194;
	if(Yr == mem64) goto pos37455;
	mem65 = Yr;
	//37196: LDA (62),y
	Ar = GetRuleByte(mem62, Yr);
	mem57 = Ar;
	Xr = Ar;
	Ar = tab36376[Xr] & 128;
	if(Ar == 0) goto pos37226;
	Xr = mem58+1;
	Ar = inputtemp[Xr];
	if (Ar != mem57) goto pos36700;
	mem58 = Xr;
	goto pos37184;
pos37226:
	Ar = mem57;
	if (Ar == ' ') goto pos37295;
	if (Ar == '#') goto pos37310;
	if (Ar == '.') goto pos37320;
	if (Ar == '&') goto pos37335;
	if (Ar == '@') goto pos37367;
	if (Ar == '^') goto pos37404;
	if (Ar == '+') goto pos37419;
	if (Ar == ':') goto pos37440;
	if (Ar == '%') goto pos37077;
	//pos37291:
	//	Code42041(); //Error
	//37294: BRK
	return 0;

	// --------------
pos37295:
	Code37066(mem58);
	Ar = Ar & 128;
	if(Ar != 0) goto pos36700;
pos37305:
	mem58 = Xr;
	goto pos37184;

	// --------------

pos37310:
	Code37066(mem58);
	Ar = Ar & 64;
	if(Ar != 0) goto pos37305;
	goto pos36700;

	// --------------


pos37320:
	Code37066(mem58);
	Ar = Ar & 8;
	if(Ar == 0) goto pos36700;

pos37330:
	mem58 = Xr;
	goto pos37184;

	// --------------

pos37335:
	Code37066(mem58);
	Ar = Ar & 16;
	if(Ar != 0) goto pos37330;
	Ar = inputtemp[Xr];
	if (Ar != 'H') goto pos36700;
	Xr++;
	Ar = inputtemp[Xr];
	if ((Ar == 'C') || (Ar == 'S')) goto pos37330;
	goto pos36700;

	// --------------


pos37367:
	Code37066(mem58);
	Ar = Ar & 4;
	if(Ar != 0) goto pos37330;
	Ar = inputtemp[Xr];
	if (Ar != 'H') goto pos36700;
	if ((Ar != 'T') && (Ar != 'C') && (Ar != 'S')) goto pos36700;
	mem58 = Xr;
	goto pos37184;

	// --------------

pos37404:
	Code37066(mem58);
	Ar = Ar & 32;
	if(Ar == 0) goto pos36700;
pos37414:
	mem58 = Xr;
	goto pos37184;

	// --------------
	
pos37419:
	Xr = mem58;
	Xr++;
	Ar = inputtemp[Xr];
	if ((Ar == 'E') || (Ar == 'I') || (Ar == 'Y')) goto pos37414;
	goto pos36700;

// ----------------------

pos37440:

	Code37066(mem58);
	Ar = Ar & 32;
	if(Ar == 0) goto pos37184;
	mem58 = Xr;
	goto pos37440;
pos37455:
	Yr = mem64;
	mem61 = mem60;

	if (debug)
		PrintRule(mem62);

pos37461:
	//37461: LDA (62),y
	Ar = GetRuleByte(mem62, Yr);
	mem57 = Ar;
	Ar = Ar & 127;
	if (Ar != '=')
	{
		mem56++;
		Xr = mem56;
		input[Xr] = Ar;
	}

	//37478: BIT 57
	//37480: BPL 37485  //not negative flag
	if ((mem57 & 128) == 0) goto pos37485; //???
	goto pos36554;
pos37485:
	Yr++;
	goto pos37461;
}



