#ifndef fonction_spiffs_h
#define fonction_spiffs_h

//////////////////////////////////////////spiffs///////////////////////////////////////////////
File fsUploadFile;

//format bytes
String formatBytes(size_t bytes)
{
    if (bytes < 1024)
    {
        return String(bytes) + "B";
    }
    else if (bytes < (1024 * 1024))
    {
        return String(bytes / 1024.0) + "KB";
    }
    else if (bytes < (1024 * 1024 * 1024))
    {
        return String(bytes / 1024.0 / 1024.0) + "MB";
    }
    else
    {
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
    }
} //formatBytes(size_t bytes)

String getContentType(String filename)
{
    if (server.hasArg("download"))
        return "application/octet-stream";
    else if (filename.endsWith(".htm"))
        return "text/html";
    else if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".png"))
        return "image/png";
    else if (filename.endsWith(".gif"))
        return "image/gif";
    else if (filename.endsWith(".jpg"))
        return "image/jpeg";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".xml"))
        return "text/xml";
    else if (filename.endsWith(".pdf"))
        return "application/x-pdf";
    else if (filename.endsWith(".zip"))
        return "application/x-zip";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
} //getContentType(String filename)

bool handleFileRead(String path)
{
#ifdef DEBUG
    Serial.println("handleFileRead: " + path);
#endif
    if (path.endsWith("/"))
        path += "index.htm";
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
    {
        if (SPIFFS.exists(pathWithGz))
            path += ".gz";
        File file = SPIFFS.open(path, "r");
        size_t sent = server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
} //handleFileRead(String path)

void handleFileUpload()
{
    if (server.uri() != "/edit")
        return;
    HTTPUpload &upload = server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if (!filename.startsWith("/"))
            filename = "/" + filename;
#ifdef DEBUG
        Serial.print("handleFileUpload Name: ");
        Serial.println(filename);
#endif
        fsUploadFile = SPIFFS.open(filename, "w");
        filename = String();
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
        if (fsUploadFile)
            fsUploadFile.write(upload.buf, upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (fsUploadFile)
            fsUploadFile.close();
#ifdef DEBUG
        Serial.print("handleFileUpload Size: ");
        Serial.println(upload.totalSize);
#endif
    }
} //handleFileUpload()

void handleFileDelete()
{
    if (server.args() == 0)
        return server.send(500, "text/plain", "BAD ARGS");
    String path = server.arg(0);
#ifdef DEBUG
    Serial.println("handleFileDelete: " + path);
#endif
    if (path == "/")
        return server.send(500, "text/plain", "BAD PATH");
    if (!SPIFFS.exists(path))
        return server.send(404, "text/plain", "FileNotFound");
    SPIFFS.remove(path);
    server.send(200, "text/plain", "");
    path = String();
} //handleFileDelete()

void handleFileCreate()
{
    if (server.args() == 0)
        return server.send(500, "text/plain", "BAD ARGS");
    String path = server.arg(0);
#ifdef DEBUG
    Serial.println("handleFileCreate: " + path);
#endif
    if (path == "/")
        return server.send(500, "text/plain", "BAD PATH");
    if (SPIFFS.exists(path))
        return server.send(500, "text/plain", "FILE EXISTS");
    File file = SPIFFS.open(path, "w");
    if (file)
        file.close();
    else
        return server.send(500, "text/plain", "CREATE FAILED");
    server.send(200, "text/plain", "");
    path = String();
} //handleFileCreate()

void handleFileList()
{
    if (!server.hasArg("dir"))
    {
        server.send(500, "text/plain", "BAD ARGS");
        return;
    }

    String path = server.arg("dir");
#ifdef DEBUG
    Serial.println("handleFileList: " + path);
#endif
    File dir = SPIFFS.open(path);
    path = String();

    if (!dir.isDirectory())
    {
        dir.close();
        server.send(500, "text/plain", "NOT DIR");
        return;
    }
    dir.rewindDirectory();

    String output = "[";

    for (int cnt = 0; true; ++cnt)
    {
        File entry = dir.openNextFile();
        if (!entry)
            break;

        if (cnt > 0)
            output += ',';

        output += "{\"type\":\"";
        output += (entry.isDirectory()) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(entry.name()).substring(1);
        output += "\"}";
        entry.close();
    }

    output += "]";
    server.send(200, "text/json", output);
} //handleFileList()

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }
}

#endif