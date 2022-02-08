#!/bin/bash

parentDir=$(pwd)
mkdir -p animPairs

echo "Copying .ANI files..."
for f in $(find . -name "*.ANI" -not -path "./animPairs/*" -size +8k); do

    shortName=$(echo "${f##*/}" | sed 's/\.ANI//g')
    concatName=$(echo $f | sed 's/\.\///g' | sed 's/\//-/g')
    concatNameNoExt=$(echo $f | sed 's/\.\///g' | sed 's/\//-/g' | sed 's/\.ANI//g' )

    soundFile=$(find . -name "$shortName.WAV")

    mkdir -p animPairs/$concatNameNoExt
    cp $f animPairs/$concatNameNoExt/$concatName

    if [ -f "$soundFile" ]; then
        cp "$soundFile" animPairs/$concatNameNoExt/"$concatNameNoExt".WAV
    fi
    
done

cd animPairs/

for d in ./*; do
    cd $d
    shortName=$(ls *.ANI | sed 's/\.ANI//g')
    frameCount=$(find . -name "*.bmp" | wc -l)
    echo $frameCount

    echo "Decoding $shortName..."
    $parentDir/FrameworxDecoder $(realpath *.ANI) $shortName

    if [ -f *.WAV ]; then
        durationMS=$(mediainfo --inform="Audio;%Duration%" *.WAV)
        durationSec=$(echo "$durationMS / 1000" | bc -l)
        frameRate=$(echo "$frameCount / $durationSec" | bc -l)

        echo "Rendering $shortName to video..."
        
        ffmpeg -hide_banner -loglevel error -y -pattern_type glob -framerate "$frameRate" -i '*.bmp' -i "$shortName.WAV" -t "$durationSec" -map 0:v -map 1:a -vcodec ffv1 "$shortName".mkv
    else
        frameRate="8"
        durationSec=$(echo "$frameCount / 8" | bc -l)

        echo "Rendering $shortName to video..."
        ffmpeg -hide_banner -loglevel error -y -pattern_type glob -framerate "$frameRate" -i '*.bmp' -t "$durationSec" -vcodec ffv1 "$shortName".mkv
    fi


    cd ..
done
