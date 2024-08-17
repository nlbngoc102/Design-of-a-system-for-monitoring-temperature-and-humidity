void eeprom_write(unsigned int8 address, unsigned int8 data)
{
    write_eeprom(address, data);
}

unsigned int8 eeprom_read(unsigned int8 address)
{
    return read_eeprom(address);
}
