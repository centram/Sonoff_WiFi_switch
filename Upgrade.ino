void initUpgrade() {
  // Добавляем функцию Update для перезаписи прошивки по Wi-Fi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);
  HTTP.on("/upgrade", webUpgrade);                // запустить обнавление
  modulesReg("upgrade");
}
// ----------------------- Обновление с сайта
void webUpgrade() {
  Serial.println("Update module...");
  String refresh = "<html><head><meta http-equiv=\"refresh\" content=\"40;/\">Update module...</head></html>";
  HTTP.send(200, "text/html", refresh);
  String spiffsData = HTTP.arg("spiffs");
  if (spiffsData != "") {
    SPIFFS.format();
    /*
      SPIFFS.remove("/js/build.chart.js.gz");
      SPIFFS.remove("/js/function.js.gz");
      SPIFFS.remove("/css/build.css.gz");
      SPIFFS.remove("/lang/lang.en.json.gz");
      SPIFFS.remove("/lang/lang.ru.json.gz");
      SPIFFS.remove("/lang/lang.lv.json.gz");
      SPIFFS.remove("/lang/lang.ua.json.gz");
    */
    spiffsData = spiffsData.substring(spiffsData.lastIndexOf("/") + 1); // выделяем имя файла
    Serial.println(spiffsData);
    ESPhttpUpdate.rebootOnUpdate(false);
    t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(HTTP.arg("spiffs"));
switch(ret) {
            case HTTP_UPDATE_FAILED:
            configJson = jsonWrite(configJson, "spiffsData", "HTTP_UPDATE_FAILED");
            statistics();
            break;
            case HTTP_UPDATE_NO_UPDATES:
            configJson = jsonWrite(configJson, "spiffsData", "HTTP_UPDATE_NO_UPDATES");
            statistics();
                break;
            case HTTP_UPDATE_OK:
            configJson = jsonWrite(configJson, "spiffsData", "HTTP_UPDATE_OK");
            statistics();
                break;
        }
    writeFile("timer.save.json", jsonTimer);
    configJson = jsonWrite(configJson, "spiffsData", spiffsData);
    writeFile("config.save.json", configJson );
  }
  String buildData = HTTP.arg("build");
  if (buildData != "") {
    buildData = buildData.substring(buildData.lastIndexOf("/") + 1); // выделяем имя файла
    Serial.println(buildData);
    configJson = jsonWrite(configJson, "buildData", buildData);
    writeFile("config.save.json", configJson );
    ESPhttpUpdate.rebootOnUpdate(true);
    t_httpUpdate_return jet = ESPhttpUpdate.update(HTTP.arg("build"));
  }
}
