// Low-level EEPROM access on ATmega328
#define PW_ADDRESS 0
#define MAX_LEN 4

//From Data Sheet
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
    // Wait for completion of previous writeP
    while (EECR & (1 << EEPE));
    // Set up address and Data Registers
    EEAR = uiAddress;
    EEDR = ucData;
    // Write logical one to EEMPE
    EECR |= (1 << EEMPE);
    // Start EEPROM write by setting EEPE
    EECR |= (1 << EEPE);
}

//From Data Sheet
unsigned char EEPROM_read(unsigned int uiAddress)
{
    // Wait for completion of previous write
    while (EECR & (1 << EEPE))
        ;
    // Set up address register
    EEAR = uiAddress;
    // Start EEPROM read by writing EERE
    EECR |= (1 << EERE);
    // Return data from Data Register
    return EEDR;
}


// Save password to EEPROM
void savePassword(const char* password) 
{
    int i = 0;
    while (password[i] != '\0' && i < MAX_LEN) 
    {
        EEPROM_write(PW_ADDRESS + i, password[i]);
        i++;
    }
    EEPROM_write(PW_ADDRESS + i, '\0'); // Null terminator
}


// Read a string (password) from EEPROM
void readPassword(char* buffer, int bufferSize) 
{
    int i = 0;
    unsigned char c = EEPROM_read(PW_ADDRESS);
    while (c != '\0' && i < bufferSize - 1) 
    {
        buffer[i++] = c;
        c = EEPROM_read(PW_ADDRESS + i);
    }
    buffer[i] = '\0'; // Null terminate
}


// Arduino setup
void setup() 
{
    Serial.begin(9600);

    // Save password 
    savePassword("0501");

    // Holds password read from EEPROM temp
    char pTemp[MAX_LEN + 1]; 
    readPassword(pTemp, sizeof(pTemp));

    Serial.print("Stored Password: ");
    Serial.println(pTemp);
}


void loop() 
{
    
}
