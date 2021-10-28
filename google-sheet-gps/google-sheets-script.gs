/************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: October 23, 2021
 *  
 *  Tutorial Video: https://youtu.be/yem5EysVloc
 *  My Channel: https://www.youtube.com/channel/UCOXYfOHgu-C-UfGyDcu5sYw/
 *  
 *  *********************************************************************************/
 
 //https://docs.google.com/spreadsheets/d/1ed1q95g3PuIdnTfH5-8dcb2yTQjQGHR4wfN_ByaMJwc/edit#gid=0

var ss = SpreadsheetApp.openById('ENTER_SHEET_ID');
var sheet = ss.getSheetByName('Sheet1');


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
function doGet(e){
  //----------------------------------------------------------------------------------
  //write_google_sheet() function in esp32 sketch, is send data to this code block
  //----------------------------------------------------------------------------------
  //get gps data from ESP32
  if (e.parameter == 'undefined') {
    return ContentService.createTextOutput("Received data is undefined");
  }
  //----------------------------------------------------------------------------------
  var dateTime = new Date();
  latitude    = e.parameters.latitude;
  longitude   = e.parameters.longitude;
  speed       = e.parameters.speed;
  satellites  = e.parameters.satellites;
  altitude    = e.parameters.altitude;
  gps_time    = e.parameters.gps_time;
  gps_date    = e.parameters.gps_date;
  //Logger.log('latitude=' + latitude);
  //----------------------------------------------------------------------------------
  var nextRow = sheet.getLastRow() + 1;
  sheet.getRange("A" + nextRow).setValue(dateTime);
  sheet.getRange("B" + nextRow).setValue(latitude);
  sheet.getRange("C" + nextRow).setValue(longitude);
  sheet.getRange("D" + nextRow).setValue(speed);
  sheet.getRange("E" + nextRow).setValue(satellites);
  sheet.getRange("F" + nextRow).setValue(altitude);
  sheet.getRange("G" + nextRow).setValue(gps_time);
  sheet.getRange("H" + nextRow).setValue(gps_date);
  //----------------------------------------------------------------------------------

  //returns response back to ESP32
  return ContentService.createTextOutput("Status Updated in Google Sheet");
  //----------------------------------------------------------------------------------
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM




//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//Extra Function. Not used in this project.
//planning to use in future projects.
//this function is used to handle POST request
function doPost(e) {
  var val = e.parameter.value;
  
  if (e.parameter.value !== undefined){
    var range = sheet.getRange('A2');
    range.setValue(val);
  }
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM