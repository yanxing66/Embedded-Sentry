# Real_Time_Embeded _System

project1: embebed_sentry
Use the data collected from a single accelerometer and/or magnetometer to record a hand movement sequence as a means to generally “unlock” a resource.
● Recorded sequence must be saved on the microcontroller, using a “Record Key”
feature.
● User then must replicate the key sequence within sufficient tolerances to unlock
the resource.
● A successful unlock must be indicated by a visual indication, such as an LED or
similar indicator.
#ide:Arduino
#devices:stm32,accelermeter axl345
#language:c
