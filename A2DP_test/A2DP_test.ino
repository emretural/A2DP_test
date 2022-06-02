/*
 Name:		A2DP_test.ino
 Created:	30-May-22 00:04:23
 Author:	Emre TURAL
*/

#include "BluetoothA2DPSink.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
BluetoothA2DPSink a2dp_sink;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (Serial.available()){}
	Serial.println("Setup start");

	SerialBT.begin("ESP32test"); //Bluetooth device name
	Serial.println("The device started, now you can pair it with bluetooth!");

	//default config
	//my_pin_config.bck_io_num = 26;
	//my_pin_config.ws_io_num = 25;
	//my_pin_config.data_out_num = 22;
	//my_pin_config.data_in_num = I2S_PIN_NO_CHANGE;

	i2s_pin_config_t my_pin_config;
	my_pin_config.bck_io_num = 15;
	my_pin_config.ws_io_num = 16;
	my_pin_config.data_out_num = 17;
	my_pin_config.data_in_num = I2S_PIN_NO_CHANGE;

	a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST);
	a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);

	
	a2dp_sink.set_pin_config(my_pin_config);
	a2dp_sink.start("MyMusic");
}

void avrc_metadata_callback(uint8_t data1, const uint8_t* data2) {
	Serial.printf("AVRC metadata rsp: attribute id 0x%x, %s\n", data1, data2);

}

uint32_t lastMillis;
uint8_t lastCmd;
// the loop function runs over and over again until power down or reset
void loop() {
	if ((millis() - lastMillis) > 20000)
	{
		lastMillis = millis();
		lastCmd++;
		if (lastCmd == 1)
		{
			Serial.println("next");
			a2dp_sink.next();
		}
		else if (lastCmd == 2)
		{
			Serial.println("previous");
			a2dp_sink.previous();
		}
		else if (lastCmd == 3)
		{
			Serial.println("pause");
			a2dp_sink.pause();
		}
		else if (lastCmd == 4)
		{
			Serial.println("play");
			a2dp_sink.play();
		}
		else if (lastCmd == 5)
		{
			Serial.println("stop");
			a2dp_sink.stop();
		}
	}



	if (Serial.available()) {
		SerialBT.write(Serial.read());
	}
	if (SerialBT.available()) {
		Serial.write(SerialBT.read());
	}
	delay(1);
}
