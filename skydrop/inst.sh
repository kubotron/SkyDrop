set -e
diskutil mount /dev/disk2s1
cd Release
../utils/build/pre_build.sh
make
../utils/build/post_build.sh
cp SKYDROP.FW /Volumes/BIBEEP/
diskutil unmount /Volumes/BIBEEP/
