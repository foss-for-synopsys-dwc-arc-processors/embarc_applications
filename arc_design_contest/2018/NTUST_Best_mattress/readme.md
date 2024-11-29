#
# 具生理訊號感測之智慧床墊設計

https://v.youku.com/v_show/id_XMzY4NTIwNDIwOA==.html?spm=a2h3j.8428770.3416059.1

# Introduction

本作品只需放置於床墊中，運用壓電感測、壓力感測與演算法的分析，可實行不用直接接觸皮膚，並對心率、呼吸及體動進行即時監測，利用心率改變及身體動作，進而分析睡眠狀態。

使用壓電感測器感測身體訊號，再經由硬體濾波得到呼吸及脈搏訊號

使用 ADC converter 收入感測訊號，再經由 ARC EMSK 運算後輸出心率及呼吸頻率

### I/O
PMOD1 => UART 38400bps
    輸出 心率，呼吸，呼吸訊號，脈搏訊號
        
PMOD2 => I2C  ad7991 ADC模組
    reference為板子的3.3V
    使用 channel 1 作為壓電感測輸入

# Hardware and Software Setup



# User manual
