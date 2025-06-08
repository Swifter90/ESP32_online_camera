Для реализации видеопотока в приложении blynk мне понадобилось:
- ESP32-CAM
- Камера OV2640
- Модуль ESP32-CAM-MB
- Провод usb-mini usb 
- Планшет Android c установленным приложением Blynk IoT
- Ноутбук с Arduino IDE

**Настройка Blynk 

Установите приложение Blynk IoT и выполните вход.
Выберите quick start device.
Выберите плату esp32 и соединение WiFi.
На зарегистрированную почту должны прийти настройки устройства.
Пройдите по ссылке и скопируйте далее в код Arduino BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME, BLYNK_AUTH_TOKEN.
В настройках устройства добавьте виджет Video Stream.
Настройки виджета:
Pin: V13 (или любой свободный виртуальный пин, не используемый для моторов).
URL: Введите http://192.168.1.100/mjpeg/1 (замените 192.168.1.100 на IP-адрес ESP32-CAM из Serial Monitor).
Stream Type: Выберите MJPEG.
Play/Pause: Включите, чтобы запускать/останавливать поток.

**Настройка Arduino IDE для ESP32-CAM

Установите Arduino IDE (если ещё не установлено):
Используйте Arduino IDE версии 2.3.3 или новее.
Добавьте поддержку ESP32:
В Arduino IDE перейдите в File → Preferences.
В поле Additional Boards Manager URLs добавьте:
https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json

Перейдите в Tools → Board → Boards Manager, найдите ESP32 и установите (автор: Espressif Systems).
Выберите плату:
В Tools → Board выберите AI Thinker ESP32-CAM.
В Tools → Port выберите порт.

Загрузите или вставьте код из файла espvideofinal.ino
Поменяйте в коде на свои значения переменные BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME, BLYNK_AUTH_TOKEN, const char* ssid, const char* password

Скомпилируйте и загрузите код на esp32-cam.
Отключите устройство от ПК и заново подключите.
Зайдите в Serial Monitor скопируйте ip адрес и вставьте в url blynk http://<ваш ip адрес>/mjpeg/1
Проверьте воспроизведение. Поток можно посмотреть также, вставив ссылку http://<ваш ip адрес>/mjpeg/1 в браузер.
В blynk у меня была задержка передачи в 5-7 секунд.




