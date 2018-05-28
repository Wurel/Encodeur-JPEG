#!/bin/bash
make
rm -rf ./Mes_images_jpeg
mkdir ./Mes_images_jpeg

./ppm2jpeg ./images/invader.pgm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/invaders-11-11-11.jpg
./ppm2jpeg ./images/invader.pgm --sample=2x1,1x1,1x1 --outfile=./Mes_images_jpeg/invaders-21-11-11.jpg

./ppm2jpeg ./images/gris.pgm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/gris-11-11-11.jpg

./ppm2jpeg ./images/bisou.pgm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/bisou-11-11-11.jpg

./ppm2jpeg ./images/heartbreak.ppm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/heartbreak-11-11-11.jpg
./ppm2jpeg ./images/heartbreak.ppm --sample=1x2,1x1,1x1 --outfile=./Mes_images_jpeg/heartbreak-12-11-11.jpg

./ppm2jpeg ./images/zig-zag.ppm --sample=2x2,1x2,1x2 --outfile=./Mes_images_jpeg/zigzag-22-12-12.jpg

./ppm2jpeg ./images/chefdoeuvre.ppm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/chefdoeuvre-11-11-11.jpg
./ppm2jpeg ./images/chefdoeuvre.ppm --sample=2x2,1x1,1x1 --outfile=./Mes_images_jpeg/chefdoeuvre-22-11-11.jpg

./ppm2jpeg ./images/thumbs.ppm --sample=2x2,2x1,2x1 --outfile=./Mes_images_jpeg/thumbs-22-21-21.jpg
./ppm2jpeg ./images/thumbs.ppm --sample=4x4,2x2,2x2 --outfile=./Mes_images_jpeg/thumbs-44-22-22.jpg

./ppm2jpeg ./images/shaun_the_sheep.ppm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/shaun-11-11-11.jpg
./ppm2jpeg ./images/shaun_the_sheep.ppm --sample=1x1,2x1,2x1 --outfile=./Mes_images_jpeg/shaun-11-21-21.jpg
./ppm2jpeg ./images/shaun_the_sheep.ppm --sample=2x2,2x1,1x1 --outfile=./Mes_images_jpeg/shaun-22-21-11.jpg

./ppm2jpeg ./images/horizontal.ppm --sample=2x2,1x1,1x1 --outfile=./Mes_images_jpeg/horizontal-22-11-11.jpg

./ppm2jpeg ./images/vertical.ppm --sample=1x4,1x1,1x1 --outfile=./Mes_images_jpeg/vertical-14-11-11.jpg

# ./ppm2jpeg ./images/complexite.pgm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/thumbs-11-11-11.jpg+
#
# ./ppm2jpeg ./images/biiiiiig.ppm --sample=1x1,1x1,1x1 --outfile=./Mes_images_jpeg/biiiiiig-11-11-11.jpg
