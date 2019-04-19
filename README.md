# ELEC458 Project 2 - Waveform Generator

Bu projede STM32F4DISCO geliştirme kartı ve 2nd Order RC Filter kullanılarak waveform generator yapılmıştır.

- Ramp, Saw, Square, Whitenoise, Sin sinyalleri üretilebilmektedir.
- 20 - 20000 Hz aralığında sinyal üretilebilmektedir.
- UART üzerinden output sinyalin tipi, frekansı ve genliği ayarlanabilmektedir.
- DAC kullanılmamıştır.
- UART Interrupt ve Timer interrupt kullanılmıştır.
- STM32F407VGT6 MCU için Keil uVision IDE kullanılarak yazılım geliştirilmiştir.

![SAW Signal (STM32F407 & 2nd Order Filter)](https://raw.githubusercontent.com/haliskilic/stm32f4-wave-generator/master/Osc-Outputs/saw.PNG)

![SETUP](https://raw.githubusercontent.com/haliskilic/stm32f4-wave-generator/master/Osc-Outputs/Setup.PNG)

## License

MIT. Copyright (c) Group4

## Maintainers

- [Halis KILIÇ](https://github.com/haliskilic)
- [Mete Can GAZİ](https://github.com/MGTRc)
- [Muhammet KARADAĞ](https://github.com/karadagmuhammet)

## Thanks!

- [Furkan ÇAYCI](https://github.com/fcayci/stm32f4-assembly)
