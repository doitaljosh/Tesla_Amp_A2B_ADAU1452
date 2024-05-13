#include "A2B.h"
#include "Amplifier.h"
#include "DspIpc.h"
#include "Settings.h"
#include "Utils.h"

#include <Wire.h>

enum States {
  STATE_DISCOVERY_NOT_DONE = 0,
  STATE_DISCOVERY_DONE = 1,
  STATE_SLAVE_INIT_DONE = 2
};

bool isPostDiscovery = false;
int currentState = STATE_DISCOVERY_NOT_DONE;

void printLineFaultMsg(int intType) {
  bool knownFault = false;
  int numMsgs = sizeof(lineFaultStrings) / sizeof(mappedMsg_t);

  Serial.print("E: A2B line fault detected: ");

  for (int i=0; i<numMsgs; i++) {
    if (lineFaultStrings[i].msgId == intType) {
      Serial.println(lineFaultStrings[i].msg);
      knownFault = true;
      break;
    }
  }

  if (!knownFault) {
    Serial.println("Unknown fault");
  }
}

void handleLineFault(int intType) {
  if (isPostDiscovery) {
    printLineFaultMsg(intType);
  } else {
    // Diagnostics during discovery
  }
}

void handleInterrupt(a2bInt_t a2bint) {
  switch (a2bint.MstrSub) {
    case A2B_INTSRC_MASTER: {
      switch (a2bint.intType) {
        case MSTR_RUNNING: {
          if (isPostDiscovery) {
            handleLineFault(a2bint.intType);
          } else {
            beginDiscovery();
          }
          break;
        }
        case DISCOVERY_DONE: {
          initSlaves();
          break;
        }
      }
      break;
    }
    case A2B_INTSRC_SUB: {
      switch (a2bint.nodeId) {
        case ampNodeID: {

          break;
        }
        case FOHCMicNodeID: {

          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
}

// For fault tolerance, this function can be re-run if a timeout has occured.
int beginDiscovery(void) {
  int discTimeout = 0;
  int intval = 0;
  byte DATA_INTMSK[3] = {0x10, 0x00, 0x09};
  // Initialize the registers necessary to begin discovery
  a2bWriteLocalReg(AD242x_REG_CONTROL, 0x04);
  delay(34);
  a2bWriteLocalRegBlock(AD242x_REG_INTMSK, 3, DATA_INTMSK);
  a2bWriteLocalReg(AD242x_REG_BECCTL, 0x00);
  a2bWriteLocalReg((AD242x_REG_INTPND + 0x02), 0x01);
  a2bWriteLocalReg(AD242x_REG_RESPCYCS, 0x92);
  a2bWriteLocalReg(AD242x_REG_CONTROL, 0x01);
  a2bWriteLocalReg(AD242x_REG_PLLCTL, 0x00);
  a2bWriteLocalReg(AD242x_REG_I2SGCFG, 0x02);
  a2bWriteLocalReg(AD242x_REG_SWCTL, 0x01);
  a2bWriteLocalReg(AD242x_REG_DISCVRY, 0x92);
  // Wait for the discovery done interrupt and timeout after 35ms
  while (true) {
    a2bInt_t receivedInterrupt = a2bReceiveInterrupt();
    if (receivedInterrupt.intType != DISCOVERY_DONE) {
      delay(5);
      discTimeout++;
      if (discTimeout > 6) {
        discTimeout = 0;
        Serial.print("E: A2B discovery timed out. Returned interrupt ");
        Serial.println(receivedInterrupt.intType);
        currentState = STATE_DISCOVERY_NOT_DONE;
        isPostDiscovery = false;
        handleInterrupt(receivedInterrupt);
      }
    } else {
      Serial.println("I: A2B discovery done.");
      currentState = STATE_DISCOVERY_DONE;
      isPostDiscovery = true;
      return DISCOVERY_DONE;
    }
    return 0;
  }
  return 0;
}

int initFOHCMicNode(void) {

}

int initAmplifierNode(void) {
  byte a2bAmpChipID[4];
  char* a2bAmpReadChipID;
  char* a2bAmpEepromData;

  if (a2bGetCurrentNodeAddr() == ampNodeID) {
    a2bAmpReadChipID = a2bReadRemoteRegBlock(ampNodeID, AD242x_REG_VENDOR, 4);
    for (int i=0; i<4; i++) {
      a2bAmpChipID[i] = a2bAmpReadChipID[i];
    }
    
    if (a2bAmpChipID == a2bAmpExpectedChipID) {
      Serial.print("I: Found Tesla A2B Amplifier at node ID: ");
      Serial.println(ampNodeID);

      a2bAmpEepromData = getAmplifierId();
      Serial.print("I: EEPROM data: ");
      for (int i=0; i<8; i++) {
        Serial.print(a2bAmpEepromData[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      // additional code for handling EEPROM data

      // Write the configuration registers for this A2B subordinate node
      for (int regidx=0; regidx < sizeof(amplifierLocalA2bConfig); regidx++) {
        a2bWriteRemoteReg(
          ampNodeID, 
          amplifierLocalA2bConfig[regidx][0],
          amplifierLocalA2bConfig[regidx][1]
          );
      }

      // Enable error correction
      a2bWriteRemoteReg(ampNodeID, AD242x_REG_BECCTL, 0x00);
      a2bWriteRemoteReg(ampNodeID, AD242x_REG_SWCTL, A2B_SWCTL_ENSW_ENABLED);

      // Configure GPIO pins
      if (a2bReceiveInterrupt().MstrSub == A2B_INTSRC_SUB) {
        delay(34);

        if (a2bReceiveInterrupt().MstrSub == A2B_INTSRC_SUB) {
          delay(23);

          if ((int)a2bReadRemoteReg(ampNodeID, AD242x_REG_GPIOIEN) != 0x20) {
            a2bWriteRemoteReg(ampNodeID, AD242x_REG_GPIOIEN, 0x20); // Enable input for GPIO5 (GPIO expander interrupt).
          }

          if ((int)a2bReadRemoteReg(ampNodeID, AD242x_REG_GPIOOEN) != 0x44) {
            a2bWriteRemoteReg(ampNodeID, AD242x_REG_GPIOOEN, 0x44); // Enable output for GPIO2 and GPIO6 (Va2b5v5 enable, unknown)
          }

          a2bWriteRemoteReg(ampNodeID, AD242x_REG_GPIODATSET, 0x04); // Set GPIO2 high (Va2b5v5 enable)
          a2bWriteRemoteReg(ampNodeID, AD242x_REG_GPIODATCLR, 0x40); // Clear the configuration for GPIO6

          // Configure all I2C peripherals inside the amplifier
          if (amplifierInit()) {
            Serial.println("I: Amplifier initialization successful.");

            return 1;
          } else {
            Serial.println("E: Amplifier initialization failed.");

            return 0;
          }
        }
      }
    } else {
      printf("E: Wrong A2B chip ID detected at amplifier node address.");

      return 0;
    }
  }
}

int initSlaves(void) {
  a2bWriteLocalReg(AD242x_REG_SWCTL, (byte)(A2B_SWCTL_MODE_USE_VIN & A2B_SWCTL_ENSW_ENABLED));
  
  if (currentState = STATE_DISCOVERY_DONE) {
    if (FOHCMicNodeID > 0) {
      Serial.println("I: Initializing FOHC mic array...");
      a2bSetNodeAddr(FOHCMicNodeID, A2B_NODEADR_BCAST_DIS, A2B_NODEADR_PERI_DIS);
      initFOHCMicNode();

      Serial.println("I: Initializing amplifier...");
      a2bSetNodeAddr(ampNodeID, A2B_NODEADR_BCAST_DIS, A2B_NODEADR_PERI_DIS);
      initAmplifierNode();
    } else {
      Serial.println("I: Initializing amplifier...");
      a2bSetNodeAddr(ampNodeID, A2B_NODEADR_BCAST_DIS, A2B_NODEADR_PERI_DIS);
      initAmplifierNode();
    }
    Serial.println("I: All A2B devices found and initialized!");
    currentState = STATE_SLAVE_INIT_DONE;
    return 1;
  } else {
    return 0;
  }
  return 0; // We should never get here.
}

void setup() {
  beginDiscovery();
  initSlaves();
}

void loop() {
  handleInterrupt(a2bReceiveInterrupt());
  delay(500);
}