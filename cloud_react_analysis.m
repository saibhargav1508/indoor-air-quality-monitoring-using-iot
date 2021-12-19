% This code is added to the MATLAB analysis in ThingSpeak and is triggered by React.
% code sends an email whenever the readings meet the react condition

alert_body = 'The PM2 dust sensor has detected a reading above the set threshold';
alert_subject = 'Dust sensor reading above threshold';

alert_api_key = '*****'; % add your channel alert API key here

alert_url= "https://api.thingspeak.com/alerts/send";
jsonmessage = sprintf(['{"subject": "%s", "body": "%s"}'], alert_subject,alert_body);
options = weboptions("HeaderFields", {'Thingspeak-Alerts-API-Key', alert_api_key; 'Content-Type','application/json'});
result = webwrite(alert_url, jsonmessage, options);
