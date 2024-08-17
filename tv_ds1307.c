unsigned int8 second, minute, hour, date, month, year, day;

void convert_data()
{
   second = (second >> 4) * 10 + (second & 0x0F);
   minute = (minute >> 4) * 10 + (minute & 0x0F);
   hour = (hour >> 4) * 10 + (hour & 0x0F);
   date = (date >> 4) * 10 + (date & 0x0F);
   month = (month >> 4) * 10 + (month & 0x0F);
   year = (year >> 4) * 10 + (year & 0x0F);
}

void ds1307_write(unsigned int8 address, data_)
{
   i2c_start();                                   // Start I2C protocol
   i2c_write(0xD0);                               // DS1307 address
   i2c_write(address);                            // Send register address
   i2c_write(data_);                              // Write data to the selected register
   i2c_stop();                                    // Stop I2C protocol
}

void i2c_read_data()
{
   i2c_start();                                  // Start I2C protocol
   i2c_write(0xD0);                              // DS1307 address
   i2c_write(0);                                 // Send register address
   i2c_start();                                  // Restart I2C
   i2c_write(0xD1);                              // Initialize data read
   second = i2c_read(1);                         // Read seconds from register 0
   minute = i2c_read(1);                         // Read minuts from register 1
   hour   = i2c_read(1);                         // Read hour from register 2
   day    = i2c_read(1);                         // Read day from register 3
   date   = i2c_read(1);                         // Read date from register 4
   month  = i2c_read(1);                         // Read month from register 5
   year   = i2c_read(0);                         // Read year from register 6
   i2c_stop();                                   // Stop I2C protocol
}
