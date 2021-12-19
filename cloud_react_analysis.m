alert_body = 'The PM2 dust sensor has detected a reading above the set threshold';
alert_subject = 'Dust sensor reading above threshold';
alert_api_key = 'TAKM642RV1WZ0QNM1PGJ4';
alert_url= "https://api.thingspeak.com/alerts/send";
jsonmessage = sprintf(['{"subject": "%s", "body": "%s"}'], alert_subject,alert_body);
options = weboptions("HeaderFields", {'Thingspeak-Alerts-API-Key', alert_api_key; 'Content-Type','application/json'});
result = webwrite(alert_url, jsonmessage, options);