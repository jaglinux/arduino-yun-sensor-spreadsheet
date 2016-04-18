#include <Bridge.h>
#include <Temboo.h>
#include <Console.h>
#include "TembooAccount.h" // contains Temboo account information
#include "GoogleAccount.h" //contains Google account information

#define DEBUG      0
#define ITERATION  12 // Iterations of temperature sensor data

int sensorPin = 0;
int ledPin = 13;

void setup() {

  Bridge.begin();
  pinMode (ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
#if DEBUG
  while (!Console);
#endif
}

void loop()
{
    Process time;
    String rowData;
    String timeString = "";
    //Indicate through LED that we have passed the setup() 
    digitalWrite(ledPin, HIGH);
    
    time.runShellCommand("date");
    
    while(time.available()) {
      char c = time.read();
      timeString += c;
    }

    // get the value we want to append to our spreadsheet
    float sensorValue = getSensorValue();

#if DEBUG
    Console.println("Appending value to spreadsheet");
#endif
    TembooChoreo AppendRowChoreo;
    AppendRowChoreo.begin();
    // set Temboo account credentials
    AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
    AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);
    AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");
    // your Google Client ID
    AppendRowChoreo.addInput("ClientID", GOOGLE_CLIENT_ID);
    // your Google Client Secret
    AppendRowChoreo.addInput("ClientSecret", GOOGLE_CLIENT_SECRET);
    // your Google Refresh Token
    AppendRowChoreo.addInput("RefreshToken", GOOGLE_REFRESH_TOKEN);
    // the title of the spreadsheet you want to append to
    AppendRowChoreo.addInput("SpreadsheetTitle", SPREADSHEET_TITLE);

    // convert the date and sensor values to a comma separated string
    rowData += timeString;
    rowData += ",";
    rowData += sensorValue;

    // add the RowData input item
    AppendRowChoreo.addInput("RowData", rowData);

    // run the Choreo and wait for the results
    // The return code (returnCode) will indicate success or failure 
    unsigned int returnCode = AppendRowChoreo.run();

    // return code of zero (0) means success
    if (returnCode == 0) {
#if DEBUG
      Console.println("Success! Appended " + rowData);
      Console.println("");
#endif
    } else {
      // return code of anything other than zero means failure  
      // read and display any error messages
      while (AppendRowChoreo.available()) {
        char c = AppendRowChoreo.read();
#if DEBUG
        Console.print(c);
#endif
      }
    }

    AppendRowChoreo.close();
    delay(5000); // wait 5 seconds between AppendRow calls
}

float getSensorValue() {
  int reading;
  float voltage;
  float temperatureC;
  float temperatureF;
  float average_tempF = 0;
  int i;
    
  for (i = 0; i < ITERATION; i++) {
    // ADC calculations
    reading = analogRead(sensorPin);
    voltage = reading * 5.0;
    voltage /= 1024.0;     
    temperatureC = (voltage - 0.5) * 100 ; 
    temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
    average_tempF += temperatureF;
#ifdef DEBUG
    Console.print(temperatureF);
    Console.print("\n");
#endif
    delay(5000); //wait 5 seconds between consecutive sensor read
  }
  average_tempF = average_tempF / ITERATION;
#ifdef DEBUG
  Console.print("Average \t");
  Console.print(average_tempF);
  Console.print("\n");
#endif
  return average_tempF;
}
