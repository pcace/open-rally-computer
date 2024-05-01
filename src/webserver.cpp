
#include <time.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// Replace with your network credentials
const char *ssid = "orc";
const char *password = "12345678";

AsyncWebServer server(80);

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

void listFilesInDir(File dir, String &html, String path)
{
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            // No more files
            break;
        }
        // Ignore hidden files and directories (names starting with '.')
        if (entry.name()[0] == '.')
        {
            entry.close();
            continue;
        }
        String entryPath = path + entry.name();
        if (entry.isDirectory())
        {
            listFilesInDir(entry, html, entryPath + "/");
        }
        else
        {
            // Create a link for each file
            html += "<a href=\"" + entryPath + "\" download>" + entry.name() + "</a> <form action=\"/delete\" method=\"post\" style=\"display:inline;\"><button name=\"file\" value=\"" + entryPath + "\">Delete</button></form><br/>";
        }
        entry.close();
    }
}

String generateFileListHTML()
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
        listFilesInDir(root, html, "/");
    }
    root.close();
    html += "</body></html>";
    return html;
}

void initializeWebserver()
{
    // Web server routes
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        String htmlContent = generateFileListHTML();
        request->send(200, "text/html", htmlContent); });

    // Serve files from the SD card
    server.serveStatic("/", SD, "/");

    // Route to handle file deletion
    server.on("/delete", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("file", true)) {
            AsyncWebParameter* p = request->getParam("file", true);
            String filePath = p->value();
            if (SD.remove(filePath.c_str())) {
                Serial.println("File Deleted");
            } else {
                Serial.println("Delete Failed");
            }
        }
        request->redirect("/");
    });

    // Start the web server
    server.begin();
}