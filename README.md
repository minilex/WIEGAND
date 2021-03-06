# wiegand
Библиотека приема/перадачи данных по интерфейсу [Wiegand](https://ru.wikipedia.org/wiki/%D0%92%D0%B8%D0%B3%D0%B0%D0%BD%D0%B4_(%D0%B8%D0%BD%D1%82%D0%B5%D1%80%D1%84%D0%B5%D0%B9%D1%81))

Библиотека предназначена для микроконтроллеров STM32 и предполагается, что проект создается в CUBEMX.

Передатчик использзует один таймер для формирования временных интервалов и два GPIO в режиме выхода для линии передачи. 
Для его использования:
1. Настройте два GPIO в CUBEMX на выход c user label WIEGAND_D1 и WIEGAND_D0.
2. Настройте любой таймер с частотой тактирования 1МГц, включите TIM Global interrupt.
3. Передайте драйверу указатель на структуру таймера функцией `WiegandInit(&htimx)`.
4. Из прерывания таймера вызовите `WiegandProcessTx()`.
5. Для передачи используйте функцию `WiegandTransmit(uint32_t data, uint32_t bits)`.

Пример: https://github.com/minilex/WIEGAND_TX

Приемник использует один таймер для формирования таймаута и два GPIO в режиме EXTI для приема данных.
Для его использования:
1. То же.
2. То же, частота 10кГц.
3. То же.
4. Из прерывания таймера вызовите `WiegandTimeout()`.
5. Из внешних прерываний вызовите `WiegandSaveBit(uint32_t bit)`
6. Принятые данные выводятся в консоль отладчика.

Пример: https://github.com/minilex/WIEGAND_RX

Работа примеров в паре:

![wiegand _osc](https://user-images.githubusercontent.com/42541058/118799031-b7604e00-b8b7-11eb-844e-41f971243a78.jpg)
![wiegand_debug](https://user-images.githubusercontent.com/42541058/118799048-bcbd9880-b8b7-11eb-98d6-14e630f105c8.JPG)


