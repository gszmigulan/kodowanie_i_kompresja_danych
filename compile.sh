gcc -o  compress code/main.c code/low.c code/high.c common/bits.c -lm
echo ' program kodujący:  ./compress <plik_wejściowy.tga> <plik_wyjściowy> <bity>'
gcc -o decompress decode/main.c common/bits.c -lm
echo ' program dekdujący:  ./decompress <plik_wejściowy> <plik_wyjściowy.tga>'
gcc -o results wyniki/errors.c -lm
echo ' program obliczający mse i snr:  ./results <plik_pierwszy.tga> <plik_drugi.tga>'


