: Delete log files first.
cd OutCmp
del output1_Model0_vs_Model1.txt
del whiteNoise_Model1_vs_Model2.txt
del whiteNoise_Model2_vs_Model3.txt




cd ..

: Execute Model 0, Model 1, Model 2 and Model 3
cd Debug
"multitapEcho_model0.exe" "..//..//TestStreams//speech.wav" "..//OutStreams//out_speech_0.wav" 
"multitapEcho_model1.exe" "..//..//TestStreams//speech.wav" "..//OutStreams//out_speech_1.wav" 
"multitapEcho_model2.exe" "..//..//TestStreams//speech.wav" "..//OutStreams//out_speech_2.wav" 

cd ..

: Generate new logs
"..//tools//PCMCompare.exe" OutStreams//out_speech_0.wav OutStreams//out_speech_1.wav 2> OutCmp//whiteNoise_Model0_vs_Model1.txt
"..//tools//PCMCompare.exe" OutStreams//out_speech_1.wav OutStreams//out_speech_2.wav 2> OutCmp//whiteNoise_Model1_vs_Model2.txt


