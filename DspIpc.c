#include <Arduino.h>
#include "DspIpc.h"
#include "Settings.h"

/*
@brief Convert 32 bit fixed integer to an array of 4 bytes.
@returns void
*/
void f32toint8 (int32_t f32val, byte buf[4]) {
    buf[0] = (f32val >> 24) & 0xFF;
    buf[1] = (f32val >> 16) & 0xFF;
    buf[2] = (f32val >> 8) & 0xFF;
    buf[3] = (f32val) & 0xFF;
}

#if DSP_CORE == SIGMA300
    int32_t fixpoint_convert(double val) { return round(val * (0.01 * (1 << 24))); }
#else
    int32_t fixpoint_convert(double val) { return int32_t(val * (0.01 * (1 << 23))); }
#endif

#if SPI_ENABLED
/*
@returns The depth of a memory address, given its location.
*/
byte getMemBlockDepth(uint32_t addr) {
#if DSP_CORE == SIGMA100
    if (addr < 0x0400)
        return 4;
    else {
        return 5;
    }
#elif DSP_CORE == SIGMA200
    if (addr < 0x0800) {
        return 4;
    } else {
        return 5;
    }
#elif DSP_CORE == SIGMA300
    if (addr < 0xF000) {
        return 4;
    } else {
        return 2;
    }
#else
    return 0;
#endif
}
#endif

/*
@brief Write to a block of registers in the address space
@returns 1 upon successful execution
*/
int dspWriteReg_Block(int addr, int len, byte data[]) {
#if SPI_ENABLED
    digitalWrite(DSP_PIN_SS, LOW);
    SPI.beginTransaction(spiSettings);
    SPI.transfer(0x0);
    SPI.transfer(addr >> 8);
    SPI.transfer(addr & 0xFF);
    for (int i = 0; i < len; i++) {
        SPI.transfer(data[i]);
    }
    SPI.endTransaction();
    digitalWrite(DSP_PIN_SS, HIGH);
    return 1;
#else
    if (len < I2C_MAX_DATA_LEN) {
        Wire.beginTransmission(I2CADDR_DSP);
        Wire.write(addr >> 8);
        Wire.write(addr & 0xff);
        Wire.write(data, len);
        Wire.endTransmission();
        return 1;
    } else {
        int index = 0;

        while (index < len) {
            Wire.beginTransmission(I2CADDR_DSP);
            Wire.write(addr >> 8);
            Wire.write(addr & 0xff);

            int sentBytes = 0;

            while ((sentBytes + getMemBlockDepth(uint32_t(addr)) <= I2C_MAX_DATA_LEN) && (index < len)) {
                for (byte i = 0; i < getMemBlockDepth(uint32_t(addr)); i++) {
                    Wire.write(data[index++]);
                    sentBytes++;
                }

                addr++;
            }

            Wire.endTransmission();

            if(index == len) {
                break;
            }
        }
        return 1;
    }
#endif
}

/*
@brief Writes an integer value to a DSP address
@returns the return value of dspWriteReg_Block()
*/
int dspWriteReg_Int(int addr, int32_t data) {
    byte regdata[4];
    f32toint8(data, regdata);
    return dspWriteReg_Block(addr, 4, regdata);
}

/*
@brief Writes a floating point value to a DSP address
@returns the return value of dspWriteReg_Int()
*/
int dspWriteReg_Float(int addr, double data) {
    return dspWriteReg_Int(addr, fixpoint_convert(data));
}

/*
@brief Delay sending instructions to the DSP
@returns 1 upon successful execution
*/
int dspDelay(byte dspAddr, int len, byte data[]) {
    int delayLen = 0;
    for (byte i = len; i > 0; i--) {
        delayLen = (delayLen << 8) + data[i];
    }
    delay(delayLen);
    return 1;
}

/*
@brief Reads byte data from a DSP address into the provided buffer
@returns 1 upon successful execution
*/
int dspReadReg_Byte(int addr, int len, byte *data) {
#if SPI_ENABLED
    digitalWrite(DSP_PIN_SS, LOW);
    SPI.beginTransaction(spiSettings);
    SPI.transfer(0x1);
    SPI.transfer(addr >> 8);
    SPI.transfer(addr & 0xff);
    for (int i = 0; i < len; i++) {
        data[i] = SPI.transfer(0);
    }
    SPI.endTransaction();
    digitalWrite(DSP_PIN_SS, HIGH);
    return 1;
#else
    Wire.beginTransmission(I2CADDR_DSP);
    Wire.write(addr >> 8);
    Wire.write(addr & 0xff);
    Wire.endTransmission(false);
    Wire.requestFrom(I2CADDR_DSP, len);
    for (int i = 0; i < len; i++) {
        if (Wire.available()) data[i] = Wire.read();
    }
    return 1;
#endif
}

/*
@brief Reads integer data from a DSP address
@returns 32 bit integer data
*/
int32_t dspReadReg_Int(int addr, int len) {
    int32_t readByte = 0;
    byte regVal[len];
    dspReadReg_Byte(addr, len, regVal);
    for (int i = 0; i < len; i++) {
        readByte = (readByte << 8) + regVal[i];
    }
    return readByte;
}

/*
@brief Reads floating point data from a DSP address
@returns floating point data
*/
double dspReadReg_Float(int addr) {
    int32_t regVal_Int = dspReadReg_Int(addr, 4);
#if DSP_CORE == SIGMA300
    return (double)(regVal_Int) / (1 << 24);
#else
    return (double)(regVal_Int) / (1 << 23);
#endif
}

/*
@brief Prints the register value and the following data of 'len' length to the serial console
@returns void
*/
void printDspReg(int addr, int len) {
    char printBuf[40];
    sprintf(printBuf, "\n\nRegister: 0x%04X Value: 0x", addr);
    byte regVal[len];
    dspReadReg_Byte(addr, len, regVal);
    Serial.print(printBuf);
    for (int i = 0; i < len; i++) {
        if (regVal[i] < 16) {
            Serial.print('0');
        }
        sprintf(printBuf, "%04X ", regVal);
        Serial.print(printBuf);
    }
    Serial.println();
}

/*
@brief Sets a register to a level value with a granularity determined by numLevels.
@returns 1 on successful execution
*/
int dspSetLevel(int level, int numLevels, int _addr, byte data[][4]) {
    int min_val = (numLevels - 1) / 2 - (numLevels - 1);
    Serial.println(min_val);
    
    int index = level - min_val;

    for (int i = 0; i < 4; i++) {
        if (data[index][i] < 16) {
            Serial.print('0');
        }
        Serial.print(data[index][i], HEX);
        Serial.print(' ');
    }

    dspWriteReg_Block(_addr, 4, data[index]);
    Serial.println(' ');

    return 1;
}

/*
@brief Safeload a runtime parameter in DSP memory
@returns 1 upon successful execution
*/
int dspSafeload(int addr, int numSafeload, byte data[5]) {
    if (numSafeload < 5) {
        // Ensures that only one safeload can be executed per one audio sample.
        delayMicroseconds((1000000 / sampleRate) + 10);
        
        dspWriteReg_Block(REG_SAFELOAD_BASE, numSafeload, data);
        dspWriteReg_Int(REG_SAFELOAD_TARGET_ADDR, addr);
        dspWriteReg_Int(REG_SAFELOAD_NUM, numSafeload);

        return 1;
    }
    return 0;
}