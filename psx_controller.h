//pin definitions
#define PSX_DIRECTION DDRB
#define PSX_PORT PORTB
#define PSX_PIN PINB
#define PSX_DATA PB4
#define PSX_CMD PB3
#define PSX_ATT PB2
#define PSX_CLK PB1
#define PSX_ACK PB0
#define PSX_CLK_DELAY_US 2
#define PSX_BYTE_DELAY_US 17
typedef unsigned char BYTE;

//initializes the AVR pins
void psx_bus_init()
{
    PSX_DIRECTION =
            (1 << PSX_CLK) |
            (1 << PSX_ATT) |
            (1 << PSX_CMD);
    PSX_PORT =
            (1 << PSX_DATA) |
            (1 << PSX_CLK) |
            (1 << PSX_ATT) |
            (1 << PSX_CMD) |
            (1 << PSX_ACK);
}

void psx_att_low()
{
    PSX_PORT &= ~(1 << PSX_ATT);
}

void psx_att_high()
{
    PSX_PORT |= (1 << PSX_ATT);
}

void psx_clk_low()
{
    PSX_PORT &= ~(1 << PSX_CLK);
}

void psx_clk_high()
{
    PSX_PORT |= (1 << PSX_CLK);
}

void psx_read_state(int * data)
{
    BYTE cmd, counter;

    psx_att_low();                      //ATT low to enable controller

    //byte 1
    PSX_PORT |= (1 << PSX_CMD);
    cmd = 0x01;                         //first command to init communication
    _delay_us(2);                       //2us delay
    for(counter = 0; counter < 8; counter++)
    {
        PSX_PORT &= ~(1 << PSX_CMD);
        PSX_PORT |= ((cmd >> counter)&0x1)<<PSX_CMD;
        psx_clk_low();                      //CLK low for full duration because we dont need to read anything on 0x1
        _delay_us(PSX_CLK_DELAY_US); //delay
        psx_clk_high();
        _delay_us(PSX_CLK_DELAY_US); //delay
    }
    PSX_PORT |= (1 << PSX_CMD);

    _delay_us(PSX_BYTE_DELAY_US);

    //byte 2
    cmd = 0x42;
    PSX_PORT |= (1 << PSX_CMD);
    for(counter = 0; counter < 8; counter++)
    {
        PSX_PORT &= ~(1 << PSX_CMD);
        PSX_PORT |= ((cmd >> counter)&0x1)<<PSX_CMD;
        psx_clk_low();
        _delay_us(PSX_CLK_DELAY_US); //delay
        psx_clk_high();
        _delay_us(PSX_CLK_DELAY_US); //delay
    }
    PSX_PORT |= (1 << PSX_CMD);

    _delay_us(PSX_BYTE_DELAY_US);

    //byte 3
    cmd = 0;
    PSX_PORT |= (1 << PSX_CMD);
    for(counter = 0; counter < 8; counter++)
    {
        PSX_PORT &= ~(1 << PSX_CMD);
        PSX_PORT |= ((cmd >> counter)&0x1)<<PSX_CMD;
        psx_clk_low();
        _delay_us(PSX_CLK_DELAY_US); //delay
        psx_clk_high();
        _delay_us(PSX_CLK_DELAY_US); //delay
    }
    PSX_PORT |= (1 << PSX_CMD);

    _delay_us(PSX_BYTE_DELAY_US);

    //byte 4
    *data = 0;
    cmd = 0;
    PSX_PORT |= (1 << PSX_CMD);
    for(counter = 0; counter < 8; counter++)
    {
        PSX_PORT &= ~(1 << PSX_CMD);
        PSX_PORT |= ((cmd >> counter)&0x1)<<PSX_CMD;
        psx_clk_low();
        _delay_us(PSX_CLK_DELAY_US); //delay
        *data |= ((~PSX_PIN & (1 << PSX_DATA))>>4) << counter;
        psx_clk_high();
        _delay_us(PSX_CLK_DELAY_US); //delay
    }
    PSX_PORT |= (1 << PSX_CMD);

    _delay_us(PSX_BYTE_DELAY_US);

    //byte 5
    cmd = 0;
    PSX_PORT |= (1 << PSX_CMD);
    for(counter = 0; counter < 8; counter++)
    {
        PSX_PORT &= ~(1 << PSX_CMD);
        PSX_PORT |= ((cmd >> counter)&0x1)<<PSX_CMD;
        psx_clk_low();
        _delay_us(PSX_CLK_DELAY_US); //delay
        *data |= ((~PSX_PIN & (1 << PSX_DATA))>>4) << (counter + 8);
        psx_clk_high();
        _delay_us(PSX_CLK_DELAY_US); //delay
    }
    PSX_PORT |= (1 << PSX_CMD);

    psx_att_high();
}
