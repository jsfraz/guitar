#include "esp_system.h"

// Min tokens in message
const int MIN_TOKENS = 2;
// Max tokens in message
const int MAX_TOKENS = 2;
// Message tokens
String tokens[MAX_TOKENS];
// Message token count
int tokenCount = 0;

// The most important part of the program. IF YOU DELETE THIS, THE PROGRAM WILL EXPLODE.
String quotes[] = { "I turned myself into a guitar, Morty.", "Boom, big reveal, I'm a guitar.", "What do you think about that?", "I turned myslef into a guitar!", "I'm a guitar!!!" };

// Message types
enum MessageType {
  UNKOWN_MSG,
  CMD
};
// Commands
enum Command {
  UNKOWN_CMD,
  SYSTEMINFO,
  HELLO
};

void setup() {
  // Setup Serial
  Serial.begin(115200);
}

void loop() {
  // Check if there is data coming
  if (!Serial.available()) {
    return;
  }

  // Read string until newline character
  String str = Serial.readStringUntil('\n');
  // Split string
  tokenCount = splitString(str, ';', tokens, MAX_TOKENS);
  // Check minimum token count
  if (tokenCount < MIN_TOKENS) {
    Serial.println("Invalid token count");
    return;
  }

  // Handle by message type
  switch (parseMessageType(tokens[0])) {

    // Command
    case CMD:
      handleCommand(tokens[1]);
      break;

    // Unknown message type
    default:
      Serial.println("Unknown message type '" + tokens[0] + "'");
      break;
  }
}

// Handle command
void handleCommand(String cmd) {
  switch (parseCommand(cmd)) {

    // System info
    case SYSTEMINFO:
      Serial.println(String("Model: ") + ESP.getChipModel() + ", Revision: " + String(ESP.getChipRevision()) + ", Cores: " + String(ESP.getChipCores()) + ", Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz");
      break;

    // Hello
    case HELLO:
      for (int i = 0; i < sizeof(quotes) / sizeof(quotes[0]); i++) {
        Serial.println(quotes[i]);
        delay(1000);
      }
      break;

    // Unknown command
    default:
      Serial.println("Unknown command '" + cmd + "'");
      break;
  }
}

// Function to split a string based on a delimiter
int splitString(String str, char delimiter, String* output, int maxParts) {
  // Initialize the count of parts to 0
  int count = 0;
  // Set the starting index of the current part to 0
  int startIndex = 0;
  // Find the index of the first occurrence of the delimiter in the string
  int endIndex = str.indexOf(delimiter);

  // Iterate while the delimiter is found and the maximum number of parts is not reached
  while (endIndex != -1 && count < maxParts) {
    // Extract the current part from the string using substring()
    output[count++] = str.substring(startIndex, endIndex);
    // Update the starting index to the position after the delimiter
    startIndex = endIndex + 1;
    // Find the index of the next occurrence of the delimiter starting from the updated startIndex
    endIndex = str.indexOf(delimiter, startIndex);
  }

  // If there are still characters left in the string and the maximum number of parts is not reached
  if (startIndex < str.length() && count < maxParts) {
    // Extract the last part of the string and add it to the output array
    output[count++] = str.substring(startIndex);
  }

  // Return the count of parts that were successfully stored in the output array
  return count;
}

// Parse MessageType enum from string
MessageType parseMessageType(String msg) {
  if (msg == "CMD") {
    return CMD;
  }
  return UNKOWN_MSG;
}

// Parse Command enum from string
Command parseCommand(String cmd) {
  if (cmd == "SYSTEMINFO") {
    return SYSTEMINFO;
  }
  if (cmd == "HELLO") {
    return HELLO;
  }
  return UNKOWN_CMD;
}
