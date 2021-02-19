
#include "EEPROM.h"


/* --- Arduino-like interface -------------------------------------------- */

void EEPROM_write( int idx, uint8_t val )
{
	eeprom_unlock();
	if (eeprom_is_unlocked())
	{
		// write only after a successful unlock.
		EERef(idx) = val;
		// re-lock the EEPROM again.
		FLASH->IAPSR &= FLASH_FLAG_DUL;
	}
}
