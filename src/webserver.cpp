
#include <time.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <ElegantOTA.h>
#include <memory.h>
// Replace with your network credentials
const char *ssid = "orc";
const char *password = "12345678";

AsyncWebServer server(80);
const int filesPerPage = 10;

void initializeWifi()
{
    WiFi.mode(WIFI_AP);
    if (WiFi.softAP(ssid, password))
    {
        Serial.println("Access Point Started");
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
    }
    else
    {
        Serial.println("Access Point Failed to Start");
    }
}

String generateStatusHTML()
{
    String html = "<!DOCTYPE html><html><body><h3>Tracks</h3><table>";

    html += "<tr><td>state.currentScreen</td><td>" + String(state.currentScreen) + "</td></tr>";
    html += "<tr><td>state.menuCurrentSelection</td><td>" + String(state.menuCurrentSelection) + "</td></tr>";
    html += "<tr><td>state.menuCurrentValue</td><td>" + String(state.menuCurrentValue) + "</td></tr>";
    html += "<tr><td>state.menuLastCommand</td><td>" + String(state.menuLastCommand) + "</td></tr>";
    html += "<tr><td>state.menuCurrentLanguage</td><td>" + String(state.menuCurrentLanguage) + "</td></tr>";
    html += "<tr><td>state.inputValue</td><td>" + String(state.inputValue) + "</td></tr>";
    html += "<tr><td>state.units</td><td>" + String(state.units) + "</td></tr>";
    html += "<tr><td>state.currentTemperature</td><td>" + String(state.currentTemperature) + "</td></tr>";
    html += "<tr><td>state.currentHeading</td><td>" + String(state.currentHeading) + "</td></tr>";
    html += "<tr><td>state.currentSpeed</td><td>" + String(state.currentSpeed) + "</td></tr>";
    html += "<tr><td>state.average</td><td>" + String(state.average) + "</td></tr>";
    html += "<tr><td>state.previousLatitude</td><td>" + String(state.previousLatitude) + "</td></tr>";
    html += "<tr><td>state.previousLongitude</td><td>" + String(state.previousLongitude) + "</td></tr>";
    html += "<tr><td>state.currentLatitude</td><td>" + String(state.currentLatitude) + "</td></tr>";
    html += "<tr><td>state.currentLongitude</td><td>" + String(state.currentLongitude) + "</td></tr>";
    html += "<tr><td>state.gpsSatellites</td><td>" + String(state.gpsSatellites) + "</td></tr>";
    html += "<tr><td>state.currentAltitude</td><td>" + String(state.currentAltitude, 2) + "</td></tr>";
    html += "<tr><td>state.timeHours</td><td>" + String(state.timeHours) + "</td></tr>";
    html += "<tr><td>state.timeMinutes</td><td>" + String(state.timeMinutes) + "</td></tr>";
    html += "<tr><td>state.timeSeconds</td><td>" + String(state.timeSeconds) + "</td></tr>";
    html += "<tr><td>state.gpsPrecision</td><td>" + String(state.gpsPrecision) + "</td></tr>";
    html += "<tr><td>state.gpsDataAge</td><td>" + String(state.gpsDataAge) + "</td></tr>";
    html += "<tr><td>state.gpsFix</td><td>" + String(state.gpsFix) + "</td></tr>";
    html += "<tr><td>state.gpsCharsProcessed</td><td>" + String(state.gpsCharsProcessed) + "</td></tr>";
    html += "<tr><td>state.gpsLastSecond</td><td>" + String(state.gpsLastSecond) + "</td></tr>";
    html += "<tr><td>state.gpsDiscPos</td><td>" + String(state.gpsDiscPos) + "</td></tr>";
    html += "<tr><td>state.secsStopped</td><td>" + String(state.secsStopped) + "</td></tr>";
    html += "<tr><td>state.alreadySaved</td><td>" + String(state.alreadySaved) + "</td></tr>";
    html += "<tr><td>state.isMoving</td><td>" + String(state.isMoving) + "</td></tr>";
    html += "<tr><td>state.selectButtonPressedSince</td><td>" + String(state.selectButtonPressedSince) + "</td></tr>";
    html += "<tr><td>state.quickViewScreen</td><td>" + String(state.quickViewScreen) + "</td></tr>";
    html += "<tr><td>state.currentUpdateStep</td><td>" + String(state.currentUpdateStep) + "</td></tr>";
    html += "<tr><td>state.updatingFirmware</td><td>" + String(state.updatingFirmware) + "</td></tr>";
    html += "<tr><td>state.currentTrackFile</td><td>" + String(state.currentTrackFile.c_str()) + "</td></tr>";
    html += "<tr><td>state.currentGPXFile</td><td>" + String(state.currentGPXFile.c_str()) + "</td></tr>";

    html += "<tr><td>memory.config.avgAccumulator</td><td>" + String(memory.config.avgAccumulator) + "</td></tr>";
    html += "<tr><td>memory.config.avgSamples</td><td>" + String(memory.config.avgSamples) + "</td></tr>";
    html += "<tr><td>memory.config.language</td><td>" + String(memory.config.language) + "</td></tr>";
    html += "<tr><td>memory.config.declAngle</td><td>" + String(memory.config.declAngle) + "</td></tr>";
    html += "<tr><td>memory.config.tripPartial</td><td>" + String(memory.config.tripPartial) + "</td></tr>";
    html += "<tr><td>memory.config.tripTotal</td><td>" + String(memory.config.tripTotal) + "</td></tr>";
    html += "<tr><td>memory.config.tripTime</td><td>" + String(memory.config.tripTime) + "</td></tr>";
    html += "<tr><td>memory.config.maxSpeed</td><td>" + String(memory.config.maxSpeed) + "</td></tr>";
    html += "<tr><td>memory.config.units</td><td>" + String(memory.config.units) + "</td></tr>";
    html += "<tr><td>memory.config.timeZone</td><td>" + String(memory.config.timeZone) + "</td></tr>";
    html += "<tr><td>memory.config.backlight</td><td>" + String(memory.config.backlight) + "</td></tr>";
    html += "<tr><td>memory.config.precision</td><td>" + String(memory.config.precision) + "</td></tr>";
    html += "<tr><td>memory.config.quickViewEnabled</td><td>" + String(memory.config.quickViewEnabled) + "</td></tr>";
    html += "<tr><td>memory.config.flipScreen</td><td>" + String(memory.config.flipScreen) + "</td></tr>";
    html += "<tr><td>memory.config.memoryInitialized</td><td>" + String(memory.config.memoryInitialized) + "</td></tr>";
    html += "<tr><td>memory.config.saveInterval</td><td>" + String(memory.config.saveInterval) + "</td></tr>";

    html += "</table></body></html>";
    return html;
}

void listFilesInDir(File dir, String &html, String path, int start, int count)
{
    int currentFile = 0;
    int end = start + count;
    while (currentFile < end)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            break; // No more files
        }
        if (entry.name()[0] == '.') // Ignore hidden files
        {
            entry.close();
            continue;
        }
        if (currentFile >= start)
        {
            String entryPath = path + entry.name();
            if (!entry.isDirectory())
            {
                html += "<a href=\"" + entryPath + "\" download>" + entry.name() + "</a> ";
                html += "<form action=\"/delete\" method=\"post\" style=\"display:inline;\" onsubmit=\"return confirm('Really delete?');\">";
                html += "<button name=\"file\" value=\"" + entryPath + "\">Delete</button></form><br/>";
            }
        }
        entry.close();
        currentFile++;
    }
}

String generateFileListHTML(int page)
{
    String html = "<!DOCTYPE html><html><body><h3>Tracks</h3>";
    File root = SD.open("/");
    if (!root)
    {
        html += "Failed to open directory";
    }
    else if (!root.isDirectory())
    {
        html += "Not a directory";
    }
    else
    {
        listFilesInDir(root, html, "/", page * filesPerPage, filesPerPage);
    }
    root.close();

    // Pagination controls
    html += "<br/>";
    if (page > 0)
    {
        html += "<a href=\"/?page=" + String(page - 1) + "\">Previous</a> ";
    }
    html += "<a href=\"/?page=" + String(page + 1) + "\">Next</a>";

    html += "</body></html>";
    return html;
}

void initializeWebserver()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  int page = 0;
                  if (request->hasParam("page"))
                  {
                      page = request->getParam("page")->value().toInt();
                  }
                  String htmlContent = generateFileListHTML(page);
                  request->send(200, "text/html", htmlContent); });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  String htmlContent = generateStatusHTML();
                  request->send(200, "text/html", htmlContent); });

    // Serve files from the SD card
    server.serveStatic("/", SD, "/");

    // Route to handle file deletion
    server.on("/delete", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  if (request->hasParam("file", true))
                  {
                      AsyncWebParameter *p = request->getParam("file", true);
                      String filePath = p->value();
                      if (SD.remove(filePath.c_str()))
                      {
                          Serial.println("File Deleted");
                      }
                      else
                      {
                          Serial.println("Delete Failed");
                      }
                  }
                  request->redirect("/"); });

    ElegantOTA.begin(&server); // Start ElegantOTA

    // Start the web server
    server.begin();
}
