
#include "EEPROM.h"


/* --- Arduino-like interface -------------------------------------------- */

void EEPROM_update( int idx, uint8_t val )
{
	// make sure not to write data beyond the end of the EEPROM area
	// (this could accidentally hit the option byte area)
	if (idx >= EEPROM_end()) return;
	idx += (uint16_t)FLASH_DATA_START_PHYSICAL_ADDRESS;

	if (*((uint8_t*)idx) != val)
	{
		eeprom_unlock();
		if (eeprom_is_unlocked())
		{
			// write only after a successful unlock.
			*((uint8_t*)idx) = val;
			// re-lock the EEPROM again.
			FLASH->IAPSR &= FLASH_FLAG_DUL;
		}
	}
}
