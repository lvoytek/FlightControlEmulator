/*
 * PWMAutoLog.js
 *
 * Log PWM Channel 1-6 average data for control configurations until user types 'exit'
 *
 * Author: Lena Voytek
*/

var channelText = Tool.getText("Configuration Name:","idle");

while(channelText != "exit" && channelText != "")
{
    print(channelText);
    
    channelText = Tool.getText("Configuration Name:","idle");
}