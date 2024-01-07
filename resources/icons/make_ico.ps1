inkscape.exe -w 256 -h 256 -o 256.png .\Naivis512x512.svg
inkscape.exe -w 128 -h 128 -o 128.png .\Naivis512x512.svg
inkscape.exe -w 64 -h 64 -o 64.png .\Naivis512x512.svg
inkscape.exe -w 32 -h 32 -o 32.png .\Naivis512x512.svg
inkscape.exe -w 24 -h 24 -o 24.png .\Naivis512x512.svg
magick convert .\256.png  .\128.png .\64.png .\32.png .\24.png Naivis.ico
